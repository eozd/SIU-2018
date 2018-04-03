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

#include <cmath>

#include <dkm/dkm.hpp>

#include "dkm_utils.hpp"
#include "linearity_stats.hpp"

namespace feature {
namespace details {

void linearity_stats(player_cit begin, player_cit end,
                     std::vector<double>& features) {
    constexpr int n_clusters = 4;
    double vert_linearity = feature::default_value();

    // if we have at least n_clusters many points
    if (std::distance(begin, end) >= n_clusters) {
        // cluster only in x coordinate
        dkm_point_seq<2> points = players_to_points(begin, end);
        dkm_point_seq<1> x(points.size());
        std::transform(
            points.begin(), points.end(), x.begin(),
            [](const dkm_point<2>& p) { return dkm_point<1>{p[0]}; });
        auto means = kmeans(x, n_clusters);

        vert_linearity = max_vertical_linearity(means, x);
    }

    features[name_to_index("playerVerticalLinearity")] = vert_linearity;
}

double max_vertical_linearity(const dkm_means<1>& means,
                              const dkm_point_seq<1>& points) {
    // destructure the clustering
    dkm_point_seq<1> centroids;
    dkm_label_seq labels;
    std::tie(centroids, labels) = means;

    double max_vert_density = std::numeric_limits<double>::lowest();

    // for each cluster
    for (size_t curr_label = 0; curr_label < centroids.size(); ++curr_label) {
        // Center and points of the current cluster
        dkm_point<1> center = centroids[curr_label];
        dkm_point_seq<1> cluster = get_cluster(points, labels, curr_label);

        if (cluster.empty()) {
            continue;
        }

        // distance of each point in the cluster to the cluster center
        std::vector<double> diff(cluster.size(), 0);
        std::transform(cluster.begin(), cluster.end(), diff.begin(),
                       [&center](const dkm_point<1>& p) {
                           return fabs(dist<1>(p, center));
                       });

        double max_dist = *std::max_element(diff.begin(), diff.end());

        // if there is some distance between players
        bool distant_players = max_dist >= 1;

        // calculate the density statistic for the current cluster
        double vert_density = 0;

        // if no distance, don't calculate vert_density for this cluster
        if (distant_players) {
            vert_density = cluster.size() / max_dist;
        }

        max_vert_density = std::max(max_vert_density, vert_density);
    }

    return max_vert_density;
}

}; // namespace details
}; // namespace feature
