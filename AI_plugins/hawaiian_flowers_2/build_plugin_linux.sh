#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'hawaiian_flowers_2.so') $TOP/LinuxBinaries

cd .. 

cd hawaiian_flowers_1 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'hawaiin_flowers_1.so') $TOP/LinuxBinaries

cd .. 

cd threshold_alpha2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'thresholdalpha2.so') $TOP/LinuxBinaries

