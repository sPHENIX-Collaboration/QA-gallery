#!/usr/bin/env bash

if [ "$#" -ne 1 ]; then	
	echo "Usage $0 notebook_name.ipynb"
	exit 1;
fi

nbname=$1;

echo '-------------------------'
echo 'jupyter env check'
echo '-------------------------'

jupyter --version

jupyter kernelspec list --json
 
echo '-------------------------'
echo 'Processing $nbname'
echo '-------------------------'
jupyter nbconvert --to notebook --inplace --execute --ExecutePreprocessor.kernel_name=python $nbname
jupyter nbconvert --to html --template classic $nbname


