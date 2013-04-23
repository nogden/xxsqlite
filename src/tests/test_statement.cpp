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

#include <sqlite3.h>

#include <gtest/gtest.h>

#include "mem/memory.h"

class statement: public testing::Test {
protected:
    void SetUp() {
        db = std::make_unique<sqlite::database>(
            sqlite::in_memory, sqlite::read_write_create
        );
        (void) db->execute(
            "CREATE TABLE test("
            "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    name TEXT"
            ");"
        );
        (void) db->execute("INSERT INTO test (id, name) VALUES (1, 'test');");
    }

    std::unique_ptr<sqlite::database> db;
};

TEST_F(statement, binds_any_parameter_when_name_is_present_in_sql_string) {
    auto statement(db->prepare_statement("SELECT * FROM test WHERE id = :param;"));
    EXPECT_NO_THROW(statement.bind(":param", static_cast<double>(0.0)));
    EXPECT_NO_THROW(statement.bind(":param", static_cast<int>(0)));
    EXPECT_NO_THROW(statement.bind(":param", static_cast<int64_t>(0)));
    EXPECT_NO_THROW(statement.bind(":param", sqlite::null));
    EXPECT_NO_THROW(statement.bind(":param", "string"));
}

TEST_F(statement, throws_database_error_when_binding_to_absent_parameter) {
    sqlite::statement statement(db->prepare_statement("SELECT * FROM test;"));
    EXPECT_THROW(
        statement.bind(":param", static_cast<double>(0.0)),
        sqlite::bad_parameter
    );
    EXPECT_THROW(
        statement.bind(":param", static_cast<int>(0)),
        sqlite::bad_parameter
    );
    EXPECT_THROW(
        statement.bind(":param", static_cast<int64_t>(0)),
        sqlite::bad_parameter
    );
    EXPECT_THROW(
        statement.bind(":param", sqlite::null),
        sqlite::bad_parameter
    );
    EXPECT_THROW(
        statement.bind(":param", "string"),
        sqlite::bad_parameter
    );
}

TEST_F(statement, can_count_its_parameters) {
    auto statement(db->prepare_statement("SELECT * FROM test WHERE id = :id;"));
    EXPECT_EQ(1, statement.parameter_count());
    statement = db->prepare_statement(
        "SELECT * FROM test WHERE id = :id AND name = :name;"
    );
    EXPECT_EQ(2, statement.parameter_count());
}
