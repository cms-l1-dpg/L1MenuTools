import csv
import xml.etree.ElementTree as ET
import datetime
import argparse
import os

def main(csv_file, output):
    """
    Prescale table csv to xml converter: takes a prescale table in csv format and converts it to the xml format.

    csv_file -- path to the input prescale table in csv format
    output -- path to the output file. If the file exists already, it will be overwritten. If directories in the path do not exit, they will be created. Directories that exist already will be used.
    """

    # read the CSV file
    with open(csv_file, mode='r') as csv_in:
        csv_reader = csv.reader(csv_in, delimiter=',')
        header = next(csv_reader)  # read the header to check that it looks like a valid PS table and to get the number of columns
        assert header[0] == 'Index', 'Invalid prescale table, the first column has to be the "Index"'
        assert header[1] == 'Name', 'Invalid prescale table, the second column has to be the "Name"'
        num_columns = len(header)

        # create the root element
        root = ET.Element('run-settings')
        root.set('id', 'uGT')

        context = ET.SubElement(root, 'context')
        context.set('id', 'uGtProcessor')

        # create the index and prescales elements
        index = ET.SubElement(context, 'param')
        index.set('id', 'index')
        index.set('type', 'uint')
        prescales = ET.SubElement(context, 'param')
        prescales.set('id', 'prescales')
        prescales.set('type', 'table')

        # set the columns and types for the prescales table
        prescale_columns = 'algo/prescale-index'
        prescale_types = 'uint'
        for i in range(num_columns - 2):
            prescale_columns += ',' + str(i)
            prescale_types += ',uint'
        columns = ET.SubElement(prescales, 'columns')
        columns.text = prescale_columns
        types = ET.SubElement(prescales, 'types')
        types.text = prescale_types

        rows = ET.SubElement(prescales, 'rows')

        # iterate over the CSV rows and create XML rows and param elements
        for row in csv_reader:
            algo_index = row[0]
        #   print(algo_index)
            prescale_values = row[2:]

            # set the index value
        # index.text = algo_index - str(504)
            index.text = "2" #hardcoded to match with the standard prescale xml files 
            # create XML row for prescales table
            xml_row = ET.SubElement(rows, 'row')
            xml_row.text = algo_index + ',' + ','.join(prescale_values)

    # add header comments to the root element
    header_comments = [
        'csv to xml Prescale Convertor',
        'Generated on ' + datetime.datetime.now().strftime('%a %b %d %Y %H:%M:%S %Z%z'),
        'from' + ' ' + csv_file
    ]
    for comment in header_comments:
        root.insert(0, ET.Comment(comment))

    # add line breaks and indentation to the XML output
    ET.indent(root, space="    ")
    xml_output = ET.tostring(root, encoding='UTF-8', xml_declaration=True)

    out_dir = os.path.dirname(output)  # get the path to the directory of the output
    os.makedirs(out_dir, exist_ok=True)  # create the output directory in case it doesn't exist
    # write the XML to a file
    with open(output, 'wb') as xml_file:
        xml_file.write(xml_output)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Prescale table csv to xml converter: takes a prescale table in csv format and converts it to the xml format.')
    parser.add_argument('-c', '--csv_table', action='store', type=str, required=True, help='path to the input prescale table in csv format')
    parser.add_argument('-o', '--output', action='store', type=str, required=True, help='path to the output file. If the file exists already, it will be overwritten. If directories in the path do not exit, they will be created. Directories that exist already will be used.')

    args = parser.parse_args()
    main(args.csv_table, args.output)
