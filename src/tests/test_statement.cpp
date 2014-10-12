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

#include "mem/memory.hpp"

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
        for (int i(0); i < 5; ++i) {
            std::stringstream ss;
            ss << "INSERT INTO test (id, name) VALUES (" << i << ", 'test" << i << "');";
            (void) db->execute(ss.str());
        }
    }

    std::unique_ptr<sqlite::database> db;
};

TEST_F(statement, binds_any_parameter_when_name_is_present_in_sql_string) {
    auto statement(db->prepare_statement("SELECT * FROM test WHERE id = :param;"));
    EXPECT_NO_THROW(statement.bind(":param", double(0.0)));
    EXPECT_NO_THROW(statement.bind(":param", int(0)));
    EXPECT_NO_THROW(statement.bind(":param", int64_t(0)));
    EXPECT_NO_THROW(statement.bind(":param", true));
    EXPECT_NO_THROW(statement.bind(":param", sqlite::null));
    EXPECT_NO_THROW(statement.bind(":param", "string"));
    EXPECT_NO_THROW(statement.bind(":param", std::string("string")));
}

TEST_F(statement, throws_database_error_when_binding_to_absent_parameter) {
    sqlite::statement statement(db->prepare_statement("SELECT * FROM test;"));
    EXPECT_THROW(statement.bind(":param", double(0.0)), sqlite::error);
    EXPECT_THROW(statement.bind(":param", int(0)), sqlite::error);
    EXPECT_THROW(statement.bind(":param", int64_t(0)), sqlite::error);
    EXPECT_THROW(statement.bind(":param", sqlite::null), sqlite::error);
    EXPECT_THROW(statement.bind(":param", "string"), sqlite::error);
    EXPECT_THROW(statement.bind(":param", std::string("string")), sqlite::error);
}

TEST_F(statement, can_count_its_parameters) {
    auto statement(db->prepare_statement("SELECT * FROM test WHERE id = :id;"));
    EXPECT_EQ(1, statement.parameter_count());
    statement = db->prepare_statement(
        "SELECT * FROM test WHERE id = :id AND name = :name;"
    );
    EXPECT_EQ(2, statement.parameter_count());
}

TEST_F(statement, can_be_executed_with_more_than_one_bind_sets) {
    auto statement(db->prepare_statement("SELECT * FROM test WHERE id = :id;"));
    for (int i(0); i < 5; ++i) {
        statement.bind(":id", i);
        auto result(db->execute(statement));
        sqlite::row row(*std::begin(result));
        std::stringstream expected;
        expected << "test" << i;
        EXPECT_EQ(row["name"].as<std::string>(), expected.str());
    }
}
