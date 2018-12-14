import os
import argparse
import tempfile
import subprocess

from psutils.psio import read_prescale_table

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
            help='Any number of valid options for the unix diff command, without dash prefixes (i.e., don\'t use "-" or "--")',
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
    # parser.add_argument('--left-column',
    #         help='Diff output only the left column of common lines',
    #         action='store_true',
    #         dest='DIFF_LEFTCOLUMN')
    # parser.add_argument('--suppress-common-lines',
    #         help='Diff does not output common lines',
    #         action='store_true',
    #         dest='DIFF_SUPPRESSCOMMONLINES')
    # parser.add_argument('--side-by-side', '-y',
    #         help='diff output in two columns',
    #         action='store_true',
    #         dest='DIFF_SIDEBYSIDE')
            

    args = parser.parse_args()

    modelist = [args.BYLINE, args.BYNAME]
    if modelist.count(True) > 1:
        raise RuntimeError('Only one comparison mode can be invoked at a time')
    elif modelist.count(True) == 1:
        if args.BYLINE: MODE = 'BYLINE'
        if args.BYNAME: MODE = 'BYNAME'
    else:
        MODE = 'BYLINE'  # default value

    PStable_left = read_prescale_table(args.PSTABLE_LEFT)
    PStable_right = read_prescale_table(args.PSTABLE_RIGHT)

    tempdir = tempfile.mkdtemp(prefix='psdiff_')
    fleft_path = os.path.join(tempdir, 'PStable_left.csv')
    fright_path = os.path.join(tempdir, 'PStable_right.csv')

    with open(fleft_path, 'w') as fleft:
        fleft_content = 'Column names:\n{}\n\n'.format(
                '\n'.join(['\t{}:\t{}'.format(idx,name) for idx,name in enumerate(PStable_left.columns)]))
        fleft_content += 'Table contents:\n'
        for l in range(0, PStable_left.shape[0]):
            fleft_content += ' '.join(['{}'.format(val) for val in PStable_left.loc[l,:]])
            fleft_content += '\n'

        fleft.write(fleft_content)

    with open(fright_path, 'w') as fright:
        fright_content = 'Column names:\n{}\n\n'.format(
                '\n'.join(['\t{}:\t{}'.format(idx,name) for idx,name in enumerate(PStable_right.columns)]))
        fright_content += 'Table contents:\n'
        for l in range(0, PStable_right.shape[0]):
            fright_content += ' '.join(['{}'.format(val) for val in PStable_right.loc[l,:]])
            fright_content += '\n'

        fright.write(fright_content)

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
        diff_cmd_args[0] = 'diff'
        diff = subprocess.run(diff_cmd_args,
            stdout=subprocess.PIPE).stdout.decode('utf-8')

    print(diff)
