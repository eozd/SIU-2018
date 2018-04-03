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
#include <cmath>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

/**
 * @brief Read raw bytes from file in the given filepath and return.
 *
 * This function reads the raw bytes of the file in binary mode.
 *
 * @param filepath Path to the file to read in binary mode.
 *
 * @return All the bytes of the file as a std::string object.
 */
std::string read_bytes(const std::string& filepath);

/**
 * @brief Split a string with respect to a delimiter and return individual parts
 * in a vector.
 *
 * Example:
 * @code
 *     str = "abc def gh";
 *     delim = ' ';
 *     std::vector<std::string> expected = {"abc", "def", "gh"};
 *     assert(expected == str_split(str, delim));
 * @endcode
 *
 * @param str String to split to parts.
 * @param delim Delimiter to split with respect to.
 *
 * @return vector of individual parts of the string.
 */
std::vector<std::string> str_split(const std::string& str, const char delim);

/**
 * @brief Trim any whitespace from the rear of the given string in-place.
 *
 * @param s string to trim in-place.
 *
 * @see
 * https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
void rtrim(std::string& s);

/**
 * @brief Check whether the difference between the given doubles is less than
 * or equal to epsilon value eps.
 *
 * This function returns true iff \f$ |d1 - d2| \leq eps \f$.
 *
 * @param d1 First double to compare.
 * @param d2 Second double to compare.
 * @param eps Epsilon value
 */
bool close(double d1, double d2, double eps = 1e-6);

/**
 * @brief Calculate the Euclidean distance between two points (x1, y1) and (x2,
 * y2).
 *
 * Euclidean distance is calculated as \f$\sqrt{(x1 - x2)^2 + (y1 - y2)^2}\f$.
 *
 * @param x1 x coordinate of the first point.
 * @param y1 y coordinate of the first point.
 * @param x2 x coordinate of the second point.
 * @param y2 y coordinate of the second point.
 *
 * @return Euclidean distance between points (x1, y1) and (x2, y2.
 */
double dist(double x1, double y1, double x2, double y2);

/**
 * @brief Calculate Euclidean distance between two points \f$(a_0, a_1, \dots,
 * a_n)\f$ and \f$(b_0, b_1, \dots, b_n)\f$.
 *
 * Euclidean distance is calculated as \f$\sqrt{\sum_i (a_i - b_i)^2}\f$.
 *
 * @tparam N Size of the given Point types.
 * @tparam Point A sequence type that has an implemented 'const operator[]'.
 * @param p1 First point, \f$(a_0, a_1, \dots, a_n)\f$.
 * @param p2 First point, \f$(b_0, b_1, \dots, b_n)\f$.
 *
 * @return Euclidean distance between the given two points.
 */
template <size_t N, typename Point>
double dist(const Point& p1, const Point& p2) {
    double res = 0;
    for (size_t i = 0; i < N; ++i)
        res += std::pow(p1[i] - p2[i], 2);

    return std::sqrt(res);
}

/**
 * @brief Return Gini impurity of a vector of integers.
 *
 * Gini impurity of a vector of labels \f$v\f$ that contains elements from the
 * set of labels \f$L\f$ is calculated as
 *
 * \f[
 *     \sum_{x \in L} p_x(1 - p_x)
 * \f]
 *
 * where \f$p_x\f$ is the frequency of label \f$x\f$ and is calculated as
 *
 * \f[
 *     p_x = \frac{|\{v_i : v_i = x, \forall i\}|}{|v|}
 * \f]
 *
 * @param labels vector of integers.
 *
 * @return Gini impurity of the given label.
 */
double gini_impurity(const std::vector<int>& labels);

/**
 * @brief Calculate the inverse map of the map given in the range [begin, end).
 *
 * This function constructs the inverse of the given map by reversing the
 * key-value pairs one-by-one. If the map in range [begin, end) contains
 * multiple keys mapped to the same value, then the inverse mapping is
 * unspecified.
 *
 * @tparam MapIterator An iterator that points to a map type such as std::map or
 * std::unordered_map.
 * @tparam OutputIterator An iterator that points to a map type such as std::map
 * or std::unordered_map.
 * @param begin Beginning of the input map range [begin, end).
 * @param end End of the input map range [begin, end).
 * @param out Iterator to write the inverse map one-by-one.
 *
 * @return End of the resulting output inverse map.
 */
template <typename MapIterator, typename OutputIterator>
OutputIterator inverse_map(MapIterator begin, MapIterator end,
                           OutputIterator out) {
    using out_container_type = typename decltype(out)::container_type;
    using out_type = typename out_container_type::value_type;
    return std::transform(begin, end, out,
                          [](const auto& map_pair) -> out_type {
                              return {map_pair.second, map_pair.first};
                          });
}

/**
 * @brief Check whether the given two maps are inverse of each other.
 *
 * This function checks if the maps are inverse of each other by ensuring that
 * keys of the first map are the values of the second map, and vice versa. This
 * equality check must hold for key and value types, as well, i.e. key type of
 * the first map must be the same as the value type of the second map, and vice
 * versa.
 *
 * Given two maps do not have to have the same type, i.e. one of the maps may
 * be std::map and the other may be std::unordered_map.
 *
 * @tparam Map Type of the first map.
 * @tparam InverseMap Type of the second map.
 * @param mapping First map.
 * @param inv_mapping Second map.
 *
 * @return true if the two maps are inverse of each other; false otherwise.
 */
template <typename Map, typename InverseMap>
bool is_inv_map(const Map& mapping, const InverseMap& inv_mapping) {
    typedef typename Map::key_type map_key;
    typedef typename Map::mapped_type map_val;
    typedef typename InverseMap::key_type inv_map_key;
    typedef typename InverseMap::mapped_type inv_map_val;

    constexpr bool same_key_val = std::is_same<map_key, inv_map_val>::value;
    constexpr bool same_val_key = std::is_same<map_val, inv_map_key>::value;
    static_assert(same_key_val,
                  "Map::key_type must be the same as InverseMap::value_type");
    static_assert(same_val_key,
                  "Map::value_type must be the same as InverseMap::key_type");

    for (const auto& key_val : mapping) {
        if (inv_mapping.find(key_val.second) == inv_mapping.end())
            return false;
        if (inv_mapping.at(key_val.second) != key_val.first)
            return false;
    }
    return true;
}
