import argparse

from psgenerate.readers import read_prescale_table, get_seeds_from_xml
from psgenerate.writers import write_prescale_table


def fill_empty_val(name):
    # TODO extend functionality of this method
    return None 


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('PStable', help='Existing prescale table (xlsx format)')
    parser.add_argument('NewMenu', help='New L1 menu XML')
    args = parser.parse_args()

    PStable_in, colnames = read_prescale_table(args.PStable)
    colnames = [c for c in colnames if c != 'Name']
    newSeeds, indices = get_seeds_from_xml(args.NewMenu)

    PStable_out = {}
    for seed,index in zip(newSeeds, indices):
        PStable_out[seed] = {}
        if seed in PStable_in.keys():
            for col in colnames:
                if col == 'Index':
                    PStable_out[seed][col] = index
                else:
                    PStable_out[seed][col] = PStable_in[seed][col]
        else:
            for col in colnames:
                if col == 'Index':
                    PStable_out[seed]['Index'] = index
                else:
                    PStable_out[seed][col] = fill_empty_val(seed)

    write_prescale_table(PStable_out)
