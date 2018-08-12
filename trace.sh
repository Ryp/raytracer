#!/usr/bin/env bash

FILE=$(mktemp -t)

./build/raytracer > $FILE

FILE_PNG="$FILE.png"
convert $FILE -format ppm $FILE_PNG

echo $FILE_PNG
feh $FILE_PNG
