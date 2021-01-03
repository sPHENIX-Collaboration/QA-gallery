#!/usr/bin/env bash

which python
python --version

echo "-------------------------------------"
echo "Install virtual env ./env"
echo "-------------------------------------"

python -m venv ./env
source ./env/bin/activate 

echo "-------------------------------------"
echo "Install local jupyterlab"
echo "-------------------------------------"

pip install --upgrade pip
pip install wheel
pip install jupyterlab ipywidgets

pip freeze
 
echo "-------------------------------------"
echo "Final virtual env checks:"
echo "-------------------------------------"

which python
python --version

which ipython
which jupyter

jupyter --version

jupyter kernelspec list     

echo "-------------------------------------"
echo "Done. To start jupyterlab server locally for devlopping:"
echo "source ./env/bin/activate && jupyter lab"
echo "-------------------------------------"


