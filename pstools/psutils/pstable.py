from typing import Union
import pandas as pd

def fill_empty_val(name: str, fillVal: float):
    # TODO extend functionality of this method
    print("Setting new seed {0} to default value {1}.".format(name, fillVal))
    return fillVal 


def find_table_value(pstable: pd.DataFrame, seed: str,
        col: str, newSeedVal: float, includeBptxSeeds: bool) -> Union[int, float, str]:
    """
    Retrieve a specific value corresponding to a given seed and column name.

    Parameters
    ----------
    pstable : pandas.DataFrame
        PS table from which the value should be obtained
    seed : str
        Name of the desired seed
    col : str
        Name of the desired column
    newSeedVal : float
        Default PS value for new seeds

    Returns
    -------
    Cell content corresponding to seed and col. If cell is not found, the
    function fill_empty_val is called and its return value is forwarded.

    """

    if 'Name' not in pstable.columns:
        raise KeyError('PS table does not have a column \'Name\'')

    if (not includeBptxSeeds) and ("Bptx" in seed):
        # Only print a message if the prescale wasn't already 0, as --includeBptx will not change the value in that case
        if (pstable.loc[pstable['Name'] == seed])[col].values[0] != 0:
            print("Setting prescale for {0} to 0. Change this by using --includeBptx.".format(seed))
        return 0
    elif seed in pstable['Name'].values:
        return (pstable.loc[pstable['Name'] == seed])[col].values[0]
    else:
        return fill_empty_val(seed,newSeedVal)


def make_empty_table(pstable: pd.DataFrame) -> pd.DataFrame:
    """
    Create a new, empty dataframe with the structure of a given one.

    Parameters
    ----------
    pstable : pandas.DataFrame
        DataFrame that serves as a template for the new DataFrame

    Returns
    -------
    pandas.DataFrame
        DataFrame that has the same column structure as the input DataFrame, but
        is empty.

    """

    empty_table = pd.DataFrame(columns=pstable.columns)
    return empty_table

