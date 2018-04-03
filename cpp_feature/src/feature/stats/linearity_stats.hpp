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

#include "dkm_utils.hpp"

namespace feature {
namespace details {

/**
 * @brief Calculate features related to linearity of players.
 *
 * This function calculate playerVerticalLinearity.
 *
 * @param begin Beginning of Player range [begin, end).
 * @param end End of Player range [begin, end).
 * @param features features vector to write the calculated feature in-place.
 */
void linearity_stats(player_cit begin, player_cit end,
                     std::vector<double>& features);

/**
 * @brief Calculate vertical linearity for each cluster in the given clustering
 * and return the maximum vertical linearity.
 *
 * @param means A particular clustering in one dimension.
 * @param points Original sequence of points that were clustered.
 *
 * @return Maximum of vertical linearity of all clusters.
 */
double max_vertical_linearity(const dkm_means<1>& means,
                              const dkm_point_seq<1>& points);
}; // namespace details
}; // namespace feature
