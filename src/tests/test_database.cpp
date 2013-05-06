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

#include "database.hpp"
#include "statement.hpp"
#include "error.hpp"

#include <gtest/gtest.h>

TEST(database, executes_valid_sql_sucessfully) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    EXPECT_NO_THROW({
        (void) db.execute("CREATE TABLE test (id INTEGER PRIMARY KEY);");
        (void) db.execute("SELECT * FROM test;");
    });
}

TEST(database, throws_database_error_when_executing_invalid_sql) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    EXPECT_THROW(db.execute("INVALID STATEMENT"), sqlite::error);
}

TEST(database, returns_prepared_statement_when_given_valid_sql) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    EXPECT_NO_THROW(
        auto statement(db.prepare_statement(
            "CREATE TABLE test (id INTEGER PRIMARY KEY);"
        ));
    );
}

TEST(database, throws_database_error_when_preparing_invalid_sql) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    EXPECT_THROW(
        (void) db.prepare_statement("INVALID STATEMENT"),
        sqlite::error
    );
}

TEST(database, can_be_used_to_execute_transactions) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    (void) db.execute("CREATE TABLE test (id INTEGER, value TEXT);");
    (void) db.execute("INSERT INTO test (id, value) VALUES (1, '');");
    EXPECT_NO_THROW(
        sqlite::as_transaction(db, [](sqlite::database &db) {
            (void) db.execute("UPDATE test SET value = 'new' WHERE id = 1;");
        });
    );
}

TEST(database, throws_if_an_external_modification_occurs_during_a_transaction) {
    sqlite::database db(
        sqlite::in_memory, sqlite::read_write_create, sqlite::shared_cache
    );
    (void) db.execute("CREATE TABLE test (id INTEGER, value TEXT);");
    (void) db.execute("INSERT INTO test (id, value) VALUES (1, '');");
    EXPECT_THROW(
        sqlite::as_transaction(db, [](sqlite::database &db) {
            (void) db.execute("UPDATE test SET value = 'one' WHERE id = 1;");
            sqlite::database db2(
                sqlite::in_memory, sqlite::read_write, sqlite::shared_cache
            );
            (void) db2.execute("UPDATE test SET value = 'two' WHERE id = 1;");
            (void) db.execute("UPDATE test SET value = 'one' WHERE id = 1;");
        }), sqlite::transaction_failed
    );
}

TEST(database, can_execute_scalar_queries_returning_the_result_immediately) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    (void) db.execute("CREATE TABLE test (id INTEGER, value TEXT);");
    (void) db.execute("INSERT INTO test (id, value) VALUES (1, '');");
    EXPECT_EQ(1, db.execute_scalar<int>("SELECT id FROM test;"));
    sqlite::statement statement(db.prepare_statement(
        "SELECT count(id) FROM test;"
    ));
    EXPECT_EQ(1, db.execute_scalar<int>(statement));
}

TEST(database, can_report_its_size) {
    sqlite::database db(sqlite::in_memory, sqlite::read_write_create);
    (void) db.execute("CREATE TABLE test (id INTEGER, value TEXT);");
    (void) db.execute("INSERT INTO test (id, value) VALUES (1, '');");
    std::size_t page_size(db.execute_scalar<std::size_t>("PRAGMA page_size;"));
    std::size_t page_count(db.execute_scalar<std::size_t>("PRAGMA page_count;"));
    EXPECT_EQ(page_size * page_count, db.size());
}
