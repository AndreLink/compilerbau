#!/bin/bash

# Driver to run compiler (dhbwcc) and linker (cat) 

DIR="$(dirname ${BASH_SOURCE[0]})"
CC=$DIR/../../bin/dhbwcc
LD=`which cat`
CRT_S=$DIR/rt/crt.s
LIBC_S=$DIR/rt/libc.s
CRT_IR=
LIBC_IR=$DIR/rt/libc.h

# Check that compiler exists
if [[ ! -e "$CC" ]]; then
	echo "ERROR: Compiler ("$CC") could not be found"
	exit 1
fi

# Check that linker exists
if [[ ! -e "$LD" ]]; then
	echo "ERROR: Linker ("$LD") could not be found"
	exit 1
fi

# Launch the compiler (with all options forwarded)
cc_command="$CC $@"

cat <<- EOM
	----------------------------------------
	Running compiler: $cc_command
	Compiler output:
EOM

cc_output=`eval "$cc_command" 2>&1`
cc_result=$?

cat <<- EOM
	===========================
	$cc_output
	===========================
	Return code: $cc_result
	----------------------------------------
EOM

if [ "$cc_result" -eq 0 ]; then
	echo "SUCCESS: Compiled into file: "$cc_output_file""
else
	echo "ERROR: Stopped toolchain as compilation failed"
	exit 1
fi

# Determine where the assembly and IR went to
cc_output_file_s=`echo "$cc_output" | egrep -m 1 "^Output: " | sed 's/^Output: \(.*\)$/\1/'`
cc_output_file_ir=`echo "$cc_output" | egrep -m 1 "^IR: " | sed 's/^IR: \(.*\)$/\1/'`

function linker {
	cc_output_file=$1
	CRT=$2
	LIBC=$3

	# Launch the linker
	ld_output_file="${cc_output_file}.ld"
	ld_command="$LD $CRT $LIBC $cc_output_file > $ld_output_file"

	cat <<- EOM
		----------------------------------------
		Running linker: $ld_command
		Linker output:
	EOM

	ld_output=`eval "$ld_command" 2>&1`
	ld_result=$?

	cat <<- EOM
		===========================
		$ld_output
		===========================
		Return code: $ld_result
		----------------------------------------
	EOM

	if [ "$ld_result" -eq 0 ]; then
		echo "SUCCESS: Linked into file "$ld_output_file""
	else
		echo "ERROR: Stopped toolchain as linking failed"
		exit 1
	fi
}

linker $cc_output_file_s $CRT_S $LIBC_S
if [ -f "$cc_output_file_ir" ]; then
	linker $cc_output_file_ir $CRT_IR $LIBC_IR
else
	echo "INFO: No intermediate representation has been generated, therefore not trying to link IR code"
fi
echo $cc_output_file_ir
