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

#include "referee_stats.hpp"
#include <utils.hpp>

namespace feature {
namespace details {

void referee_stats(player_cit ref_begin, player_cit ref_end,
                   std::vector<double>::const_iterator speed_it,
                   std::vector<double>& features) {
    // default values
    double x = feature::default_value();
    double y = feature::default_value();
    double speed = feature::default_value();

    // if there is a referee
    if (ref_begin != ref_end) {
        x = ref_begin->x;
        y = ref_begin->y;
        speed = *speed_it;
    }

    features[name_to_index("refX")] = x;
    features[name_to_index("refY")] = y;
    features[name_to_index("refSpeed")] = speed;
}

}; // namespace details
}; // namespace feature
