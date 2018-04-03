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

#include <vector>

#include <catch/catch.hpp>

#include <feature/computer.hpp>

TEST_CASE("Test Computer::compute_features", "[compute_features]") {
    feature::Computer fc;

    SECTION("Test Row with no players in it: All values should be -1") {
        feature::Row row;
        std::vector<double> features = fc.compute_features(row);

        std::vector<double> expected(feature::num_features(), -1);
        REQUIRE(features == expected);
    }

    // TODO: Test a Row with player type values set to -1.
    SECTION("Test Row with some players having type == -1: They sholdn't be "
            "counted on any statistic") {}
}
