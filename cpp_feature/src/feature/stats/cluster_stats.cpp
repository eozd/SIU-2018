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
#include <iostream>

#include "cluster_stats.hpp"
#include "dkm_utils.hpp"

namespace feature {
namespace details {

void cluster_stats(player_cit begin, player_cit end, const std::string& prefix,
                   std::vector<double>& features) {
    constexpr int n_clusters = 2;
    // density of the two clusters
    std::pair<double, double> densities{feature::default_value(),
                                        feature::default_value()};

    // at least n_clusters many players
    if (std::distance(begin, end) >= n_clusters) {
        // calculate clustering
        dkm_point_seq<2> points = players_to_points(begin, end);
        dkm_means<2> means = kmeans(points, n_clusters);

        densities = cluster_densities(means, points);
    }

    features[name_to_index(prefix + "DenseClusterDensity")] = densities.first;
    features[name_to_index(prefix + "SparseClusterDensity")] = densities.second;
}

std::pair<double, double> cluster_densities(const dkm_means<2>& means,
                                            const dkm_point_seq<2>& points) {
    // destructure the clustering
    dkm_point_seq<2> centroids;
    dkm_label_seq labels;
    std::tie(centroids, labels) = means;

    double max_density = std::numeric_limits<double>::lowest();
    double min_density = std::numeric_limits<double>::max();

    // for each cluster
    for (uint32_t curr_label = 0; curr_label < centroids.size(); ++curr_label) {
        // Center and points of the current cluster
        dkm_point<2> center = centroids[curr_label];
        dkm_point_seq<2> cluster = get_cluster(points, labels, curr_label);

        if (cluster.empty()) {
            continue;
        }

        // distance of each point in the cluster to the cluster center
        std::vector<double> diff(cluster.size(), 0);
        dist_to_center(cluster, center, diff.begin());

        double max_dist = *std::max_element(diff.begin(), diff.end());

        // true if there is some distance between players in the cluster
        bool distant_players = !close(max_dist, 0);

        // calculate the density statistic for the current cluster
        double density = 0;

        // if there is no distance between any players, don't use this cluster
        if (distant_players) {
            density = cluster.size() / max_dist;
        }
        max_density = std::max(max_density, density);
        min_density = std::min(min_density, density);
    }

    return {max_density, min_density};
}

}; // namespace details
}; // namespace feature
