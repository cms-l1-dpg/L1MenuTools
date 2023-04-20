#!/usr/bin/env python3
import argparse
import sys
import os
import re
import pprint
import subprocess
from math import sqrt
import csv
import numpy as np

parser = argparse.ArgumentParser(
    description='A wrapper around testMenu2016 program to enable parallel processing',
    formatter_class=argparse.ArgumentDefaultsHelpFormatter
)

parser.add_argument('--njobs', dest='njobs', type=int, default=16,
                    help='number of parallel jobs to run')

parser.add_argument('--just_analyze', dest='just_analyze', action='store_true',
                    help='just analyze logs without processing ntuples')

parser.add_argument('--name', dest='name', type=str, required=True,
                    help='unique name of the test')

parser.add_argument('-l', '--filelist', dest='filelist', type=str, required=True,
                    help='input ntuple list')

parser.add_argument('--tmp', dest='tmp', type=str, default="tmp/",
                    help='path to a direcrtory where to store temporary files')
parser.add_argument('-m', '--menufile', dest = 'menufile', type = str, required = True,
                    help = 'set the input menu')
parser.add_argument('--SelectCol', dest = 'SelectCol', type = str, required = True,
                    help = 'Select prescale column from input csv menu')
parser.add_argument('-b', '--nBunches', dest = 'nBunches', type = int, default = 2748,
                    help = 'set number of bunches')

args, unknown_args = parser.parse_known_args()


def check_prescales():
    """Check whether the requested prescale column contains prescale that are neither 1 or 0"""
    header = np.loadtxt(args.menufile, dtype = str, delimiter = ',', max_rows = 1)  # read the header from the prescale table
    col_number = np.argwhere(header == args.SelectCol)  # get the number of the selected ps column
    if len(col_number) != 1:  # there should be exactly one column that matches the selected one
        raise ValueError(f'The prescale column {args.SelectCol} is not found in the prescale table (or it was found several times).')
    col_number = int(col_number)
    col = np.loadtxt(args.menufile, delimiter = ',', skiprows = 1, usecols = col_number)  # get the values of the selected ps column
    n_wrong_ps = ((col != 1) & (col != 0)).sum()  # prescales are only allowed to be 1 or 0, count the number of wrong prescales
    return n_wrong_ps == 0

def prepare_input():
    if not args.filelist:
        raise Exception("filelist is not provided")

    files = []
    with open(args.filelist) as f:
        for file in f.readlines():
            files.append(file.strip())
    nfiles = len(files)
    print("Number of files: %u" % nfiles)

    filelists = []

    nfiles_per_job = int(nfiles / args.njobs)
    if nfiles_per_job * args.njobs < nfiles:
        nfiles_per_job += 1

    index = 0
    for i in range(0, nfiles, nfiles_per_job):
        filename = "%s-%02u.list" % (args.name, index)
        index += 1
        with open("%s/%s" % (args.tmp, filename), "w") as f:
            for file in files[i:i+nfiles_per_job]:
                f.write("%s\n" % file)
        filelists.append(filename)
    
    return filelists

def process_ntuples(filelists):
    processes = []
    
    for i in range(args.njobs):
        command = "./testMenu2016 -l %s/%s -o %s-%u -m %s --SelectCol %s -b %u %s > %s/%s-%u.log 2>&1" % (
            args.tmp, filelists[i], args.name, i, args.menufile, args.SelectCol, args.nBunches, " ".join(unknown_args),
            args.tmp, args.name, i)
        print(command)
        processes.append(subprocess.Popen(command, shell=True))
        print("Started subprocess with id: %s" % processes[i].pid)

    print("Waiting for all jobs to finish")
    # Check if all jobs finished
    for p in processes:
        if p.poll() is None:
            p.wait()


def report_results():
    yields = []
    nEvents = 0
    nZBEvents = 0
    nBunches = args.nBunches
    nFiredTotal = 0

    entries = os.scandir(args.tmp)
    max_len = 0
    n = 0
    for i in entries:
        if re.search(r'^%s-\d+.log$' % args.name, i.name):
            n += 1
            with open(args.tmp + "/" + i.name) as f:
                scale = None
                j = 0
                for line in f.read().splitlines():
                    # print(line)
                    match = re.search(r'^(\d+)\s+(L1_\S+)\s+(\d+)\s+(\d+)\s+(\S+)',
                                      line)
                    if match:
                        if n == 1:
                            trigger_bit = match.group(1)
                            trigger = match.group(2)
                            prescale = match.group(3)
                            if len(trigger) > max_len:
                                max_len = len(trigger)
                            yields.append([trigger_bit, trigger, prescale, 0.0, 0.0, 0])  # append a list that contains (or will contain) the trigger bit, the trigger name, the prescale, the trigger rate, the rate error and the total firecounts
                        yields[j][5] += int(match.group(4))  # read the firecounts and add them to the counts from previous files
                        if float(match.group(5)) > 0:
                            scale = int(match.group(4))/float(match.group(5))
                        j+=1
                    match = re.search(r'Total Event:\s+(\d+)', line)
                    if match:
                        nEvents += int(match.group(1))  # read the total number of events
                    match = re.search(r'nZeroBiasevents =\s+(\d+)', line)
                    if match:
                        nZBEvents += int(match.group(1))  # read the number of ZB events
                    match = re.search(r'Total rate  =\s+(\S+)', line)
                    if match and scale is not None:
                        nFiredTotal += float(match.group(1)) * scale * 1e3

    print("Number of log files found: %u" % n)
    print("Total Event: %u" % nEvents)
    print("Total ZB Event: %u" % nZBEvents)

    if nZBEvents:
        scale = 11246. * nBunches / nZBEvents

        print("Total rate: %5.2f +/- %5.2f kHz" % (nFiredTotal * scale / 1e3,
                                                   sqrt(nFiredTotal) * scale / 1e3))

        for entry in yields:
            entry[3] = round(entry[5] * scale, 2)
            entry[4] = round(sqrt(entry[5]) * scale, 2)
            #format = "% -" + str(max_len + 1) + "s: %4u %5.2f +/- %5.2f kHz"
            #print(format % (entry, yields[entry], yields[entry] * scale / 1e3,
                            #sqrt(yields[entry]) * scale / 1e3))
        
        with open(f'results/{args.name}_combined.csv', 'w') as csv_file:
            csv_writer = csv.writer(csv_file, delimiter = ',')
            csv_writer.writerow(['L1Bit', 'L1SeedName', 'pre-scale0', 'rate0', 'error_rate0', 'firecounts'])
            csv_writer.writerows(yields)
            csv_writer.writerow([])
            csv_writer.writerow([])
            csv_writer.writerow([])
            csv_writer.writerow([9999, 'Total rate', 1, round(nFiredTotal * scale / 1e3, 2), round(sqrt(nFiredTotal) * scale / 1e3, 2), round(nFiredTotal)])
        
        with open(f'results/{args.name}_combined.txt', 'w') as txt_file:
            # print the header
            seed_name_len = max_len + 2
            print_str = f'{"L1Bit":10}{"L1SeedName":{seed_name_len}}{"pre-scale":10}{"firecounts":15}{"rate@13TeV":10} +/- {"error_rate@13TeV":20}{"passevts@13TeV":15}'
            print(print_str)
            txt_file.write(f'{print_str}\n')
            
            # print the values (for each trigger
            for entry in yields:
                print_str = f'{entry[0]:10}{entry[1]:{seed_name_len}}{entry[2]:10}{entry[5]:<15}{entry[3]:<10} +/- {entry[4]:<20}{entry[5]:<15}'
                print(print_str)
                txt_file.write(f'{print_str}\n')
            
            print('')
            txt_file.write('\n')
            
            print_str = "Total rate: %5.2f +/- %5.2f kHz" % (nFiredTotal * scale / 1e3, sqrt(nFiredTotal) * scale / 1e3)
            print(print_str)
            txt_file.write(f'{print_str}\n')


# print(sys.argv)
# print(unknown)

if __name__ == "__main__":
    if not os.path.exists(args.tmp):
        raise Exception("Path doesn't exists: %s" % args.tmp)

    print(unknown_args)

    if not args.just_analyze:
        if not check_prescales():
            raise Exception(f'The prescale column {args.SelectCol} of the table {args.menufile} contains prescales that are neither 1 nor 0. This script is an intermediate solution and only supports unprescaled or deactivated trigger seeds. Please decide for each prescaled seed whether you want to deactivate it or use it unprescaled and change the prescale table correspondingly')
        filelists = prepare_input()
    
        process_ntuples(filelists)

    report_results()
