#!/bin/bash

# Wrapper to run the Mars MIPS32 emulator

DIR="$(dirname ${BASH_SOURCE[0]})"
CPU=$DIR/Mars.jar
JVM=`which java`

# Check that emulator exists
if [[ ! -e "$CPU" ]]; then
	echo "ERROR: The MIPS32 emulator ("$CPU") could not be found"
	exit 1
fi

# Check that JVM exists
if [[ ! -e "$JVM" ]]; then
	echo "ERROR: JVM ("$JVM") could not be found"
	exit 1
fi

# Run the emulator
$JVM -jar $CPU $@
