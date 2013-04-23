/*
   Copyright (C) 2013  Nick Ogden <nick@nickogden.org>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
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
