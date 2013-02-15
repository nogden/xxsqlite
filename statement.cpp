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

#include <sstream>
#include <cassert>

namespace sqlite {

int find_parameter_index(sqlite3_stmt *stmt, std::string const &parameter) {
    int index(sqlite3_bind_parameter_index(stmt, parameter.c_str()));
    if (! index) {
        std::stringstream ss;
        ss << "unknown parameter '" << parameter << "' in sql statment '"
           << sqlite3_sql(stmt) << "'";
        throw  unknown_parameter(ss.str());
    }
    return index;
}

void throw_on_error(int status, std::string const &parameter) {
    if (status != SQLITE_OK) {
        std::stringstream ss;
        ss << sqlite3_errstr(status) << " while binding parameter '"
           << parameter << "'";
        throw bind_failed(ss.str());
    }
}

statement::statement(sqlite3_stmt *statement): stmt(statement) {}

statement::statement(statement &&other) {
    assert(&other != this);
    std::swap(stmt, other.stmt);
}

statement::~statement() {
    sqlite3_finalize(stmt); // Return value is irrelevant
}

statement& statement::operator=(statement &&other) {
    assert(&other != this);
    std::swap(stmt, other.stmt);
    return *this;
}

void statement::bind(std::string const &parameter, blob const &value) {
    assert(false && "blob support not implemented");
}

void statement::bind(std::string const &parameter, double value) {
    assert(stmt && "null sqlite3_stmt provided");
    int index(find_parameter_index(stmt, parameter));
    throw_on_error(sqlite3_bind_double(stmt, index, value), parameter);
}

void statement::bind(std::string const &parameter, int value) {
    assert(stmt && "null sqlite3_stmt provided");
    int index(find_parameter_index(stmt, parameter));
    throw_on_error(sqlite3_bind_int(stmt, index, value), parameter);
}

void statement::bind(std::string const &parameter, int64_t value) {
    assert(stmt && "null sqlite3_stmt provided");
    int index(find_parameter_index(stmt, parameter));
    throw_on_error(sqlite3_bind_int64(stmt, index, value), parameter);
}

void statement::bind(std::string const &parameter, null_value value) {
    assert(stmt && "null sqlite3_stmt provided");
    int index(find_parameter_index(stmt, parameter));
    throw_on_error(sqlite3_bind_null(stmt, index), parameter);
}

void statement::bind(std::string const &parameter, std::string const &value) {
    assert(stmt && "null sqlite3_stmt provided");
    int index(find_parameter_index(stmt, parameter));
    throw_on_error(
        sqlite3_bind_text(
            stmt, index, value.c_str(), value.size(), SQLITE_STATIC
        ),
        parameter
    );
}

void statement::bind(std::string const &parameter, std::wstring const &value) {
    assert(stmt && "null sqlite3_stmt provided");
    int index(find_parameter_index(stmt, parameter));
    throw_on_error(
        sqlite3_bind_text16(
            stmt, index, value.c_str(), value.size(), SQLITE_STATIC
        ),
        parameter
    );
}

std::ostream & operator<<(std::ostream &os, statement const &statement) {
    os << "statement: (display not implemented)\n";
    return os;
}

} // namespace sqlite
