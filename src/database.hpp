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

#include "mem/memory.hpp"
#include <string>

struct sqlite3;

namespace sqlite {

static char const *temporary = "";
static char const *in_memory = ":memory:";

enum access_mode {
    read_only         = 0x01,
    read_write        = 0x02,
    read_write_create = 0x06
};

class database {
public:
    database(const std::string &path, const access_mode &permissions);
    database(const database &other) = delete;
    database(database &&other) = default;
    ~database();

    database& operator=(const database &other) = delete;
    database& operator=(database &&other) = default;

    result execute(const std::string &sql);
    result execute(const statement &statement);

    statement prepare_statement(const std::string &sql) const;

    friend std::ostream& operator<<(std::ostream &stream, const database &db);

private:
    void close();
    std::shared_ptr<sqlite3_stmt> create_statement(const std::string &sql) const;

private:
    sqlite3 *db = nullptr;
};

} // namespace sqlite

#endif // SQLITE_DATABASE_H
