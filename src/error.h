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

#ifndef SQLITE_ERROR_H
#define SQLITE_ERROR_H

#include "mem/memory.h"
#include <stdexcept>

struct sqlite3;
struct sqlite3_stmt;

namespace sqlite {

class error: public std::runtime_error {
public:
    error(const int error_code);
    error(const std::string &msg);
    const int sqlite_error_code() const noexcept { return code; }

private:
    const int code = 0;
};

class bad_statement: public error {
public:
    bad_statement(const std::shared_ptr<sqlite3_stmt> &stmt);
    bad_statement(const int error_code, const std::string &sql);
    std::string sql() const noexcept { return sql_statement; }
    const char* what() const noexcept;

private:
    std::string sql_statement = "";
};

class bad_parameter: public bad_statement {
public:
    bad_parameter(
            const std::string &parameter,
            const std::shared_ptr<sqlite3_stmt> &stmt
    );
    std::string parameter() const noexcept { return param; }
    const char* what() const noexcept;

private:
    std::string param;
};

class bind_error: public bad_parameter {
public:
    bind_error(
            const std::string &parameter,
            const std::shared_ptr<sqlite3_stmt> &stmt
    );
    const char* what() const noexcept;

private:
    std::size_t index;
};

class out_of_range: public error {
public:
    out_of_range(const std::size_t &index);
    std::size_t index() const noexcept;
    const char* what() const noexcept;

private:
    std::size_t idx;
};

} // namespace sqlite

#endif // SQLITE_ERROR_H
