# will convert the L1 menu prescale csv files to xml files that could be read by the L1 skimmer.

# $wget https://raw.githubusercontent.com/cms-l1-dpg/L1MenuRun3/master/development/L1Menu_Collisions2023_v1_1_0/PrescaleTable/L1Menu_Collisions2023_v1_1_0.csv
# $python3 prescale_xml_convertor.py

import csv
import xml.etree.ElementTree as ET
import datetime

csv_file = "L1Menu_Collisions2023_v1_1_0.csv"

L1_menu_file = csv_file

# read the CSV file
with open(csv_file, mode='r') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    next(csv_reader)  # skip header row

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
    prescale_columns = 'algo/prescale-index,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21'
    prescale_types = 'uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint,uint'
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
    'from' + ' ' + L1_menu_file
]
for comment in header_comments:
    root.insert(0, ET.Comment(comment))

# add line breaks and indentation to the XML output
ET.indent(root, space="    ")
xml_output = ET.tostring(root, encoding='UTF-8', xml_declaration=True)

# write the XML to a file
with open('UGT_BASE_RS_PRESCALES_' + L1_menu_file.replace('csv', 'xml'), 'wb') as xml_file:
    xml_file.write(xml_output)
