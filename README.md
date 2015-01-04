++sqlite
========

++sqlite is thin C++ wrapper around the sqlite3 database library with a focus on
using the C++11 feature set.

Contributions and issue reports welcome!

Examples
--------
### Basic usage
    sqlite::database db("company.db", sqlite::read_write_create);
    db.execute(
        "CREATE TABLE employee ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL,"
            "role INTEGER NOT NULL"
        ");"
    );

### Transactions
    sqlite::as_transaction(db, [](sqlite::database &db) {
        db.execute("INSERT INTO employee (name, role) VALUES ('J. Smith', 1);");
        db.execute("INSERT INTO employee (name, role) VALUES ('A. Jones', 2);");
        db.execute("INSERT INTO employee (name, role) VALUES ('F. Bar', 2);");
    });

### Simple scalar queries
    std::size_t record_count(db.execute_scalar<std::size_t>(
        "SELECT count(name) FROM employee;"
    );

### Prepared statements
    sqlite::statement stmt(
        db.prepare_statement("SELECT * FROM employee WHERE role = :role;")
    );
    stmt.bind(":role", 2);
    sqlite::result results(db.execute(stmt));

### Stl compatible iterators
    for (const sqlite::row &row : results) {
        std::cout << row["name"].as<std::string>() << ": "
                  << row["role"].as<int>() << std::endl;
    }

++sqlite...
-----------
 * uses the latest C++11 techniques to ensure high performance, readable code.
 * is entirely stack based, making it extremely lightweight.
 * has a comprehensive set of unit tests to ensure its quality.
 * is released under the Apache 2.0 license allowing use in both proprietary and
   open-source software.

### Runtime dependencies
 * sqlite 3 (tested with version 3.7)

### Build dependencies
 * cmake 2.6 or later
 * A C++11 compatible compiler (tested with gcc-4.8 and clang-3.2)
 * Google Test (gtest) for building the unit tests

### Platform support
 * Linux    (reference platform)
 * Mac Os X (unconfirmed)
 * Windows  (unconfirmed)
 * Should build and run on any platform with a compatible C++11 compiler that is
   supported by sqlite3.
 * If you are able to confirm that the library builds and all unit tests pass on
   any platform, please get in touch.

### Build instructions
 1. git clone git://github.com/nogden/xxsqlite.git
 2. cd xxsqlite
 3. mkdir build
 4. cd build
 5. cmake ..
 6. make
 7. make test

Happy coding!
