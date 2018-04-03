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

#include <vector>

#include <feature/constants.hpp>
#include <feature/row.hpp>

namespace feature {
namespace details {

/**
 * @brief Calculate speed of each Player in current Row object and return it as
 * a vector.
 *
 * Speed of a player is calculate as \f$\frac{dx}{dt}\f$ and returned as
 * metres/second.
 *
 * Players are identified by their id field. Therefore, speed of a player that
 * is in the current row but not in previous row is be calculated. Similarly,
 * speed of a player that is not in current Row is not calculated.
 *
 * @param curr Current feature::Row object.
 * @param prev Previous feature::Row object.
 *
 * @return Speed of each player in current Row, in the same order. If the player
 * doesn't exist in the previous Row, his/her speed is set to
 * feature::default_value().
 */
std::vector<double> calculate_speeds(const Row& curr, const Row& prev);

}; // namespace details
}; // namespace feature
