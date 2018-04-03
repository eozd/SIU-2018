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

#include <dkm/dkm.hpp>

#include "dkm_utils.hpp"
#include "player_mixing_stats.hpp"

namespace feature {
namespace details {

void player_mixing_stats(player_cit begin, player_cit end,
                         std::vector<double>& features) {
    constexpr int n_clusters = 4;
    double max_impurity = feature::default_value();

    // if we have at least n_clusters many points
    if (std::distance(begin, end) >= n_clusters) {
        // calculate k-means clustering
        dkm_point_seq<2> points = players_to_points(begin, end);
        dkm_point_seq<2> centroids;
        dkm_label_seq labels;
        std::tie(centroids, labels) = kmeans(points, n_clusters);

        // player types (home/away/gk/...)
        std::vector<int> types(std::distance(begin, end));
        std::transform(begin, end, types.begin(),
                       [](const Player& p) { return p.type; });

        max_impurity = max_cluster_impurity(types, labels, n_clusters);
    }

    features[name_to_index("maxClusterImpurity")] = max_impurity;
}

double max_cluster_impurity(const std::vector<int>& types,
                            const dkm_label_seq& labels, size_t n_clusters) {
    double max_impurity = std::numeric_limits<double>::lowest();

    // for each cluster
    for (size_t curr_label = 0; curr_label < n_clusters; ++curr_label) {
        // Team types of players that belong to this cluster.
        std::vector<int> cluster_types;
        for (size_t i = 0; i < labels.size(); ++i) {
            if (labels[i] == curr_label) {
                cluster_types.push_back(types[i]);
            }
        }

        // calculate the impurity statistic for the current cluster
        double impurity = gini_impurity(cluster_types);

        max_impurity = std::max(max_impurity, impurity);
    }

    return max_impurity;
}

}; // namespace details
}; // namespace feature
