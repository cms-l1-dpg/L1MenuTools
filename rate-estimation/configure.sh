# Configure rate estimation using menu2lib.py
#
# usage: bash configure.sh <filename>
#

set -e

SCRIPT_DIR=menu2lib
SCRIPT=$SCRIPT_DIR/menu2lib.py

ENV_DIR=menu2lib.env
ENV_PYTHON=python3

MENU_FILE=$1

if [ -z "$MENU_FILE" ]; then
	echo "usage: $0 <filename>"
	exit 1
fi

if [ ! -d "$ENV_DIR" ]; then
	echo "creating virtual environment..."
	$ENV_PYTHON -m venv $ENV_DIR
	source $ENV_DIR/bin/activate
	python -m pip install -U pip
	python -m pip install -r $SCRIPT_DIR/requirements.txt
	deactivate
	echo "creating virtual environment... done."
fi

echo "executing $SCRIPT..."

source $ENV_DIR/bin/activate

python $SCRIPT --menu $MENU_FILE

deactivate

echo "executing $SCRIPT... done."
