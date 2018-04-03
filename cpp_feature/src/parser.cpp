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
#include <string>

#include <feature/constants.hpp>
#include <feature/player.hpp>
#include <feature/row.hpp>
#include <utils.hpp>

#include "parser.hpp"

feature::Row parse_line(const std::string& line) {
    // separate row with respect to tab
    std::vector<std::string> tab_separated = str_split(line, '\t');
    if (tab_separated.size() != 6) {
        throw std::runtime_error(
            "Raw frame format error: Tab split doesn't produce 6 pieces");
    }

    // Construct a Row object from tab separated values
    feature::Row res;
    res.match_id = stoi(tab_separated[0]);
    res.timestamp = stol(tab_separated[1]);
    res.half = stoi(tab_separated[2]);
    res.minute = stoi(tab_separated[3]);
    res.second = stoi(tab_separated[4]);

    // get rid of end of the line whitespaces
    rtrim(tab_separated.back());
    if (tab_separated.back().empty()) {
        return res;
    }

    // separate with respect to space to get player data
    std::vector<std::string> space_separated =
        str_split(tab_separated.back(), ' ');

    // Construct a Player object from each split of space_separated.
    feature::player_seq players(space_separated.size());
    for (size_t i = 0; i < space_separated.size(); ++i) {
        // separate with respect to comma
        std::vector<std::string> comma_separated =
            str_split(space_separated[i], ',');
        if (comma_separated.size() != 5) {
            throw std::runtime_error(
                "Raw frame format error: Comma split doesn't produce 5 pieces");
        }

        // construct current player
        players[i].type = stoi(comma_separated[0]);
        players[i].id = stoi(comma_separated[1]);
        players[i].jersey = stoi(comma_separated[2]);
        players[i].x = stod(comma_separated[3]);
        players[i].y = stod(comma_separated[4]);
    }
    res.players = players;

    return res;
}
