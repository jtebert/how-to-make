#!/bin/bash
# A simple video compression script using ffmpeg (similar to resizer.sh)
# INPUTS: input_files width output/path
# EXAMPLE: compressor "videos/*.mp4" 1280 "videos/compressed"

mkdir -p $3

for i in $1;
  do
    path=${i%.*}
    name=${path##*/}
    ffmpeg -i "$i" -vcodec h264 -vf scale=$2:-1 -an $3/$name.mp4;
done
