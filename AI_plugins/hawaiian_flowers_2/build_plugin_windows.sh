#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'hawaiian_flowers_2.dll') $TOP/WindowsBinaries

cd .. 

cd hawaiian_flowers_1 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'hawaiin_flowers_1.dll') $TOP/WindowsBinaries

cd .. 

cd threshold_alpha2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'thresholdalpha2.dll') $TOP/WindowsBinaries

