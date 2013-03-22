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

#include "statement.h"

#include <sqlite3.h>

#include <ostream>
#include <sstream>
#include <cassert>

namespace sqlite {

int find_parameter_index(sqlite3_stmt *stmt, const std::string &parameter) {
    assert(stmt && "null sqlite3_stmt provided");
    auto index(sqlite3_bind_parameter_index(stmt, parameter.c_str()));
    if (! index) {
        std::stringstream ss;
        ss << "unknown parameter '" << parameter << "' in sql statment '"
           << sqlite3_sql(stmt) << "'";
        throw database_error{ss.str()};
    }
    return index;
}

void throw_on_bind_error(const int status, const std::string &parameter) {
    if (status != SQLITE_OK) {
        std::stringstream ss;
        ss << error_message(status) << " while binding parameter '"
           << parameter << "'";
        throw database_error{ss.str()};
    }
}

statement::statement(sqlite3_stmt *statement): stmt{statement} {
    assert(statement && "attempt to create statement with null sqlite3_stmt");
}

statement::~statement() {
    (void) sqlite3_finalize(stmt);
}

void statement::bind(const std::string &parameter, const blob &value) {
    assert(false && "blob support not implemented");
}

void statement::bind(const std::string &parameter, const double &value) {
    auto index(find_parameter_index(stmt, parameter));
    throw_on_bind_error(sqlite3_bind_double(stmt, index, value), parameter);
}

void statement::bind(const std::string &parameter, const int &value) {
    auto index(find_parameter_index(stmt, parameter));
    throw_on_bind_error(sqlite3_bind_int(stmt, index, value), parameter);
}

void statement::bind(const std::string &parameter, const int64_t &value) {
    auto index(find_parameter_index(stmt, parameter));
    throw_on_bind_error(sqlite3_bind_int64(stmt, index, value), parameter);
}

void statement::bind(const std::string &parameter, const null_t &value) {
    (void) value;
    auto index(find_parameter_index(stmt, parameter));
    throw_on_bind_error(sqlite3_bind_null(stmt, index), parameter);
}

void statement::bind(const std::string &parameter, const std::string &value) {
    auto index(find_parameter_index(stmt, parameter));
    auto status(sqlite3_bind_text(
        stmt, index, value.c_str(), value.size(), SQLITE_STATIC
    ));
    throw_on_bind_error(status, parameter);
}

void statement::clear_bindings() {
    assert(stmt && "clear_bindings() called with null sqlite3_stmt");
    if (sqlite3_clear_bindings(stmt) != SQLITE_OK) {
        std::stringstream ss;
        ss << error_message(stmt) << " while clearing bindings";
        throw database_error{ss.str()};
    }
}

result statement::execute() {
    assert(stmt && "execute() called with null sqlite3_stmt");
    (void) sqlite3_reset(stmt);
    return {stmt, ownership::none};
}

std::size_t statement::parameter_count() const {
    return sqlite3_bind_parameter_count(stmt);
}

std::ostream& operator<<(std::ostream &os, const statement &statement) {
    os << "statement:\n"
          "  sql: " << sqlite3_sql(statement.stmt) << "\n";
    return os;
}

} // namespace sqlite
