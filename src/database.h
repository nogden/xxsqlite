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

#ifndef SQLITE_DATABASE_H
#define SQLITE_DATABASE_H

#include "statement.h"

#include "mem/memory.h"
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
    database(sqlite3 *connection);
    database(const database &other) = delete;
    database(database &&other) = default;
    ~database();

    database& operator=(const database &other) = delete;
    database& operator=(database &&other) = default;

    result execute(const std::string &sql);

    std::unique_ptr<statement> make_statement(const std::string &sql) const;

    friend std::ostream& operator<<(std::ostream &stream, const database &db);

private:
    void close();
    std::shared_ptr<sqlite3_stmt> create_statement(const std::string &sql) const;

private:
    sqlite3 *db = nullptr;
};

std::unique_ptr<database> make_database(
        const std::string &path,
        const access_mode &permissions
);

} // namespace sqlite

#endif // SQLITE_DATABASE_H
