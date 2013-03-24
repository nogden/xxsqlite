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
#include "error.h"

#include <sqlite3.h>

#include <ostream>
#include <sstream>
#include <cassert>

namespace sqlite {

database::database(sqlite3 *connection): db(connection) {
    assert(connection && "attempt to create database with null connection");
}

database::~database() {
    close();
}

result database::execute(const std::string &sql) {
    return {create_statement(sql), ownership::take};
}

void database::close() {
    auto status(sqlite3_close(db));
    db = nullptr;
    // Can't throw, called from destructor
    assert(
        status == SQLITE_OK &&
        "database closed with active statements or unfinished backups"
    );
}

sqlite3_stmt* database::create_statement(const std::string &sql) {
    sqlite3_stmt *stmt(nullptr);
    auto status(sqlite3_prepare_v2(
        db, sql.c_str(), sql.size(), &stmt, nullptr
    ));
    if (status != SQLITE_OK)
        throw bad_statement(status, sql);
    return stmt;
}

std::unique_ptr<statement> database::make_statement(const std::string &sql) {
    return std::make_unique<statement>(create_statement(sql));
}

std::unique_ptr<database> make_database(
        const std::string &path,
        const access_mode &permissions
) {
    sqlite3 *db;
    auto status(sqlite3_open_v2(path.c_str(), &db, permissions, nullptr));
    if (status != SQLITE_OK) {
        sqlite3_close_v2(db);
        throw error(status);
    }
    return std::make_unique<database>(db);
}

std::ostream& operator<<(std::ostream &os, const database &db) {
    os << "database:\n"
          "  open: " << ((db.db == nullptr) ? "false" : "true") << "\n";
    return os;
}

} // namespace sqlite
