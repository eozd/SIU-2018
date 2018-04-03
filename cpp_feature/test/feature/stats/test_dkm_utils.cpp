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

#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>

#include <feature/stats/dkm_utils.hpp>

using namespace feature::details;

TEST_CASE("Test dkm_utils::get_cluster", "[dkm_utils::get_cluster]") {

    SECTION("Non-empty and same size points and labels") {
        dkm_point_seq<2> points{
            {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
            {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9},
        };
        dkm_label_seq labels{0, 2, 1, 1, 0, 2, 2, 1, 1, 0};

        SECTION("Correct points for existing labels") {
            auto cluster = get_cluster(points, labels, 0);
            dkm_point_seq<2> res{{0, 0}, {4, 4}, {9, 9}};
            REQUIRE(cluster == res);

            cluster = get_cluster(points, labels, 1);
            res = {{2, 2}, {3, 3}, {7, 7}, {8, 8}};
            REQUIRE(cluster == res);

            cluster = get_cluster(points, labels, 2);
            res = {
                {1, 1},
                {5, 5},
                {6, 6},
            };
            REQUIRE(cluster == res);
        }

        SECTION("Empty set of points for non-existing labels") {
            auto cluster = get_cluster(points, labels, 4);
            REQUIRE(cluster == dkm_point_seq<2>());
        }
    }

    SECTION("Empty points and labels") {
        dkm_point_seq<2> points;
        dkm_label_seq labels;

        SECTION("Empty set of points") {
            auto cluster = get_cluster(points, labels, 0);
            REQUIRE(cluster == dkm_point_seq<2>());
        }
    }

    SECTION("points and labels sequences with different sizes") {
        dkm_point_seq<2> points{{0, 1}, {2, 3.5}};
        dkm_label_seq labels{2, 4, 1, 1};

        SECTION("Expecting an exception") {
            REQUIRE_THROWS(get_cluster(points, labels, 2));
        }
    }
}

TEST_CASE("Test dkm_utils::dist_to_center", "[dkm_utils::dist_to_center]") {

    SECTION("Non-empty sequence of points") {
        dkm_point_seq<2> points{{1, 5},       {2.2, 3},   {8, 12},
                                {11.4, 4.87}, {0.27, 50}, {1, 1}};
        std::vector<double> out(points.size());
        dkm_point<2> center{17.2, 24.5};

        std::vector<double> res{25.3513, 26.2154, 15.5206,
                                20.4689, 30.6084, 28.5427};

        dist_to_center(points, center, out.begin());

        for (size_t i = 0; i < out.size(); ++i)
            REQUIRE(Approx(out[i]) == res[i]);
    }

    SECTION("Empty sequence of points don't change out parameter") {
        dkm_point_seq<2> points;
        std::vector<double> out{1, 2, 3, 4, 5};
        dkm_point<2> center{5, 4};

        auto prev_out = out;
        dist_to_center(points, center, out.begin());

        REQUIRE(out == prev_out);
    }
}

TEST_CASE("Test dkm_utils::sum_dist", "[dkm_utils::sum_dist]") {
    SECTION("Non-empty sequence of points") {
        dkm_point_seq<2> points{{1, 5},       {2.2, 3},   {8, 12},
                                {11.4, 4.87}, {0.27, 50}, {1, 1}};
        std::vector<double> out(points.size());
        dkm_point<2> center{17.2, 24.5};

        std::vector<double> res{25.3513, 26.2154, 15.5206,
                                20.4689, 30.6084, 28.5427};

        REQUIRE(sum_dist(points, center) == Approx(146.7073));
    }

    SECTION("Empty sequence of points returns 0") {
        dkm_point_seq<2> points;
        dkm_point<2> center{5, 4};

        REQUIRE(sum_dist(points, center) == 0);
    }
}

TEST_CASE("Test dkm_utils::means_inertia", "[dkm_utils::means_inertia]") {

    SECTION("Non-empty set of points, fixed 3 clusters") {
        dkm_point_seq<2> points{
            {66.01742226, 48.70477854}, {62.30094932, 108.44049522},
            {39.60740312, 12.07668535}, {35.57096194, -7.10722525},
            {39.90890238, 61.89509695}, {27.5850295, 85.50226002},
            {51.14012591, 27.90650051}, {58.6414776, 31.97020798},
            {14.75127435, 69.36707669}, {73.66255253, 84.73455103},
            {-1.31034384, 66.10406579}, {41.91865987, 56.5003107},
            {33.31116528, 45.92203855}, {57.12362692, 37.73753163},
            {2.68915431, 51.35514789},  {39.76543196, -5.99499795},
            {72.64312341, 61.43756623}, {30.97140948, 29.49960625},
            {25.31232669, 35.88059477}, {57.67046396, 35.05019015}};
        dkm_point_seq<2> centroids{{10, 10}, {20, 20}, {40, 30}};
        dkm_label_seq labels{0, 0, 1, 2, 2, 1, 1, 0, 0, 0,
                             1, 1, 2, 1, 0, 0, 1, 2, 1, 0};
        dkm_means<2> means{centroids, labels};

        double inertia = 0;
        for (size_t i = 0; i < labels.size(); ++i) {
            auto center = centroids[labels[i]];
            auto point = points[i];
            inertia += dist(point[0], point[1], center[0], center[1]);
        }

        REQUIRE(Approx(inertia) == means_inertia(points, means));
    }

    SECTION("Empty set of points should give 0 inertia") {
        dkm_point_seq<2> points;
        dkm_means<2> means;

        REQUIRE(means_inertia(points, means) == Approx(0));
    }
}

TEST_CASE("Test dkm_utils::get_best_means", "[dkm_utils::get_best_means]") {

    SECTION("Non-empty points") {
        dkm_point_seq<2> points{
            {66.01742226, 48.70477854}, {62.30094932, 108.44049522},
            {39.60740312, 12.07668535}, {35.57096194, -7.10722525},
            {39.90890238, 61.89509695}, {27.5850295, 85.50226002},
            {51.14012591, 27.90650051}, {58.6414776, 31.97020798},
            {14.75127435, 69.36707669}, {73.66255253, 84.73455103},
            {-1.31034384, 66.10406579}, {41.91865987, 56.5003107},
            {33.31116528, 45.92203855}, {57.12362692, 37.73753163},
            {2.68915431, 51.35514789},  {39.76543196, -5.99499795},
            {72.64312341, 61.43756623}, {30.97140948, 29.49960625},
            {25.31232669, 35.88059477}, {57.67046396, 35.05019015}};

        SECTION("Invoke dkm k-means with 5 clusters 10 separate times") {
            std::vector<dkm_means<2>> means_list;
            for (size_t i = 0; i < 10; ++i) {
                means_list.push_back(dkm::kmeans_lloyd(points, 5));
            }

            // best means via dkm_utils
            auto best_means = get_best_means(points, means_list);
            auto best_iter =
                std::find(means_list.begin(), means_list.end(), best_means);
            size_t best_index = static_cast<size_t>(
                std::distance(means_list.begin(), best_iter));
            double best_inertia = means_inertia(points, best_means);

            // best means via calculation
            double min_inertia = std::numeric_limits<double>::max();
            size_t min_inertia_index = 0;
            for (size_t i = 0; i < means_list.size(); ++i) {
                double inertia = means_inertia(points, means_list[i]);
                if (inertia < min_inertia) {
                    min_inertia = inertia;
                    min_inertia_index = i;
                }
            }

            REQUIRE(Approx(best_inertia) == min_inertia);
            REQUIRE(best_index == min_inertia_index);
        }
    }
}
