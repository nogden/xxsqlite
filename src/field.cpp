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

#include "field.h"

#include <sqlite3.h>

#include <cassert>

namespace sqlite {

class blob {};

field::field(sqlite3_stmt *statement, const std::size_t &parameter_index):
    stmt(statement), index(parameter_index) {
    assert(statement && "received null sqlite3_stmt");
}

bool field::is_null() const {
    return sqlite3_column_type(stmt, index) == SQLITE_NULL;
}

field::operator bool() const {
    return ! is_null();
}

template<>
blob field::as<blob>() const {
    assert(false && "blob support not implemented");
    return blob();
}

template<>
double field::as<double>() const {
    if (is_null())
        return 0.0;
    return sqlite3_column_double(stmt, index);
}

template<>
int field::as<int>() const {
    if (is_null())
        return 0;
    return sqlite3_column_int(stmt, index);
}

template<>
int64_t field::as<int64_t>() const {
    if (is_null())
        return 0;
    return sqlite3_column_int64(stmt, index);
}

template<>
std::string field::as<std::string>() const {
    if (is_null())
        return "";
    return reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
}

}   // namespace sqlite
