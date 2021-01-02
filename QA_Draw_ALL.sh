#!/usr/bin/env bash

echo "-----------------------"
echo "Common block: env check"
echo "-----------------------"

jupyter --version

jupyter kernelspec list     

echo "-----------------------"
echo "Specific notebook updates"
echo "-----------------------"

jupyter nbconvert --to notebook --inplace --execute QA-calorimeter.ipynb
jupyter nbconvert --to html --template classic      QA-calorimeter.ipynb 


