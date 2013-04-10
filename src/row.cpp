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

#include "row.h"
#include "parameter_lookup.h"

#include <cassert>

namespace sqlite {

row::row(sqlite3_stmt *statement): stmt(statement) {
    assert(statement && "received null sqlite3_stmt");
}

field row::operator[](const std::string &column_name) const {
    return {stmt, internal::find_column_index(column_name, stmt)};
}

field row::operator[](const std::size_t &column_index) const {
    return {stmt, column_index};
}

} // namespace sqlite
