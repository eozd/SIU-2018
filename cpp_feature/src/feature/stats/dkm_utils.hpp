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

#include <algorithm>
#include <array>
#include <tuple>
#include <vector>

#include <dkm/dkm.hpp>

#include <feature/constants.hpp>
#include <utils.hpp>

namespace feature {
namespace details {

/**
 * @brief Typedef to denote an N dimensional point to use with dkm library.
 */
template <size_t N> using dkm_point = std::array<double, N>;

/**
 * @brief Typedef to denote a sequence of N dimensional points to use with dkm
 * library.
 */
template <size_t N> using dkm_point_seq = std::vector<dkm_point<N>>;

/**
 * @brief Typedef to denote a sequence of labels to use with dkm library.
 */
using dkm_label_seq = std::vector<uint32_t>;

/**
 * @brief Typedef to denote a particular clustering to use with dkm library.
 *
 * @code
 *     dkm_point_seq<N> centroids;
 *     dkm_label_seq labels;
 *     std::tie(centroids, labels) = dkm_means;
 * @endcode
 *
 * In this code snippet, centroids[0] is the cluster center of cluster 0.
 * labels[i] is the cluster label of point[i].
 */
template <size_t N>
using dkm_means = std::tuple<dkm_point_seq<N>, dkm_label_seq>;

/**
 * @brief Construct a seuqence of dkm_points (dkm_point_seq<2>) from the given
 * Player range [begin, end).
 *
 * This function transforms a Player range [begin, end) to a dkm compatible
 * sequence, dkm_point_seq<2>. It is assumed that each Player has a
 * 2-dimensional point in the coordinate system.
 *
 * @param begin Beginning of Player range [begin, end).
 * @param end   End of Player range [begin, end).
 *
 * @return Sequence of dkm_points.
 */
dkm_point_seq<2> players_to_points(player_cit begin, player_cit end);

/**
 * @brief Calculate the Euclidean distance from each point in the given
 * dkm_point_seq to the given center point and write the results to given output
 * iterator.
 *
 * @tparam N Dimension of the points.
 * @tparam OutputIterator Iterator type of the output range. Since the Euclidean
 * distance returns a double, preferably, value_type of the given iterator
 * should be able to hold a double value.
 * @param points Point sequence.
 * @param center Center point from which distance of each point will be
 * calculated.
 * @param out    Beginning of the output sequence.
 */
template <size_t N, typename OutputIterator>
void dist_to_center(const dkm_point_seq<N>& points, const dkm_point<N>& center,
                    OutputIterator out) {
    std::transform(
        points.begin(), points.end(), out,
        [&center](const dkm_point<N>& p) { return dist<N>(p, center); });
}

/**
 * @brief Calculate sum of distances from each point in the given point
 * sequence to the given center point.
 *
 * @tparam N Dimension of the points.
 * @param points Point sequence.
 * @param center Center point from which distance of each point will be
 * calculated.
 *
 * @return Sum of distances from each point to the center.
 */
template <size_t N>
double sum_dist(const dkm_point_seq<N>& points, const dkm_point<N>& center) {
    std::vector<double> distances(points.size(), 0);
    dist_to_center(points, center, distances.begin());

    return std::accumulate(distances.begin(), distances.end(), 0.0);
}

/**
 * @brief Return a points of a cluster with the given label as a dkm_point_seq
 * object.
 *
 * @tparam N Dimension of the points.
 * @param points Sequence of dkm_point objects that were sent to
 * dkm::kmeans_lloyd clustering algorithm.
 * @param labels Sequence of labels that were obtained from dkm::kmeans_lloyd
 * algorithm indicating the label of each corresponding input point.
 * @param label Label of the cluster whose points will be returned.
 *
 * @return Sequence of dkm_point objects that all belong to the cluster
 * indicated by label.
 */
template <size_t N>
dkm_point_seq<N> get_cluster(const dkm_point_seq<N>& points,
                             const dkm_label_seq& labels,
                             const uint32_t label) {
    if (points.size() != labels.size())
        throw std::runtime_error("points and labels have different sizes");

    // construct the cluster
    dkm_point_seq<N> cluster;
    for (size_t point_index = 0; point_index < points.size(); ++point_index) {
        if (labels[point_index] == label) {
            cluster.push_back(points[point_index]);
        }
    }
    return cluster;
}

/**
 * @brief Calculate inertia of a given clustering.
 *
 * Inertia is defined as sum of distances of each point to its closest cluster
 * center, and can be defined mathematically as follows:
 *
 * \f[
 *     \sum_{p \in P} D(p, center(c_p))
 * \f]
 *
 * where \f$P\f$ is the set of all points in the clustering, \f$D\f$ is the
 * distance between two points, and \f$c_p\f$ is the class of point \f$p\f$.
 *
 * @tparam N Dimension of the points.
 * @param points Sequence of dkm_point objects that were sent to
 * dkm::kmeans_lloyd clustering algorithm.
 * @param means Result of dkm::kmeans_lloyd algorithm that represents a
 * particular clustering.
 *
 * @return Total inertia of the given clustering.
 */
template <size_t N>
double means_inertia(const dkm_point_seq<N>& points,
                     const dkm_means<N>& means) {
    // destructure the clustering
    dkm_point_seq<N> centroids;
    dkm_label_seq labels;
    std::tie(centroids, labels) = means;

    // get a list of unique labels
    dkm_label_seq labels_copy(labels.size());
    std::copy(labels.begin(), labels.end(), labels_copy.begin());
    std::sort(labels_copy.begin(), labels_copy.end());
    auto labels_end = std::unique(labels_copy.begin(), labels_copy.end());

    double inertia = 0;
    for (auto it = labels_copy.begin(); it != labels_end; ++it) {
        auto label = *it;

        // get all points belonging to cluster given as label
        auto cluster = get_cluster(points, labels, label);

        // sum the distance of all points in the given cluster to their centroid
        inertia += sum_dist(cluster, centroids[label]);
    }
    return inertia;
}

/**
 * @brief Find the clustering with the lowest inertia from a vector of
 * clusterings
 *
 * @tparam N Dimension of the points.
 * @param points Sequence of dkm_point objects that were sent to
 * dkm::kmeans_lloyd clustering algorithm.
 * @param means_list vector of clusterings.
 *
 * @return Clustering with the lowest inertia.
 */
template <size_t N>
dkm_means<N> get_best_means(const dkm_point_seq<N>& points,
                            const std::vector<dkm_means<N>>& means_list) {
    double min_inertia = std::numeric_limits<double>::max();
    const dkm_means<N>* best_means;

    for (const auto& means : means_list) {
        double inertia = means_inertia(points, means);
        if (inertia < min_inertia) {
            min_inertia = inertia;
            best_means = &means;
        }
    }
    return dkm_means<N>(*best_means); // copy and return
}

/**
 * @brief Calculate k-means clustering of the given points n_init times and
 * return the clustering with the lowest inertia.
 *
 * @tparam N Dimension of the points.
 * @param points Sequence of dkm_point objects that were sent to
 * dkm::kmeans_lloyd clustering algorithm.
 * @param n_clusters Number of clusters to compute in k-means (k).
 * @param n_init Number of times k-means algorithm will be run.
 *
 * @return Clustering with the lowest inertia.
 */
template <size_t N>
dkm_means<N> kmeans(const dkm_point_seq<N>& points, int n_clusters,
                    int n_init = 10) {
    // Run k-means algorithm n_init times and collect the results.
    std::vector<dkm_means<N>> means_list;
    for (int i = 0; i < n_init; ++i) {
        means_list.push_back(dkm::kmeans_lloyd(points, n_clusters));
    }

    // Return the best k-means result.
    return get_best_means(points, means_list);
}

}; // namespace details
}; // namespace feature
