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

#include <catch/catch.hpp>

#include <vector>

#include <feature/constants.hpp>
#include <feature/player.hpp>
#include <feature/row.hpp>

using namespace feature;
using namespace feature::details;

TEST_CASE("Test Row::Row", "[Row::Row]") {
    Row r1;
    r1.match_id = r1.timestamp = r1.half = r1.minute = r1.second = -1;
    r1.players = std::vector<Player>();

    CHECK(r1.match_id == -1);
    CHECK(r1.timestamp == -1);
    CHECK(r1.half == -1);
    CHECK(r1.minute == -1);
    CHECK(r1.second == -1);
    CHECK(r1.players == std::vector<Player>());
}

TEST_CASE("Test Row equality operators", "[Row::operator==]") {
    Row r;
    r.match_id = r.timestamp = r.half = r.minute = r.second = -1;
    r.players = std::vector<Player>();

    REQUIRE(r == Row());
}
