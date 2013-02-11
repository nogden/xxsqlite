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

#include <string>
#include <vector>
#include <ostream>
#include <stdexcept>

struct sqlite3;

namespace sqlite {

enum access_mode {
    read_only,
    read_write,
    read_write_create
};

class database_error: public std::runtime_error {
public:
    database_error(std::string const &msg): std::runtime_error(msg) {}
};

class database {
public:
    database(std::string const &path);
    ~database();

    void open(access_mode const &mode);
    void close();

    statement make_statement(std::string const &statement);

    friend std::ostream& operator<<(std::ostream &stream, database const &db);

private:
    sqlite3 **db = nullptr;
    std::string path;
};

} // namespace sqlite

#endif // SQLITE_DATABASE_H
