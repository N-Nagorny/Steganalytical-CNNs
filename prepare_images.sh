#!/bin/bash

set -e

SIZE=512

for PARAM in "$@"
do
  case $PARAM in
    --size=?*)
      SIZE=${PARAM#*=} # Delete everything up to "=" and assign the remainder.
      ;;
    --download)
      wget http://dde.binghamton.edu/download/ImageDB/BOSSbase_1.01.zip
      wget http://dde.binghamton.edu/download/stego_algorithms/download/WOW_linux_make_v10.tar.gz
      ;;
    *)
      echo "Wrong argument."
      exit 1
      ;;
  esac
done

unzip BOSSbase_1.01.zip
mkdir -p BOSSbase_1.01-$SIZE/cover
mkdir -p BOSSbase_1.01-$SIZE/stego_wow_0.4

if [ $SIZE == 512 ]; then
    mv BOSSbase_1.01/* BOSSbase_1.01-$SIZE/cover
else
    for FILE in BOSSbase_1.01/*.pgm;
    do
        convert -verbose $FILE -resize $SIZE'x'$SIZE BOSSbase_1.01-$SIZE/cover/${FILE#*/}
    done
fi

rm -rf BOSSbase_1.01
tar -xvf WOW_linux_make_v10.tar.gz

for (( I=0; I < 10000; I++ ));
do
    DIRNAME=$(( I/1000 ))
    if [ $(( I%1000 )) == 0 ]; then
        mkdir -p BOSSbase_1.01-$SIZE/cover/$DIRNAME
        mkdir -p BOSSbase_1.01-$SIZE/stego_wow_0.4/$DIRNAME
    fi
    mv BOSSbase_1.01-$SIZE/cover/$(( I+1 )).pgm BOSSbase_1.01-$SIZE/cover/$DIRNAME/$(( I+1 )).pgm
done

for I in {0..9}
do
    ./WOW_linux_make_v10/executable/WOW -v -I BOSSbase_1.01-$SIZE/cover/$I/ -O BOSSbase_1.01-$SIZE/stego_wow_0.4/$I/ -a 0.4
done
