#!/usr/bin/env bash

os=`uname`

FILE=$(mktemp -t "")

time (./build/raytracer > $FILE)

FILE_PNG="$FILE.png"
convert $FILE -format ppm $FILE_PNG

echo "Saved at: " $FILE_PNG
if [ "$os" = "Darwin" ]; then
  open $FILE_PNG
else
  feh $FILE_PNG
fi
