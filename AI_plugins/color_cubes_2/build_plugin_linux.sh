#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'colorcubes2.so') $TOP/LinuxBinaries

cd .. 

cd color_cubes_1 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'colorcubes1.so') $TOP/LinuxBinaries

cd .. 


