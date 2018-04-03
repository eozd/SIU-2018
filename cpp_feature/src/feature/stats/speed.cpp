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

#include <algorithm>
#include <vector>

#include <feature/constants.hpp>
#include <utils.hpp>

#include "speed.hpp"

namespace feature {
namespace details {

std::vector<double> calculate_speeds(const Row& curr, const Row& prev) {
    // time difference between curr and prev Row objects
    const int timediff_ms = curr.timestamp - prev.timestamp;
    const double timediff_sec = static_cast<double>(timediff_ms) / 1000;

    std::vector<double> speed(curr.players.size(), feature::default_value());
    for (size_t i = 0; i < curr.players.size(); ++i) {
        const auto& curr_player = curr.players[i];

        // try to find current player in previous Row
        auto it = std::find_if(prev.players.begin(), prev.players.end(),
                               [&curr_player](const Player& prev) {
                                   return curr_player.id == prev.id;
                               });

        // if player exists, update speed. If not, speed remains as
        // default_value.
        if (it != prev.players.end()) {
            speed[i] =
                dist(curr_player.x, curr_player.y, it->x, it->y) / timediff_sec;
        }
    }
    return speed;
}

}; // namespace details
}; // namespace feature
