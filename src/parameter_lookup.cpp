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

#include "parameter_lookup.hpp"
#include "error.hpp"

#include <sqlite3.h>

#include <cassert>

namespace sqlite {
namespace internal {

std::size_t find_parameter_index(
        const std::string &parameter,
        const std::shared_ptr<sqlite3_stmt> &stmt
) {
    assert(stmt && "bind() called on null sqlite::statement");
    auto index(sqlite3_bind_parameter_index(stmt.get(), parameter.c_str()));
    if (! index)
        throw bad_parameter(parameter, stmt);
    return index;
}

std::size_t find_column_index(
        const std::string &column_name,
        const std::shared_ptr<sqlite3_stmt> &stmt
) {
    assert(stmt && "null sqlite3_stmt provided");
    std::size_t column_count(sqlite3_column_count(stmt.get()));
    for (std::size_t i(0); i < column_count; ++i) {
        if (sqlite3_column_name(stmt.get(), i) == column_name)
            return i;
    }
    assert(false && "invalid column name provided");
    throw error("invalid column name: " + column_name);
}

}   // namespace internal
}   // namespace sqlite
