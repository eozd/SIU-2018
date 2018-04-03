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
#include <feature/stats/linearity_stats.hpp>

using namespace feature;
using namespace feature::details;

TEST_CASE("Test linearity_stats::linearity_stats", "[linearity_stats]") {

    SECTION("Multiple players") {
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
            linearity_stats(players.begin(), players.end(), features);

            REQUIRE(features[name_to_index("playerVerticalLinearity")] != -10);
        }
    }

    SECTION("Less than 4 players") {
        std::vector<double> features = default_features();

        SECTION("3") {
            std::vector<Player> players{
                Player(742, 15),
                Player(176, 715),
                Player(72, 48),
            };
            linearity_stats(players.begin(), players.end(), features);
            REQUIRE(features[name_to_index("playerVerticalLinearity")] ==
                    default_value());
        }

        SECTION("2") {
            std::vector<Player> players{
                Player(742, 15),
                Player(72, 48),
            };
            linearity_stats(players.begin(), players.end(), features);
            REQUIRE(features[name_to_index("playerVerticalLinearity")] ==
                    default_value());
        }

        SECTION("1") {
            std::vector<Player> players{
                Player(742, 15),
            };
            linearity_stats(players.begin(), players.end(), features);
            REQUIRE(features[name_to_index("playerVerticalLinearity")] ==
                    default_value());
        }

        SECTION("0") {
            std::vector<Player> players;
            linearity_stats(players.begin(), players.end(), features);
            REQUIRE(features[name_to_index("playerVerticalLinearity")] ==
                    default_value());
        }
    }
}

TEST_CASE("Test for linearity_stats::max_vertical_linearity",
          "[linearity_stats::max_vertical_linearity]") {

    SECTION("Multiple clusters") {
        dkm_point_seq<1> points{
            {47.856751},   {78.91993306}, {65.03668249}, {62.24491594},
            {74.52460286}, {34.86547034}, {38.44695801}, {55.57280045},
            {39.29078589}, {51.87140041}, {70.5286887},  {59.88527591},
            {45.19611276}, {28.62813627}, {42.1894136},  {76.86539827},
            {41.76808337}, {33.04974881}, {34.4765347},  {53.57296666},
            {61.59429962}, {40.72074205}, {38.95572506}, {67.05412306},
            {57.9748812},  {48.14945745}, {75.24596968}, {54.86167832},
            {49.63678676}, {67.35966344}, {62.10649622}, {71.43551803},
            {41.63822999}, {47.70705743}, {34.44363385}, {19.42462858},
            {51.33036516}, {77.12891585}, {61.61398219}, {37.78016614},
            {36.42854033}, {47.00144774}, {62.89054927}, {45.32659582},
            {45.4635398},  {41.17942255}, {77.41246838}, {48.40249516},
            {41.06456852}, {34.36725415}};
        dkm_point_seq<1> centroids{
            {38.1665}, {62.7760}, {24.0263}, {49.4249}, {75.2576},
        };
        dkm_label_seq labels{3, 4, 1, 1, 4, 0, 0, 3, 0, 3, 4, 1, 3, 2, 0, 4, 0,
                             0, 0, 3, 1, 0, 0, 1, 1, 3, 4, 3, 3, 1, 1, 4, 0, 3,
                             0, 2, 3, 4, 1, 0, 0, 3, 1, 3, 3, 0, 4, 3, 0, 0};
        dkm_means<1> means{centroids, labels};

        double res = max_vertical_linearity(means, points);
        REQUIRE(res == Approx(3.126984).epsilon(1e-6));
    }

    SECTION("Missing clusters") {
        dkm_point_seq<1> points{
            {47.856751},
            {78.91993306},
            {65.03668249},
        };
        dkm_point_seq<1> centroids{
            {38.1665},
            {24.0263},
            {49.4249},
            {75.2576},
        };
        dkm_label_seq labels{3, 1, 1};
        dkm_means<1> means{centroids, labels};

        double res = max_vertical_linearity(means, points);
        REQUIRE(res == Approx(0.036495).epsilon(1e-6));
    }

    SECTION("One player cluster") {
        dkm_point_seq<1> points{
            {78.91993306},
            {65.03668249},
        };
        dkm_point_seq<1> centroids{
            {78.91993306},
            {65.03668249},
        };
        dkm_label_seq labels{0, 1};
        dkm_means<1> means{centroids, labels};

        double res = max_vertical_linearity(means, points);
        REQUIRE(res == Approx(0).epsilon(1e-6));
    };

    SECTION("Single cluster") {

        dkm_point_seq<1> points{
            {47.856751},
            {78.91993306},
            {65.03668249},
        };
        dkm_point_seq<1> centroids{
            {38.1665},
        };
        dkm_label_seq labels{0, 0, 0};
        dkm_means<1> means{centroids, labels};

        double res = max_vertical_linearity(means, points);
        REQUIRE(res == Approx(0.073613).epsilon(1e-6));
    }
}
