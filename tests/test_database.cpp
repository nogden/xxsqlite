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

#include <gtest/gtest.h>

TEST(database, ignores_request_to_open_when_already_open) {
    sqlite::database db(sqlite::in_memory);
    db.open(sqlite::read_write_create);
    EXPECT_NO_THROW({
        db.open(sqlite::read_write_create);
        db.open(sqlite::read_write);
        db.open(sqlite::read_only);
    });
}

TEST(database, ignores_request_to_close_when_already_closed) {
    sqlite::database db(sqlite::in_memory);
    EXPECT_NO_THROW({
        db.close();
        db.close();
        db.close();
    });
}

TEST(database, can_make_prepared_statement) {
    sqlite::database db(sqlite::in_memory);
    db.open(sqlite::read_write_create);
    EXPECT_NO_THROW({
        sqlite::statement stmt(db.make_statement(
            "CREATE TABLE test (id INTEGER PRIMARY KEY AUTOINCREMENT);"
        ));
    });
}

TEST(database, throws_exception_on_invalid_statement) {
    sqlite::database db(sqlite::in_memory);
    db.open(sqlite::read_write_create);
    EXPECT_THROW(
        sqlite::statement stmt(db.make_statement("INVALID STATEMENT")),
        sqlite::database_error
    );
}

TEST(database, triggers_assert_on_attempt_to_access_closed_database) {
    sqlite::database db(sqlite::in_memory);
    EXPECT_DEATH({
        sqlite::statement stmt(db.make_statement(
            "CREATE TABLE test (id INTEGER PRIMARY KEY AUTOINCREMENT);"
        ));
    }, "");
}
