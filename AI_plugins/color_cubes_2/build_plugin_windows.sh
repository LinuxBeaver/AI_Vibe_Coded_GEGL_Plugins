#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'colorcubes2.dll') $TOP/WindowsBinaries

cd .. 

cd color_cubes_1 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'colorcubes1.dll') $TOP/WindowsBinaries

cd .. 


