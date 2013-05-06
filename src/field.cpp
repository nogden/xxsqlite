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

#include "field.hpp"

#include <sqlite3.h>

#include <cassert>

namespace sqlite {

class blob {};

field::field(
        const std::shared_ptr<sqlite3_stmt> &statement,
        const std::size_t &parameter_index
): stmt(statement), index(parameter_index) {
    assert(statement && "received null sqlite3_stmt");
}

bool field::is_null() const {
    return sqlite3_column_type(stmt.get(), index) == SQLITE_NULL;
}

field::operator bool() const {
    return ! is_null();
}

std::string field::column_name() const {
    const char *name(sqlite3_column_name(stmt.get(), index));
    return name ? name : "";
}

template<>
blob field::as<blob>() const {
    assert(false && "blob support not implemented");
    return blob();
}

template<>
double field::as<double>() const {
    if (is_null())
        return 0.0;
    return sqlite3_column_double(stmt.get(), index);
}

template<>
int field::as<int>() const {
    if (is_null())
        return 0;
    return sqlite3_column_int(stmt.get(), index);
}

template<>
bool field::as<bool>() const {
    if (is_null())
        return false;
    return static_cast<bool>(sqlite3_column_int(stmt.get(), index));
}

template<>
int64_t field::as<int64_t>() const {
    if (is_null())
        return 0;
    return sqlite3_column_int64(stmt.get(), index);
}

template<>
std::size_t field::as<std::size_t>() const {
    if (is_null())
        return 0;
    return sqlite3_column_int64(stmt.get(), index);
}

template<>
char field::as<char>() const {
    if (is_null())
        return '\0';
    return *reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), index));
}

template<>
std::string field::as<std::string>() const {
    if (is_null())
        return "";
    return reinterpret_cast<const char*>(sqlite3_column_text(stmt.get(), index));
}

}   // namespace sqlite
