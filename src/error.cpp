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

#include "error.hpp"

#include <sqlite3.h>

#include <sstream>
#include <cassert>

namespace sqlite {

namespace {

std::string error_message(const int status) { return sqlite3_errstr(status); }

std::string error_message(sqlite3 *db) { return sqlite3_errmsg(db); }

std::string error_message(const std::shared_ptr<sqlite3_stmt> &stmt) {
    sqlite3 *db(sqlite3_db_handle(stmt.get()));
    std::stringstream ss;
    ss << error_message(db) << " while executing sql statement '"
       << sqlite3_sql(stmt.get()) << "'";
    return ss.str();
}

const int error_code(sqlite3* db) { return sqlite3_errcode(db); }

const int error_code(const std::shared_ptr<sqlite3_stmt> &stmt) {
    sqlite3 *db(sqlite3_db_handle(stmt.get()));
    return sqlite3_errcode(db);
}

}

error::error(const int error_code, const std::string &msg):
    std::runtime_error(error_message(error_code) + " " + msg),
    code(error_code) {}

error::error(const std::string &msg): std::runtime_error(msg) {}

error::error(const std::shared_ptr<sqlite3_stmt> &stmt, const std::string &msg):
    std::runtime_error(error_message(stmt) + " " + msg) {}



} // namespace sqlite
