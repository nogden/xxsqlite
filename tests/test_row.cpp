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
#include "sqlite/row.h"
#include "sqlite/field.h"

#include <gtest/gtest.h>

class row: public testing::Test {
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
    }

    sqlite::row make_row() {
        sqlite::result result(db->execute("SELECT * FROM test;"));
        return *result.begin();
    }

    std::unique_ptr<sqlite::database> db;
};

TEST_F(row, provides_corresponding_field_when_given_column_name) {
//    sqlite::row row(make_row());
//    sqlite::field field(row["id"]);
//    EXPECT_EQ("test", field.as<std::string>());
}
