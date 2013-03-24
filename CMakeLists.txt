# Builds the sqlite wrapper library

set(SQLITE_SOURCE_FILES
    database.h
    database.cpp
    error.h
    error.cpp
    result.h
    result.cpp
    row.h
    row.cpp
    statement.h
    statement.cpp
)

add_library(sqlite STATIC
    ${SQLITE_SOURCE_FILES}
)

target_link_libraries(sqlite
    sqlite3
)

if(BUILD_UNIT_TESTS)
    add_subdirectory("tests")
endif()
