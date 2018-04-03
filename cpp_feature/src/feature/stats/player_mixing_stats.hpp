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
 * @brief Calculate features related to how mixed/separated the players are.
 *
 * This function calculates maxClusterImpurity feature.
 *
 * @param begin Beginning of Player range [begin, end).
 * @param end End of Player range [begin, end).
 * @param features features vector to write the calculated feature in-place.
 */
void player_mixing_stats(player_cit begin, player_cit end,
                         std::vector<double>& features);

/**
 * @brief Calculate Gini Impurity of each cluster in the given clustering and
 * return the maximum Gini Impurity.
 *
 * @param types vector holding the type of each player.
 * @param labels Corresponding cluster labels of each player.
 * @param n_clusters Number of clusters in the given clustering.
 *
 * @return Maximum gini impurity of all given clustering labels.
 */
double max_cluster_impurity(const std::vector<int>& types,
                            const dkm_label_seq& labels, size_t n_clusters);

}; // namespace details
}; // namespace feature
