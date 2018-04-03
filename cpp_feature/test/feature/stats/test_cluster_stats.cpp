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
#include <feature/stats/cluster_stats.hpp>

using namespace feature;
using namespace feature::details;

TEST_CASE("Test cluster_stats::cluster_stats", "[cluster_stats]") {

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

        std::vector<std::string> prefixes{"player", "home", "away"};
        for (const auto& prefix : prefixes) {
            cluster_stats(players.begin(), players.end(), prefix, features);

            REQUIRE(features[name_to_index(prefix + "DenseClusterDensity")] !=
                    -10);
            REQUIRE(features[name_to_index(prefix + "SparseClusterDensity")] !=
                    -10);
        }
    }

    SECTION("Two players") {
        std::vector<Player> players{Player(123, 456), Player(742, 456)};
        std::vector<double> features(num_features(), -10);

        std::vector<std::string> prefixes{"player", "home", "away"};
        for (const auto& prefix : prefixes) {
            cluster_stats(players.begin(), players.end(), prefix, features);

            REQUIRE(features[name_to_index(prefix + "DenseClusterDensity")] !=
                    -10);
            REQUIRE(features[name_to_index(prefix + "SparseClusterDensity")] !=
                    -10);
        }
    }

    SECTION("Single player") {
        std::vector<Player> players{Player(123, 456)};
        std::vector<double> features = default_features();

        std::vector<std::string> prefixes{"player", "home", "away"};
        for (const auto& prefix : prefixes) {
            cluster_stats(players.begin(), players.end(), prefix, features);

            REQUIRE(features[name_to_index(prefix + "DenseClusterDensity")] !=
                    -10);
            REQUIRE(features[name_to_index(prefix + "SparseClusterDensity")] !=
                    -10);
        }
    }

    SECTION("No players : Stats shouldn't change") {
        std::vector<Player> players;
        std::vector<double> features = default_features();

        std::vector<std::string> prefixes{"player", "home", "away"};
        for (const auto& prefix : prefixes) {
            cluster_stats(players.begin(), players.end(), prefix, features);

            REQUIRE(features[name_to_index(prefix + "DenseClusterDensity")] ==
                    default_value());
            REQUIRE(features[name_to_index(prefix + "SparseClusterDensity")] ==
                    default_value());
        }
    }
}

TEST_CASE("Test for cluster_stats::cluster_densities",
          "[cluster_stats::cluster_densities]") {

    SECTION("Multiple clusters") {
        dkm_point_seq<2> points{
            {47.856751, 50.21719257},   {78.91993306, 41.80005636},
            {65.03668249, 86.17347712}, {62.24491594, 36.20383972},
            {74.52460286, 83.89700268}, {34.86547034, 85.08598683},
            {38.44695801, 56.66566638}, {55.57280045, 50.92501345},
            {39.29078589, 53.79380756}, {51.87140041, 53.40142365},
            {70.5286887, 60.90101162},  {59.88527591, 33.05821799},
            {45.19611276, 48.68597413}, {28.62813627, 51.64829857},
            {42.1894136, 46.20926605},  {76.86539827, 89.25654071},
            {41.76808337, 70.93262098}, {33.04974881, 38.24531401},
            {34.4765347, 46.58323849},  {53.57296666, 56.31221508},
            {61.59429962, 52.23203429}, {40.72074205, 24.51448248},
            {38.95572506, 49.67590415}, {67.05412306, 79.61388429},
            {57.9748812, 57.46543173},  {48.14945745, 58.09576134},
            {75.24596968, 41.88262766}, {54.86167832, 42.92866898},
            {49.63678676, 51.74923262}, {67.35966344, 52.98992491},
            {62.10649622, 75.20483762}, {71.43551803, 72.75522062},
            {41.63822999, 32.07477572}, {47.70705743, 61.78910396},
            {34.44363385, 76.81714104}, {19.42462858, 65.70971118},
            {51.33036516, 93.8770833},  {77.12891585, 67.27877204},
            {61.61398219, 64.71030961}, {37.78016614, 43.46304084},
            {36.42854033, 48.79140442}, {47.00144774, 49.39476455},
            {62.89054927, 58.06340305}, {45.32659582, 50.09541902},
            {45.4635398, 39.79481855},  {41.17942255, 41.46653397},
            {77.41246838, 47.37504291}, {48.40249516, 56.7820991},
            {41.06456852, 81.65928904}, {34.36725415, 63.12446966}};
        dkm_point_seq<2> centroids{{48.6312, 48.7002}, {59.2690, 77.7259}};
        dkm_label_seq labels{0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1,
                             0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
                             1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
        dkm_means<2> means{centroids, labels};

        double max_density, min_density;
        std::tie(max_density, min_density) = cluster_densities(means, points);
        REQUIRE(max_density == Approx(1.065132).epsilon(1e-6));
        REQUIRE(min_density == Approx(0.549250).epsilon(1e-6));
    }

    SECTION("Non-existing clusters") {
        dkm_point_seq<2> points{
            {1, 2},
            {3, 4},
        };
        dkm_point_seq<2> centroids{{2, 3}, {5, 8}};
        dkm_label_seq labels{1, 1};
        dkm_means<2> means{centroids, labels};

        REQUIRE_NOTHROW(cluster_densities(means, points));
    }

    SECTION("Single cluster") {
        dkm_point_seq<2> points{
            {1, 2},
            {3, 4},
        };
        dkm_point_seq<2> centroids{{4, 5}, {2, 3}};
        dkm_label_seq labels{1, 1};
        dkm_means<2> means{centroids, labels};

        double max_density, min_density;
        std::tie(max_density, min_density) = cluster_densities(means, points);
        REQUIRE(max_density == Approx(1.414213).epsilon(1e-6));
        REQUIRE(min_density == Approx(1.414213).epsilon(1e-6));
    }

    SECTION("Cluster with a single point") {
        dkm_point_seq<2> points{{1, 2}};
        dkm_point_seq<2> centroids{{0, 7}, {4, 10}, {8, 3}, {1, 2}};
        dkm_label_seq labels{3};
        dkm_means<2> means{centroids, labels};

        double max_density, min_density;
        std::tie(max_density, min_density) = cluster_densities(means, points);
        REQUIRE(max_density == Approx(0).epsilon(1e-6));
        REQUIRE(min_density == Approx(0).epsilon(1e-6));
    }
}
