#!/bin/bash
#
# Run ps-generate.py in an adequate environment
#
# usage: bash run-ps-generate.sh <args>
#

set -e

SCRIPT=ps-generate.py

ENV_DIR=pstools.env
ENV_PYTHON=python3

if [ ! -d "$ENV_DIR" ]; then
    echo "Creating virtual environment..."
    $ENV_PYTHON -m venv $ENV_DIR
    source $ENV_DIR/bin/activate
    python -m pip install -U pip
    python -m pip install -r requirements.txt
    deactivate
    echo "Creating virtual environment...done"
fi

. $ENV_DIR/bin/activate
python3 $SCRIPT $@
EXITCODE=$?
deactivate

exit $EXITCODE
