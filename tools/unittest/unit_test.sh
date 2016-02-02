#!/bin/sh

set -e
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
cd $SCRIPTPATH

die() {
	echo "$@" 1>&2
	exit 1
}

if [ ! -d output ]
then
	mkdir output
fi

for f in tests/*.test
do
	echo "Processing $f file.."
	input=$(cat $f | sed -n 1p)
	output=$(cat $f | sed -n 2p)
	shouldbe=$(cat $f | sed -n 3p)
	commands=$(tail -n +4 $f)
	commandstring="l patterns/$input 10 10 d 0 q "
	commandstring="$commandstring $commands"
	commandstring="$commandstring x output/$output"
	echo "$commandstring" | gol
	
	DIFFERENCE=$(diff output/$output control/$shouldbe)
	if [ "$DIFFERENCE" ] 
	then
		die "Test $f failed!"
	fi
done

rm -rf output
echo "Success!"
