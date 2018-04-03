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

#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "feature_computation.hpp"
#include "parser.hpp"
#include "utils.hpp"

/**
 * @brief Atomic variable that holds the last signal sent to this program.
 *
 * This variable is used to detect interrupts by the user and exit the
 * program gracefully.
 */
static std::sig_atomic_t g_signal_status;

/**
 * @brief Convert the given half, minute, second triple to a unique second
 * value.
 *
 * @param half Half of the match.
 * @param minute minute of the match.
 * @param second second of the match.
 *
 * @return A unique second value corresponding to given (h, m, s) triple.
 */
static size_t hms(size_t half, size_t minute, size_t second) {
    return 3600 * half + 60 * minute + second;
}

/**
 * @brief Flip x coordinates of all players in the given row so that players in
 * the left half of the pitch are in the right, and vice versa.
 *
 * This function flips all the players in the given feature::Row object in
 * place. For a feature::Row containing players with x coordinates 10 and 70,
 * the x coordinates after this function exits will be 95 and 35. It is
 * assumed that the length of the football pitch is 105 metres.
 *
 * @param row feature::Row object to modify in-place so that all the players in
 * it flip their halves in the pitch.
 */
static void flip_players(feature::Row& row) {
    for (auto& player : row.players) {
        player.x = 105 - player.x;
    }
}

/**
 * @brief Computes one second apart features from unsmoothed, 100 millisecond
 * apart raw player coordinate data.
 *
 * This function takes filepaths for the raw data and the output feature data,
 * computes features for each second of the raw data and writes them te output
 * file.
 *
 * The format of the given raw file must be exactly as described in
 * feature_construction.ipynb notebook under notebooks directory of this
 * repository. In this format, a raw coordinate data contains player coordinates
 * for every 100 ms of the match. However, this function computes features for
 * every 1000 ms (1 second). This is done by considering only the first data
 * point in all 10 points for a given second.
 *
 * @param raw_filepath Relative filepath to the raw player coordinate data.
 * @param feature_filepath Relative filepath to output feature data.
 */
static void features_from_raw(const std::string& raw_filepath,
                              const std::string& feature_filepath) {
    // feature data buffer
    std::ostringstream out;

    // write feature names
    auto feature_names = feature::feature_list();
    out << "half,minute,second";
    for (const auto& name : feature_names) {
        out << "," << name;
    }
    out << '\n';

    std::string line;
    feature::Computer fc;
    size_t prev_hms = hms(1, 0, -1);

    // read boolean values
    std::ifstream in(raw_filepath);
    bool converted, home_left;
    in >> converted >> home_left;

    // flush newline
    std::getline(in, line);

    while (std::getline(in, line)) {
        // if we have SIGINT
        if (g_signal_status == SIGINT) {
            std::cerr << "\nInterrupt: Exiting program" << std::endl;
            return;
        }
        // compute features
        feature::Row row = parse_line(line);
        size_t curr_hms = hms(row.half, row.minute, row.second);
        if (curr_hms != prev_hms) {
            // if the x values are not converted, we need to do manual flipping.
            if (!converted) {
                if ((home_left && row.half == 2) ||
                    (!home_left && row.half == 1)) {
                    flip_players(row);
                }
            }

            auto features = fc.compute_features(row);

            // write hms
            out << row.half << "," << row.minute << "," << row.second;
            // write the features
            for (const double value : features) {
                out << "," << std::fixed << std::setprecision(12) << value;
            }
            out << '\n';

            // update prev_hms
            prev_hms = curr_hms;
        }
    }

    // write the computed features to output file
    std::ofstream feature_file(feature_filepath);
    feature_file << out.str();
}

/**
 * @brief Function to set the given signal to atomic global g_signal_status
 * variable.
 *
 * @param signal Signal to set.
 */
static void signal_setter(int signal) { g_signal_status = signal; }

/**
 * @brief Print program usage and command line argument help to the given output
 * stream.
 *
 * @param os Output stream to print info.
 * @param program_name Name of the program to print in usage.
 */
static void print_usage(std::ostream& os, const std::string& program_name) {
    os << "feature" << std::endl;
    os << "=======" << std::endl;
    os << "Usage: " << program_name << " <rawdata_path> <out_feature_path>"
       << std::endl
       << std::endl;
    os << "Reads raw data from the given file in <rawdata_path> and"
          "\ncomputes features for each second of the game."
       << std::endl;
    os << std::endl;
    os << "Raw data must contain position data of all players for\n"
       << "each 100 milliseconds of the game." << std::endl;
    os << std::endl;
    os << "To learn more about raw data format, refer to\n"
       << "feature_construction.ipynb" << std::endl;
}

/**
 * @brief Main function.
 *
 * This function reads rawdata and output feature filepaths from command
 * line arguments, computes the features and writes them to output file by
 * calling features_from_raw function.
 */
int main(int argc, char** argv) {
    // set signal_setter to SIGINT signals.
    std::signal(SIGINT, signal_setter);

    // Number of arguments must be 3, including program name.
    if (argc != 3) {
        // print usage
        print_usage(std::cout, argv[0]);
        return -1;
    }

    const std::string raw_filepath{argv[1]};
    const std::string feature_filepath{argv[2]};
    features_from_raw(raw_filepath, feature_filepath);

    return 0;
}
