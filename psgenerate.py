import argparse
import pandas as pd
import xml.etree.ElementTree as ET


def read_prescale_table(filepath):
    data = pd.read_excel(filepath, convert_float=True)
    return data


def make_empty_table(pstable):
    empty_table = pd.DataFrame(columns=pstable.columns)
    return empty_table


def get_seeds_from_xml(filepath):
    tree = ET.parse(filepath)
    root = tree.getroot()

    seeds = [name[0].text for name in root.findall('algorithm')]
    indices = [int(name[2].text) for name in root.findall('algorithm')]

    return seeds, indices


def write_prescale_table(PStable, filepath='PStable_new', output_format='xlsx'):
    if not filepath.endswith(output_format): filepath += '.' + output_format

    if output_format in ['xlsx']:
        PStable.to_excel(filepath, index=False)
    else:
        raise NotImplementedError('Invalid output file format: {}'.format(
            output_format))


def fill_empty_val(name):
    # TODO extend functionality of this method
    return None 


def find_table_value(pstable, seed, col):
    if 'Name' not in pstable.columns:
        raise KeyError('PS table does not have a column \'Name\'')

    if seed in pstable['Name'].values:
        return (pstable.loc[pstable['Name'] == seed])[col].values[0]
    else:
        return fill_empty_val(seed)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('PStable', help='Existing prescale table (xlsx format)')
    parser.add_argument('NewMenu', help='New L1 menu XML')
    args = parser.parse_args()

    PStable_in = read_prescale_table(args.PStable)
    PStable_out = make_empty_table(PStable_in)

    newSeeds, indices = get_seeds_from_xml(args.NewMenu)

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
    PStable_out = PStable_out[PStable_in.columns]
    
    write_prescale_table(PStable_out)
