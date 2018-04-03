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

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "avg_min_max_stats.hpp"
#include <feature/constants.hpp>

namespace feature {
namespace details {

void avg_min_max_stats(player_cit begin, player_cit end,
                       const std::string& prefix,
                       std::vector<double>& features) {
    double avg_x = 0, avg_y = 0;
    double size = static_cast<double>(std::distance(begin, end));

    // if no players
    if (begin == end) {
        avg_x = avg_y = feature::default_value();
    }

    for (auto it = begin; it != end; ++it) {
        double x = it->x;
        double y = it->y;

        avg_x += x / size;
        avg_y += y / size;
    }

    features[name_to_index(prefix + "AvgX")] = avg_x;
    features[name_to_index(prefix + "AvgY")] = avg_y;
}

}; // namespace details
}; // namespace feature
