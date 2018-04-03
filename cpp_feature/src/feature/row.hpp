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

#include <iostream>
#include <vector>

#include <feature/constants.hpp>

namespace feature {

/**
 * @brief Row class represents all the data in a single timeframe of a match.
 */
class Row {
  public:
    /**
     * @brief Default constructor that constructs all the fields with
     * default values.
     *
     * Default values of the variables are as follows:
     *
     * Member    | Default value
     * --------- | -------------
     * match_id  | -1
     * timestamp | -1
     * half      | -1
     * minute    | -1
     * second    | -1
     * players   | empty vector
     */
    explicit Row();

  public:
    /**
     * @brief ID of the match.
     */
    int match_id;
    /**
     * @brief Timestamp of the current timeframe.
     */
    long timestamp;
    /**
     * @brief Half of the current timeframe.
     */
    int half;
    /**
     * @brief Minute of the current timeframe.
     */
    int minute;
    /**
     * @brief Second of the current timeframe.
     */
    int second;
    /**
     * @brief Sequence of Player objects whose data were gathered in this
     * timeframe.
     */
    player_seq players;
};

/**
 * @brief Equality operator between Row objects.
 *
 * Two Row objects are equal if all of their members are equal.
 */
bool operator==(const Row& p1, const Row& p2);

/**
 * @brief Inequality operator between Row objects.
 *
 * Two Row objects are inequal if they are not equal.
 */
bool operator!=(const Row& p1, const Row& p2);

/**
 * @brief Output stream operator for Row objects.
 *
 * This function prints a row object in a user-friendly manner.
 */
std::ostream& operator<<(std::ostream& os, const Row& r);

}; // namespace feature
