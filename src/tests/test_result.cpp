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
#include "result.h"
#include "error.h"

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
