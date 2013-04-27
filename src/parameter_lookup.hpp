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

#ifndef SQLITE_PARAMETER_LOOKUP_H
#define SQLITE_PARAMETER_LOOKUP_H

#include "mem/memory.hpp"
#include <string>

struct sqlite3_stmt;

namespace sqlite {
namespace internal {

std::size_t find_parameter_index(
        const std::string &parameter,
        const std::shared_ptr<sqlite3_stmt> &stmt
);

std::size_t find_column_index(
        const std::string &column_name,
        const std::shared_ptr<sqlite3_stmt> &stmt
);

}   // namespace internal
}   // namespace sqlite

#endif // SQLITE_PARAMETER_LOOKUP_H
