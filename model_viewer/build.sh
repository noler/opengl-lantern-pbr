#!/bin/bash

if [ -z "$ASSIGNMENT3_ROOT" ]
then
    echo "ASSIGNMENT3_ROOT is not set"
    exit 1
fi

export MODEL_VIEWER_ROOT=$ASSIGNMENT3_ROOT/model_viewer && \

# Generate build script
cd $MODEL_VIEWER_ROOT && \
if [ ! -d build ]; then
    mkdir build
fi
cd build && \
cmake ../ -DCMAKE_INSTALL_PREFIX=$MODEL_VIEWER_ROOT && \

# Build and install the program
make -j4 && \
make install && \

# Run the program
cd ../bin && \
./model_viewer
