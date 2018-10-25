import argparse
import pandas as pd
import xml.etree.ElementTree as ET


def read_prescale_table(filepath):

    data = pd.read_excel(filepath).to_dict('records')
    pstable = {}
    for entry in data:
        pstable[entry['Name']] = {}
        for col in entry.keys():
            if col != 'Name':
                pstable[entry['Name']][col] = entry[col]

    colnames = []
    for col in data[0].keys():
        colnames.append(col)

    return pstable, colnames


def get_seeds_from_xml(filepath):
    tree = ET.parse(filepath)
    root = tree.getroot()

    seeds = [name[0].text for name in root.findall('algorithm')]
    indices = [int(name[2].text) for name in root.findall('algorithm')]

    return seeds, indices


def write_prescale_table(PStable, filepath='PStable_new', output_format='xlsx'):
    if not filepath.endswith(output_format): filepath += '.' + output_format

    if isinstance(PStable, dict):
        # colnames = ['Name'].append([name for name in PStable.keys()])
        PStable = pd.DataFrame(PStable).transpose()

    PStable = PStable.fillna('-123')

    int_cols = [col for col in PStable.columns if PStable[col].dtype != str]
    for col in int_cols:
        PStable[col] = PStable[col].astype(int)

    PStable = PStable.replace(-123, '')

    PStable.index.rename('Name', inplace=True)

    if output_format in ['xlsx']:
        PStable.to_excel(filepath)
    else:
        raise NotImplementedError('Invalid output file format: {}'.format(
            output_format))


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
