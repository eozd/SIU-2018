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

#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

#include <feature/player.hpp>
#include <feature/stats/avg_min_max_stats.hpp>

using namespace feature;
using namespace feature::details;

TEST_CASE("Test avg_min_max_stats::avg_min_max_stats", "[avg_min_max_stats]") {
    SECTION("Check for multiple players") {
        std::vector<Player> players{
            Player(47.856751, 50.21719257),   Player(78.91993306, 41.80005636),
            Player(65.03668249, 86.17347712), Player(62.24491594, 36.20383972),
            Player(74.52460286, 83.89700268), Player(34.86547034, 85.08598683),
            Player(38.44695801, 56.66566638), Player(55.57280045, 50.92501345),
            Player(39.29078589, 53.79380756), Player(51.87140041, 53.40142365),
            Player(70.5286887, 60.90101162),  Player(59.88527591, 33.05821799),
            Player(45.19611276, 48.68597413), Player(28.62813627, 51.64829857),
            Player(42.1894136, 46.20926605),  Player(76.86539827, 89.25654071),
            Player(41.76808337, 70.93262098), Player(33.04974881, 38.24531401),
            Player(34.4765347, 46.58323849),  Player(53.57296666, 56.31221508),
            Player(61.59429962, 52.23203429), Player(40.72074205, 24.51448248),
            Player(38.95572506, 49.67590415), Player(67.05412306, 79.61388429),
            Player(57.9748812, 57.46543173),  Player(48.14945745, 58.09576134),
            Player(75.24596968, 41.88262766), Player(54.86167832, 42.92866898),
            Player(49.63678676, 51.74923262), Player(67.35966344, 52.98992491),
            Player(62.10649622, 75.20483762), Player(71.43551803, 72.75522062),
            Player(41.63822999, 32.07477572), Player(47.70705743, 61.78910396),
            Player(34.44363385, 76.81714104), Player(19.42462858, 65.70971118),
            Player(51.33036516, 93.8770833),  Player(77.12891585, 67.27877204),
            Player(61.61398219, 64.71030961), Player(37.78016614, 43.46304084),
            Player(36.42854033, 48.79140442), Player(47.00144774, 49.39476455),
            Player(62.89054927, 58.06340305), Player(45.32659582, 50.09541902),
            Player(45.4635398, 39.79481855),  Player(41.17942255, 41.46653397),
            Player(77.41246838, 47.37504291), Player(48.40249516, 56.7820991),
            Player(41.06456852, 81.65928904), Player(34.36725415, 63.12446966)};
        std::vector<double> features(num_features(), -10);

        SECTION("Check whether the results are written to correct indices") {
            std::vector<std::string> prefixes{"home", "away"};
            for (const auto& prefix : prefixes) {
                avg_min_max_stats(players.begin(), players.end(), prefix,
                                  features);

                REQUIRE(features[name_to_index(prefix + "AvgX")] != -10);
                REQUIRE(features[name_to_index(prefix + "AvgY")] != -10);
            }
        }

        SECTION("Check for correct results") {
            std::string prefix = "home";
            avg_min_max_stats(players.begin(), players.end(), prefix, features);

            REQUIRE(features[name_to_index(prefix + "AvgX")] ==
                    Approx(51.6097));
            REQUIRE(features[name_to_index(prefix + "AvgY")] ==
                    Approx(56.8274));
        }
    }

    SECTION("Check for a single player") {
        const std::string prefix = "home";
        std::vector<Player> players{Player(123, 456)};
        std::vector<double> features(num_features(), -10);
        avg_min_max_stats(players.begin(), players.end(), prefix, features);

        REQUIRE(features[name_to_index(prefix + "AvgX")] == 123);
        REQUIRE(features[name_to_index(prefix + "AvgY")] == 456);
    }

    SECTION("Check for no players") {
        const std::string prefix = "away";
        std::vector<Player> players;
        std::vector<double> features(num_features(), -10);
        REQUIRE_NOTHROW(avg_min_max_stats(players.begin(), players.end(),
                                          prefix, features));

        REQUIRE(features[name_to_index(prefix + "AvgX")] ==
                feature::default_value());
        REQUIRE(features[name_to_index(prefix + "AvgY")] ==
                feature::default_value());
    }
}
