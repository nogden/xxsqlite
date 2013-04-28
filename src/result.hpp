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

#ifndef SQLITE_RESULT_H
#define SQLITE_RESULT_H

#include "row.hpp"
#include "error.hpp"

#include <memory>
#include <cstddef>

struct sqlite3_stmt;

namespace sqlite {

class transaction_failed: public error {
public:
    transaction_failed(const int status): error(status) {}
};

class result
{
public:
    class const_iterator {
    public:
        const_iterator(
                const std::shared_ptr<sqlite3_stmt> &statement,
                bool &at_end
        );
        const_iterator(const const_iterator &other) = delete;
        const_iterator(const_iterator &&other) = default;

        const_iterator& operator=(const const_iterator &other) = delete;
        const_iterator& operator=(const_iterator &&other) = default;

        bool operator==(const const_iterator &other) const;
        bool operator!=(const const_iterator &other) const;

        const_iterator& operator++();
        const row& operator*() const;
        const row* operator->() const;

    private:
        std::shared_ptr<sqlite3_stmt> stmt;
        bool &end_reached;
        row current_row;
    };

public:
    result(const std::shared_ptr<sqlite3_stmt> &statement);
    result(const result &other) = delete;
    result(result &&other);

    result& operator=(const result &other) = delete;
    result& operator=(result &&other);

    std::size_t row_modification_count() const;

    const_iterator begin() const;
    const_iterator end() const;

private:
    std::shared_ptr<sqlite3_stmt> stmt;
    mutable bool end_reached = false;
};

} // namespace sqlite

#endif // SQLITE_RESULT_H
