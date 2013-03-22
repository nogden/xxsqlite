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

#include "result.h"

#include <sqlite3.h>

#include <sstream>
#include <cassert>

namespace sqlite {

namespace {

bool step_result(sqlite3_stmt *stmt) {
    assert(stmt && "attempt to step null sqlite3_stmt");
    auto status(sqlite3_step(stmt));
    if (status != SQLITE_ROW && status != SQLITE_DONE) {
        std::stringstream ss;
        ss << error_message(stmt)
           << " while executing sql statement '" << sqlite3_sql(stmt) << "'";
        throw database_error(ss.str());
    }
    return status == SQLITE_DONE;
}

}

result::result(sqlite3_stmt *statement, const ownership &ownership):
        stmt(statement), owns_statement(ownership == ownership::take) {
    assert(statement && "null sqlite3_stmt provided");
    end_reached = step_result(stmt);
}

result::result(result &&other) {
    assert(&other != this && "attempt to move into self");
    replace_members_with(other);
}

result::~result() {
    if (owns_statement) {
        (void) sqlite3_finalize(stmt);
    }
}

result& result::operator=(result &&other) {
    assert(&other != this && "attempt to move into self");
    if (owns_statement)
        (void) sqlite3_finalize(stmt);
    replace_members_with(other);
    return *this;
}

std::size_t result::column_count() const {
    return sqlite3_column_count(stmt);
}

std::string result::column_name(const std::size_t &column_index) const {
    const char *name(sqlite3_column_name(stmt, column_index));
    return name ? name : "";
}

result::const_iterator result::begin() const {
    return {stmt, end_reached ? iterator_pos::end : iterator_pos::element};
}

result::const_iterator result::end() const {
    return {stmt, iterator_pos::end};
}

void result::replace_members_with(result &other) {
    stmt = other.stmt;
    owns_statement = other.owns_statement;
    other.stmt = nullptr;
    other.owns_statement = false;
}

const char* error_message(const int status) {
    return sqlite3_errstr(status);
}

const char* error_message(sqlite3_stmt *statement) {
    return sqlite3_errmsg(sqlite3_db_handle(statement));
}

result::const_iterator::const_iterator(
        sqlite3_stmt *statement,
        const iterator_pos &position
): stmt(statement), at_end(position == iterator_pos::end) {
    assert(statement && "null sqlite3_stmt provided");
}

bool result::const_iterator::operator==(
        const result::const_iterator &other
) const {
    return stmt == other.stmt && at_end == other.at_end;
}

bool result::const_iterator::operator!=(
        const result::const_iterator &other
) const {
    return ! (*this == other);
}

result::const_iterator& result::const_iterator::operator++() {
    at_end = step_result(stmt);
    return *this;
}

} // namespace sqlite
