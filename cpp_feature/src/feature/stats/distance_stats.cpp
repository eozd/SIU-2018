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

#include <string>
#include <vector>

#include "distance_stats.hpp"
#include <utils.hpp>

namespace feature {
namespace details {

void distance_stats(player_cit begin, player_cit end, const std::string& prefix,
                    std::vector<double>& features) {
    double inner_dist = 0;

    // if no players, write the default value
    if (begin == end) {
        inner_dist = feature::default_value();
    }

    // sum of distances of all possible pairs
    for (auto left = begin; left != end; ++left) {
        for (auto right = std::next(left); right != end; ++right) {
            inner_dist += dist(left->x, left->y, right->x, right->y);
        }
    }

    features[name_to_index(prefix + "InnerDistance")] = inner_dist;
}

}; // namespace details
}; // namespace feature
