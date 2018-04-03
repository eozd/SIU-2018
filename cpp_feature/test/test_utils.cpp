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

#include <catch/catch.hpp>

#include <fstream>
#include <iterator>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <utils.hpp>

TEST_CASE("Test utils::read_bytes", "[utils::read_bytes]") {
    SECTION("Non-existing file raises exception") {
        REQUIRE_THROWS(read_bytes(""));
    }

    SECTION("All the bytes are read and returned") {
        const std::string filepath = "tests";
        std::string function_bytes = read_bytes(filepath);

        std::ifstream ifs(filepath, std::ifstream::binary);
        ifs.seekg(0, std::ios::end);
        size_t num_bytes = ifs.tellg();

        REQUIRE(num_bytes == function_bytes.size());
    }
}

TEST_CASE("Test utils::str_split", "[utils::str_split]") {
    SECTION("Comma delimeter") {
        const char delim = ',';

        SECTION("Empty string") {
            std::string input{""};
            std::vector<std::string> res{""};

            REQUIRE(str_split(input, delim) == res);
        }

        SECTION("No delimeter char in the given string") {
            std::string input{"abcde"};
            std::vector<std::string> res{"abcde"};

            REQUIRE(str_split(input, delim) == res);
        }

        SECTION("String consisting of only the delimeter char") {
            std::string input{",,,"};
            std::vector<std::string> res{"", "", "", ""};

            REQUIRE(str_split(input, delim) == res);
        }

        SECTION("A regular input case") {
            std::string input{"ab,cd,ef,gh"};
            std::vector<std::string> res{"ab", "cd", "ef", "gh"};

            REQUIRE(str_split(input, delim) == res);
        }

        SECTION("String starts with delimeter char") {
            std::string input{",ab,cd"};
            std::vector<std::string> res{"", "ab", "cd"};

            REQUIRE(str_split(input, delim) == res);
        }

        SECTION("String ends with delimeter char") {
            std::string input{"ab,cd,"};
            std::vector<std::string> res{"ab", "cd", ""};

            REQUIRE(str_split(input, delim) == res);
        }
    }

    SECTION("Space delimeter") {
        const char delim = ' ';
        std::string input{"a 1 2 e d qaoe"};
        std::vector<std::string> res{"a", "1", "2", "e", "d", "qaoe"};

        REQUIRE(str_split(input, delim) == res);
    }

    SECTION("Newline delimeter") {
        const char delim = '\n';
        std::string input{"a\nb\nc\n\n"};
        std::vector<std::string> res{"a", "b", "c", "", ""};

        REQUIRE(str_split(input, delim) == res);
    }

    SECTION("Backslash delimeter") {
        const char delim = '\\';
        std::string input{"abc\\123 \\457\n12"};
        std::vector<std::string> res{"abc", "123 ", "457\n12"};

        REQUIRE(str_split(input, delim) == res);
    }
}

TEST_CASE("Test utils::rtrim", "[utils::rtrim]") {
    std::string s{""};
    rtrim(s);
    REQUIRE(s == "");

    s = "  ";
    rtrim(s);
    REQUIRE(s == "");

    s = "abc ";
    rtrim(s);
    REQUIRE(s == "abc");

    s = "abc\n\n";
    rtrim(s);
    REQUIRE(s == "abc");

    s = "\t\t123\t\n";
    rtrim(s);
    REQUIRE(s == "\t\t123");

    s = "\n1 2\n35 \n\t";
    rtrim(s);
    REQUIRE(s == "\n1 2\n35");
}

TEST_CASE("Test utils::close", "[utils::close]") {
    REQUIRE(close(1.1235473727, 1.123547372));

    REQUIRE(!close(1, -1));

    double a = 0.1232542;
    REQUIRE(close(a - a, 0));
}

TEST_CASE("Test utils::dist", "[utils::dist]") {
    double d = dist(3, 4, 0, 0);
    REQUIRE(d == Approx(5));

    d = dist(0, 0, 0, 0);
    REQUIRE(d == Approx(0));

    d = dist(3, 4, 3, -4);
    REQUIRE(d == Approx(8));

    d = dist(3, 4, 8, 4);
    REQUIRE(d == Approx(5));

    d = dist(std::numeric_limits<double>::max(),
             std::numeric_limits<double>::lowest(),
             std::numeric_limits<double>::max(),
             std::numeric_limits<double>::lowest());
    REQUIRE(d == Approx(0));
}

TEST_CASE("Test utils::gini_impurity", "[utils::gini_impurity]") {
    std::vector<int> labels;

    labels = {0, 1, 0, 1};
    REQUIRE(gini_impurity(labels) == Approx(0.5));

    labels = {1, 1, 1, 1};
    REQUIRE(gini_impurity(labels) == Approx(0));

    labels = {};
    REQUIRE(gini_impurity(labels) == Approx(0));

    labels = {0, 1, 2};
    REQUIRE(gini_impurity(labels) == Approx(2. / 3));
}

TEST_CASE("Test utils::is_inv_map", "[utils::is_inv_map]") {
    SECTION("Non-matching types do not compile") {
        /* std::map<int, double> m; */
        /* std::map<std::string, int> im; */

        /* REQUIRE(is_inv_map(m, im)); */
    }

    SECTION("Empty maps are inverse of each other") {
        std::map<int, double> m;
        std::map<double, int> im;

        REQUIRE(is_inv_map(m, im));
    }

    SECTION("A regular example") {
        std::unordered_map<std::string, size_t> m{
            {"abc", 42}, {"12", 12}, {"", 0}};
        std::unordered_map<size_t, std::string> im{
            {42, "abc"}, {12, "12"}, {0, ""}};

        REQUIRE(is_inv_map(m, im));
    }

    SECTION("Different STL map types can be used together") {
        std::map<int, bool> m{{0, false}, {1, true}};
        std::unordered_map<bool, int> im{{false, 0}, {true, 1}};

        REQUIRE(is_inv_map(m, im));
    }
}

TEST_CASE("Test utils::inverse_map", "[utils::inverse_map]") {
    SECTION("Empty maps") {
        std::map<int, std::string> m;
        std::map<std::string, int> res;

        inverse_map(m.begin(), m.end(), inserter(res, res.end()));
        REQUIRE(is_inv_map(m, res));
    }

    SECTION("A regular case") {
        std::map<int, std::string> m{{1, "a"}, {2, "b"}, {3, "c"}};
        std::map<std::string, int> res;

        inverse_map(m.begin(), m.end(), inserter(res, res.end()));
        REQUIRE(is_inv_map(m, res));
    }
}
