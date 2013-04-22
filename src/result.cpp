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
#include "error.h"

#include <sqlite3.h>

#include <sstream>
#include <cassert>

namespace sqlite {

namespace {

bool step_result(const std::shared_ptr<sqlite3_stmt> &stmt) {
    assert(stmt && "attempt to step null sqlite3_stmt");
    auto status(sqlite3_step(stmt.get()));
    if (status != SQLITE_ROW && status != SQLITE_DONE)
        throw bad_statement(stmt);
    return status == SQLITE_DONE;
}

bool iterator_end(true);

}

result::result(const std::shared_ptr<sqlite3_stmt> &statement):
        stmt(statement) {
    assert(statement && "null sqlite3_stmt provided");
    end_reached = step_result(stmt);
}

result::result(result &&other) {
    assert(&other != this && "attempt to move into self");
    stmt = std::move(other.stmt);
    end_reached = other.end_reached;
}

result& result::operator=(result &&other) {
    assert(&other != this && "attempt to move into self");
    stmt = std::move(other.stmt);
    end_reached = other.end_reached;
    return *this;
}

std::size_t result::row_modification_count() const {
    sqlite3 *db(sqlite3_db_handle(stmt.get()));
    assert(db && "unable to aquire associated database");
    return sqlite3_stmt_readonly(stmt.get()) ? 0 : sqlite3_changes(db);
}

result::const_iterator result::begin() const {
    return {stmt, end_reached};
}

result::const_iterator result::end() const {
    return {stmt, iterator_end};
}

result::const_iterator::const_iterator(
        const std::shared_ptr<sqlite3_stmt> &statement,
        bool &at_end
): stmt(statement), end_reached(at_end), current_row(statement) {
    assert(statement && "null sqlite3_stmt provided");
}

bool result::const_iterator::operator==(
        const result::const_iterator &other
) const {
    return stmt == other.stmt && end_reached == other.end_reached;
}

bool result::const_iterator::operator!=(
        const result::const_iterator &other
) const {
    return ! (*this == other);
}

result::const_iterator& result::const_iterator::operator++() {
    assert(!end_reached && "attempt to increment past last result");
    end_reached = step_result(stmt);
    current_row = stmt;
    return *this;
}

const row& result::const_iterator::operator*() const {
    return current_row;
}

const row* result::const_iterator::operator->() const {
    return &current_row;
}

} // namespace sqlite
