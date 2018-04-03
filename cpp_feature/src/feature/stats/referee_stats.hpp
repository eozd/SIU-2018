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

namespace feature {
namespace details {
/**
 * @brief Calculate features related to referee position, speed.
 *
 * @param ref_begin Player iterator pointing to the referee.
 * @param ref_end Player iterator pointing to one-past element of referee.
 * @param speed_it Iterator pointing to the speed of referee.
 * @param features features vector to write the calculated feature in-place.
 */
void referee_stats(player_cit ref_begin, player_cit ref_end,
                   std::vector<double>::const_iterator speed_it,
                   std::vector<double>& features);
}; // namespace details
}; // namespace feature
