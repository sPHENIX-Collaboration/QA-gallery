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
echo "Install jupyterlab kernel for sPHENIX ROOT"
echo "-------------------------------------"

kernel_path='~/.local/share/jupyter/kernels/sphenix-root/';

if [ -d $kernel_path ] 
then
    echo "Will use the sPHENIX ROOT kernel spec at $kernel_path" 
else
		echo "WARNING: this macro will install the sPHENIX ROOT jupyter kernel to your home folder:"
		
		rsync -avl .local/ ~/.local/
		ls -lhvc  $kernel_path
fi

echo "-------------------------------------"
echo "Env checks:"
echo "-------------------------------------"

which python
python --version

which ipython
which jupyter

jupyter --version

jupyter kernelspec list     

