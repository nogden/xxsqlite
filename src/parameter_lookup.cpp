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

#include "parameter_lookup.h"
#include "error.h"

#include <sqlite3.h>

#include <cassert>

namespace sqlite {
namespace internal {

std::size_t find_parameter_index(
        const std::string &parameter,
        sqlite3_stmt *stmt
) {
    assert(stmt && "null sqlite3_stmt provided");
    auto index(sqlite3_bind_parameter_index(stmt, parameter.c_str()));
    if (! index)
        throw bad_parameter(parameter, stmt);
    return index;
}

std::size_t find_column_index(
        const std::string &column_name,
        sqlite3_stmt *stmt
) {
    assert(stmt && "null sqlite3_stmt provided");
    std::size_t column_count(sqlite3_column_count(stmt));
    for (std::size_t i(0); i < column_count; ++i) {
        if (sqlite3_column_name(stmt, i) == column_name)
            return i;
    }
    assert(false && "invalid column name provided");
    throw error("invalid column name: " + column_name);
}

}   // namespace internal
}   // namespace sqlite
