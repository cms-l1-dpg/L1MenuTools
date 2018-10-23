def read_prescale_table(filepath):
    import pandas as pd

    data = pd.read_excel(filepath)
    return data.to_dict()


def get_seeds_from_xml(filepath):
    import xml.etree.ElementTree as ET

    tree = ET.parse(filepath)
    root = tree.getroot()

    seeds = [name[0].text for name in root.findall('algorithm')]

    return seeds
