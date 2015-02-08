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

#ifndef SQLITE_DATABASE_H
#define SQLITE_DATABASE_H

#include "statement.hpp"
#include "result.hpp"

#include <memory>
#include <functional>

struct sqlite3;

namespace sqlite {

static char const *temporary = "";

enum class special_t {
    in_memory
};
static const special_t in_memory = special_t::in_memory;

enum access_mode {
    read_only         = 0x01,
    read_write        = 0x02,
    read_write_create = 0x06
};

enum cache_type {
    shared_cache  = 0x00020000,
    private_cache = 0x00040000
};

class database {
public:
    database(
            const std::string &path,
            const access_mode &permissions,
            const cache_type &visibility = private_cache
    );
    database(
            const special_t &in_memory,
            const access_mode &permissions,
            const cache_type &visibility = private_cache
    );
    database(const database &other) = delete;
    database(database &&other) = default;
    ~database();

    database& operator=(const database &other) = delete;
    database& operator=(database &&other) = default;

    statement prepare_statement(const std::string &sql) const;

    result execute(const std::string &sql);
    result execute(const statement &statement);
    template<typename T> T execute_scalar(const std::string &sql) const {
        result results(create_statement(sql));
        return (*results.begin())[0].as<T>();
    }
    template<typename T> T execute_scalar(const statement &statement) const {
        result results(make_result(statement));
        return (*results.begin())[0].as<T>();
    }

    std::size_t size() const;

    friend std::ostream& operator<<(std::ostream &stream, const database &db);

private:
    void close() noexcept;
    std::shared_ptr<sqlite3_stmt> create_statement(const std::string &sql) const;

private:
    sqlite3 *db = nullptr;
};

void as_transaction(
        database &db,
        const std::function<void(database &)> &operations
);

} // namespace sqlite

#endif // SQLITE_DATABASE_H
