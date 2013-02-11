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

#include <cassert>

namespace sqlite {

statement::statement(sqlite3_stmt *statement): stmt(statement) {
    assert(statement && "null sqlite3_stmt received");
}

statement& statement::operator=(statement &&other) {
    std::swap(stmt, other.stmt);
    return *this;
}

statement::~statement() {
    sqlite3_finalize(stmt); // Return value is irrelevant
}

statement::statement(statement &&other) {
    std::swap(stmt, other.stmt);
}

std::ostream & operator<<(std::ostream &os, statement const &statement) {
    os << "statement: (display not implemented)\n";
    return os;
}

} // namespace sqlite
