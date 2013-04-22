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
#include "row.h"
#include "field.h"

#include <gtest/gtest.h>

#include <string>
#include <algorithm>

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
