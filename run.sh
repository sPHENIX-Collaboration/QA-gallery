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
echo "Processing $nbname"
echo '-------------------------'

jupyter nbconvert --to notebook --inplace --execute --ExecutePreprocessor.kernel_name=python $nbname
if [ $build_ret -ne 0 ]; then
	echo "======================================================="
	echo "${nbname}: Failed build with return = ${build_ret}. ";
	echo "======================================================="
	exit $build_ret;
fi

jupyter nbconvert --to html $nbname


