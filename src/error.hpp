/*
   Copyright (C) 2013  Nick Ogden <nick@nickogden.org>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef SQLITE_ERROR_H
#define SQLITE_ERROR_H

#include <memory>
#include <string>
#include <stdexcept>

struct sqlite3;
struct sqlite3_stmt;

namespace sqlite {

class error: public std::runtime_error {
public:
    error(const int error_code, const std::string &msg = std::string());
    error(const std::string &msg);
    error(
            const std::shared_ptr<sqlite3_stmt> &stmt,
            const std::string &msg = std::string()
    );

    const int sqlite_error_code() const noexcept { return code; }

private:
    const int code = 0;
};

} // namespace sqlite

#endif // SQLITE_ERROR_H
