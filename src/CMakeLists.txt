# Builds the sqlite wrapper library

set(XXSQLITE_SOURCE_FILES
    database.cpp
    error.cpp
    field.cpp
    result.cpp
    row.cpp
    statement.cpp
)

add_library(++sqlite STATIC
    ${XXSQLITE_SOURCE_FILES}
)

include_directories(${SQLITE_INCLUDE_DIRS})

target_link_libraries(++sqlite
    ${SQLITE_LIBRARIES}
)

if(BUILD_UNIT_TESTS)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(tests)
endif()
