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
#include <iostream>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/multi_point.hpp>
#include <boost/geometry/geometries/point.hpp>

#include "convex_stats.hpp"
#include <utils.hpp>

using namespace feature;

namespace bg = boost::geometry;
typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::multi_point<point> multi_point;

/**
 * Find the convex hull of Player sequence between [begin, end) and return the
 * indices of Player objects that are on the convex hull.
 */
/**
 * @brief Find the convex hull of a Player range [begin, end) and return the
 * indices of Player objects that are on the convex hull.
 *
 * This function computes the convex hull of Player objects given in [begin,
 * end) and then finds the indices of points on the hull by doing an \f$O(NK)\f$
 * time worst-case search where \f$K\f$ is the size of the convex hull and
 * \f$N\f$ is the number of total players.
 *
 * @param begin Beginning of the player range [begin, end).
 * @param end End of the player range [begin, end).
 *
 * @return Indices of the players on the convex hull.
 *
 * @todo Reduce index finding time complexity to \f$O(KlogN)\f$ by sorting the
 * points. We should do this if \f$NK \gg KlogN\f$ which is not the case for
 * small \f$N\f$.
 */
static std::vector<int> convex_indices(player_cit begin, player_cit end) {
    // construct points
    multi_point points;
    for (auto it = begin; it != end; ++it) {
        bg::append(points, point(it->x, it->y));
    }
    // compute convex hull
    multi_point hull;
    bg::convex_hull(points, hull);

    // find the index of each point on the hull by searching for it in the
    // original range.
    std::vector<int> indices;
    // bg::convex_hull puts the first point twice. Don't count it in the end.
    for (auto it = boost::begin(hull); it != std::prev(boost::end(hull));
         ++it) {
        auto point_it = std::find_if(
            boost::begin(points), boost::end(points), [it](const point& p) {
                return close(p.get<0>(), it->get<0>()) &&
                       close(p.get<1>(), it->get<1>());
            });
        indices.push_back(std::distance(boost::begin(points), point_it));
    }

    return indices;
}

/**
 * Constructs a sequence of Point objects from Player objects at the given
 * indices of the given Player sequence starting at begin.
 */

/**
 * @brief Construct a vector of point types at the given indices and return it.
 *
 * This function filters the Players at the given indices in the range that
 * starts at begin and returns them as a vector of point types.
 *
 * @param indices Indices of points to return as a separate vector.
 * @param begin Beginning of a Player range.
 */
static std::vector<point> points_from_indices(const std::vector<int>& indices,
                                              player_cit begin) {
    std::vector<point> res(indices.size());
    std::transform(indices.begin(), indices.end(), res.begin(),
                   [begin](const int i) {
                       auto player = std::next(begin, i);
                       return point(player->x, player->y);
                   });

    return res;
}

namespace feature {
namespace details {

void convex_stats(player_cit begin, player_cit end,
                  std::vector<double>::iterator speed_begin,
                  const std::string& prefix, std::vector<double>& features) {
    // initialize features with default values
    double min_x = feature::default_value();
    double min_y = feature::default_value();
    double max_x = feature::default_value();
    double max_y = feature::default_value();
    double max_dist = feature::default_value();
    double min_dist = feature::default_value();
    double max_speed = feature::default_value();
    point center(feature::default_value(), feature::default_value());

    // if there are at least 3 points (no convex hull of 2 or less points)
    if (std::distance(begin, end) > 2) {
        // get convex indices and corresponding points
        std::vector<int> indices = convex_indices(begin, end);
        std::vector<point> convex_points = points_from_indices(indices, begin);

        min_x = std::numeric_limits<double>::max();
        min_y = std::numeric_limits<double>::max();
        max_x = std::numeric_limits<double>::lowest();
        max_y = std::numeric_limits<double>::lowest();
        center = point(0, 0);
        double size = static_cast<double>(convex_points.size());

        // calculate min/max x/y and center
        for (const auto& point : convex_points) {
            min_x = std::min(min_x, point.get<0>());
            min_y = std::min(min_y, point.get<1>());
            max_x = std::max(max_x, point.get<0>());
            max_y = std::max(max_y, point.get<1>());

            center.set<0>(center.get<0>() + point.get<0>() / size);
            center.set<1>(center.get<1>() + point.get<1>() / size);
        }

        // farDistance, closestDistance
        max_dist = std::numeric_limits<double>::lowest();
        min_dist = std::numeric_limits<double>::max();
        for (const auto& point : convex_points) {
            double distance = dist(point.get<0>(), point.get<1>(),
                                   center.get<0>(), center.get<1>());
            max_dist = std::max(max_dist, distance);
            min_dist = std::min(min_dist, distance);
        }

        // maxSpeed
        max_speed = std::numeric_limits<double>::lowest();
        for (int i : indices) {
            double speed = *std::next(speed_begin, i);
            max_speed = std::max(max_speed, speed);
        }
    }
    // write the results
    features[name_to_index(prefix + "ConvexMaxX")] = max_x;
    features[name_to_index(prefix + "ConvexMinX")] = min_x;
    features[name_to_index(prefix + "ConvexMaxY")] = max_y;
    features[name_to_index(prefix + "ConvexMinY")] = min_y;
    features[name_to_index(prefix + "ConvexCenterX")] = center.get<0>();
    features[name_to_index(prefix + "ConvexCenterY")] = center.get<1>();
    features[name_to_index(prefix + "ConvexMaxSpeed")] = max_speed;
    features[name_to_index(prefix + "ConvexFarDistance")] = max_dist;
    features[name_to_index(prefix + "ConvexClosestDistance")] = min_dist;
}

}; // namespace details
}; // namespace feature
