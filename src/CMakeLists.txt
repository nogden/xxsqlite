# Builds the sqlite wrapper library

set(SQLITE_SOURCE_FILES
    database.hpp
    database.cpp
    error.hpp
    error.cpp
    field.hpp
    field.cpp
    result.hpp
    result.cpp
    row.hpp
    row.cpp
    statement.hpp
    statement.cpp
)

add_library(sqlite STATIC
    ${SQLITE_SOURCE_FILES}
)

target_link_libraries(sqlite
    sqlite3
)

if(BUILD_UNIT_TESTS)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(tests)
endif()
