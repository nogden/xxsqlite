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
