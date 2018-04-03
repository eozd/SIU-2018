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
#include <feature/row.hpp>
#include <parser.hpp>

using namespace feature;
using namespace feature::details;

TEST_CASE("Test parser::parse_line", "[parser::parse_line]") {
    SECTION("Parse a regular line") {
        std::string line =
            "116001217	78392209	2	58	27	"
            "4955,0,3,52.94,19.45 5264,1,14,80.03,47.38 5844,1,6,76.54,33.21 "
            "4886,1,18,56.07,16.18 6116,6,0,91.82,68.8 4933,1,9,49.06,34.72 "
            "6080,1,11,55.06,56.11 6117,7,0,48,0.52 4934,0,37,49.27,31.62 "
            "6081,0,19,52.94,55.03 6118,1,-1,104.79,41.69 "
            "6079,0,89,72.26,43.93 5241,1,35,85.37,38.82 5827,3,1,10.94,34.52 "
            "6047,1,26,85.21,24.03 6089,1,17,74.45,44.34 5469,2,0,65.47,42.17 "
            "6012,0,7,75.38,26.2 5433,1,19,78.81,54.66 5757,0,39,88.01,30.63 "
            "5722,0,33,48.78,40.87 5990,0,8,61.65,32.3 5876,1,25,79.99,19.66 "
            "5840,0,28,83.62,41.41 6100,0,17,72.82,50.32 ";
        std::vector<Player> players{Player(4955, 0, 3, 52.94, 19.45),
                                    Player(5264, 1, 14, 80.03, 47.38),
                                    Player(5844, 1, 6, 76.54, 33.21),
                                    Player(4886, 1, 18, 56.07, 16.18),
                                    Player(6116, 6, 0, 91.82, 68.8),
                                    Player(4933, 1, 9, 49.06, 34.72),
                                    Player(6080, 1, 11, 55.06, 56.11),
                                    Player(6117, 7, 0, 48, 0.52),
                                    Player(4934, 0, 37, 49.27, 31.62),
                                    Player(6081, 0, 19, 52.94, 55.03),
                                    Player(6118, 1, -1, 104.79, 41.69),
                                    Player(6079, 0, 89, 72.26, 43.93),
                                    Player(5241, 1, 35, 85.37, 38.82),
                                    Player(5827, 3, 1, 10.94, 34.52),
                                    Player(6047, 1, 26, 85.21, 24.03),
                                    Player(6089, 1, 17, 74.45, 44.34),
                                    Player(5469, 2, 0, 65.47, 42.17),
                                    Player(6012, 0, 7, 75.38, 26.2),
                                    Player(5433, 1, 19, 78.81, 54.66),
                                    Player(5757, 0, 39, 88.01, 30.63),
                                    Player(5722, 0, 33, 48.78, 40.87),
                                    Player(5990, 0, 8, 61.65, 32.3),
                                    Player(5876, 1, 25, 79.99, 19.66),
                                    Player(5840, 0, 28, 83.62, 41.41),
                                    Player(6100, 0, 17, 72.82, 50.32)};
        Row row = parse_line(line);

        REQUIRE(row.match_id == 116001217);
        REQUIRE(row.timestamp == 78392209);
        REQUIRE(row.half == 2);
        REQUIRE(row.minute == 58);
        REQUIRE(row.second == 27);
        REQUIRE(row.players == players);
    }

    SECTION("Parse a line with no player data") {
        std::string line =
            "116001217	78392209	2	58	27	";
        Row row = parse_line(line);

        REQUIRE(row.match_id == 116001217);
        REQUIRE(row.timestamp == 78392209);
        REQUIRE(row.half == 2);
        REQUIRE(row.minute == 58);
        REQUIRE(row.second == 27);
        REQUIRE(row.players == std::vector<Player>());
    }
}
