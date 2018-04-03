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
#include <vector>

#include "constants.hpp"
#include "row.hpp"

namespace feature {

/**
 * @brief Computer class computes features from match timeframes.
 */
class Computer {
  public:
    /**
     * @brief Default constructor.
     */
    Computer();

    /**
     * @brief Compute all the features from a Row object and return them as a
     * vector.
     *
     * If there is no Player data at a given Row, the returned features are
     * guaranteed to be the same as the features returned from the previous call
     * to this function.
     *
     * If a feature cannot be computed using the given Row object, the most
     * recently computed value for that feature will be returned.
     *
     * @param row Row object representing the current timeframe of the match.
     *
     * @return vector of doubles containing the features.
     */
    std::vector<double> compute_features(const Row& row);

  private:
    /**
     * @brief Current row object.
     *
     * Current row is stored because we must sort Players; hence, we need a
     * copy.
     */
    Row curr_row;
    /**
     * @brief Previous row object.
     *
     * Previous row is stored to calculate speeds.
     */
    Row prev_row;
    /**
     * @brief Most recently computed features.
     *
     * prev_features is stored to fill missing feature values in the current
     * timeframe.
     */
    std::vector<double> prev_features;
};

}; // namespace feature
