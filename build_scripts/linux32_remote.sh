#!/bin/sh
BUILD=linux32
BUILD_SERVER=linuxbuild

BUILD_DIR=/home/caret/${BUILD}/caret7_dev
ssh -v caret@${BUILD_SERVER} "/bin/bash -c \"cd ${BUILD_DIR};./build32.sh caret\"" > $PWD/remote_launch_${BUILD}.txt 2>&1
cat $PWD/remote_launch_${BUILD}.txt | mailx -s 'Caret7 Linux 32 Build Result' john@brainvis.wustl.edu tsc5yc@mst.edu
