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

#ifndef SQLITE_ROW_H
#define SQLITE_ROW_H

#include "field.h"

#include <string>
#include <cstdint>

struct sqlite3_stmt;

namespace sqlite {

class row
{
public:
    row(sqlite3_stmt *statement);
    row(const row &other) = default;
    row(row &&other) = default;

    row& operator=(const row &other) = default;
    row& operator=(row &&other) = default;

    field operator[](const std::string &column_name) const;
    field operator[](const std::size_t &column_index) const;

private:
    sqlite3_stmt *stmt;
};

} // namespace sqlite

#endif // SQLITE_ROW_H
