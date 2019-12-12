#!/bin/bash

echo ""
echo "Creating virtual environment..."
virtualenv -p python3 env
if [ $? -ne 0 ]; then
	echo "Error creating virtual environment."
	exit 1
fi

echo ""
echo "Activating virtual environment..."
. env/bin/activate

echo ""
echo "Installing requirements..."
pip install -r requirements.txt
if [ $? -ne 0 ]; then
	echo "Error installing requirements."
	exit 1
fi

echo ""
echo "Setup complete."
echo ">>>>> Do not forget to activate the new environment before" \
	"running the software by executing: \`source env/bin/activate\` <<<<<"
echo ""

