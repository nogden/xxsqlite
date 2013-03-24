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
#include "sqlite/statement.h"
#include "sqlite/error.h"

#include <gtest/gtest.h>

#include "mem/memory.h"

class database: public testing::Test {
protected:
    void SetUp() {
        db = sqlite::make_database(sqlite::in_memory, sqlite::read_write_create);
    }

    std::unique_ptr<sqlite::database> db;
};

TEST_F(database, can_be_move_constructed) {
    EXPECT_NO_THROW(
        sqlite::database database{std::move(*db.release())}
    );
}

TEST_F(database, can_be_move_assigned) {
    EXPECT_NO_THROW(
        sqlite::database database = std::move(*db.release())
    );
}

TEST_F(database, asserts_when_created_with_null_pointer) {
    EXPECT_DEATH(
        sqlite::database database{nullptr},
        ""
    );
}

TEST_F(database, executes_valid_sql_sucessfully) {
    EXPECT_NO_THROW({
        (void) db->execute("CREATE TABLE test (id INTEGER PRIMARY KEY);");
        (void) db->execute("SELECT * FROM test;");
    });
}

TEST_F(database, throws_database_error_when_executing_invalid_sql) {
    EXPECT_THROW(db->execute("INVALID STATEMENT"), sqlite::bad_statement);
}

TEST_F(database, returns_prepared_statement_when_given_valid_sql) {
    EXPECT_NO_THROW(
        auto statement{db->make_statement(
            "CREATE TABLE test (id INTEGER PRIMARY KEY);"
        )};
    );
}

TEST_F(database, throws_database_error_when_given_invalid_sql) {
    EXPECT_THROW(
        db->make_statement("INVALID STATEMENT"),
        sqlite::bad_statement
    );
}
