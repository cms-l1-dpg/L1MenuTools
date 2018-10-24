def read_prescale_table(filepath):
    import pandas as pd

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
    import xml.etree.ElementTree as ET

    tree = ET.parse(filepath)
    root = tree.getroot()

    seeds = [name[0].text for name in root.findall('algorithm')]
    indices = [int(name[2].text) for name in root.findall('algorithm')]

    return seeds, indices
