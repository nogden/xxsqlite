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

#include "statement.hpp"
#include "error.hpp"

#include <sqlite3.h>

#include <ostream>
#include <sstream>
#include <cassert>

namespace sqlite {

namespace {

std::size_t find_parameter_index(
        const std::string &parameter,
        const std::shared_ptr<sqlite3_stmt> &stmt
) {
    assert(stmt && "bind() called on null sqlite::statement");
    auto index(sqlite3_bind_parameter_index(stmt.get(), parameter.c_str()));
    if (! index)
        throw bad_parameter(parameter, stmt);
    return index;
}

}

bad_parameter::bad_parameter(
        const std::string &parameter,
        const std::shared_ptr<sqlite3_stmt> &stmt
): bad_statement(stmt), param(parameter) {}

const char *bad_parameter::what() const noexcept {
    std::stringstream ss;
    ss << "unknown parameter '" << param << "' in sql statement '"
       << sql() << "'";
    return ss.str().c_str();
}

bind_error::bind_error(
        const std::string &parameter,
        const std::shared_ptr<sqlite3_stmt> &stmt
): bad_parameter(parameter, stmt) {}

const char *bind_error::what() const noexcept {
    std::stringstream ss;
    ss << error::what() << " while binding parameter '" << parameter()
       << "' in sql statement '" << sql() << "'";
    return ss.str().c_str();
}

statement::statement() {}

statement::statement(const std::shared_ptr<sqlite3_stmt> &statement):
        stmt(statement) {
    assert(statement && "attempt to create statement with null sqlite3_stmt");
}

statement::operator bool() const {
    return bool(stmt);
}

bool statement::is_valid() const {
    return bool(stmt);
}

void statement::bind(const std::string &parameter, const blob &value) {
    assert(false && "blob support not implemented");
}

void statement::bind(const std::string &parameter, const double &value) {
    auto index(find_parameter_index(parameter, stmt));
    throw_on_bind_error(sqlite3_bind_double(stmt.get(), index, value), parameter);
}

void statement::bind(const std::string &parameter, const int &value) {
    auto index(find_parameter_index(parameter, stmt));
    throw_on_bind_error(sqlite3_bind_int(stmt.get(), index, value), parameter);
}

void statement::bind(const std::string &parameter, const int64_t &value) {
    auto index(find_parameter_index(parameter, stmt));
    throw_on_bind_error(sqlite3_bind_int64(stmt.get(), index, value), parameter);
}

void statement::bind(const std::string &parameter, const null_t &value) {
    (void) value;
    auto index(find_parameter_index(parameter, stmt));
    throw_on_bind_error(sqlite3_bind_null(stmt.get(), index), parameter);
}

void statement::bind(const std::string &parameter, const std::string &value) {
    auto index(find_parameter_index(parameter, stmt));
    auto status(sqlite3_bind_text(
        stmt.get(), index, value.c_str(), value.size(), SQLITE_STATIC
    ));
    throw_on_bind_error(status, parameter);
}

void statement::clear_bindings() {
    assert(stmt && "clear_bindings() called on null sqlite::statement");
    auto status(sqlite3_clear_bindings(stmt.get()));
    if (status != SQLITE_OK)
        throw error(status);
}

std::size_t statement::parameter_count() const {
    assert(stmt && "parameter_count() called on null sqlite::statement");
    return sqlite3_bind_parameter_count(stmt.get());
}

void statement::throw_on_bind_error(
        const int status,
        const std::string &parameter
) const {
    if (status != SQLITE_OK)
        throw bind_error(parameter, stmt);
}

std::ostream& operator<<(std::ostream &os, const statement &statement) {
    os << "statement:\n"
          "  sql: " << sqlite3_sql(statement.stmt.get()) << "\n";
    return os;
}

result make_result(const statement &statement) {
    assert(statement && "execute() called with null sqlite::statement");
    (void) sqlite3_reset(statement.stmt.get());
    return statement.stmt;
}

} // namespace sqlite
