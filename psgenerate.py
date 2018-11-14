import os
import argparse
from typing import Union, Any
import tempfile
import wget
import shutil
import pandas as pd
import xml.etree.ElementTree as ET


def download_file(url: str) -> Union[str, None]:
    """
    Download a file from a given url

    Parameters
    ----------
    url : str
        URL of the file that is to be downloaded

    Returns
    -------
    str, None
        Filepath of the downloaded file, None if the download failed

    """

    tempdir = tempfile.mkdtemp(prefix='psgenerate_')
    local_filepath = os.path.join(tempdir, os.path.basename(url))

    try:
        wget.download(url, local_filepath)
        return local_filepath
    except:
        return None


def read_prescale_table(filepath: Any) -> pd.DataFrame:
    """
    Import an existing xlsx prescale table as pandas dataframe
    
    Parameters
    ----------
    filepath : str, path object
        Location of the input file (can be any format accepted by the pandas
        'read_excel' function), can be a local path or a URL

    Returns
    -------
    pandas.DataFrame
        Imported xlsx file as a DataFrame

    """

    if not os.path.exists(filepath):
        print('\nNo local file found, trying to download {}...'.format(filepath))
        filepath = download_file(filepath)
        if filepath is None:
            raise RuntimeError('File does not exist and/or could not be '
                    'downloaded: {}'.format(filepath))
        else:
            print('\nFile downloaded: {}'.format(filepath))

    data = pd.read_excel(filepath, convert_float=True)
    return data


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


def get_seeds_from_xml(filepath: str) -> (list,list):
    """
    Import seeds and indices from an existing L1 Menu XML file.

    Parameters
    ----------
    filepath: str
        Location of the input file, can be a local path or a URL

    Returns
    -------
    list of str, list of int
        Seed names and corresponding indices ('bits') as two separate lists

    """

    if not os.path.exists(filepath):
        print('No local file found, trying to download {}...'.format(filepath))
        filepath = download_file(filepath)
        if filepath is None:
            raise RuntimeError('File does not exist and/or could not be '
                    'downloaded: {}'.format(filepath))

    tree = ET.parse(filepath)
    root = tree.getroot()

    seeds = [name[0].text for name in root.findall('algorithm')]
    indices = [int(name[2].text) for name in root.findall('algorithm')]

    return seeds, indices


def write_prescale_table(PStable: pd.DataFrame, filepath: str = 'PStable_new',
        output_format: str = 'xlsx') -> None:
    """
    Save a prescale table to disk.

    Parameters
    ----------
    PStable : pandas.DataFrame
        Presacle table that should be written
    filepath : str (default: 'PStable_new')
        Name of the output file (without file extension)
    output_format : str (default: 'xlsx')
        Output file format, specified via the file extension

    """

    supported_formats = ['xlsx']

    if not filepath.endswith(output_format): filepath += '.' + output_format

    if output_format in supported_formats:
        PStable.to_excel(filepath, index=False)
    else:
        raise NotImplementedError('Invalid output file format: {}'.format(
            output_format))

    return


def fill_empty_val(name: str):
    # TODO extend functionality of this method
    return None 


def find_table_value(pstable: pd.DataFrame, seed: str,
        col: str) -> Union[int, float, str]:
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

    Returns
    -------
    Cell content corresponding to seed and col. If cell is not found, the
    function fill_empty_val is called and its return value is forwarded.

    """

    if 'Name' not in pstable.columns:
        raise KeyError('PS table does not have a column \'Name\'')

    if seed in pstable['Name'].values:
        return (pstable.loc[pstable['Name'] == seed])[col].values[0]
    else:
        return fill_empty_val(seed)


if __name__ == '__main__':

    # define CLI elements
    parser = argparse.ArgumentParser()
    parser.add_argument('PStable',
            help='Existing prescale table (xlsx format)',
            type=str)
    parser.add_argument('NewMenu',
            help='New L1 menu XML',
            type=str)
    parser.add_argument('-output', '--output',
            help='Name of the created output file (w/o file extension)',
            type=str,
            default='new_PStable',
            dest='output')

    args = parser.parse_args()

    # read all data and prepare the output
    PStable_in = read_prescale_table(args.PStable)
    PStable_out = make_empty_table(PStable_in)
    newSeeds, indices = get_seeds_from_xml(args.NewMenu)

    # create new PS table according to the new menu and fill in values from the
    # old PS table, if possible
    for seed,index in zip(newSeeds, indices):
        newData = {}
        for col in PStable_out.columns:
            if col == 'Index':
                newData[col] = index
            elif col == 'Name':
                newData[col] = seed
            else:
                newData[col] = find_table_value(PStable_in, seed, col)

        line = pd.DataFrame(newData, index=[index])
        PStable_out = PStable_out.append(line, ignore_index=False, sort=True)
    
    # PStable_out = PStable_out.sort_index().reset_index(drop=True)

    # sort output table according to the old table column layout
    PStable_out = PStable_out[PStable_in.columns]
    
    # save new table to the disk
    write_prescale_table(PStable_out, filepath=args.output)
