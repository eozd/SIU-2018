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

#include <iomanip>

#include "player.hpp"
#include <utils.hpp>

namespace feature {

Player::Player(int type_, int id_, int jersey_, double x_, double y_)
    : type(type_), id(id_), jersey(jersey_), x(x_), y(y_) {}

Player::Player() : Player(-1, -1, -1, -1, -1) {}

Player::Player(double x_, double y_) : Player() {
    this->x = x_;
    this->y = y_;
}

bool operator==(const Player& p1, const Player& p2) {
    return (p1.type == p2.type) & (p1.id == p2.id) & (p1.jersey == p2.jersey);
}

bool operator!=(const Player& p1, const Player& p2) { return !(p1 == p2); }

std::ostream& operator<<(std::ostream& os, const Player& p) {
    using namespace std;
    constexpr char delim[] = "    ";
    os << "Player(type=" << setw(1) << p.type << ',' << delim
       << "id=" << setw(6) << p.id << ',' << delim << "jersey=" << setw(2)
       << p.jersey << ',' << delim << "x=" << setw(6) << fixed
       << setprecision(2) << p.x << ',' << delim << "y=" << setw(6) << fixed
       << setprecision(2) << p.y << ")";
    return os;
}

}; // namespace feature
