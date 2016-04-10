#!/bin/bash

if [ $# -eq 0 ]; then
    DEST="../build-scopetemplates-Desktop-Default/src/"

    mkdir -p $DEST
    cp -r data/images $DEST/
    
    echo "Setup complete."
    exit 0;
fi
