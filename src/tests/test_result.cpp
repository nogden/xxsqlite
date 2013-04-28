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
#include "result.hpp"
#include "error.hpp"

#include <gtest/gtest.h>

#include <mem/memory.hpp>

class result: public testing::Test {
protected:
    void SetUp() {
        db = std::make_unique<sqlite::database>(
            sqlite::in_memory, sqlite::read_write_create
        );
        db->execute(
            "CREATE TABLE test("
            "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    name TEXT"
            ");"
        );
        db->execute("INSERT INTO test (id, name) VALUES (1, 'testing');");
        db->execute("INSERT INTO test (id, name) VALUES (2, 'result');");
        db->execute("INSERT INTO test (id, name) VALUES (3, 'map');");
    }

    std::unique_ptr<sqlite::database> db;
};

TEST_F(result, states_no_rows_modified_on_pure_query) {
    sqlite::result results(db->execute("SELECT id, name FROM test;"));
    EXPECT_EQ(0, results.row_modification_count());
}

TEST_F(result, gives_number_of_rows_changed_on_a_modifying_statement) {
    sqlite::result one(db->execute(
        "UPDATE test SET name = 'test' WHERE id = 1;"
    ));
    EXPECT_EQ(1, one.row_modification_count());
    sqlite::result three(db->execute(
        "UPDATE test SET name = 'test';"
    ));
    EXPECT_EQ(3, three.row_modification_count());
}

TEST_F(result, gives_same_iterator_for_begin_and_end_with_empty_data_set) {
    sqlite::result results(db->execute(
        "SELECT id, name FROM test WHERE id = 100;"
    ));
    auto start(results.begin());
    auto end(results.end());
    EXPECT_TRUE(start == end);
}

TEST_F(result, gives_different_iterator_for_begin_and_end_when_data_present) {
    sqlite::result results(db->execute("SELECT id, name FROM test;"));
    auto start(results.begin());
    auto end(results.end());
    EXPECT_NE(start, end);
}

TEST_F(result, begin_equals_end_after_one_increment_per_returned_row) {
    sqlite::result results(db->execute("SELECT id, name FROM test;"));
    auto it(results.begin());
    EXPECT_FALSE(it == results.end());
    EXPECT_FALSE(++it == results.end());
    EXPECT_FALSE(++it == results.end());
    EXPECT_TRUE(++it == results.end());
}

TEST_F(result, asserts_or_throws_out_of_range_on_attempt_to_increment_past_end) {
    sqlite::result results(
        db->execute("SELECT id, name FROM test WHERE id = 1;")
    );
    auto it(results.begin());
    EXPECT_NO_THROW(++it);
    EXPECT_DEBUG_DEATH(++it, "");
}
