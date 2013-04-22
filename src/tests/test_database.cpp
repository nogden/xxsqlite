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

#include "database.h"
#include "statement.h"
#include "error.h"

#include <gtest/gtest.h>

#include "mem/memory.h"

TEST(database, executes_valid_sql_sucessfully) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    EXPECT_NO_THROW({
        (void) db.execute("CREATE TABLE test (id INTEGER PRIMARY KEY);");
        (void) db.execute("SELECT * FROM test;");
    });
}

TEST(database, throws_database_error_when_executing_invalid_sql) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    EXPECT_THROW(db.execute("INVALID STATEMENT"), sqlite::bad_statement);
}

TEST(database, returns_prepared_statement_when_given_valid_sql) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    EXPECT_NO_THROW(
        auto statement(db.prepare_statement(
            "CREATE TABLE test (id INTEGER PRIMARY KEY);"
        ));
    );
}

TEST(database, throws_database_error_when_given_invalid_sql) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    EXPECT_THROW(
        db.prepare_statement("INVALID STATEMENT"),
        sqlite::bad_statement
    );
}
