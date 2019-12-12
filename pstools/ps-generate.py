import os
import argparse
from typing import Union, Any
import tempfile
import wget
import shutil
import pandas as pd
import xml.etree.ElementTree as ET

from psutils.psio import download_file, read_prescale_table, get_seeds_from_xml, \
        write_prescale_table
from psutils.pstable import fill_empty_val, find_table_value, make_empty_table


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
    
    PStable_out = PStable_out.sort_index().reset_index(drop=True)

    # sort output table according to the old table column layout
    PStable_out = PStable_out[PStable_in.columns]
    
    # save new table to the disk
    write_prescale_table(PStable_out, filepath=args.output)
