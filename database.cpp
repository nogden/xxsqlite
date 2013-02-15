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

#include <sstream>
#include <cassert>

namespace sqlite {

static decltype(SQLITE_OPEN_READONLY) toSqliteMode(
        access_mode const &mode
) {
    switch (mode) {
    case read_only:         return SQLITE_OPEN_READONLY;
    case read_write:        return SQLITE_OPEN_READWRITE;
    case read_write_create: return SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    default:                assert(false && "unknown access mode");
    }
}

database::database(std::string const &path): path(path) {}

database::~database() {
    close();
}

void database::open(access_mode const &mode) {
    if (db) return;
    int status(sqlite3_open_v2(path.c_str(), db, mode, nullptr));
    if (status != SQLITE_OK) {
        std::stringstream ss;
        ss << sqlite3_errmsg(*db)
           << "\n" "while opening database: " << path << "\n";
        throw database_error(ss.str());
    }
}

void database::close() {
    if (! db) return;
    auto status(sqlite3_close(*db));
    db = nullptr;
    // Can't throw, called from destructor
    assert(
        status == SQLITE_OK &&
        "database closed with active statements or unfinished backups"
    );
}

std::ostream& operator<<(std::ostream &os, database const &db) {
    os << "database:\n"
          "  path: " << db.path << "\n"
          "  open: " << ((db.db == nullptr) ? "false" : "true") << "\n";
    return os;
}

} // namespace sqlite