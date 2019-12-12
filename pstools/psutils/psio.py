import os
import tempfile
import wget
from typing import Union, Any
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
    Import an existing xlsx prescale table as pandas dataframe from a local
    path or a URL
    
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

