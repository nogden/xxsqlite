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

#include "field.h"
#include "database.h"

#include <gtest/gtest.h>

class field: public testing::Test {
protected:
    void SetUp() {
        db = sqlite::make_database(sqlite::in_memory, sqlite::read_write_create);
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

TEST_F(field, knows_the_name_of_its_source_column) {
    sqlite::row row(make_row("SELECT id, name FROM test;"));
    EXPECT_EQ("id", row["id"].column_name());
    EXPECT_EQ("name", row["name"].column_name());
}

TEST_F(field, can_be_tested_for_null) {
    (void) db->execute("INSERT INTO test (id) VALUES (4);");
    sqlite::row row(make_row("SELECT * FROM test WHERE id = 4;"));
    EXPECT_TRUE(row["name"].is_null());
    EXPECT_FALSE(row["id"].is_null());
}

TEST_F(field, supports_cxx11_explicit_bool_syntax) {
    (void) db->execute("INSERT INTO test (id) VALUES (4);");
    sqlite::row row(make_row("SELECT * FROM test WHERE id = 4;"));
    EXPECT_TRUE(bool(row["id"]));
    EXPECT_FALSE(bool(row["name"]));
}
