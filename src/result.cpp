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

#include "result.hpp"

#include <sqlite3.h>

#include <cassert>

namespace sqlite {

namespace {

bool step_result(const std::shared_ptr<sqlite3_stmt> &stmt) {
    assert(stmt && "attempt to step null sqlite3_stmt");
    auto status(sqlite3_step(stmt.get()));
    switch (status) {
    case SQLITE_DONE:   return true;
    case SQLITE_ROW:    return false;
    case SQLITE_LOCKED: // fall-through
    case SQLITE_BUSY:   throw transaction_failed(status);
    default:            throw error(stmt);
    }
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
