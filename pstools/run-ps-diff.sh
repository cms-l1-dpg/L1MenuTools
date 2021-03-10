#!/bin/bash
#
# Run ps-diff.py in an adequate environment
#
# usage: bash run-ps-diff.sh <args>
#

set -e

SCRIPT=ps-diff.py

ENV_DIR=pstools.env
ENV_PYTHON=python3

if [ ! -d "$ENV_DIR" ]; then
    echo "Creating virtual environment..."
    $ENV_PYTHON -m venv $ENV_DIR
    source $ENV_DIR/bin/activate
    python -m pip install -U pip
    python -m pip install -r requirements.txt
    deactivate
    echo "\rCreating virtual environment...done"
fi

. $ENV_DIR/bin/activate
python $SCRIPT $@
EXITCODE=$?
deactivate

exit $EXITCODE
