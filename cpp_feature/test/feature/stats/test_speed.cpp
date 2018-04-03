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

#include <feature/player.hpp>
#include <feature/stats/speed.hpp>

using namespace feature;
using namespace feature::details;

TEST_CASE("Test feature::details::calculate_speeds", "[calculate_speeds]") {

    SECTION("Returned vector is the same size as curr.players") {
        Row curr;
        Row prev;

        auto speeds = calculate_speeds(curr, prev);
        REQUIRE(speeds.empty());

        curr.players = std::vector<Player>(10);
        speeds = calculate_speeds(curr, prev);
        REQUIRE(speeds.size() == 10);
    }

    SECTION("Players that are in curr but not in prev get assigned the "
            "default_val") {
        Row curr;
        Player p1;
        p1.id = 22;
        Player p2;
        p2.id = 5;
        curr.players = {p1, p2};

        Row prev;
        prev.players = {p1};

        auto speeds = calculate_speeds(curr, prev);
        REQUIRE(speeds[1] == feature::default_value());
    }

    SECTION("Check for correct speed values") {
        const int dt = 100; // ms
        Row curr;
        curr.players = {
            Player(47.856751, 50.21719257),   Player(78.91993306, 41.80005636),
            Player(65.03668249, 86.17347712), Player(62.24491594, 36.20383972),
            Player(74.52460286, 83.89700268), Player(34.86547034, 85.08598683),
            Player(38.44695801, 56.66566638), Player(55.57280045, 50.92501345),
        };
        std::vector<int> curr_ids{1, 5, 11, 23, 49, 30, 9, 2};
        for (size_t i = 0; i < curr.players.size(); ++i)
            curr.players[i].id = curr_ids[i];

        Row prev;
        prev.players = {
            Player(39.29078589, 53.79380756), Player(51.87140041, 53.40142365),
            Player(70.5286887, 60.90101162),  Player(59.88527591, 33.05821799),
            Player(45.19611276, 48.68597413), Player(28.62813627, 51.64829857),
        };
        std::vector<int> prev_ids{11, 23, 2, 72, 5, 30};

        prev.timestamp = -1234678;
        curr.timestamp = prev.timestamp + dt;

        for (size_t i = 0; i < prev.players.size(); ++i)
            prev.players[i].id = prev_ids[i];

        double default_val = feature::default_value();
        std::vector<double> expected{default_val, 344.196443,  413.677917,
                                     200.839915,  default_val, 340.144576,
                                     default_val, 179.777399};

        std::vector<double> speeds = calculate_speeds(curr, prev);
        for (size_t i = 0; i < speeds.size(); ++i)
            REQUIRE(speeds[i] == Approx(expected[i]).epsilon(1e-6));
    }
}
