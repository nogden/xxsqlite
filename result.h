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

#include <string>
#include <stdexcept>

struct sqlite3_stmt;

namespace sqlite {

class database_error: public std::runtime_error {
public:
    database_error(const std::string &msg): std::runtime_error(msg) {}
};

enum class ownership {
    take,
    none
};

enum class iterator_pos {
    element,
    end
};

class result
{
public:
    class const_iterator {
    public:
        const_iterator(
                sqlite3_stmt *statement,
                const iterator_pos &position = iterator_pos::element
        );
        const_iterator(const iterator_pos &position);
        const_iterator(const const_iterator &other) = delete;
        const_iterator(const_iterator &&other) = default;

        const_iterator& operator=(const const_iterator &other) = delete;
        const_iterator& operator=(const_iterator &&other) = default;

        bool operator==(const const_iterator &other) const;
        bool operator!=(const const_iterator &other) const;

        const_iterator& operator++();

    private:
        sqlite3_stmt *stmt = nullptr;
        bool at_end = false;
    };

public:
    result(sqlite3_stmt *statement, const ownership &ownership);
    result(const result &other) = delete;
    result(result &&other);
    ~result();

    result& operator=(const result &other) = delete;
    result& operator=(result &&other);

    std::size_t column_count() const;
    std::string column_name(const std::size_t &column_index) const;

    const_iterator begin() const;
    const_iterator end() const;

private:
    void replace_members_with(result &other);

private:
    sqlite3_stmt *stmt = nullptr;
    bool owns_statement = false;
    bool end_reached = false;
};

const char* error_message(const int status);
const char* error_message(sqlite3_stmt *statement);

} // namespace sqlite

#endif // SQLITE_RESULT_H
