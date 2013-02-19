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

#ifndef SQLITE_STATEMENT_H
#define SQLITE_STATEMENT_H

#include <ostream>
#include <stdexcept>
#include <cstdint>

struct sqlite3_stmt;

namespace sqlite {

class blob;

class database_error: public std::runtime_error {
public:
    database_error(std::string const &msg): std::runtime_error(msg) {}
};

enum class null_value {
    null
};

class statement
{
public:
    statement(sqlite3_stmt *statement);
    ~statement();
    statement(statement &&other);
    statement(statement const &other) = delete;

    statement & operator=(statement &&other);
    statement & operator=(statement const &other) = delete;

    void bind(std::string const &parameter, sqlite::blob const &value);
    void bind(std::string const &parameter, double value);
    void bind(std::string const &parameter, int value);
    void bind(std::string const &parameter, int64_t value);
    void bind(std::string const &parameter, sqlite::null_value value);
    void bind(std::string const &parameter, std::string const &value);
    void bind(std::string const &parameter, std::wstring const &value);

    friend std::ostream& operator<<(
        std::ostream &os, statement const &statement
    );

private:
    sqlite3_stmt *stmt = nullptr;
};

} // namespace sqlite

#endif // SQLITE_STATEMENT_H
