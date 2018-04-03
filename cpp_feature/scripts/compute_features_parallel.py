# Copyright 2018 Esref Ozdemir
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


"""
Compute features for all raw files in a directory in parallel. This script uses
C++ feature computing executable.
"""

import argparse
import sys
import multiprocessing
import os
from os.path import join
import subprocess
import re
from signal import signal, SIGINT

# constants to adjust.
# raw match data files are assumed to be in <id>_rawdata.txt format.
raw_regex = re.compile(r'\d+_rawdata.txt')
# output folder
feature_dir = 'out'
# number of processes to spawn in parallel
num_processes = multiprocessing.cpu_count()


def interrupt_handler(signal, frame):
    subprocess.call('pkill feature', shell=True)
    sys.exit()


def process_raw(raw_path):
    """
    Process a raw data file with name raw_name inside raw_dir.

    This function externally calls ./feature executable to compute the
    features and write them to feature_dir with a corresponding feature filename.
    """
    print('Processing {}'.format(raw_path))

    match_id = raw_path[raw_path.rfind('/') + 1:raw_path.rfind('_')]
    feature_name = '{}_feature.csv'.format(match_id)
    feature_path = os.path.join(feature_dir, feature_name)

    subprocess.call(
        './feature {} {}'.format(raw_path, feature_path), shell=True)


def main():
    """
    Get all the raw data filenames from raw_dir and call map process_raw
    function to raw_names in parallel.
    """
    if len(sys.argv) != 2:
        print('Error')
        print('Usage: {} <rawdata_dir>'.format(sys.argv[0]))
        sys.exit()

    raw_dir = sys.argv[1]

    signal(SIGINT, interrupt_handler)
    raw_files = (os.path.join(raw_dir, name) for name in os.listdir(raw_dir)
                 if raw_regex.match(name))
    pool = multiprocessing.Pool(processes=num_processes)
    pool.map(process_raw, raw_files)


if __name__ == '__main__':
    main()
