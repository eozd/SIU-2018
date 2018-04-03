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
#include <string>
#include <vector>

#include "computer.hpp"
#include "stats.hpp"

namespace feature {

Computer::Computer()
    : curr_row(), prev_row(), prev_features(default_features()) {}

using namespace details;

std::vector<double> Computer::compute_features(const Row& row) {
    if (row.timestamp == this->prev_row.timestamp) {
        return this->prev_features;
    }
    //
    // comparator function to compare players when sorting
    auto player_type_comp = [](const Player& p1, const Player& p2) {
        return p1.type < p2.type;
    };

    // copy and sort the current row with respect to player type
    this->curr_row = row;
    std::sort(this->curr_row.players.begin(), this->curr_row.players.end(),
              player_type_comp);

    // calculate speeds (indices are the same as players)
    std::vector<double> speed =
        calculate_speeds(this->curr_row, this->prev_row);

    // find player_ranges for home using binary search
    Player p;
    p.type = player_name_to_type("home");
    player_crange home_players =
        std::equal_range(this->curr_row.players.cbegin(),
                         this->curr_row.players.cend(), p, player_type_comp);
    auto home_speed_begin =
        std::next(speed.begin(), std::distance(this->curr_row.players.cbegin(),
                                               home_players.first));

    // find player_ranges for away using binary search
    p.type = player_name_to_type("away");
    player_crange away_players =
        std::equal_range(this->curr_row.players.begin(),
                         this->curr_row.players.end(), p, player_type_comp);
    auto away_speed_begin =
        std::next(speed.begin(), std::distance(this->curr_row.players.cbegin(),
                                               away_players.first));

    // home and away follow each other to create "player" range
    player_crange both_players = {home_players.first, away_players.second};
    auto both_speed_begin = home_speed_begin;

    // referee iterators
    int ref_type = player_name_to_type("referee");

    // begin iterator for referee (find referee)
    player_cit ref_begin = std::find_if(
        this->curr_row.players.begin(), this->curr_row.players.end(),
        [ref_type](const Player& p) { return p.type == ref_type; });

    // if no referee, keep ref_end == ref_begin
    player_cit ref_end = ref_begin;
    if (ref_begin != this->curr_row.players.end())
        ref_end = std::next(ref_end);

    // speed of referee
    auto ref_speed_it =
        std::next(speed.begin(),
                  std::distance(this->curr_row.players.cbegin(), ref_begin));

    // if no players in this row, return the previous row
    std::vector<double> features = default_features();
    if (row.players.empty()) {
        this->prev_row = this->curr_row;
        return features;
    }

    // Calculate all the features
    avg_min_max_stats(home_players.first, home_players.second, "home",
                      features);
    avg_min_max_stats(away_players.first, away_players.second, "away",
                      features);

    referee_stats(ref_begin, ref_end, ref_speed_it, features);

    convex_stats(home_players.first, home_players.second, home_speed_begin,
                 "home", features);
    convex_stats(away_players.first, away_players.second, away_speed_begin,
                 "away", features);
    convex_stats(both_players.first, both_players.second, both_speed_begin,
                 "player", features);

    distance_stats(home_players.first, home_players.second, "home", features);
    distance_stats(away_players.first, away_players.second, "away", features);

    cluster_stats(both_players.first, both_players.second, "player", features);
    cluster_stats(home_players.first, home_players.second, "home", features);
    cluster_stats(away_players.first, away_players.second, "away", features);

    linearity_stats(both_players.first, both_players.second, features);

    player_mixing_stats(both_players.first, both_players.second, features);

    // use the last usable value for features that couldn't be computed
    for (size_t i = 0; i < features.size(); ++i) {
        if (features[i] == feature::default_value()) {
            features[i] = this->prev_features[i];
        }
    }

    // set the last usable value for features that was computed
    for (size_t i = 0; i < features.size(); ++i) {
        if (features[i] != feature::default_value()) {
            this->prev_features[i] = features[i];
        }
    }

    // store the previous row for calculation that require it such as speed
    this->prev_row = this->curr_row;

    return features;
}

}; // namespace feature
