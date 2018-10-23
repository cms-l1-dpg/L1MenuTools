import argparse

from psgenerate.readers import read_prescale_table, get_seeds_from_xml


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('PStable', help='Existing prescale table (xlsx format)')
    parser.add_argument('NewMenu', help='New L1 menu XML')
    args = parser.parse_args()

    PStable_in = read_prescale_table(args.PStable)
    newSeeds = get_seeds_from_xml(args.NewMenu)

    PStable_out = {}

    # print(PStable)
    # print(newSeeds)
