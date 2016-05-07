#!/bin/bash

if [ $# -eq 0 ]; then
    DEST="../scopetemplates-build/src/"

    mkdir -p $DEST
    cp -r data/images $DEST/
    
    echo "Setup complete."
    exit 0;
fi
