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
#include <feature/stats/referee_stats.hpp>

using namespace feature;
using namespace feature::details;

TEST_CASE("Test referee_stats::referee_stats", "[referee_stats]") {

    SECTION("Referee exists") {
        std::vector<Player> refs{Player(47.856751, 50.21719257)};
        std::vector<double> speed{5.12};
        int dt = 300;
        std::vector<double> features(num_features(), -10);

        SECTION("Check if the results are written to correct indices") {
            referee_stats(refs.begin(), refs.end(), speed.begin(), features);

            REQUIRE(features[name_to_index("refX")] != -10);
            REQUIRE(features[name_to_index("refY")] != -10);
            REQUIRE(features[name_to_index("refSpeed")] != -10);
        }

        SECTION("Check if X and Y values are correct") {
            referee_stats(refs.begin(), refs.end(), speed.begin(), features);

            REQUIRE(features[name_to_index("refX")] == Approx(47.856751));
            REQUIRE(features[name_to_index("refY")] == Approx(50.21719257));
        }

        SECTION("Check refSpeed values for different delta time values") {
            referee_stats(refs.begin(), refs.end(), speed.begin(), features);

            REQUIRE(features[name_to_index("refSpeed")] == speed[0]);
        }
    }

    SECTION("Referee doesn't exist") {
        std::vector<Player> refs;
        std::vector<double> speed;
        std::vector<double> features(num_features(), -10);

        referee_stats(refs.begin(), refs.end(), speed.begin(), features);

        REQUIRE(features[name_to_index("refX")] == feature::default_value());
        REQUIRE(features[name_to_index("refY")] == feature::default_value());
        REQUIRE(features[name_to_index("refSpeed")] ==
                feature::default_value());
    }
}
