#!/bin/bash

build() {
    # build emulator
    cd baka_emulator/
    make
    mv emulator ../
    # build assembler
    cd ../baka_assembler/
    make
    mv assembler ../
}

clean() {
    rm emulator
    rm assembler
}

if [ $1 = "build" ]; then
    build
elif [ $1 = "clean" ]; then
    clean
else
    # build by default
    build
fi