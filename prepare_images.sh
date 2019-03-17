#!/bin/bash

set -e

SIZE=512
IM_THOUSANDS_NUMBER=10
PAYLOAD=0.4

for PARAM in "$@"
do
  case $PARAM in
    --side-size=?*)
      SIZE=${PARAM#*=} # Delete everything up to "=" and assign the remainder.
      ;;
    --download)
      wget http://dde.binghamton.edu/download/ImageDB/BOSSbase_1.01.zip
      wget http://dde.binghamton.edu/download/stego_algorithms/download/WOW_linux_make_v10.tar.gz
      ;;
    --set-size=?*)
      IM_THOUSANDS_NUMBER=${PARAM#*=}
      ;;
    --payload=?*)
      PAYLOAD=${PARAM#*=}
      ;;
    *)
      echo "Wrong argument."
      exit 1
      ;;
  esac
done

if [ "$(md5sum BOSSbase_1.01.zip | awk '{print $1}')" != "58fe0159a8a825adfc0d008fec9b9287"] && [ "$(md5sum WOW_linux_make_v10.tar.gz | awk '{print $1}')" != "d9b440b0b3de59425b4db3d072017247"]; then
    echo "MD5 sums don't match"
    exit 1
fi

unzip BOSSbase_1.01.zip
mkdir -p BOSSbase_1.01-$SIZE/cover
mkdir -p BOSSbase_1.01-$SIZE/stego_wow_$PAYLOAD

if [ $SIZE == 512 ]; then
    mv BOSSbase_1.01/{1..$((IM_THOUSANDS_NUMBER * 1000))} BOSSbase_1.01-$SIZE/cover
else
    for I in $(seq 1 $((IM_THOUSANDS_NUMBER * 1000)));
    do
        FILE="BOSSbase_1.01/${I}.pgm"
        convert -verbose $FILE -resize $SIZE'x'$SIZE BOSSbase_1.01-$SIZE/cover/${FILE#*/}
    done
fi

rm -rf BOSSbase_1.01
tar -xvf WOW_linux_make_v10.tar.gz

./WOW_linux_make_v10/executable/WOW -v -I BOSSbase_1.01-$SIZE/cover/ -O BOSSbase_1.01-$SIZE/stego_wow_$PAYLOAD/ -a $PAYLOAD
