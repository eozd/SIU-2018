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


import warnings
import functools
import itertools
from collections import namedtuple
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from math import sqrt


def dist(x0, y0, x1, y1):
    """
    Return Euclidean distance between points (x0, y0) and (x1, y1).

    Parameters
    ----------
    x0 : float
        x coordinate of the 1st point.

    y0 : float
        y coordinate of the 1st point.

    x1 : float
        x coordinate of the 2nd point.

    y1 : float
        y coordinate of the 2nd point.

    Returns
    -------
    dist : float
        Euclidean distance between (x0, y0) and (x1, y1) points.
    """
    return sqrt((x0 - x1)**2 + (y0 - y1)**2)


def timestamp_to_min(t, half_time_threshold=10):
    '''
    Converts a timestamp array for a match to a float array containing
    the values in minute format, starting from 0.

    A timestamp array contains a timestamp gap between two halves of the match.
    This function removes the gap such that the second half of the match starts
    at 45'.

    Parameters
    ----------
    t : ndarray
        Timestamp array of a match.

    half_time_threshold : scalar (int, float, etc.)
        Minimum amount of minutes half-time would take.

    Returns
    -------
    out : ndarray
        A minute array, starting from 0, going until the end of the match.

    Complexity
    ----------
    Linear in the size of parameter t.
    '''
    t = np.asarray(t)

    # milliseconds to minutes
    out = (t - t[0]) / 1000 / 60

    # difference between two consecutive timestamps
    diff = out[1:] - out[:-1]

    # array index of half time
    half_index = np.where(diff >= half_time_threshold)[0]
    assert len(half_index) == 1, 'There are {} indices after half-time'.format(
        len(half_index))

    half_index = half_index[0]

    # half time duration
    half_time = diff[half_index]

    # remove excess half time duration from second half minutes
    out[half_index + 1:] -= half_time

    return out


def group_indices(arr):
    """
    Given an (n, 1) or (n, 2) ndarray, returns the begin and end indices of
    each group of consecutive same elements.

    Parameters
    ----------
    arr : ndarray
        (n, 1) or (n, 2) ndarray of elements

    Returns
    -------
    out : ndarray
        begin and end indices of each group as a 1D ndarray. Each element is
        the index to end-past-1 element of the previous group and the index
        to the first element of the current group.

    Examples
    --------
    >>> import numpy as np
    >>> arr = np.array([
    ...     [1, 2, 3],
    ...     [1, 2, 3],
    ...     [1, 3, 3],
    ...     [1, 3, 3],
    ...     [1, 3, 4],
    ...     [2, 3, 4],
    ...     [2, 3, 4],
    ...     [2, 3, 4],
    ... ])
    >>> res = np.array([0, 2, 4, 5, 8])
    >>> np.array_equal(res, group_indices(arr))
    True

    Complexity
    ----------
    Linear in the size of parameter arr.
    """
    # difference of consecutive pairs
    change = np.abs(arr[1:] - arr[:-1])
    if len(arr.shape) == 2:
        change = np.sum(change, axis=1)

    # changepoints
    change_indices = np.where(change != 0)[0]

    out = np.empty(len(change_indices) + 2, dtype='int')
    out[0] = 0
    out[-1] = len(arr)

    # put the change indices in between begin and end.
    out[1:-1] = change_indices + 1

    return out


def hms_to_sec(hms):
    """
    Converts a given half-min-sec iterable to a unique second value.

    Parameters
    ----------
    hms : Iterable (tuple, list, array, ...)
        A pack of half-min-sec values. This may be a tuple (10, 5, 2), list
        [10, 5, 2], ndarray, and so on

    Returns
    -------
    out : scalar (int, float, ...) depending on the input values.
        Unique second value.
    """
    h, m, s = hms
    return 3000 * h + 60 * m + s


def hms_to_sec_vec(hms_arr):
    """
    Converts a given 2D hms ndarray to a 1D second ndarray. Each row of the given
    2D array must contain a hms iterable. The resulting array contains the
    corresponding second value of each hms.

    Parameters
    ----------
    hms_arr : ndarray
        A 2D numpy array with half-min-sec columns, respectively.

    Returns
    -------
    out : ndarray
        1D numpy array with corresponding second values of each row.
    """
    h, m, s = hms_arr.T
    return 3000 * h + 60 * m + s


def remove_player_features(df):
    """
    Removes all columns from the dataframe whose name starts with 'player'.

    Parameters
    ----------
    df : pandas.DataFrame
        pandas.DataFrame object.

    Returns
    -------
    out : pandas.DataFrame
        A new dataframe with columns whose names start with player are removed.

    Complexity
    ----------
    Linear in the number of columns of parameter df.
    """
    # get columns not starting with player
    cols = list(df.columns)
    cols = [c for c in cols if not c.startswith('player')]

    # construct a new dataframe with columns as cols
    out = df.loc[:, cols].copy()

    return out


def plot_hbar_nameval(names, values, xlabel, max_bars=30):
    """
    Plots a horizontal bar chart with names as labels and values as the length
    of the bars.

    Parameters
    ----------
    names : Iterable
        Bar labels.

    values : Iterable
        Corresponding value of each label given in names.

    xlabel : str
        Label of the horizontal axis.

    max_bars : int
        Maximum number of horizontal bars to plot.

    Returns
    -------
    fig : matplotlib.figure
        Plot figure.

    ax : matplotlib.Axes
        matplotlib.Axes object with horizontal bar chart plotted.
    """
    name_val = list(zip(names, values))
    name_val.sort(key=lambda t: t[1], reverse=True)
    if len(name_val) > max_bars:
        name_val = name_val[:max_bars]
    names, values = zip(*name_val)

    plt.rcdefaults()
    fig, ax = plt.subplots()

    y_pos = np.arange(len(names))

    ax.barh(y_pos, values, align='center')
    ax.set_yticks(y_pos)
    ax.set_yticklabels(names)
    ax.invert_yaxis()
    ax.set_xlabel(xlabel)
    fig.tight_layout()

    plt.close()
    return fig, ax


def plot_confusion_matrix(cm,
                          classes,
                          xlabel,
                          ylabel,
                          normalize=False,
                          cmap=plt.cm.Blues):
    """
    Plot the given confusion matrix cm as a matrix using and return the
    resulting axes object containing the plot.

    Parameters
    ----------
    cm : ndarray
        Confusion matrix as a 2D numpy.array.

    classes : list of str
        Names of classified classes.

    xlabel : str
    Label of the horizontal axis.

    ylabel : str
    Label of the vertical axis.

    normalize : bool
        If True, the confusion matrix will be normalized. Otherwise, the values
        in the given confusion matrix will be plotted as they are.

    cmap : matplotlib.colormap
        Colormap to use when plotting the confusion matrix.

    Returns
    -------
    fig : matplotlib.figure
        Plot figure.

    ax : matplotlib.Axes
        matplotlib.Axes object with horizontal bar chart plotted.

    References
    ----------
    http://scikit-learn.org/stable/auto_examples/model_selection/plot_confusion_matrix.html#sphx-glr-auto-examples-model-selection-plot-confusion-matrix-py
    """
    vmin = None
    vmax = None
    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        vmin = 0
        vmax = 1

    fig, ax = plt.subplots()
    cax = ax.imshow(
        cm, interpolation='nearest', vmin=vmin, vmax=vmax, cmap=cmap)
    fig.colorbar(cax)

    tick_marks = np.arange(len(classes))
    ax.set_xticks(tick_marks)
    ax.set_xticklabels(classes, rotation=45)

    ax.set_yticks(tick_marks)
    ax.set_yticklabels(classes)

    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        cell_str = '{:.2f}'.format(cm[i, j]) if normalize else str(cm[i, j])
        ax.text(
            j,
            i,
            cell_str,
            horizontalalignment="center",
            color="white" if cm[i, j] > thresh else "black")

    fig.tight_layout()
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)

    plt.close()
    return fig, ax


def merge_home_away(event_ids, exclude=(0, -20, -21)):
    """
    Merge home and away event ids into a single event.

    For an event id xy, home version is -xy1 and away version is
    -xy0.

    Parameters
    ----------
    event_ids : ndarray
        1D numpy array of event_ids.

    exclude : tuple
        Events that will not be merged.

    Returns
    -------
    out : ndarray
        A new numpy array of event_ids with home and away events merged.

    Examples
    --------
    >>> ids = np.array([0, -20, -21, -600, -601, -620, -621, -800, -801])
    >>> res = np.array([0, -20, -21, 60, 60, 62, 62, 80, 80])
    >>> np.array_equal(merge_home_away(ids), res)
    True
    """
    out = np.copy(event_ids)
    merge_mask = ~np.in1d(out, exclude)
    out[merge_mask] = -out[merge_mask] // 10
    return out


def separate_home_away(event_ids,
                       team_ids,
                       custom,
                       home,
                       exclude=(0, -20, -21)):
    """
    Separates event ids into home and away versions.

    For an event id xy, home version is -xy1 and away version is
    -xy0.

    Parameters
    ----------
    event_ids : ndarray
        Array of event_ids.

    team_ids : ndarray
        Corresponding array of team_ids.

    custom : ndarray
        Array of custom event ids.

    home : int
        id of the home team.

    exclude : tuple
        Events that will not be separated.

    Returns
    -------
    out : ndarray
        A new array of event_ids with home and away events separated.

    Examples
    --------
    >>> ids = np.array([0, -20, -21, 60, 60, 62, 62, 80, 93, 93])
    >>> custom = np.array([-1, -1, -1, -1, -1, -1, -1, -1, 1, 3])
    >>> home = 20
    >>> team = np.array([20, 30, 30, 20, 30, 30, 20, 20, 30, 20])
    >>> res = np.array([0, -20, -21, -601, -600, -620, -621, -801, -930, -930])
    >>> np.array_equal(res, separate_home_away(ids, team, custom, home))
    True
    """
    out = np.copy(event_ids)

    # filter excluded events
    separate_mask = ~np.in1d(out, exclude)

    # bitmask of home events
    is_home = (team_ids[separate_mask] == home).astype(int)

    # construct separated event ids
    out[separate_mask] = -(out[separate_mask] * 10 + is_home)

    # correction for penalty events
    penalty_mask = event_ids == 93

    # if custom id is 0 or 3, penalty side must be flipped
    wrong_custom_mask = (custom == 0) | (custom == 3)
    indices = np.where(penalty_mask & wrong_custom_mask)[0]

    # flip penalty ids
    for i in indices:
        eid = out[i]
        if eid == -930:
            out[i] = -931
        elif eid == -931:
            out[i] = -930
        else:
            raise RuntimeError(
                'Penalty ID ({}) is not -930 or -931'.format(eid))

    return out


def deprecated(msg):
    """
    This is a decorator which can be used to mark functions as deprecated. It
    will result in a warning being emmitted when the function is used.

    Parameters
    ----------
    msg : str
        Deprecation message.

    References
    ----------
    https://stackoverflow.com/questions/2536307/decorators-in-the-python-standard-lib-deprecated-specifically
    """

    def decorator(func):
        @functools.wraps(func)
        def new_func(*args, **kwargs):
            warnings.simplefilter('always',
                                  DeprecationWarning)  # turn off filter
            warnings.warn(msg, category=DeprecationWarning, stacklevel=2)
            warnings.simplefilter('ignore', DeprecationWarning)  # reset filter
            return func(*args, **kwargs)

        return new_func

    return decorator


def read_feature_csv(path):
    """
    Reads home team id, away team id and dataframe from the csv file in the
    given path and returns them.

    This function is used to read feature csv files that are formatted as follows:

    <home_id> <away_id>
    <feature_00>,<feature_01>,...,<feature_0n>
    .
    .
    .
    <feature_m0>,<feature_m1>,...,<feature_mn>

    Parameters
    ----------
    path : str
        Path to the feature csv file.

    Returns
    -------
    home_id : int
        Home team ID

    away_id : int
        Away team ID

    df : pandas.DataFrame
        Feature dataframe.
    """
    with open(path, 'r') as f:
        home_id = f.readline().split()[-1]
        away_id = f.readline().split()[-1]
        df = pd.read_csv(f)

    return home_id, away_id, df


def remove_missing_raw_rows(raw_df):
    """
    Removes rows from raw data dataframe that contains missing player data.
    Missing players <==> (x, y) = (-1, -1).

    Parameters
    ----------
    raw_df : pandas.DataFrame
        Raw data dataframe containing 'x' and 'y' columns.

    Returns
    -------
    out : pandas.DataFrame
        Raw data dataframe with no -1 values in 'x' and 'y' columns.
    """
    missing_mask = np.isclose(raw_df['x'], -1) | np.isclose(raw_df['y'], -1)
    return raw_df[~missing_mask]


def inner_dist(points):
    """
    Given an (n, 2) array of x, y coordinates, calculates inner distance
    between each pair of points and returns the result as a `numpy.ndarray`.

    This function calculates pairwise distances by constructing a matrix of
    size (n, n) where entry (i, j) is the distance between point_i and point_j.
    Then, the distance between each pair is the upper triangle of this matrix.

    Parameters
    ----------
    points : ndarray
        (n, 2) numpy array containing the x and y coordinates.

    Returns
    -------
    out : ndarray
        (n*(n - 1)/2, ) numpy array containing the distance between each
        pair of points.

    Complexity
    ----------
    Time : O(N^2)
    Memory : O(N^2)
    """
    x, y = points.T

    # distance of every pairwise x coordinate
    x_diff = x - x[:, np.newaxis]

    # distance of every pairwise y coordinate
    y_diff = y - y[:, np.newaxis]

    # Euclidean distance
    dist = np.sqrt(x_diff**2 + y_diff**2)

    return dist[np.triu_indices(len(x))]


def gini_impurity(arr):
    """
    Gini impurity of a label array.

    Gini impurity of a label array is calculated as

    \sum_{e \in E} p_e(1 - p_e)

    where E is the set of unique labels and p_e is the frequency of class e.

    Parameters
    ----------
    arr : list, tuple, ndarray, ...
        An array-like object containing class labels that can be compared via
        equality.

    Returns
    -------
    gini : float
        Gini impurity of labels.

    Examples
    --------
    >>> gini_impurity([1, 1, 1, 1])
    0.0

    >>> gini_impurity([0, 0, 1, 0, 1, 1])
    0.5

    >>> gini_impurity([])
    0.0

    >>> np.isclose(2/3, gini_impurity([0, 1, 2]))
    True
    """
    arr = np.asarray(arr)
    n = len(arr)
    _, counts = np.unique(arr, return_counts=True)

    probs = counts / n
    gini = np.sum(probs * (1 - probs))

    return gini


def parse_raw_file(filepath):
    """
    Parse the raw match data txt file at the given filepath and return a
    pandas.DataFrame with

        match_id
        timestamp
        half
        minute
        second
        player_type
        x
        y

    columns.

    player_type column is a key of the following mapping:

    {
        0 : 'home_player',
        1 : 'away_player',
        2 : 'referee',
        3 : 'home_goalkeeper',
        4 : 'away_goalkeeper'
    }

    Parameters
    ----------
    filepath : str
        Path to the raw match data txt file.

    Returns
    -------
    df : pandas.DataFrame
        pandas.DataFrame representation of the raw match data.
    """
    with open(filepath, 'r') as f:
        converted, home_left = [
            bool(num) for num in f.readline().strip().split()
        ]

    # read the rest of file
    data = np.genfromtxt(
        filepath,
        skip_header=1,
        delimiter='\t',
        dtype=(int, int, int, int, int, '|S1000'))

    # separate player_list and extract player_type, x, y for each player
    df_data_list = []
    for row in data:
        row = list(row)
        for player_str in row[-1].decode('utf-8').split():
            comma_sep = player_str.split(',')
            df_data_list.append(row[:-1] + [
                int(comma_sep[0]),
                int(comma_sep[1]),
                float(comma_sep[-2]),
                float(comma_sep[-1])
            ])

    df_data = np.asarray(df_data_list)

    # manual conversion
    if not converted:

        # which half to convert
        half_mask = df_data[:, 2] == (1 if home_left else 2)

        # flip player x coordinates
        df_data[half_mask][:, -2] = 105 - df_data[half_mask][:, -2]

    # construct dataframe
    df = pd.DataFrame(
        columns=[
            'match_id', 'timestamp', 'half', 'minute', 'second', 'player_type',
            'player_id', 'x', 'y'
        ],
        data=df_data)

    # enforce proper types
    df = df.astype(
        dtype={
            'match_id': 'int32',
            'timestamp': 'int32',
            'half': 'int8',
            'minute': 'int8',
            'second': 'int8',
            'player_type': 'int8',
            'player_id': 'int32',
            'x': 'float64',
            'y': 'float64'
        })

    return df


def raw_to_sec_df(raw_df):
    """
    Convert a 100 millisecond apart raw match dataframe to a second apart
    match dataframe by only considering the 1st snapshot of all 10 snapshots
    for a second.

    Parameters
    ----------
    raw_df : pandas.DataFrame
        100 ms apart raw match dataframe.

    Returns
    -------
    sec_df : pandas.DataFrame
        Second apart match dataframe constructed by considering only the 1st
        snapshot out of all 10 snapshots for a second in raw match dataframe.
    """
    # Get hms vector
    data = raw_df.values
    hms = hms_to_sec_vec(data[:, 2:2 + 3])

    # Get timestamp vector
    timestamp = data[:, 1]

    # Final mask that will be applied to raw_df to obtain sec_df
    mask = np.zeros(raw_df.shape[0], dtype=bool)

    # Indices where the hms value changes
    sec_change_indices = group_indices(hms)

    prev_index = 0
    for index in sec_change_indices[1:]:
        # Indices where timestamp changes between prev_index and index
        timestamp_change_indices = group_indices(timestamp[prev_index:index])

        # Begin and end indices of data we are interested in
        beg = timestamp_change_indices[0]
        end = timestamp_change_indices[1]

        # Shift by prev_index to index original raw_df
        beg += prev_index
        end += prev_index

        # We want data in [beg, end).
        mask[beg:end] = True

        prev_index = index

    return raw_df[mask]
