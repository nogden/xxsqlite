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

#ifndef SQLITE_STATEMENT_H
#define SQLITE_STATEMENT_H

#include "result.hpp"

#include <cstddef>

struct sqlite3;
struct sqlite3_stmt;

namespace sqlite {

class blob;
class statement;

enum class null_t {
    null
};
static const null_t null = null_t::null;

class statement
{
public:
    statement(const std::shared_ptr<sqlite3_stmt> &statement);
    statement(const statement &other) = delete;
    statement(statement &&other) = default;

    statement& operator=(const statement &other) = delete;
    statement& operator=(statement &&other) = default;

    std::size_t parameter_count() const;
    template<typename T>
    void bind(const std::string &parameter, const T &value);
    void bind(const std::string &parameter, const char *value);
    void clear_bindings();

    friend std::ostream& operator<<(
        std::ostream &os, const statement &statement
    );

private:
    void throw_on_bind_error(
            const int status,
            const std::string &parameter
    ) const;
    friend result make_result(const statement &statement);

private:
    std::shared_ptr<sqlite3_stmt> stmt;
};

} // namespace sqlite

#endif // SQLITE_STATEMENT_H
