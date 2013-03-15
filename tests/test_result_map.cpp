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

#include <gtest/gtest.h>

class result_map: public testing::Test {
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

TEST_F(result_map, can_be_move_constructed) {
    sqlite::result_map from(db->execute("SELECT * FROM test;"));
    EXPECT_NO_THROW(sqlite::result_map to(std::move(from)));
}

TEST_F(result_map, can_be_move_assigned) {
    sqlite::result_map from(db->execute("SELECT * FROM test;"));
    EXPECT_NO_THROW(sqlite::result_map to = std::move(from));
}

TEST_F(result_map, can_count_number_of_columns_in_results) {
    sqlite::result_map results(db->execute("SELECT id FROM test;"));
    EXPECT_EQ(1, results.column_count());
    results = db->execute("SELECT id, name FROM test;");
    EXPECT_EQ(2, results.column_count());
    results = db->execute("SELECT * FROM test;");
    EXPECT_EQ(2, results.column_count());
}

TEST_F(result_map, can_find_name_of_column) {
    sqlite::result_map results(db->execute("SELECT id, name FROM test;"));
    EXPECT_EQ("id", results.column_name(0));
    EXPECT_EQ("name", results.column_name(1));
    EXPECT_EQ("", results.column_name(2));
}
