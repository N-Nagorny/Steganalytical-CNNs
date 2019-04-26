#!/bin/bash

set -e

SIZE=512
IM_THOUSANDS_NUMBER=1
PAYLOAD=0.4
FILTERING=FALSE
DATABASE="BOSSbase_1.01"
DOWNLOAD=FALSE
CHECKSUMS=FALSE

for PARAM in "$@"
do
  case $PARAM in
    --side-size=?*)
      SIZE=${PARAM#*=} # Delete everything up to "=" and assign the remainder.
      ;;
    --download)
      DOWNLOAD=TRUE
      ;;
    --checksums)
      CHECKSUMS=TRUE
      ;;
    --database=?*)
      DATABASE=${PARAM#*=}
      ;;
    --set-size=?*)
      IM_THOUSANDS_NUMBER=${PARAM#*=}
      ;;
    --payload=?*)
      PAYLOAD=${PARAM#*=}
      ;;
    --filter)
      FILTERING=TRUE
      ;;
    *)
      echo "Wrong argument."
      exit 1
      ;;
  esac
done

if [ $DATABASE = "BOSSbase_1.01" ]; then
    if [ $DOWNLOAD = TRUE ]; then
      wget http://dde.binghamton.edu/download/ImageDB/BOSSbase_1.01.zip
      wget http://dde.binghamton.edu/download/stego_algorithms/download/WOW_linux_make_v10.tar.gz
    fi
    if [ $CHECKSUMS = TRUE ]; then
        if [ "$(md5sum BOSSbase_1.01.zip | awk '{print $1}')" != "58fe0159a8a825adfc0d008fec9b9287" ] && [ "$(md5sum WOW_linux_make_v10.tar.gz | awk '{print $1}')" != "d9b440b0b3de59425b4db3d072017247" ]; then
            echo "MD5 sums don't match"
            exit 1
        fi
    fi
    
    unzip BOSSbase_1.01.zip
    mkdir -p BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/cover
    mkdir -p BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/stego
    
    for I in $(seq 1 $((IM_THOUSANDS_NUMBER * 1000)));
    do
        FILE="BOSSbase_1.01/${I}.pgm"
        if [ $SIZE == 512 ]; then
            mv $FILE BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/cover
        else
            convert -verbose $FILE -resize $SIZE'x'$SIZE BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/cover/${FILE#*/}
        fi
    done
    
    rm -rf BOSSbase_1.01
    
    for (( I=0; I < $((IM_THOUSANDS_NUMBER * 1000)); I++ ));
    do
        DIRNAME=$(( I/1000 ))
        if [ $(( I%1000 )) == 0 ]; then
            mkdir -p BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/cover/$DIRNAME
            mkdir -p BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/stego/$DIRNAME
        fi
        mv BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/cover/$(( I+1 )).pgm BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/cover/$DIRNAME/$(( I+1 )).pgm
    done
    
    tar -xvf WOW_linux_make_v10.tar.gz
    
    for I in {0..9}
    do
        ./WOW_linux_make_v10/executable/WOW -v -I BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/cover/$I/ -O BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/stego/$I/ -a $PAYLOAD
    done
    
    for TYPE in "cover" "stego";
    do
        find BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/${TYPE}/ -name "*.pgm" -exec mv {} BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/${TYPE}/ \;
        rm -r BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/${TYPE}/{0..9}/
    done
    
    g++ main.cpp Image.cpp
    
    for TYPE in "cover" "stego";
    do
        for I in $(seq 1 $((IM_THOUSANDS_NUMBER * 1000)));
        do
            FILE="BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/$TYPE/${I}.pgm"
            ENDFILE="BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/$TYPE/${I}.ppm"
            ./a.out convert $FILE $ENDFILE
        done
    done
    
    if [ $FILTERING == TRUE ]; then
        mkdir -p BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}-filtered/cover
        mkdir -p BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}-filtered/stego
        for TYPE in "cover" "stego";
        do
            for I in $(seq 1 $((IM_THOUSANDS_NUMBER * 1000)));
            do
                FILE="BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}/$TYPE/${I}.pgm"
                ENDFILE="BOSSbase_1.01-${SIZE}_wow_${PAYLOAD}-filtered/$TYPE/${I}.ppm"
                ./a.out process $FILE $ENDFILE
                rm $FILE
            done
        done
    fi
elif [ $DATABASE = "PPG-LIRMM-COLOR" ]; then
    FOLDER_NAME = PPG-LIRMM-GRAYSCALE-$((IM_THOUSANDS_NUMBER * 1000))

    if [ $DOWNLOAD = TRUE ]; then
      wget http://www.lirmm.fr/~chaumont/download/PPG-LIRMM-COLOR.7z
      wget http://dde.binghamton.edu/download/stego_algorithms/download/WOW_linux_make_v10.tar.gz
    fi
    if [ $CHECKSUMS = TRUE ]; then
        if [ "$(md5sum PPG-LIRMM-COLOR.7z | awk '{print $1}')" != "bcd5246cd927ecb81ce91586703784fb" ] && [ "$(md5sum WOW_linux_make_v10.tar.gz | awk '{print $1}')" != "d9b440b0b3de59425b4db3d072017247" ]; then
            echo "MD5 sums don't match"
            exit 1
        fi
    fi

    7z x -oPPG-LIRMM-COLOR PPG-LIRMM-COLOR.7z

    mkdir ${FOLDER_NAME}/
    find PPG-LIRMM-COLOR/ -name "*.ppm" -exec mv {} ${FOLDER_NAME}/ \;
    rm -rf PPG-LIRMM-COLOR/
    for I in $(seq -f "%05g" 1 $((IM_THOUSANDS_NUMBER * 1000)));
    do
        FILE="${FOLDER_NAME}/${I}.ppm"
        ppmtopgm $FILE > ${FOLDER_NAME}/$((10#${I})).pgm 
    done
    rm ${FOLDER_NAME}/*.ppm
    for (( I=0; I < $((IM_THOUSANDS_NUMBER * 1000)); I++ ));
    do
        DIRNAME=$(( I/1000 ))
        if [ $(( I%1000 )) == 0 ]; then
            mkdir -p ${FOLDER_NAME}_wow_${PAYLOAD}/cover/$DIRNAME
            mkdir -p ${FOLDER_NAME}_wow_${PAYLOAD}/stego/$DIRNAME
        fi
        mv ${FOLDER_NAME}_wow_${PAYLOAD}/cover/$(( I+1 )).pgm ${FOLDER_NAME}_wow_${PAYLOAD}/cover/$DIRNAME/$(( I+1 )).pgm
    done
    
    tar -xvf WOW_linux_make_v10.tar.gz
    
    for I in {0..$((IM_THOUSANDS_NUMBER - 1))}
    do
        ./WOW_linux_make_v10/executable/WOW -v -I ${FOLDER_NAME}_wow_${PAYLOAD}/cover/$I/ -O ${FOLDER_NAME}_wow_${PAYLOAD}/stego/$I/ -a $PAYLOAD
    done
    
    for TYPE in "cover" "stego";
    do
        find ${FOLDER_NAME}_wow_${PAYLOAD}/${TYPE}/ -name "*.pgm" -exec mv {} ${FOLDER_NAME}_wow_${PAYLOAD}/${TYPE}/ \;
        rm -r ${FOLDER_NAME}_wow_${PAYLOAD}/${TYPE}/{0..$((IM_THOUSANDS_NUMBER - 1))}/
    done
fi
