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
#include <unordered_map>
#include <utility>
#include <vector>

#include "player.hpp"

/**
 * @brief Namespace for classes and functions related to feature computation.
 */
namespace feature {
/**
 * @brief Namespace for keeping types/functions/classes/... that are to be
 * used by feature namespace only.
 */
namespace details {

/**
 * @brief Map from names to their IDs.
 */
typedef std::unordered_map<std::string, int> name_to_index_map;
/**
 * @brief Map from IDs to their names.
 */
typedef std::unordered_map<int, std::string> index_to_name_map;

}; // namespace details

/**
 * @brief Typedef for sequence of Player objects.
 */
typedef std::vector<Player> player_seq;
/**
 * @brief Typedef for Player iterator
 */
typedef player_seq::iterator player_it;
/**
 * @brief Typedef for Player const_iterator
 */
typedef player_seq::const_iterator player_cit;
/**
 * @brief Typedef for a Player range [begin, end).
 */
typedef std::pair<player_it, player_it> player_range;
/**
 * @brief Typedef for a consnt Player range [begin, end).
 */
typedef std::pair<player_cit, player_cit> player_crange;

/**
 * @brief Return a list of feature names.
 *
 * @return List of feature names to compute.
 */
std::vector<std::string> feature_list();

/**
 * @brief Return the unique ID of the feature with the given name.
 *
 * Returned IDs are guaranteed to be less than total number of features.
 *
 * @param name Feature name
 *
 * @return Unique ID of the feature.
 */
int name_to_index(const std::string& name);

/**
 * @brief Return the name of the feature with the given ID.
 *
 * @param index ID of the feature.
 *
 * @return Name of the feature.
 */
const std::string& index_to_name(const int index);

/**
 * @brief Return the number of features that will be computed.
 *
 * @return Number of features.
 */
size_t num_features();

/**
 * @brief Return integer representing the type of Player from its string
 * representation.
 *
 * @param name Player type name (home/away/...)
 *
 * @return Player type.
 */
int player_name_to_type(const std::string& name);

/**
 * @brief Return string representation of the given Player type.
 *
 * @param index Player type
 *
 * @return String representation of the given Player type.
 */
const std::string& player_type_to_name(const int index);

/**
 * @brief Return the default value to use for features that can't be computed.
 *
 * @return Default value for features.
 */
double default_value();

/**
 * @brief Return a vector of features with all the features initialized to their
 * default values.
 *
 * @return vector of features with all features initialized to their default
 * values.
 */
std::vector<double> default_features();
}; // namespace feature
