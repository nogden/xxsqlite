++sqlite
--------

++sqlite is thin C++ wrapper around the sqlite3 database library with a focus on
using the C++11 feature set.

Contributions and issue reports welcome!

Basic Example


Prepared statements


Simple scalar queries


Stl compatible iterators


Transactions



++sqlite...
...uses the latest C++11 techniques to ensure high performance, readable code.
...is entirely stack based, making it extremely lightweight.
...has a comprehensive set of unit tests to ensure its quality.
...is released under the Apache 2.0 license allowing use in both proprietary and
   open-source software.

Runtime dependencies
 * sqlite 3 (tested with version 3.7)

Build dependencies
 * cmake 2.6 or later
 * A C++11 compatible compiler (tested with gcc-4.8 and clang-3.2)

Platform support
 * Linux    (reference platform)
 * Mac Os X (unconfirmed)
 * Windows  (unconfirmed)
 * Should build and run on any platform with a compatible C++11 compiler that is
   supported by sqlite3.
 * If you are able to confirm that the library builds and all unit tests pass on
   any platform, please get in touch.

Build instructions
 1. git clone https://github.com/nogden/xxsqlite.git
 2. cd xxsqlite
 3. mkdir build
 4. cd build
 5. cmake ..
 6. make
 7. make test

Happy coding!
