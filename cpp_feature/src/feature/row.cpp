/*
 * Copyright 2018 Esref Ozdemir
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "row.hpp"

namespace feature {

Row::Row()
    : match_id(-1), timestamp(-1), half(-1), minute(-1), second(-1), players() {
}

bool operator==(const Row& r1, const Row& r2) {
    return (r1.match_id == r2.match_id) && (r1.timestamp == r2.timestamp) &&
           (r1.half == r2.half) && (r1.minute == r2.minute) &&
           (r1.second == r2.second) && (r1.players == r2.players);
}

bool operator!=(const Row& r1, const Row& r2) { return !(r1 == r2); }

std::ostream& operator<<(std::ostream& os, const Row& r) {
    os << "Row(match_id=" << r.match_id << ", "
       << "timestamp=" << r.timestamp << ", "
       << "half=" << r.half << ", "
       << "minute=" << r.minute << ", "
       << "second=" << r.second << ", "
       << "players={";
    for (const auto& player : r.players) {
        os << "\n\t" << player;
    }
    os << "\n})";
    return os;
}

}; // namespace feature
