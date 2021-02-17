#!/bin/bash

########################################################################
# Data

source test_directories.sh

source test_dir_generate.sh
source test_dir_graphs.sh
source test_dir_internal.sh
source test_dir_linarr.sh
source test_dir_memory.sh
source test_dir_numeric.sh
source test_dir_properties.sh
source test_dir_utilities.sh

########################################################################
# Functions

function display_two() {
	test=$1
	total=$2
	echo -n " ("
	if [ $total -lt 10 ]; then
		# do nothing
		echo -n ""
	elif [ $total -lt 100 ]; then
		if [ $test -lt 10 ]; then
			echo -n "0"
		fi
	elif [ $total -lt 1000 ]; then
		if [ $test -lt 10 ]; then
			echo -n "00"
		elif [ $test -lt 100 ]; then
			echo -n "0"
		fi
	fi
	echo -n "$test/$total) "
}

function show_usage() {
	echo "Welcome to the LAL tester"
	echo ""
	echo "This script has several options, some of them mandatory, some"
	echo "others optional."
	echo ""
	echo "In order to see this message again run this script with:"
	echo ""
	echo "    [-h, --help]: shows the usage"
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
	echo "    --exe-group=t : execute a group of tests. Available groups are:"
	echo "        all : execute all tests"
	echo ""
	echo "        generate : execute generation tests"
	echo "        generate_trees_alf : execute generation (All Labelled Free) tests"
	echo "        generate_trees_alr : execute generation (All Labelled Rooted) tests"
	echo "        generate_trees_auf : execute generation (All Unlabelled Free) tests"
	echo "        generate_trees_aur : execute generation (All Unlabelled Rooted) tests"
	echo "        generate_trees_rlf : execute generation (Rand Labelled Free) tests"
	echo "        generate_trees_rlr : execute generation (Rand Labelled Rooted) tests"
	echo "        generate_trees_ruf : execute generation (Rand Unlabelled Free) tests"
	echo "        generate_trees_rur : execute generation (Rand Unlabelled Rooted) tests"
	echo "        generate_arr_all_proj : execute generation (All Projective Arrangements) tests"
	echo "        generate_arr_rand_proj : execute generation (Rand Projective Arrangements) tests"
	echo ""
	echo "        graphs : execute all graph tests"
	echo "        graphs_construction : execute graph construction tests"
	echo "        graphs_tree_types : execute tree classification tests"
	echo ""
	echo "        internal : execute internal tests"
	echo "        internal_sorting : execute sorting internal tests"
	echo "        internal_centre : execute centre internal tests"
	echo "        internal_centroid : execute centroid internal tests"
	echo "        internal_traversal : execute traversal internal tests"
	echo ""
	echo "        linarr : execute linear arrangement tests"
	echo "        linarr_E_2Cd : execute linear arrangement (approximation of C) tests"
	echo "        linarr_C : execute linear arrangement (crossings) tests"
	echo "        linarr_C_dyn_prog : execute linear arrangement (crossings) tests (dynamic programming algorithm)"
	echo "        linarr_C_ladder : execute linear arrangement (crossings) tests (ladder algorithm)"
	echo "        linarr_C_stack_based : execute linear arrangement (crossings) tests (stack-based algorithm)"
	echo ""
	echo "        linarr_Dmin : execute all Minimum Linear Arrangement (mLA) tests"
	echo "        linarr_Dmin_Comparison: execute mLA tests to compare the algorithms"
	echo "        linarr_Dmin_Projective : execute mLA tests for projective arrangements"
	echo "        linarr_Dmin_Planar : execute mLA tests for planar arrangements"
	echo "        linarr_Dmin_Unconstrained_YS : execute mLA tests for unconstrained arrangements -- algorithms: Yossi Shiloach's"
	echo "        linarr_Dmin_Unconstrained_FC : execute mLA tests for unconstrained arrangements -- algorithms: Fan Chung's"
	echo "        linarr_syndepstruc_class : execute tests of classification of syntactic dependencies structures"
	echo "        linarr_comp_dependency_flux : execute tests of dependency flux computation"
	echo ""
	echo "        memory : execute tests that try to stress the memory"
	echo "                 Usage of valgrind and execution of a debug compilation are encouraged"
	echo ""
	echo "        numeric : execute numeric tests"
	echo ""
	echo "        properties : execute properties tests"
	echo "        properties_basic : execute basic properties tests (tests in the root directory of inputs/properties)"
	echo "        properties_exp_var_C : execute tests for the expectation and variance of C"
	echo "        properties_exp_var_C_no_Q : execute tests for the expectation and variance of C (w/o Q)"
	echo "        properties_exp_var_D : execute tests for the expectation and variance of D"
	echo ""
	echo "        utilities : execute utilities tests"
	echo "        utilities_isomorphism : execute tree isomorphism tests"
	echo "        utilities_isomorphism_small : execute the smaller tree isomorphism tests"
	echo ""	
	echo "Optional parameters:"
	echo ""
	echo "    --log-file : indicate the name of the log file."
	echo "        Default: 'execution_log'"
	echo ""
	echo "    --no-make : prevent the script from invoking 'make'."
	echo ""
	echo "    --all : execute tests automatically. This is equivalent to"
	echo "        --exe-group=all"
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
	# input test file
	test_name=$1
	# the temporary file where the error output is stored
	test_err=$2
	# the file where to store the contents of the error output in case there were any
	valg_err=$3
	
	# When using valgrind, we only need to ensure that errors
	# (memory leaks, out of bounds accesses, ...) were NOT produced.
	# Here we can ignore base output files.
	if [ -s $test_err ]; then
		echo -en "\e[1;4;31mThe test produced errors\e[0m "
		echo "See result in $valg_err"
		mv $test_err $valg_err
		
		echo "$(date +"%Y/%m/%d.%T")        Error: when executing test $test_name -- Valgrind produced errors..." >> $log_file
	else
		# no errors were detected
		echo -e "\e[1;1;32mOk\e[0m"
	fi
}

# Check the result of the tester when valgrind is NOT used.
function check_res_no_valgrind() {
	# input test file
	test_name=$1
	# output base file
	base_out_file=$2

	# the file where to store the output of the program if said output
	# differs from the contents of the output base file
	test_out=$3
	# a temporary file where the error output is stored
	temp_test_err=$4
	# the file where to store the error output of the program if it ever
	# produced an error output (like error messages and whatnot)
	test_err=$5			

	# output of the execution of the tester
	prog_out=$6
	
	# check if base output exists. If not, issue an error
	# message and do not execute
	if [ ! -f $base_out_file ]; then
		
		echo -e "\e[1;3;31mOutput base file does not exist\e[0m Skipping comparison..."
		echo "$(date +"%Y/%m/%d.%T")            Warning: when executing test $test_name -- Output base file does not exist. Skipping comparison..." >> $log_file
		
	else
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
			
			echo "$(date +"%Y/%m/%d.%T")            Error: when executing test $test_name -- Errors were produced" >> $log_file
		else
			# test whether the error output produced is empty or not
			BASE_CONTENTS=$(cat $base_out_file)
			
			DIFF=$(diff <(echo "$BASE_CONTENTS") <(echo "$prog_out"))
			if [ ! -z "$DIFF" ]; then
				echo -en "\e[1;4;31mDifferent outputs\e[0m "
				echo "See result in $test_out"
				echo "$prog_out" > $test_out
				echo "$(date +"%Y/%m/%d.%T")            Error: when executing test $test_name -- Output of test differs from base output" >> $log_file
			else
				# output produced by library is
				# equal to the output made by hand
				echo -e "\e[1;1;32mOk\e[0m"
			fi
		fi
	fi
}

# Execute all tests in an input directory.
function execute_group() {
	input=$1
	output=$2
	progress_1=$3
	progress_2=$4
	storage=$5
	
	skip=0
	
	# Make sure the input directory exists
	input_group=inputs/$input
	if [ ! -d $input_group ]; then
		echo -en "\e[1;4;31mError:\e[0m Input directory "
		echo -en "\e[2;4;37m$input_group\e[0m"
		echo -e " does not exist. Skipping..."
		
		echo "$(date +"%Y/%m/%d.%T")            Warning: input $input does not exist. Skipping..." >> $log_file
		skip=1
	fi
	
	# Make sure the output directory exists
	output_group=outputs/$output
	if [ ! -d $output_group ]; then
		echo -en "\e[1;4;31mError:\e[0m Output directory "
		echo -en "\e[2;4;37m$output_group\e[0m"
		echo -e " does not exist. Skipping..."
		
		echo "$(date +"%Y/%m/%d.%T")            Warning: output $output does not exist. Skipping..." >> $log_file
		skip=1
	fi
	
	if [ $skip = 0 ]; then
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
		
		echo -en "\e[1;1;33mExecuting tests in\e[0m \e[1;2;33m$input_group\e[0m"
		display_two $progress_1 $progress_2
		echo ""
		
		# retrieve the names of the test files
		all_test_files=$(ls $input_group | grep test)
		n_test_files=$(ls $input_group | grep test | wc -l)
		nth_test=1
		
		for f in $all_test_files; do
			# retrieve id number from input test file
			# e.g.: 'test-0010' -> '0010'
			INFILE_LENGTH=${#f}
			ID=${f:5:($INFILE_LENGTH - 4)}
			
			# Display the progress within this group:
			# output the name of the test file plus the index of the
			# file with respect to the total tests within the group.
			echo -en "    \e[1;1;34m$f\e[0m"
			display_two $nth_test $n_test_files
			
			# Execute the program NOW
			PROG_OUT=$($EXE_COMMAND -i $input_group/$f 2> $storage/$TEST_ERR)

			# increment the amount of tests executed by 1
			nth_test=$(($nth_test + 1))
			
			# parse result of execution command differently,
			# depending on whether we are using valgrind or not.
			if [ $use_valgrind == 1 ]; then
				check_res_valgrind 				\
					$input_group/$f 			\
					$storage/$TEST_ERR 			\
					$storage/$VALG_ERR.$ID
			else
				check_res_no_valgrind 			\
					$input_group/$f				\
					$output_group/$f			\
					$storage/$TEST_OUT.$ID 		\
					$storage/$TEST_ERR 			\
					$storage/$TEST_ERR.$ID		\
					"$PROG_OUT"
			fi
		done
		
		# Remove those unnecessary temporary files.
		# File $VALG_ERR is never actually used 'as is'.
		rm -f $TEST_OUT $TEST_ERR
	fi
}

function apply_group() {
	exe_group=$1		# execution group
	
	# if the group is valid, then execute it
	echo "Executing group '$exe_group'"
	
	# variable name
	dir_name="$exe_group""_DIRS[@]"
	# actual array
	DIRS_TO_PROCESS=("${!dir_name}")
	# size of the array
	n_dirs=${#DIRS_TO_PROCESS[@]}
	# number of input directories
	n_dirs=$(($n_dirs/2))
	
	# execute
	for ((i=0; i<$n_dirs; ++i)); do
		idx=$(($i + 1))
		i1=$((2*$i    ))
		i2=$((2*$i + 1))
		in="${DIRS_TO_PROCESS[$i1]}"
		out="${DIRS_TO_PROCESS[$i2]}"

		echo "$(date +"%Y/%m/%d.%T")        Start executing tests in $in." >> $log_file
		execute_group $in $out $idx $n_dirs $storage_dir
		echo "$(date +"%Y/%m/%d.%T")        Finished executing tests in $in." >> $log_file
	done
}

########################################################################
# Code starts here

# show usage
usage=0
# the directory of input tests to be executed
input_dir=""
# the directory of output files corresponding to the inputs
output_dir=""
# use valgrind
use_valgrind=0
# release or debug?
debug=0
release=0
# execute tests of a certain type
exe_group=0
# log file
log_file="execution_log"
# where to put the error outputs, the outputs that differ from the base
# output files, and the valgrind outputs (if any)
storage_dir="."
# compile (or not) the executable
compile=1

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
		
		--exe-group=*)
		exe_group="${i#*=}"
		shift
		;;
		
		--log-file=*)
		log_file="${i#*=}"
		shift
		;;
		
		--storage-dir=*)
		storage_dir="${i#*=}"
		shift
		;;

		--no-make)
		compile=0
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
		exe_group="all"
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

if [ $exe_group == 0 ] && [ -z $input_dir ]; then
	echo -e "\e[1;4;31mError:\e[0m No input directory specified."
	exit
fi
if [ $exe_group == 0 ] && [ -z $output_dir ]; then
	# we only care about output files if valgrind is not used
	if [ $use_valgrind == 0 ]; then
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

# use debug by default
EXE_MODE="debug"
if [ $debug == 1 ]; then
	EXE_MODE="debug"
elif [ $release == 1 ]; then
	EXE_MODE="release"
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

if [ $compile -eq 1 ]; then
	# make sure the executable is up to date
	cd $EXE_DIR
	make -j4
	cd ..
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
# Display where the files are being written

log_file=$storage_dir/$log_file

echo "    Storage directory: '$storage_dir'"
echo "    Log file: '$log_file'"

########################################################################
# Execute the tests

echo "$(date +"%Y/%m/%d.%T")    Started test execution of group '$exe_group'." >> $log_file

if [ $exe_group != 0 ]; then
	
	# test that 'exe_group' value is valid
	array_has=0
	case "${groups_list[@]}" in
		# the '*' are important!
		*"$exe_group"*)
			array_has=1
			;;
	esac
	
	if [ $array_has == 1 ]; then
		
		if [ "$exe_group" == "all" ]; then
			for g in "generate" "graphs" "internal" "linarr" "memory" "numeric" "properties" "numeric"; do
				apply_group $g
			done
		else
			apply_group $exe_group
		fi
		
	else
		echo -e "\e[1;4;31mError:\e[0m Invalid execution group '$exe_group'"
		
		echo "$(date +"%Y/%m/%d.%T")        Error: test execution failed. Group '$exe_group' is not valid." >> $log_file
	fi
else
	execute_group $input_dir $output_dir 1 1 $storage_dir
fi

echo "$(date +"%Y/%m/%d.%T")    Finished test execution of group '$exe_group'." >> $log_file

echo "A summary of the test has been logged into file '$log_file'"
