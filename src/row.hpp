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

#ifndef SQLITE_ROW_H
#define SQLITE_ROW_H

#include "field.hpp"

#include <memory>
#include <cstddef>

struct sqlite3_stmt;

namespace sqlite {

class row
{
public:
    row(const std::shared_ptr<sqlite3_stmt> &statement);
    row(const row &other) = default;
    row(row &&other) = default;

    row& operator=(const row &other) = default;
    row& operator=(row &&other) = default;

    std::size_t column_count() const;

    field operator[](const std::string &column_name) const;
    field operator[](const std::size_t &column_index) const;

private:
    bool is_valid_index(const std::size_t &index) const;

private:
    std::shared_ptr<sqlite3_stmt> stmt;
};

} // namespace sqlite

#endif // SQLITE_ROW_H
