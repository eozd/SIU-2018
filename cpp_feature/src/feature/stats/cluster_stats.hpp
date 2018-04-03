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
#include <utility>
#include <vector>

#include <feature/constants.hpp>
#include <utils.hpp>

#include "dkm_utils.hpp"

namespace feature {
namespace details {

/**
 * @brief Calculate features related to the clustering of players.
 *
 * This function calculates DenseClusterDensity and SparseClusterDensity
 * features for the Players in the given range [begin, end).
 *
 * @param begin Beginning of Player range [begin, end).
 * @param end End of Player range [begin, end).
 * @param prefix Prefix of the feature name. If prefix == "home", then the
 * calculated feature will be written to the index designated to
 * homeInnerDistance. Must be one of "home", "away" or "player".
 * @param features features vector to write the calculated feature in-place.
 */
void cluster_stats(player_cit begin, player_cit end, const std::string& prefix,
                   std::vector<double>& features);

/**
 * @brief Calculate dense and sparse cluster densities of the given clustering
 * and return the densities as <denseDensity, sparseDensity>.
 *
 * @param means A particular clustering.
 * @param points Original sequence of points that were clustered.
 *
 * @return pair of doubles <DenseClusterDensity, SparseClusterDensity>.
 */
std::pair<double, double> cluster_densities(const dkm_means<2>& means,
                                            const dkm_point_seq<2>& points);

}; // namespace details
}; // namespace feature
