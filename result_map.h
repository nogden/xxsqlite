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

#ifndef SQLITE_RESULT_MAP_H
#define SQLITE_RESULT_MAP_H

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

class result_map
{
public:
    class const_iterator;

public:
    result_map(sqlite3_stmt *statement, const ownership &ownership);
    result_map(const result_map &other) = delete;
    result_map(result_map &&other);
    ~result_map();

    result_map& operator=(const result_map &other) = delete;
    result_map& operator=(result_map &&other);

    std::size_t column_count() const;
    std::string column_name(const std::size_t &column_index) const;

//    const_iterator begin() const;
//    const_iterator end() const;

private:
    void replace_members_with(result_map &other);

private:
    sqlite3_stmt *stmt = nullptr;
    bool owns_statement = false;
};

namespace {

//class result_map::const_iterator {
//public:
//    const_iterator(sqlite3_stmt *stmt);
//    const_iterator(const const_iterator &other);

//    bool operator==(const const_iterator &other);
//    bool operator!=(const const_iterator &other);

//    field operator[](const std::string &column_name);
//    field operator[](const std::size_t &column_index);

//    void operator++();
//    void operator++(const const_iterator &self);
//};

}

const char* error_message(const int status);
const char* error_message(sqlite3_stmt *statement);

} // namespace sqlite

#endif // SQLITE_RESULT_MAP_H
