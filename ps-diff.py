import os
import argparse
import tempfile

from psutils.psio import read_prescale_table


if __name__ == '__main__':

    # define CLI elements
    parser = argparse.ArgumentParser()
    parser.add_argument('PSTABLE_LEFT',
            help='Left PS table',
            type=str)
    parser.add_argument('PSTABLE_RIGHT',
            help='Right PS table',
            type=str)
    parser.add_argument('--by-line',
            help='Do a line-by-line comparison',
            action='store_true',
            dest='BYLINE')
    parser.add_argument('--by-name', '--by-seed',
            help='Do a name-by-name comparison',
            action='store_true',
            dest='BYNAME')

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
    with open(os.path.join(tempdir, 'PStable_left.csv'), 'w') as fleft:
        for l in range(1, PStable_left.shape[0]):
            fleft.write(' '.join([str(val) for val in PStable_left.iloc[[l]]]))

    with open(os.path.join(tempdir, 'PStable_right.csv'), 'w') as fright:
        for l in range(1, PStable_right.shape[0]):
            fright.write(' '.join([str(val) for val in PStable_right.iloc[[l]]]))

    print(tempdir)
