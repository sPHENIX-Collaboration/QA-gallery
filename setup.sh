#!/usr/bin/env bash

which python
python --version

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


