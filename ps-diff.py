import os
import argparse
import tempfile
import subprocess

from psutils.psio import read_prescale_table

# determine the size of the current terminal
term_rows, term_columns = os.popen('stty size', 'r').read().split()


if __name__ == '__main__':

    # define CLI elements
    parser = argparse.ArgumentParser()
    parser.add_argument('PSTABLE_LEFT',
            help='Left PS table',
            type=str)
    parser.add_argument('PSTABLE_RIGHT',
            help='Right PS table',
            type=str)
    parser.add_argument('DIFF_OPTIONS',
            help='Any number of valid options for the unix diff command, ' \
                    'without dash prefixes (i.e., don\'t use "-" or "--")',
            nargs='*',
            type=str)
    parser.add_argument('--by-line',
            help='Do a line-by-line comparison',
            action='store_true',
            dest='BYLINE')
    parser.add_argument('--by-name', '--by-seed',
            help='Do a name-by-name comparison',
            action='store_true',
            dest='BYNAME')
    parser.add_argument('--no-color', '--nocolor',
            help='Do not colorize the terminal output',
            action='store_true',
            dest='DIFF_NOCOLOR')
    args = parser.parse_args()

    modelist = [args.BYLINE, args.BYNAME]
    if modelist.count(True) > 1:
        raise RuntimeError('Only one comparison mode can be invoked at a time')
    elif modelist.count(True) == 1:
        if args.BYLINE: MODE = 'BYLINE'
        if args.BYNAME: MODE = 'BYNAME'
    else:
        MODE = 'BYLINE'  # default value


    # read the PS table files
    PStable_left = read_prescale_table(args.PSTABLE_LEFT)
    PStable_right = read_prescale_table(args.PSTABLE_RIGHT)

    # create a temporary directory for the files that will be compared
    tempdir = tempfile.mkdtemp(prefix='psdiff_')
    fleft_path = os.path.join(tempdir, 'PStable_left.csv')
    fright_path = os.path.join(tempdir, 'PStable_right.csv')


    # generate files with the preferred representation of the PS table data

    for file_path, PStable in ((fleft_path, PStable_left),
            (fright_path, PStable_right)):
        with open(file_path, 'w') as f:
            if MODE == 'BYLINE':
                file_content = 'Column names:\n{}\n\n'.format('\n'.join(
                    ['\t{}:\t{}'.format(idx,name) for idx,name in enumerate(PStable.columns)]))
                file_content += 'Table contents:\n'
                for l in range(0, PStable.shape[0]):
                    file_content += ' '.join(
                            ['{}'.format(val) for val in PStable.loc[l,:]])
                    file_content += '\n'

            else:
                raise NotImplementedError('The only currently supported ' \
                        'comparison mode is "by line"')

            f.write(file_content)


    # generate the diff between the two files

    print('\nComparing files {} (left)  and  {} (right):\n'.format(
        args.PSTABLE_LEFT, args.PSTABLE_RIGHT))

    diff_cmd = 'diff' if args.DIFF_NOCOLOR else 'colordiff'

    diff_cmd_args = [diff_cmd, '--width={}'.format(term_columns),
            fleft_path, fright_path]
    for option in args.DIFF_OPTIONS:
        if len(option) == 1:
            diff_cmd_args.append('-{}'.format(option))
        elif len(option) > 1:
            diff_cmd_args.append('--{}'.format(option))

    try:
        diff = subprocess.run(diff_cmd_args,
            stdout=subprocess.PIPE).stdout.decode('utf-8')
    except FileNotFoundError:
        diff_cmd_args[0] = 'diff'  # use diff if colordiff is not installed
        diff = subprocess.run(diff_cmd_args,
            stdout=subprocess.PIPE).stdout.decode('utf-8')


    # output the diff
    print(diff)
