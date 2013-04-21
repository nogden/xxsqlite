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

#ifndef SQLITE_FIELD_H
#define SQLITE_FIELD_H

#include "mem/memory.h"
#include <string>
#include <cstdint>

struct sqlite3_stmt;
//extern "C" const unsigned char* sqlite3_column_text(sqlite3_stmt *, int);

namespace sqlite {

class field {
public:
    field(
            const std::shared_ptr<sqlite3_stmt> &statement,
            const std::size_t &parameter_index
    );

    bool is_null() const;
    explicit operator bool() const;

    std::string column_name() const;
    template<typename T>
    T as() const;

private:
    std::shared_ptr<sqlite3_stmt> stmt;
    const std::size_t index;
};

}   // namespace sqlite

#endif // SQLITE_FIELD_H
