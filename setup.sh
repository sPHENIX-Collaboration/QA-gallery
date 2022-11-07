#!/usr/bin/env bash

which python
python --version

echo "-------------------------------------"
echo "Install virtual env ./env"
echo "-------------------------------------"

# force ingore system site packages
export PYTHONPATH=$ROOTSYS/lib  

python -m venv ./env
source ./env/bin/activate 
pip list

echo "-------------------------------------"
echo "Install local jupyterlab"
echo "-------------------------------------"

pip install --upgrade pip
pip install wheel
pip install jupyterlab ipywidgets

# pip install -r requirements.txt

echo "-------------------------------------"
echo "Package list"
echo "-------------------------------------"

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
echo "Done. To start jupyterlab server locally for developing the plots:"
echo "source ./env/bin/activate && jupyter lab"
echo "-------------------------------------"


