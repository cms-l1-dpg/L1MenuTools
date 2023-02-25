#!/usr/bin/env python3
import argparse
import sys
import os
import re
import pprint
import subprocess
from math import sqrt

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

args, unknown_args = parser.parse_known_args()

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
        command = "./testMenu2016 -l %s/%s -o %s-%u %s > %s/%s-%u.log 2>&1" % (
            args.tmp, filelists[i], args.name, i, " ".join(unknown_args),
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
    yields = dict()
    nEvents = 0
    nZBEvents = 0
    nBunches = 2450
    nFiredTotal = 0

    entries = os.scandir(args.tmp)
    max_len = 0
    n = 0
    for i in entries:
        if re.search(r'^%s-\d+.log$' % args.name, i.name):
            n += 1
            with open(args.tmp + "/" + i.name) as f:
                scale = None
                for line in f.read().splitlines():
                    # print(line)
                    match = re.search(r'^(\d+)\s+(L1_\S+)\s+\d+\s+(\d+)\s+(\S+)',
                                      line)
                    if match:
                        trigger = match.group(2)
                        if trigger not in yields:
                            if len(trigger) > max_len:
                                max_len = len(trigger)
                            yields[trigger] = 0
                        yields[trigger] += int(match.group(3))
                        if float(match.group(4)) > 0:
                            scale = int(match.group(3))/float(match.group(4))
                    match = re.search(r'Total Event:\s+(\d+)', line)
                    if match:
                        nEvents += int(match.group(1))
                    match = re.search(r'nZeroBiasevents =\s+(\d+)', line)
                    if match:
                        nZBEvents += int(match.group(1))
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

        for entry in sorted(yields):
            format = "% -" + str(max_len + 1) + "s: %4u %5.2f +/- %5.2f kHz"
            print(format % (entry, yields[entry], yields[entry] * scale / 1e3,
                            sqrt(yields[entry]) * scale / 1e3))


# print(sys.argv)
# print(unknown)

if __name__ == "__main__":
    if not os.path.exists(args.tmp):
        raise Exception("Path doesn't exists: %s" % args.tmp)

    print(unknown_args)

    if not args.just_analyze:
        filelists = prepare_input()
    
        process_ntuples(filelists)

    report_results()
