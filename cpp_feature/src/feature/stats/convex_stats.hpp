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

#pragma once

#include <string>
#include <vector>

#include <feature/constants.hpp>

namespace feature {
namespace details {

/**
 * @brief Calculate features obtained from the convex hull of players.
 *
 * This function calculates convex hull related features using all the Player
 * objects in the range given as [begin, end). To calculate the speed related
 * features, it also uses the speed of each player given as a range starting
 * at speed_begin.
 *
 * @param begin Beginning of Player range [begin, end).
 * @param end End of Player range [begin, end).
 * @param speed_begin Beginning of the range holding the speed of each Player.
 * @param prefix Prefix of the feature name. If prefix == "home", then the
 * calculated feature will be written to the index designated to
 * homeInnerDistance. Must be one of "home", "away" or "player".
 * @param features features vector to write the calculated feature in-place.
 */
void convex_stats(player_cit begin, player_cit end,
                  std::vector<double>::iterator speed_begin,
                  const std::string& prefix, std::vector<double>& features);

}; // namespace details
}; // namespace feature
