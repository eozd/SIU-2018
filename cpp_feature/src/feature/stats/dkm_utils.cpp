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

#include "dkm_utils.hpp"

namespace feature {
namespace details {

dkm_point_seq<2> players_to_points(player_cit begin, player_cit end) {
    dkm_point_seq<2> points(std::distance(begin, end));

    // point is created using x and y coordinates of a Player
    std::transform(begin, end, points.begin(),
                   [](const auto& player) -> dkm_point<2> {
                       return {player.x, player.y};
                   });
    return points;
}

}; // namespace details
}; // namespace feature
