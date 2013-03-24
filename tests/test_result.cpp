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

#include "sqlite/database.h"
#include "sqlite/result.h"
#include "sqlite/error.h"

#include <gtest/gtest.h>

class result: public testing::Test {
protected:
    void SetUp() {
        db = sqlite::make_database(sqlite::in_memory, sqlite::read_write_create);
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

TEST_F(result, can_be_move_constructed) {
    sqlite::result from(db->execute("SELECT * FROM test;"));
    EXPECT_NO_THROW(sqlite::result to(std::move(from)));
}

TEST_F(result, can_be_move_assigned) {
    sqlite::result from(db->execute("SELECT * FROM test;"));
    EXPECT_NO_THROW(sqlite::result to = std::move(from));
}

TEST_F(result, can_count_number_of_columns_in_results) {
    sqlite::result results(db->execute("SELECT id FROM test;"));
    EXPECT_EQ(1, results.column_count());
    results = db->execute("SELECT id, name FROM test;");
    EXPECT_EQ(2, results.column_count());
    results = db->execute("SELECT * FROM test;");
    EXPECT_EQ(2, results.column_count());
}

TEST_F(result, can_find_name_of_column) {
    sqlite::result results(db->execute("SELECT id, name FROM test;"));
    EXPECT_EQ("id", results.column_name(0));
    EXPECT_EQ("name", results.column_name(1));
    EXPECT_EQ("", results.column_name(2));
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
