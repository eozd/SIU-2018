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
#include <string>
#include <unordered_map>

namespace feature {

/**
 * @brief Player class represents a player in a football match.
 */
class Player {
  public:
    /**
     * @brief Default constructor that constructs all the fields with
     * default values.
     *
     * Default values of the variables are as follows:
     *
     * Member    | Default value
     * --------- | -------------
     * type      | -1
     * id        | -1
     * jersey    | -1
     * x         | -1
     * y         | -1
     */
    Player();

    /**
     * @brief Construct a Player by giving all members manually.
     *
     * @param type Type of the player (home/away/home-gk/away-gk/referee).
     * @param id Id of the player.
     * @param jersey Jersey number of the player.
     * @param x x coordinate of the player in the pitch.
     * @param y y coordinate of the player in the pitch.
     */
    Player(int type, int id, int jersey, double x, double y);

    /**
     * @brief Construct a Player using only coordinates.
     *
     * Rest of the fields are created using their default values as specified
     * in the default constructor.
     *
     * @param x x coordinate of the player in the pitch.
     * @param y y coordinate of the player in the pitch.
     */
    Player(double x, double y);

  public:
    /**
     * @brief Type of the player (home/away/home-gk/away-gk/referee).
     */
    int type;
    /**
     * @brief ID of the player.
     */
    int id;
    /**
     * @brief Jersey number of the player.
     */
    int jersey;
    /**
     * @brief x coordinate of the player in the pitch.
     */
    double x;
    /**
     * @brief y coordinate of the player in the pitch.
     */
    double y;
};

/**
 * @brief Equality operator for two Player objects.
 *
 * Two Player objects are equal iff their types, IDs and jersey numbers are
 * equal. x and y coordinates are not used when comparing two Player objects
 * for equality.
 */
bool operator==(const Player& p1, const Player& p2);

/**
 * @brief Inequality operator for two Player objects.
 *
 * Two Player objects are inequal if they are not equal.
 */
bool operator!=(const Player& p1, const Player& p2);

/**
 * @brief Output stream operator for Player objects.
 *
 * This operator is used to output Player objects in a user-friendly manner.
 */
std::ostream& operator<<(std::ostream& os, const Player& p);

}; // namespace feature
