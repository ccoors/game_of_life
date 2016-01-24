#!/bin/sh

set -e
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
cd $SCRIPTPATH
clang-format -style=file -i ../src/*.cpp ../src/*.h
