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

#ifndef SQLITE_RESULT_H
#define SQLITE_RESULT_H

#include "row.h"

#include "mem/memory.h"
#include <string>
#include <cstdint>

struct sqlite3_stmt;

namespace sqlite {

class result
{
public:
    class const_iterator {
    public:
        const_iterator(
                const std::shared_ptr<sqlite3_stmt> &statement,
                bool &at_end
        );
        const_iterator(const const_iterator &other) = delete;
        const_iterator(const_iterator &&other) = default;

        const_iterator& operator=(const const_iterator &other) = delete;
        const_iterator& operator=(const_iterator &&other) = default;

        bool operator==(const const_iterator &other) const;
        bool operator!=(const const_iterator &other) const;

        const_iterator& operator++();
        const row& operator*() const;
        const row* operator->() const;

    private:
        std::shared_ptr<sqlite3_stmt> stmt;
        bool &end_reached;
        row current_row;
    };

public:
    result(const std::shared_ptr<sqlite3_stmt> &statement);
    result(const result &other) = delete;
    result(result &&other);

    result& operator=(const result &other) = delete;
    result& operator=(result &&other);

    std::size_t row_modification_count() const;

    const_iterator begin() const;
    const_iterator end() const;

private:
    std::shared_ptr<sqlite3_stmt> stmt;
    mutable bool end_reached = false;
};

} // namespace sqlite

#endif // SQLITE_RESULT_H
