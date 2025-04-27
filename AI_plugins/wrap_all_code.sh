#!/bin/bash

for dir in *
do
    if test -f "$dir/grab_code.sh"
    then
      echo "building in $dir"
     ( cd "$dir" && bash ./grab_code.sh ) || exit 1
    fi
done


