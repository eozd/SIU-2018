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

#include <cmath>
#include <fstream>
#include <unordered_map>

#include "utils.hpp"

std::string read_bytes(const std::string& filepath) {
    std::ifstream ifs(filepath, std::ifstream::binary);
    // Get the size of the file
    ifs.seekg(0, ifs.end);
    int len = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    // read the bytes into buffer
    std::string buffer(len, 0);
    ifs.read(&buffer[0], len);

    return buffer;
}

std::vector<std::string> str_split(const std::string& str, const char delim) {
    std::vector<std::string> accum;
    size_t curr = 0, prev = 0;
    while (curr != std::string::npos) {
		// find the next index
        curr = str.find(delim, prev);

		// create a substring
        auto part = str.substr(prev, curr - prev);
        accum.push_back(part);

		// advance the iterator
        prev = curr + 1;
    }
    return accum;
}

void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](int ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

bool close(double d1, double d2, double eps) { return fabs(d1 - d2) <= eps; }

double dist(double x1, double y1, double x2, double y2) {
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

double gini_impurity(const std::vector<int>& labels) {
    if (labels.size() == 0)
        return 0;

	// count the occurence count of each label
    std::unordered_map<int, int> counts;
    for (const int label : labels) {
        counts[label]++;
    }

    double n = static_cast<double>(labels.size());
    double gini = 0;
    for (const auto& p : counts) {
        double prob = p.second / n;
        gini += prob * (1 - prob);
    }
    return gini;
}
