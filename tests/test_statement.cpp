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

#include <sqlite3.h>

#include <gtest/gtest.h>

#include "mem/memory.h"

class statement: public testing::Test {
protected:
    void SetUp() {
        db = sqlite::make_database(sqlite::in_memory, sqlite::read_write_create);
        db->execute("CREATE TABLE test (id INTEGER PRIMARY KEY AUTOINCREMENT);");
    }

    std::unique_ptr<sqlite::database> db;
};

TEST_F(statement, binds_any_parameter_when_name_is_present_in_sql_string) {
    auto statement(db->make_statement("SELECT * FROM test WHERE id = @param;"));
    EXPECT_NO_THROW(statement->bind("@param", static_cast<double>(0.0)));
    EXPECT_NO_THROW(statement->bind("@param", static_cast<int>(0)));
    EXPECT_NO_THROW(statement->bind("@param", static_cast<int64_t>(0)));
    EXPECT_NO_THROW(statement->bind("@param", sqlite::null));
    EXPECT_NO_THROW(statement->bind("@param", "string"));
}

TEST_F(statement, throws_exception_when_binding_to_non_existant_parameter) {
    auto statement(db->make_statement("SELECT * FROM test;"));
    EXPECT_THROW(
        statement->bind("@param", static_cast<double>(0.0)),
        sqlite::database_error
    );
    EXPECT_THROW(
        statement->bind("@param", static_cast<int>(0)),
        sqlite::database_error
    );
    EXPECT_THROW(
        statement->bind("@param", static_cast<int64_t>(0)),
        sqlite::database_error
    );
    EXPECT_THROW(
        statement->bind("@param", sqlite::null),
        sqlite::database_error
    );
    EXPECT_THROW(
        statement->bind("@param", "string"),
        sqlite::database_error
    );
}
