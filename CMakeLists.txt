# Builds the sqlite wrapper library

set(SQLITE_SOURCE_FILES
    database.h
    database.cpp
    statement.h
    statement.cpp
)

add_library(sqlite STATIC
    ${SQLITE_SOURCE_FILES}
)

target_link_libraries(sqlite
    libsqlite3
)
