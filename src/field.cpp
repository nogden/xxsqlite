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

field::field(sqlite3_stmt *statement, const std::size_t &parameter_index):
    stmt(statement), index(parameter_index) {
    assert(statement && "received null sqlite3_stmt");
}

bool field::is_null() const {
    return sqlite3_column_type(stmt, index) == SQLITE_NULL;
}

field::operator bool() const {
    return is_null();
}



}   // namespace sqlite
