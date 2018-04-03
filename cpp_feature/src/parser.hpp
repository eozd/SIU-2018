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

#include <feature/row.hpp>

/**
 * @brief Parse a line from raw match data and return it as a Row object.
 *
 * Refer to the format of raw match data in feature_construction.ipynb notebook.
 *
 * @param line A single line of the raw match data to be parsed.
 *
 * @return Row object that keeps all the information in the given raw match
 * line.
 */
feature::Row parse_line(const std::string& line);
