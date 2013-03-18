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

#include "result.h"

#include <cstdint>

struct sqlite3_stmt;

namespace sqlite {

class blob;

enum class null_t {
    null
};
static const null_t null = null_t::null;

class statement
{
public:
    statement(sqlite3_stmt *statement);
    statement(const statement &other) = delete;
    statement(statement &&other) = default;
    ~statement();

    statement & operator=(const statement &other) = delete;
    statement & operator=(statement &&other) = default;


    void bind(const std::string &parameter, const sqlite::blob &value);
    void bind(const std::string &parameter, const double &value);
    void bind(const std::string &parameter, const int &value);
    void bind(const std::string &parameter, const int64_t &value);
    void bind(const std::string &parameter, const null_t &value);
    void bind(const std::string &parameter, const std::string &value);

    void clear_bindings();

    result execute();

    std::size_t parameter_count() const;

    friend std::ostream& operator<<(
        std::ostream &os, const statement &statement
    );

private:
    sqlite3_stmt *stmt = nullptr;
};

} // namespace sqlite

#endif // SQLITE_STATEMENT_H
