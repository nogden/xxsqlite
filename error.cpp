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

#include "error.h"

#include <sqlite3.h>

#include <sstream>
#include <cassert>

namespace sqlite {

namespace {

std::string error_message(const int status) { return sqlite3_errstr(status); }

std::string error_message(sqlite3 *db) { return sqlite3_errmsg(db); }

std::string error_message(sqlite3_stmt *stmt) {
    sqlite3 *db(sqlite3_db_handle(stmt));
    std::stringstream ss;
    ss << error_message(db) << " while executing sql statement '"
       << sqlite3_sql(stmt) << "'";
    return ss.str();
}

const int error_code(sqlite3* db) { return sqlite3_errcode(db); }

const int error_code(sqlite3_stmt *statement) {
    sqlite3 *db(sqlite3_db_handle(statement));
    return sqlite3_errcode(db);
}

}

error::error(const int error_code):
    std::runtime_error(error_message(error_code)), code(error_code) {}


bad_statement::bad_statement(sqlite3_stmt *stmt):
    error(error_code(stmt)), sql_statement(sqlite3_sql(stmt)) {}

bad_statement::bad_statement(const int error_code, const std::string &sql):
    error(error_code), sql_statement(sql) {}

const char* bad_statement::what() const noexcept {
    std::stringstream ss;
    ss << error::what() << " with sql statement '" << sql() << "'";
    return ss.str().c_str();
}


bad_parameter::bad_parameter(
        const std::string &parameter,
        sqlite3_stmt *stmt
): bad_statement(stmt), param(parameter) {}

const char *bad_parameter::what() const noexcept {
    std::stringstream ss;
    ss << "unknown parameter '" << param << "' in sql statement '"
       << sql() << "'";
    return ss.str().c_str();
}


bind_error::bind_error(const std::string &parameter, sqlite3_stmt *stmt):
    bad_parameter(parameter, stmt) {}

const char *bind_error::what() const noexcept {
    std::stringstream ss;
    ss << error::what() << " while binding parameter '" << parameter()
       << "' in sql statement '" << sql() << "'";
    return ss.str().c_str();
}



} // namespace sqlite
