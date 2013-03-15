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

#include "result_map.h"

#include <sqlite3.h>

#include <sstream>
#include <cassert>

namespace sqlite {

result_map::result_map(sqlite3_stmt *statement, const ownership &ownership):
    stmt(statement), owns_statement(ownership == ownership::take) {
    assert(statement && "null sqlite3_stmt provided");
    auto status(sqlite3_step(stmt));
    if (status != SQLITE_ROW && status != SQLITE_DONE) {
        std::stringstream ss;
        ss << error_message(stmt)
           << " while executing sql statement '" << sqlite3_sql(stmt) << "'";
        throw database_error(ss.str());
    }
}

result_map::~result_map() {
    if (owns_statement) {
        (void) sqlite3_finalize(stmt);
    }
}

std::size_t result_map::column_count() const {
    return sqlite3_column_count(stmt);
}

std::string result_map::column_name(const std::size_t &column_index) const {
    return sqlite3_column_name(stmt, column_index);
}

const char* error_message(const int status) {
    return sqlite3_errstr(status);
}

const char* error_message(sqlite3_stmt *statement) {
    return sqlite3_errmsg(sqlite3_db_handle(statement));
}

} // namespace sqlite
