/*
  Copyright (C) 2013  Nick Ogden <nick@nickogden.org>
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the
  
  Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "database.h"

#include <sqlite3.h>

#include <ostream>
#include <sstream>
#include <cassert>

namespace sqlite {

database::database(std::string const &path): path(path) {}

database::database(database &&other) {
    assert(&other !=  this && "attempt to move database to itself");
    std::swap(db, other.db);
    std::swap(path, other.path);
}

database::~database() {
    close();
}

database &database::operator=(database &&other) {
    assert(&other != this && "attempt to assign database to itself");
    std::swap(db, other.db);
    std::swap(path, other.path);
    return *this;
}

void database::open(access_mode const &mode) {
    if (db) return;
    if (sqlite3_open_v2(path.c_str(), &db, mode, nullptr) != SQLITE_OK) {
        std::stringstream ss;
        ss << sqlite3_errmsg(db)
           << "\n" "while opening database: " << path;
        throw database_error(ss.str());
    }
}

void database::close() {
    if (! db) return;
    auto status(sqlite3_close(db));
    db = nullptr;
    // Can't throw, called from destructor
    assert(
        status == SQLITE_OK &&
        "database closed with active statements or unfinished backups"
    );
}

std::unique_ptr<statement> database::make_statement(std::string const &sql) {
    assert(db && "make_statement() called on closed database");
    sqlite3_stmt *stmt(nullptr);
    auto status(sqlite3_prepare_v2(
        db, sql.c_str(), sql.size(), &stmt, nullptr
    ));
    if (status != SQLITE_OK) {
        sqlite3_finalize(stmt);
        std::stringstream ss;
        ss << sqlite3_errstr(status)
           << "\n" "while preparing statement '" << sql << "'";
        throw database_error(ss.str());
    }
    return std::make_unique<statement>(stmt);
}

std::ostream& operator<<(std::ostream &os, database const &db) {
    os << "database:\n"
          "  path: " << db.path << "\n"
          "  open: " << ((db.db == nullptr) ? "false" : "true") << "\n";
    return os;
}

} // namespace sqlite
