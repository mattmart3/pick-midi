#!/bin/bash

#TODO: add usage if -h (--help) argument is provided
#Usage: PickMidi [BIN_DIR] [PICKMIDI_HOME_DIR] [BUILD_DIR]

#TODO: Provide a clean option ( is there any cmake clean option we can use here? )

BIN_NAME="PickMidi"

DEFAULT_PICKMIDI_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )" #path of this script
DEFAULT_BUILD_DIR="$DEFAULT_PICKMIDI_DIR/build"
DEFAULT_BIN_DIR="$DEFAULT_PICKMIDI_DIR/bin"

BUILD_DIR=${1:-$DEFAULT_BUILD_DIR}
PICKMIDI_DIR=${2:-$DEFAULT_PICKMIDI_DIR}
BIN_DIR=${3:-$DEFAULT_BIN_DIR}

if [ ! -d $BUILD_DIR ] 
then
	mkdir $BUILD_DIR
fi
if [ ! -d $BIN_DIR ]
then
	mkdir $BIN_DIR
fi

cd $BUILD_DIR
cmake $PICKMIDI_DIR
make all
cp $BIN_NAME $BIN_DIR
