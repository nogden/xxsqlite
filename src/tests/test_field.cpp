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

#include "field.h"
#include "database.h"

#include <gtest/gtest.h>

#include "mem/memory.h"

class field: public testing::Test {
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
