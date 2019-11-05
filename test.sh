#!/bin/bash

# all tests available
IN_DIRS=("numeric/integer" \
		 "numeric/rational" \
		 "construction" \
		 "properties" \
		 "properties/exp-var-C/general-formula" \
		 "properties/exp-var-C/trees" \
		 "properties/exp-var-C/forests" \
		 "properties/exp-var-D/general-formula" \
		 "properties/E_2Cd" \
		 "linear-arrangements/headedness" \
		 "linear-arrangements/D" \
		 "linear-arrangements/C/brute-force-Q" \
		 "linear-arrangements/C/dyn-prog" \
		 "linear-arrangements/C/dyn-prog-list" \
		 "linear-arrangements/C/ladder" \
		 "linear-arrangements/C/ladder-list" \
		 "linear-arrangements/C/stack-based" \
		 "generate" \
		)

OUT_DIRS=("numeric/integer" \
		 "numeric/rational" \
		 "construction" \
		 "properties" \
		 "properties/exp-var-C" \
		 "properties/exp-var-C" \
		 "properties/exp-var-C" \
		 "properties/exp-var-D" \
		 "properties/E_2Cd" \
		 "linear-arrangements/headedness" \
		 "linear-arrangements/D" \
		 "linear-arrangements/C" \
		 "linear-arrangements/C" \
		 "linear-arrangements/C" \
		 "linear-arrangements/C" \
		 "linear-arrangements/C" \
		 "linear-arrangements/C" \
		 "generate" \
		 )

########################################################################
# Functions

function show_usage() {
	echo "Welcome to the LAL tester"
	echo ""
	echo "This script has several options, some of them mandatory, some"
	echo "others optional."
	echo ""
	echo "In order to see this message again run this script with:"
	echo ""
	echo "    [-h, --help]: show the usage"
	echo ""
	echo "Mandatory parameters:"
	echo ""
	echo "    --input=d : specify the input directory 'd' of tests to be performed."
	echo "        This string is the path to the directory with all the"
	echo "        input tests. It can not contain the last slash ('/')"
	echo "        and can not start with 'inputs'."
	echo ""
	echo "    --output=d : specify the output directory 'd' that will be"
	echo "        compared against the outputs of the inputs tests."
	echo "        This string is the path to the directory with the"
	echo "        output tests. It can not contain the last slash ('/')"
	echo "        and can not start with 'outputs'."
	echo ""
	echo "Optional parameters:"
	echo ""
	echo "    --all : execute tests automatically. This option does not"
	echo "        need '--input' and '--output'. If indicated they are"
	echo "        ignored. The tests execute are all the tests implemented"
	echo "        so far, except for those 'brute-force'"
	echo ""
	echo "    --valgrind : use the memory error detector on every input"
	echo "        test. This option makes the test ignore the output"
	echo "        produced and errors are only reported when valgrind"
	echo "        does so. Therefore, when this option is given, there"
	echo "        is no need to specify an output (--output)."
	echo "        The release mode should not be indicated."
	echo ""
	echo "    --debug --release: use one of these two options to indicate"
	echo "        what type of build you want to run the tests with."
	echo ""
	echo "        --debug : suitable for valgrind in order to detect memory leaks"
	echo "        --release : suitable when testing lengthy computations."
	echo ""
	echo "        The default option is '--debug'"
	echo ""
}

# Check the result of the tester when valgrind is used.
function check_res_valgrind() {
	test_err=$1
	valg_err=$2
	
	# When using valgrind, we only need to ensure that
	# errors (memory leaks, out of bounds accesses, ...)
	# were produced.
	if [ -s $test_err ]; then
		echo -en "\e[1;4;31mThe test produced errors\e[0m "
		echo "See result in $valg_err"
		mv $test_err $valg_err
	else
		# no errors were detected
		echo -e "\e[1;1;32mOk\e[0m"
	fi
}

# Check the result of the tester when valgrind is NOT used.
function check_res_no_valgrind() {
	test_out=$1			# $TEST_OUT.$ID
	temp_test_err=$2	# $TEST_ERR
	test_err=$3			# $TEST_ERR.$ID
	prog_out=$4			# $PROG_OUT
	base_out_file=$5	# $output_group/$f
	
	# replace Windows-like endlines with Linux-like endlines
	prog_out=$(echo "$prog_out" | sed 's/\n\r/\n/g')
	
	# If the output is incorrect, store it into a file.
	
	# Check whether there were errors or not. If there were,
	# issue an error message and stop. If there were not,
	# check the output produced.
	
	if [ -s $temp_test_err ]; then
		echo -en "\e[1;4;31mErrors were produced\e[0m "
		echo "See errors in $test_err"
		mv $temp_test_err $test_err
		echo "$prog_out" > $test_out
	else
		# test whether the error output produced is empty or not
		BASE_CONTENTS=$(cat $base_out_file)
		
		DIFF=$(diff <(echo "$BASE_CONTENTS") <(echo "$prog_out"))
		if [ ! -z "$DIFF" ]; then
			echo -en "\e[1;4;31mDifferent outputs\e[0m "
			echo "See result in $test_out"
			echo "$prog_out" > $test_out
		else
			# output produced by library is
			# equal to the output made by hand
			echo -e "\e[1;1;32mOk\e[0m"
		fi
	fi
}

# Execute all tests in an input directory.
function exe_group() {
	input=$1
	output=$2
	progress=$3
	
	# Make sure the input directory exists
	input_group=inputs/$input
	if [ ! -d $input_group ]; then
		echo -en "\e[1;4;31mError:\e[0m Input directory "
		echo -en "\e[2;4;37m$input_group\e[0m"
		echo -e " does not exist"
		exit
	fi
	
	# Make sure the output directory exists
	output_group=outputs/$output
	if [ ! -d $output_group ]; then
		echo -en "\e[1;4;31mError:\e[0m Output directory "
		echo -en "\e[2;4;37m$output_group\e[0m"
		echo -e " does not exist"
		exit
	fi
	
	# ------------------------------------------------
	# Prepare filenames for standard and error outputs
	
	# Given an input directory a/b/c
	# keys: is the list a b c
	keys=$(echo $input | tr "/" "\n");
	# id: is the string a.b.c
	id=$(echo $keys | tr ' ' '.')
	
	# TEST_OUT and TEST_ERR are temporary files: a test named, for example,
	# test-0015 will produce a standard output and an error output into
	# $TEST_OUT and $TEST_ERR. Then, in case of errors, each file will be
	# moved to a unique file for this test, namely into the files
	# $TEST_OUT.0015 and $TEST_ERR.0015. In case of valgrind the contents
	# of $TEST_ERR will be moved to $VALG_ERR.0015.
	TEST_OUT=.out.$id
	TEST_ERR=.err.$id
	VALG_ERR=.vgd.$id
	
	echo -e "\e[1;1;33mExecuting tests in\e[0m \e[1;2;33m$input_group\e[0m ($progress)"
	
	# execute all tests
	all_test_files=$(ls $input_group | grep test)
	n_test_files=$(ls $input_group | grep test | wc -l)
	nth_test=1
	
	for f in $all_test_files; do
		# retrieve id number from input test file
		# e.g.: 'test-0010' -> '0010'
		INFILE_LENGTH=${#f}
		ID=${f:5:($INFILE_LENGTH - 4)}
		
		echo -en "    \e[1;1;34m$f\e[0m ($nth_test/$n_test_files) "
		PROG_OUT=$($EXE_COMMAND -i $input_group/$f 2> $TEST_ERR)
		nth_test=$(($nth_test + 1))
		
		# parse result of execution command differently,
		# depending on whether we are using valgrind or not.
		if [ $use_valgrind == 1 ]; then
			check_res_valgrind 		\
				$TEST_ERR 			\
				$VALG_ERR.$ID
		else
			check_res_no_valgrind 	\
				$TEST_OUT.$ID 		\
				$TEST_ERR 			\
				$TEST_ERR.$ID		\
				"$PROG_OUT"			\
				$output_group/$f
		fi
	done
	
	# Remove those unnecessary temporary files.
	# File $VALG_ERR is never actually used 'as is'.
	rm -f $TEST_OUT $TEST_ERR
}

########################################################################
# Code starts here

# show usage
usage=0
# the directory of input tests to be executed
input_dir=""
# the directory of output tests to be executed
output_dir=""
# use valgrind
use_valgrind=0
# release or debug?
debug=0
release=0
# execute all tests
exe_all=0

for i in "$@"; do
	case $i in
		
		--help|-h)
		usage=1
		shift
		;;
		
		--input=*)
		input_dir="${i#*=}"
		shift
		;;
		
		--output=*)
		output_dir="${i#*=}"
		shift
		;;
		
		--valgrind)
		use_valgrind=1
		shift
		;;
		
		--debug)
		debug=1
		release=0
		shift
		;;
		
		--release)
		debug=0
		release=1
		shift
		;;
		
		--all)
		exe_all=1
		shift
		;;
		
		*)
		echo -e "\e[1;4;31mError:\e[0m Option $i unknown"
		;;
	esac
done

# show usage if indicated
if [ $usage == 1 ]; then
	show_usage
	exit
fi

########################################################################
# Parameter errors

if [ $exe_all = 0 ] && [ -z $input_dir ]; then
	echo -e "\e[1;4;31mError:\e[0m No input directory specified."
	exit
fi
if [ $use_valgrind == 0 ]; then
	if [ $exe_all = 0 ] && [ -z $output_dir ]; then
		echo -e "\e[1;4;31mError:\e[0m No output directory specified."
		exit
	fi
fi

if [ $release == 1 ] && [ $use_valgrind == 1 ]; then
	echo -e "\e[1;4;31mError:\e[0m Conflicting options"
	echo "    --valgrind --release"
	exit
fi

########################################################################
# Execution mode (debug/release)

EXE_MODE=""
if [ $debug == 1 ]; then
	EXE_MODE="debug"
elif [ $release == 1 ]; then
	EXE_MODE="release"
else
	# use debug by default
	EXE_MODE="debug"
fi

########################################################################
# Make sure we can execute something

EXE_COMM=""
EXE_DIR=""
if [ "$EXE_MODE" == "debug" ]; then
	EXE_DIR="build-debug"
elif [ "$EXE_MODE" == "release" ]; then
	EXE_DIR="build-release"
fi
EXE_COMM=$EXE_DIR/tests

if [ ! -f $EXE_COMM ]; then
	echo -e "\e[1;4;31mError:\e[0m C++ executable file $EXE_COMM does not exist"
	exit
fi
EXE_COMM=./$EXE_COMM

# Prepare execution command. If valgrind is
# requested, make the command appropriately.
if [ $use_valgrind == 1 ]; then
	EXE_COMMAND="valgrind -q --leak-check=full $EXE_COMM"
else
	EXE_COMMAND="$EXE_COMM"
fi

########################################################################
# Display information about what software is being tested.
echo -en "Testing "
if [ "$EXE_MODE" == "debug" ]; then
	echo -en "in \e[1;3;36mdebug\e[0m mode"
	if [ $use_valgrind == 1 ]; then
		echo " and using valgrind."
	else
		echo "."
	fi
elif [ "$EXE_MODE" == "release" ]; then
	echo -e "in \e[1;3;36mrelease\e[0m mode."
fi

########################################################################
# Execute the tests

if [ $exe_all == 1 ]; then
	n_tests="${#IN_DIRS[@]}"
	lim=$(($n_tests - 1))
	for i in `seq 0 $lim`; do
		idx=$(($i + 1))
		in="${IN_DIRS[$i]}"
		out="${OUT_DIRS[$i]}"
		exe_group $in $out $idx/$n_tests
	done
else
	exe_group $input_dir $output_dir "1/1"
fi


