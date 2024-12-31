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

echo jupyter nbconvert --to notebook --inplace --execute --ExecutePreprocessor.kernel_name=python $nbname
jupyter nbconvert --to notebook --inplace --execute --ExecutePreprocessor.kernel_name=python $nbname

build_ret=$? 
if [ $build_ret -ne 0 ]; then

	echo "======================================================="
	echo "${nbname}: Failed build with return = ${build_ret}. Try again....";
	echo "======================================================="
	
	timeout --preserve-status --signal=9 5m jupyter nbconvert --to notebook --inplace --execute --ExecutePreprocessor.kernel_name=python $nbname
	
	build_ret=$? 
	if [ $build_ret -ne 0 ]; then
	
		echo "======================================================="
		echo "${nbname}: Second failed build with return = ${build_ret}. return fail...";
		echo "======================================================="

		exit $build_ret;
	fi
fi

touch dummy.html
# jupyter nbconvert --to html $nbname


