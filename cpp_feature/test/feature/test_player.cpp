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

#include <feature/constants.hpp>
#include <feature/player.hpp>
#include <utils.hpp>

using namespace feature;
using namespace feature::details;

TEST_CASE("Test Player equality operators", "[Player::operator==]") {
    Player p;
    p.type = p.id = p.jersey = p.x = p.y = -1;
    REQUIRE(p == Player());
}

TEST_CASE("Test Player::Player", "[Player::Player]") {
    Player p1;
    p1.type = p1.id = p1.jersey = p1.x = p1.y = -1;
    Player p2;
    CHECK(p1.type == p2.type);
    CHECK(p1.id == p2.id);
    CHECK(p1.jersey == p2.jersey);
    CHECK(close(p1.x, p2.x));
    CHECK(close(p1.y, p2.y));
}

TEST_CASE("Test Player::name_type", "[Player::name_type]") {
    CHECK(player_name_to_type("home") == 0);
    CHECK(player_name_to_type("away") == 1);
    CHECK(player_name_to_type("referee") == 2);
    CHECK(player_name_to_type("home_gk") == 3);
    CHECK(player_name_to_type("away_gk") == 4);
}
