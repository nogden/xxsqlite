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

#include "row.hpp"
#include "error.hpp"

#include <sqlite3.h>

#include <cassert>

namespace sqlite {

namespace {

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
    throw error("invalid column name '" + column_name + "'");
}

}

row::row(const std::shared_ptr<sqlite3_stmt> &statement): stmt(statement) {
    assert(statement && "received null sqlite3_stmt");
}

std::size_t row::column_count() const {
    return sqlite3_column_count(stmt.get());
}

field row::operator[](const std::string &column_name) const {
    return {stmt, find_column_index(column_name, stmt)};
}

field row::operator[](const std::size_t &column_index) const {
    assert(is_valid_index(column_index) && "invalid column index requested");
    if (! is_valid_index(column_index))
        throw error("no column at index " + std::to_string(column_index));
    return {stmt, column_index};
}

bool row::is_valid_index(const std::size_t &index) const {
    return index < column_count();
}

} // namespace sqlite
