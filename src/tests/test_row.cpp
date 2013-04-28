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
#include "row.hpp"
#include "field.hpp"

#include <gtest/gtest.h>

#include <string>
#include <algorithm>
#include <mem/memory.hpp>

class row: public testing::Test {
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
        (void) db->execute("INSERT INTO test (id, name) VALUES (2, 'sqlite');");
        (void) db->execute("INSERT INTO test (id, name) VALUES (3, 'row');");
    }

    sqlite::row make_row(const std::string &sql) {
        sqlite::result result(db->execute(sql));
        return *result.begin();
    }

    std::unique_ptr<sqlite::database> db;
};

TEST_F(row, can_count_number_of_columns_in_results) {
    sqlite::row row(make_row("SELECT id FROM test;"));
    EXPECT_EQ(1, row.column_count());
    row = make_row("SELECT id, name FROM test;");
    EXPECT_EQ(2, row.column_count());
    row = make_row("SELECT * FROM test;");
    EXPECT_EQ(2, row.column_count());
}

TEST_F(row, provides_corresponding_field_when_given_column_name) {
    sqlite::row row(make_row("SELECT * FROM test;"));
    EXPECT_EQ(1, row["id"].as<int>());
    EXPECT_EQ("test", row["name"].as<std::string>());
}

TEST_F(row, provides_corresponding_field_when_given_column_index) {
    sqlite::row row(make_row("SELECT * FROM test;"));
    EXPECT_EQ(1, row[0].as<int>());
    EXPECT_EQ("test", row[1].as<std::string>());
}

TEST_F(row, throws_error_when_asked_for_non_existing_column_name) {
    sqlite::row row(make_row("SELECT * FROM test;"));
    EXPECT_DEBUG_DEATH(row["bad_column"], "");
}

TEST_F(row, throws_error_when_asked_for_non_existing_column_id) {
    sqlite::row row(make_row("SELECT * FROM test;"));
    EXPECT_DEBUG_DEATH(row[5], "");
}

TEST_F(row, can_be_used_in_a_range_based_for_loop) {
    sqlite::result results(db->execute("SELECT * FROM test;"));
    for (const sqlite::row &row : results) {
        (void) row;
    }
}

TEST_F(row, can_be_used_in_a_std_for_each_function) {
    sqlite::result results(db->execute("SELECT * FROM test;"));
    std::for_each(results.begin(), results.end(), [](const sqlite::row &row) {
        (void) row;
    });
}
