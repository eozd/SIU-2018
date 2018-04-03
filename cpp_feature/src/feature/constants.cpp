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
#include <iterator>

#include "constants.hpp"
#include <utils.hpp>

namespace feature {

std::vector<std::string> feature_list() {
    return {
        "awayAvgX",
        "awayAvgY",
        "awayConvexCenterX",
        "awayConvexCenterY",
        "awayConvexClosestDistance",
        "awayConvexFarDistance",
        "awayConvexMaxSpeed",
        "awayConvexMaxX",
        "awayConvexMaxY",
        "awayConvexMinX",
        "awayConvexMinY",
        "awayDenseClusterDensity",
        "awayInnerDistance",
        "awaySparseClusterDensity",
        "homeAvgX",
        "homeAvgY",
        "homeConvexCenterX",
        "homeConvexCenterY",
        "homeConvexClosestDistance",
        "homeConvexFarDistance",
        "homeConvexMaxSpeed",
        "homeConvexMaxX",
        "homeConvexMaxY",
        "homeConvexMinX",
        "homeConvexMinY",
        "homeDenseClusterDensity",
        "homeInnerDistance",
        "homeSparseClusterDensity",
        "maxClusterImpurity",
        "playerConvexCenterX",
        "playerConvexCenterY",
        "playerConvexClosestDistance",
        "playerConvexFarDistance",
        "playerConvexMaxSpeed",
        "playerConvexMaxX",
        "playerConvexMaxY",
        "playerConvexMinX",
        "playerConvexMinY",
        "playerDenseClusterDensity",
        "playerSparseClusterDensity",
        "playerVerticalLinearity",
        "refSpeed",
        "refX",
        "refY",
    };
}

double default_value() { return -1; }

std::vector<double> default_features() {
    double default_val = default_value();
    return std::vector<double>(num_features(), default_val);
}

using namespace feature::details;

/**
 * @brief Construct a mapping from feature names to their unique integer IDs.
 *
 * @return Mapping from each feature name to its unique ID.
 */
static name_to_index_map construct_feature_name_to_index() {
    // sort list of features
    auto feature_vec = feature_list();
    std::sort(feature_vec.begin(), feature_vec.end());

    // construct a map from feature name to a unique ID
    name_to_index_map features_indices;
    for (size_t i = 0; i < feature_vec.size(); ++i) {
        features_indices[feature_vec[i]] = i;
    }
    return features_indices;
}

/**
 * @brief Construct a mapping from player names (home, away, ...) to their
 * unique IDs.
 *
 * @return Mapping from player name (home, away, ...) to its unique ID.
 */
static name_to_index_map construct_player_name_to_index() {
    name_to_index_map res{{"home", 0},
                          {"away", 1},
                          {"referee", 2},
                          {"home_gk", 3},
                          {"away_gk", 4}};
    return res;
}

/**
 * @brief Construct a mapping from ID to its corresponding unique name using an
 * already built name to index map.
 *
 * This function computes the inverse map of the given map and returns it.
 *
 * @param imap Mapping from name to its unique ID.
 *
 * @return Inverse of imap.
 */
static index_to_name_map
construct_index_to_name(const name_to_index_map& imap) {
    index_to_name_map nmap;
    inverse_map(imap.begin(), imap.end(), std::inserter(nmap, nmap.end()));
    return nmap;
}

int name_to_index(const std::string& name) {
    // build the map only once
    static const name_to_index_map* const imap =
        new name_to_index_map(construct_feature_name_to_index());

    return imap->at(name);
}

const std::string& index_to_name(const int index) {
    // build the map only once
    static const index_to_name_map* const nmap = new index_to_name_map(
        construct_index_to_name(construct_feature_name_to_index()));
    return nmap->at(index);
}

size_t num_features() {
    static size_t size = feature_list().size();
    return size;
}

int player_name_to_type(const std::string& name) {
    // build the map only once
    static const name_to_index_map* const imap =
        new name_to_index_map(construct_player_name_to_index());

    return imap->at(name);
}

const std::string& player_type_to_name(const int index) {
    // build the map only once
    static const index_to_name_map* const nmap = new index_to_name_map(
        construct_index_to_name(construct_player_name_to_index()));

    return nmap->at(index);
}

}; // namespace feature
