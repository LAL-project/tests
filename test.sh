#!/bin/bash

################################################################################
# Data

source test_groups.sh

source test_dir_detail.sh
source test_dir_generate.sh
source test_dir_graphs.sh
source test_dir_io.sh
source test_dir_linarr.sh
source test_dir_memory.sh
source test_dir_numeric.sh
source test_dir_properties.sh
source test_dir_utilities.sh

################################################################################
# Functions

function display_two() {
	local test=$1
	local total=$2
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
	echo "        small-tests : execute a small amout of tests"
	echo ""
	echo "        detail : execute detail tests"
	echo "            detail_sorting : execute sorting detail tests"
	echo "            detail_traversal : execute traversal detail tests"
	echo "            detail_avl : execute AVL detail tests"
	echo ""
	echo "        generate : execute generation tests"
	echo "            generate_trees : execute generation tests for all trees"
	echo "                generate_trees_all : execute tests for exhaustive generation of trees"
	echo "                    generate_trees_alf : execute generation (All Labelled Free) tests"
	echo "                    generate_trees_alr : execute generation (All Labelled Rooted) tests"
	echo "                    generate_trees_auf : execute generation (All Unlabelled Free) tests"
	echo "                    generate_trees_aur : execute generation (All Unlabelled Rooted) tests"
	echo "                generate_trees_rand : execute tests for random generation of trees"
	echo "                    generate_trees_rlf : execute generation (Rand Labelled Free) tests"
	echo "                    generate_trees_rlr : execute generation (Rand Labelled Rooted) tests"
	echo "                    generate_trees_ruf : execute generation (Rand Unlabelled Free) tests"
	echo "                    generate_trees_rur : execute generation (Rand Unlabelled Rooted) tests"
	echo "            generate_arrangements : execute generation tests for all arrangements"
	echo "                generate_arrangements_all : execute tests for exhaustive generation of arrangements"
	echo "                    generate_arrangements_all_unc : execute generation (All unconstrained Arrangements) tests"
	echo "                    generate_arrangements_all_bipartite : execute generation (All bipartite Arrangements) tests"
	echo "                    generate_arrangements_all_projective : execute generation (All projective Arrangements) tests"
	echo "                    generate_arrangements_all_planar : execute generation (All planar Arrangements) tests"
	echo "                generate_arrangements_rand : execute tests for random generation of arrangements"
	echo "                    generate_arrangements_rand_unc : execute generation (Rand unconstrained Arrangements) tests"
	echo "                    generate_arrangements_rand_bipartite : execute generation (Rand bipartite Arrangements) tests"
	echo "                    generate_arrangements_rand_projective : execute generation (Rand projective Arrangements) tests"
	echo "                    generate_arrangements_rand_planar : execute generation (Rand planar Arrangements) tests"
	echo ""
	echo "        graphs : execute all graph tests"
	echo "            graphs_construction : execute graph construction tests"
	echo "            graphs_tree_types : execute tree classification tests"
	echo "            graphs_graph_literals : execute graph literals tests"
	echo ""
	echo "        io : execute io tests"
	echo "            io_correctness : execute correctness tests of treebanks"
	echo ""
	echo "        linarr : execute linear arrangement tests"
	echo "            linarr_arrangement_validity : tests for the classification of arrangements in projective, planar and unconstrained"
	echo "            linarr_linear_arrangement : tests for the construction of linear arrangements"
	echo "            linarr_E_2Cd : execute linear arrangement (approximation of C) tests"
	echo "            linarr_chunking : chunking tests"
	echo "                linarr_chunking_Anderson : chunking tests -- Anderson's algorithm"
	echo "                linarr_chunking_Macutek : chunking tests -- Macutek's algorithm"
	echo "            linarr_C : number of edge crossings tests"
	echo "                linarr_C_bruteforce : number of edge crossings tests (brute force algorithm)"
	echo "                    linarr_C_bruteforce_C"
	echo "                    linarr_C_bruteforce_C_list"
	echo "                    linarr_C_bruteforce_C_upper_bound"
	echo "                    linarr_C_bruteforce_C_list_upper_bound"
	echo "                    linarr_C_bruteforce_C_list_upper_bound_list"
	echo "                linarr_C_dyn_prog : number of edge crossings tests (dynamic programming algorithm)"
	echo "                    linarr_C_dyn_prog_C"
	echo "                    linarr_C_dyn_prog_C_list"
	echo "                    linarr_C_dyn_prog_C_upper_bound"
	echo "                    linarr_C_dyn_prog_C_list_upper_bound"
	echo "                    linarr_C_dyn_prog_C_list_upper_bound_list"
	echo "                linarr_C_ladder : number of edge crossings tests (ladder algorithm)"
	echo "                    linarr_C_ladder_C"
	echo "                    linarr_C_ladder_C_list"
	echo "                    linarr_C_ladder_C_upper_bound"
	echo "                    linarr_C_ladder_C_list_upper_bound"
	echo "                    linarr_C_ladder_C_list_upper_bound_list"
	echo "                linarr_C_stack_based : number of edge crossings tests (stack-based algorithm)"
	echo "                    linarr_C_stack_based_C"
	echo "                    linarr_C_stack_based_C_list"
	echo "                    linarr_C_stack_based_C_upper_bound"
	echo "                    linarr_C_stack_based_C_list_upper_bound"
	echo "                    linarr_C_stack_based_C_list_upper_bound_list"
	echo "            linarr_D : sum of edge lengths tests"
	echo "            linarr_Dmin : execute all Minimum Linear Arrangement (mLA) tests"
	echo "                linarr_Dmin_Comparison : execute mLA tests to compare the algorithms for Dmin"
	echo "                    linarr_Dmin_Comparison_Plan_vs_Plan : execute mLA tests to compare the algorithms for Dmin Planar among themselves"
	echo "                    linarr_Dmin_Comparison_Plan_vs_Unc : execute mLA tests to compare the algorithms for Dmin Planar between Dmin Unconstrained"
	echo "                    linarr_Dmin_Comparison_Unc_vs_Unc : execute mLA tests to compare the algorithms for Dmin Unconstrained among themselves"
	echo "                linarr_Dmin_Projective : execute mLA tests for Projective minLA"
	echo "                    linarr_Dmin_Projective_AEF : execute mLA tests for Projective miNLA -- algorithm: AEF"
	echo "                    linarr_Dmin_Projective_HS : execute mLA tests for Projective minLA -- algorithm: HS"
	echo "                linarr_Dmin_Planar : execute mLA tests for Planar minLA"
	echo "                    linarr_Dmin_Planar_AEF : execute mLA tests for Planar minLA -- algorithm: AEF"
	echo "                    linarr_Dmin_Planar_HS : execute mLA tests for Planar minLA -- algorithm: HS"
	echo "                linarr_Dmin_Bipartite : execute mLA tests for Bipartite minLA"
	echo "                    linarr_Dmin_Bipartite_AEF : execute mLA tests for Bipartite minLA -- algorithm: AEF"
	echo "                linarr_Dmin_Unconstrained : execute mLA tests for Unconstrained minLA"
	echo "                    linarr_Dmin_Unconstrained_YS : execute mLA tests for Unconstrained minLA -- algorithm: Yossi Shiloach's"
	echo "                    linarr_Dmin_Unconstrained_FC : execute mLA tests for Unconstrained minLA -- algorithm: Fan Chung's"
	echo "            linarr_DMax : execute all Maximum Linear Arrangement (MLA) tests"
	echo "                linarr_DMax_Comparison : execute MLA tests to compare the algorithms"
	echo "                linarr_DMax_Projective : execute MLA tests for Projective MaxLA"
	echo "                    linarr_DMax_Projective_AEF : execute MLA tests for Projective MaxLA. This includes calculation of DMax for all roots using DMax planar function -- algorithm: AEF"
	echo "                linarr_DMax_Planar : execute MLA tests for Planar MaxLA"
	echo "                    linarr_DMax_Planar_AEF : execute MLA tests for Planar MaxLA -- algorithm: AEF"
	echo "                linarr_DMax_Bipartite : execute MLA tests for Bipartite MaxLA"
	echo "                    linarr_DMax_Bipartite_AEF : execute MLA tests for Bipartite MaxLA -- algorithm: AEF"
	echo "                linarr_DMax_1_eq_thistle : execute MLA tests for 1-thistle MaxLA"
	echo "                    linarr_DMax_1_eq_thistle_AEF : execute MLA tests for 1-thistle MaxLA -- algorithm: AEF"
	echo "                linarr_DMax_1_le_thistle : execute MLA tests for 1-thistle MaxLA"
	echo "                    linarr_DMax_1_le_thistle_AEF : execute MLA tests for 1-thistle MaxLA -- algorithm: AEF"
	echo "            linarr_syndepstruc_class : execute tests of classification of syntactic dependencies structures"
	echo "            linarr_comp_dependency_flux : execute tests of dependency flux computation"
	echo "            linarr_headedness : execute headedness tests"
	echo "            linarr_k_level : execute tests of k-level metrics"
	echo "            linarr_MDD : execute tests of mean dependency distance computation"
	echo ""
	echo "        memory : execute tests that try to stress the memory (Usage of valgrind and execution of a debug compilation are encouraged)"
	echo "            memory_graphs : execute tests for the graphs namespace only"
	echo "            memory_numeric : execute tests for the numeric namespace only"
	echo "            memory_detail : execute tests for the detail namespace only"
	echo ""
	echo "        numeric : execute numeric tests"
	echo ""
	echo "        properties : execute properties tests"
	echo "            properties_basic : execute basic properties tests (tests in the root directory of inputs/properties)"
	echo "            properties_coloring : execute coloring tests"
	echo "            properties_centre : execute centre tests"
	echo "            properties_centroid : execute centroid tests"
	echo "            properties_diameter : execute diameter tests"
	echo "            properties_branchless_paths : execute branchless paths tests"
	echo "            properties_exp_var_C : execute tests for the expectation and variance of C"
	echo "                properties_exp_var_C_no_Q : execute tests for the expectation and variance of C (w/o Q)"
	echo "                properties_exp_var_C_Q : execute tests for the expectation and variance of C (w/ Q)"
	echo "                properties_exp_var_C_trees : execute tests for the expectation and variance of C for trees"
	echo "                properties_exp_var_C_forests : execute tests for the expectation and variance of C for forests"
	echo "            properties_exp_var_D : execute tests for the expectation and variance of D"
	echo "                properties_exp_var_D_unconstrained : execute tests for the expectation and variance of D in unconstrained arrangements"
	echo "                properties_exp_D_bipartite : execute tests for the expectation of D in bipartite arrangements"
	echo "                properties_exp_D_planar : execute tests for the expectation of D in planar arrangements"
	echo "                properties_exp_D_projective : execute tests for the expectation of D in projective arrangements"
	echo "            properties_maximum_subtrees : execute tests for finding maximum subtrees in a graph"
	echo "                properties_maximum_subtrees_caterpillar : execute tests for finding maximum spanning caterpillars"
	echo ""
	echo "        utilities : execute utilities tests"
	echo "            utilities_isomorphism : execute tree isomorphism tests"
	echo "                utilities_isomorphism_small : execute the smaller tree isomorphism tests"
	echo "                utilities_isomorphism_bid : execute the bigger tree isomorphism tests"
	echo ""	
	echo "Optional parameters:"
	echo ""
	echo "    --log-file=name : indicate the name of the log file."
	echo "        Default: 'execution_log'"
	echo ""
	echo "    --no-make : prevent the script from invoking 'make'."
	echo ""
	echo "    --all : execute tests automatically."
	echo "        This is equivalent to"
	echo "        --exe-group=all"
	echo ""
	echo "    --small-tests : execute smaller tests."
	echo "        This is equivalent to"
	echo "        --exe-group=small-tests"
	echo ""
	echo "    --valgrind : use the memory error detector on every input"
	echo "        test. This option makes the test ignore the output"
	echo "        produced and errors are only reported when valgrind"
	echo "        does so. Therefore, when this option is given, there"
	echo "        is no need to specify an output (--output)."
	echo "        The release mode should not be indicated."
	echo ""
	echo "    --exe-directory=dir : specify the directory that contains"
	echo "        the executable files."
	echo ""
	echo "    --debug --release: indicate what type of build you want to"
	echo "        run the tests with."
	echo ""
	echo "        --debug : if the build contains debugging symbols,"
	echo "        --release : if the build is optimized and does not contain"
	echo "            debugging symbol."
	echo ""
	echo "    --run-debug, --run-release : execute a debug or release build"
	echo ""
	echo "        These are equivalent to"
	echo ""
	echo "        --exe-directory=build-debug --debug"
	echo "        --exe-directory=build-release --release"
	echo ""
}

# Check the result of the tester when valgrind is used.
function check_res_valgrind() {
	# input test file
	local test_name=$1
	# the temporary file where the error output is stored
	local test_err=$2
	# the file where to store the contents of the error output in case there were any
	local valg_err=$3
	
	# When using valgrind, we only need to ensure that errors
	# (memory leaks, out of bounds accesses, ...) were NOT produced.
	# Here we can ignore base output files.
	if [ -s $test_err ]; then
		echo -en "\e[1;4;31mThe test produced errors\e[0m "
		echo "See result in $valg_err"
		mv $test_err $valg_err
		
		echo "$(date +"%Y/%m/%d.%T")            Error: when executing test $test_name -- Valgrind produced errors..." >> $log_file
	else
		# no errors were detected
		echo -e "\e[1;1;32mOk\e[0m"
	fi
}

# Check the result of the tester when valgrind is NOT used.
function check_res_no_valgrind() {
	# input test file
	local test_name=$1
	# output base file
	local base_out_file=$2

	# the file where to store the output of the program if said output
	# differs from the contents of the output base file
	local test_out=$3
	# a temporary file where the error output is stored
	local temp_test_err="$4.tmp"
	# the file where to store the error output of the program if it ever
	# produced an error output (like error messages and whatnot)
	local test_err=$4

	# output of the execution of the tester
	local prog_out=$5
	
	# check if base output exists. If not, issue an error
	# message and do not execute
	if [ ! -f $base_out_file ]; then
		
		echo -e "\e[1;3;31mOutput base file does not exist\e[0m Skipping comparison..."
		echo "$(date +"%Y/%m/%d.%T")            Warning: when executing test $test_name -- Output base file does not exist. Skipping comparison..." >> $log_file
		
	else
		# replace Windows-like endlines with Linux-like endlines
		local prog_out=$(echo "$prog_out" | sed 's/\n\r/\n/g')
		
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
			# no errors were produced -- remove the error file
			rm $temp_test_err
			
			# test whether the "normal" output produced is empty or not
			local BASE_CONTENTS=$(cat $base_out_file)
			
			local DIFF=$(diff <(echo "$BASE_CONTENTS") <(echo "$prog_out"))
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
	local input=$1
	local output=$2
	local progress_1=$3
	local progress_2=$4
	local storage=$5
	
	local skip=0
	
	# Make sure the input directory exists
	local input_group=inputs/$input
	if [ ! -d $input_group ]; then
		echo -en "\e[1;4;31mError:\e[0m Input directory "
		echo -en "\e[2;4;37m$input_group\e[0m"
		echo -e " does not exist. Skipping..."
		
		echo "$(date +"%Y/%m/%d.%T")            Warning: input $input does not exist. Skipping..." >> $log_file
		skip=1
	fi
	
	# Make sure the output directory exists
	local output_group=outputs/$output
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
		local keys=$(echo $input | tr "/" "\n");
		# id: is the string a.b.c
		local id=$(echo $keys | tr ' ' '.')
		
		# TEST_OUT and TEST_ERR are temporary files: a test named, for example,
		# test-0015 will produce a standard output and an error output into
		# $TEST_OUT and $TEST_ERR. Then, in case of errors, each file will be
		# moved to a unique file for this test, namely into the files
		# $TEST_OUT.0015 and $TEST_ERR.0015. In case of valgrind the contents
		# of $TEST_ERR will be moved to $VALG_ERR.0015.
		local TEST_OUT=.out.$id
		local TEST_ERR=.err.$id
		local VALG_ERR=.vgd.$id
		
		echo -en "\e[1;1;33mExecuting tests in\e[0m \e[1;2;33m$input_group\e[0m"
		display_two $progress_1 $progress_2
		echo ""
		
		# retrieve the names of the test files
		local all_test_files=$(ls $input_group | grep test)
		local n_test_files=$(ls $input_group | grep test | wc -l)
		local nth_test=1
		
		for f in $all_test_files; do
			# retrieve id number from input test file
			# e.g.: 'test-0010' -> '0010'
			local INFILE_LENGTH=${#f}
			local ID=${f:5:($INFILE_LENGTH - 4)}
			
			# Display the progress within this group:
			# output the name of the test file plus the index of the
			# file with respect to the total tests within the group.
			echo -en "    \e[1;1;34m$f\e[0m"
			display_two $nth_test $n_test_files
			
			# Execute the program NOW
			local PROG_OUT=$($EXECUTION_COMMAND -i $input_group/$f 2> $storage/$TEST_ERR.$ID.tmp)

			# increment the amount of tests executed by 1
			nth_test=$(($nth_test + 1))
			
			# parse result of execution command differently,
			# depending on whether we are using valgrind or not.
			if [ $use_valgrind == 1 ]; then
				check_res_valgrind 				\
					$input_group/$f 			\
					$storage/$TEST_ERR.$ID		\
					$storage/$VALG_ERR.$ID
			else
				check_res_no_valgrind 			\
					$input_group/$f				\
					$output_group/$f			\
					$storage/$TEST_OUT.$ID 		\
					$storage/$TEST_ERR.$ID		\
					"$PROG_OUT"
			fi
		done
		
		# Remove those unnecessary temporary files.
		# File $VALG_ERR is never actually used 'as is'.
		rm -f $TEST_OUT $TEST_ERR
	fi
}

function small_tests() {
	# Pairs of input and output files.
	# Input file: the file that describes what to test
	# Output file: the "ground truth" of the corresponding input file
	local files=(																					\
		"graphs/construction/test-000.000"						"graphs/construction/test-000.000"	\
		"graphs/construction/test-000.001"						"graphs/construction/test-000.001"	\
		"graphs/construction/test-000.002"						"graphs/construction/test-000.002"	\
		"linarr/C/bruteforce/test-0000"							"linarr/C/test-0000"				\
		"linarr/C/dyn-prog/test-0000"							"linarr/C/test-0000"				\
		"linarr/C/ladder/test-0000"								"linarr/C/test-0000"				\
		"linarr/C/stack-based/test-0000"						"linarr/C/test-0000"				\
		"properties/exp-var-C/formula-no_Q-no-reuse/test-0017"	"properties/exp-var-C/test-0017"	\
		"properties/centroid/test-000.000"						"properties/centroid/test-000.000"	\
	)
	
	local n_files=${#files[@]}
	n_files=$(( $n_files/2 ))
	
	for ((i=0; i<$n_files; ++i)); do
		local idx=$(( $i + 1 ))
		local i1=$((2*$i    ))
		local i2=$((2*$i + 1))
		
		input_file=${files[$i1]}
		output_file=${files[$i2]}
		
		# ------------------------------------------------
		# Prepare filenames for standard and error outputs
		
		# Given an input directory a/b/c
		# keys: is the list a b c
		local keys=$(echo $input_file | tr "/" "\n");
		# id: is the string a.b.c
		local id=$(echo $keys | tr ' ' '.')
		
		# TEST_OUT and TEST_ERR are temporary files: a test named, for example,
		# test-0015 will produce a standard output and an error output into
		# $TEST_OUT and $TEST_ERR. Then, in case of errors, each file will be
		# moved to a unique file for this test, namely into the files
		# $TEST_OUT.0015 and $TEST_ERR.0015. In case of valgrind the contents
		# of $TEST_ERR will be moved to $VALG_ERR.0015.
		local TEST_OUT=.out.$id
		local TEST_ERR=.err.$id
		local VALG_ERR=.vgd.$id
		
		echo -en "\e[1;1;33mExecuting test ($idx/$n_files) \e[0m \e[1;2;33m$input_file\e[0m  "
		
		# Execute the program NOW
		local PROG_OUT=$($EXECUTION_COMMAND -i inputs/$input_file 2> $storage_dir/$TEST_ERR)
		
		# parse result of execution command differently,
		# depending on whether we are using valgrind or not.
		if [ $use_valgrind == 1 ]; then
			check_res_valgrind 				\
				inputs/$input_file			\
				$TEST_ERR					\
				$VALG_ERR
		else
			check_res_no_valgrind 			\
				inputs/$input_file			\
				outputs/$output_file		\
				$TEST_OUT					\
				$TEST_ERR					\
				"$PROG_OUT"
		fi
		
		# Remove those unnecessary temporary files.
		# File $VALG_ERR is never actually used 'as is'.
		rm -f $TEST_OUT $TEST_ERR
	done
}

function apply_group() {
	local exe_group=$1		# execution group
	
	# if the group is valid, then execute it
	echo "Executing group '$exe_group'"
	
	# variable name
	local dir_name="$exe_group""_DIRS[@]"
	# actual array
	local DIRS_TO_PROCESS=("${!dir_name}")
	# size of the array
	local n_dirs=${#DIRS_TO_PROCESS[@]}
	# number of input directories
	local n_dirs=$(($n_dirs/2))
	
	# execute
	for ((i=0; i<$n_dirs; ++i)); do
		local idx=$(($i + 1))
		local i1=$((2*$i    ))
		local i2=$((2*$i + 1))
		local in="${DIRS_TO_PROCESS[$i1]}"
		local out="${DIRS_TO_PROCESS[$i2]}"

		echo "$(date +"%Y/%m/%d.%T")        Start executing tests in $in." >> $log_file
		execute_group $in $out $idx $n_dirs $storage_dir
		echo "$(date +"%Y/%m/%d.%T")        Finished executing tests in $in." >> $log_file
	done
}

################################################################################
# Code starts here

# number of parameters
number_parameters=$#
# show usage
usage=0
# the directory of input tests to be executed
input_dir=""
# the directory of output files corresponding to the inputs
output_dir=""
# use valgrind
use_valgrind=0
# the name of the directory that contains the executable files
exe_directory=""
# execution mode
debug=0
release=0
# execute tests of a certain type
exe_group=""
# log file
log_file="execution_log"
# where to put the error outputs, the outputs that differ from the base
# output files, and the valgrind outputs (if any)
storage_dir="."
# compile (or not) the executable
compile=1
# the path to the gcc library
gcc_library_path=""
# the path to the LAL library
lal_library_path=""

for i in "$@"; do
	case $i in
		
		--help|-h)
		usage=1
		shift
		;;

		--lal_library_path=*)
		lal_library_path="${i#*=}"
		shift
		;;

		--gcc_library_path=*)
		gcc_library_path="${i#*=}"
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
		shift
		;;
		
		--release)
		release=1
		shift
		;;
		
		--run-debug)
		exe_directory=build-debug
		debug=1
		shift
		;;
		
		--run-release)
		exe_directory=build-release
		release=1
		shift
		;;
		
		--exe-directory=*)
		exe_directory="${i#*=}"
		shift
		;;
		
		--all)
		exe_group="all"
		shift
		;;
		
		--small-tests)
		exe_group="small-tests"
		shift
		;;
		
		*)
		echo -e "\e[1;4;31mError:\e[0m Option $i unknown"
		exit
		;;
	esac
done

if [ "$gcc_library_path" != "" ] || [ "$lal_library_path" != "" ]; then

	export LD_LIBRARY_PATH=""

	if [ "$gcc_library_path" != "" ]; then
		if [ "$LD_LIBRARY_PATH" == "" ]; then
			export LD_LIBRARY_PATH=$gcc_library_path
		else
			export LD_LIBRARY_PATH=$gcc_library_path:$LD_LIBRARY_PATH
		fi
	fi
	if [ "$lal_library_path" != "" ]; then
		if [ "$LD_LIBRARY_PATH" == "" ]; then
			export LD_LIBRARY_PATH=$lal_library_path
		else
			export LD_LIBRARY_PATH=$lal_library_path:$LD_LIBRARY_PATH
		fi
	fi
fi

# show usage if indicated
if [ $usage == 1 ] || [ $number_parameters == 0 ]; then
	show_usage
	exit
fi

################################################################################
# Parameter errors

if [ -z $exe_group ] && [ -z $input_dir ]; then
	echo -e "\e[1;4;31mError:\e[0m No input directory specified."
	exit
fi
if [ ! -z $exe_group ] && [ ! -z $input_dir ]; then
	echo -e "\e[1;4;31mError:\e[0m Both input directory and execution group were specified."
	echo -e "    Specify only one!"
	exit
fi
if [ ! -z $input_dir ] && [ -z $output_dir ] && [ $use_valgrind == 0 ]; then
	# we only care about output files if valgrind is not used
	echo -e "\e[1;4;31mError:\e[0m No output directory specified."
	exit
fi

if [ $release == 1 ] && [ $use_valgrind == 1 ]; then
	echo -e "\e[1;4;31mError:\e[0m Conflicting options"
	echo "    --valgrind --release"
	echo "    You shouldn't be using valgrind on a release build!"
	exit
fi

################################################################################
# Execution mode (debug/release)

# use debug by default
if [ $debug == 0 ] && [ $release == 0 ]; then
	echo -e "\e[1;4;31mError:\e[0m No execution mode was specified"
	echo "    --debug"
	echo "    --release"
	exit
fi
if [ $debug == 1 ] && [ $release == 1 ]; then
	echo -e "\e[1;4;31mError:\e[0m Conflicting execution modes"
	echo "    debug: $debug"
	echo "    release: $release"
	exit
fi

if [ "$exe_directory" == "" ]; then
	echo -e "\e[1;4;31mError:\e[0m Execution directory is empty: I won't be able to find the executable file"
	exit
fi

################################################################################
# Make sure we can execute something

EXECUTE_FROM_INPUT=0
EXECUTE_FROM_GROUP=0

if [ -z $exe_group ]; then
	EXECUTE_FROM_INPUT=1
	# ensure that no "inputs/" nor "outputs/" were given in the parameters
	
	IFS='/' read -ra in_array <<< "$input_dir"
	if [ "${in_array[0]}" == "inputs" ]; then
		echo -e "\e[1;4;31mError:\e[0m Can't write 'inputs/' in input directory."
		echo "    Just omit it. Like this:"
		echo "    ./test.sh --input=linarr/C/ladder ..."
		exit
	fi
	
	IFS='/' read -ra out_array <<< "$output_dir"
	if [ "${out_array[0]}" == "outputs" ]; then
		echo -e "\e[1;4;31mError:\e[0m Can't write 'outputs/' in output directory."
		echo "    Just omit it. Like this:"
		echo "    ./test.sh ... --output=linarr/C"
		exit
	fi
	
else
	EXECUTE_FROM_GROUP=1
	
	# test that 'exe_group' value is valid
	array_has=0
	case "${groups_list[@]}" in
		# the '*' are important!
		*"$exe_group"*)
			array_has=1
			;;
	esac

	if [ $array_has == 0 ]; then
		echo -e "\e[1;4;31mError:\e[0m Invalid execution group '$exe_group'."
		echo "    Make sure that the group '$exe_group' is in the variable 'groups_list'"
		
		echo "$(date +"%Y/%m/%d.%T")        Error: test execution failed. Group '$exe_group' is not valid." >> $log_file
		exit
	fi
fi

echo "exe_directory: $exe_directory"

# Locate the executable file
EXECUTABLE_FILE=""

# make sure the executable is up to date
if [ $compile -eq 1 ]; then
	wd=$PWD
	CMAKE_BUILD=0
	
	cd $exe_directory
	if [ -f cmake_install.cmake ]; then
		CMAKE_BUILD=1
	fi
	cd $wd
	
	if [ $CMAKE_BUILD == 1 ]; then
		# choose the execution directory and file for a cmake-based build
		
		if [ $EXECUTE_FROM_INPUT == 1 ]; then
			IFS='/' read -ra keywords <<< "$input_dir"
			EXECUTABLE_FILE="$exe_directory/${keywords[0]}"
		else
			if [ "$exe_group" == "all" ]; then
				EXECUTABLE_FILE="$exe_directory/tests"
			else
				IFS='_' read -ra keywords <<< "$exe_group"
				EXECUTABLE_FILE="$exe_directory/${keywords[0]}"
			fi
		fi
	else
		# choose the execution directory and file for a qmake-based build
		
		if [ $EXECUTE_FROM_INPUT == 1 ]; then
			IFS='/' read -ra keywords <<< "$input_dir"
			
			exe_directory="$exe_directory/${keywords[0]}"
			EXECUTABLE_FILE="$exe_directory/${keywords[0]}"
		else
			if [ "$exe_group" == "all" ] || [ "$exe_group" == "small-tests" ]; then
				exe_directory="$exe_directory/tests"
				EXECUTABLE_FILE="$exe_directory/tests"
			else
				echo "exe_group: $exe_group"
				IFS='_' read -ra keywords <<< "$exe_group"
				echo "KEYWORDS: ${keywords[0]}"
				exe_directory="$exe_directory/${keywords[0]}"
				EXECUTABLE_FILE="$exe_directory/${keywords[0]}"
			fi
		fi
	fi
	
	all_or_small=0
	if [ "$exe_group" == "all" ] || [ "$exe_group" == "small-tests" ]; then
		all_or_small=1
	fi
	
	if [ $CMAKE_BUILD == 0 ] && [ $EXECUTE_FROM_INPUT == 0 ] && [ $all_or_small == 1 ]; then
		
		cd $exe_directory
		cd ..
		make -j4
		cd $wd
	else
		cd $exe_directory
		make -j4
		cd $wd
	fi
fi

EXECUTION_COMMAND=""
if [ -f $EXECUTABLE_FILE ]; then
	EXECUTION_COMMAND=./$EXECUTABLE_FILE
else
	echo -e "\e[1;4;31mError:\e[0m No executable found."
	echo -e "    Looked for: $EXECUTABLE_FILE"
	exit
fi

# Prepare execution command. If valgrind is
# requested, make the command appropriately.
if [ $use_valgrind == 1 ]; then
	EXECUTION_COMMAND="valgrind -q --leak-check=full $EXECUTION_COMMAND"
else
	EXECUTION_COMMAND="$EXECUTION_COMMAND"
fi

################################################################################
# Display information about what software is being tested.

echo -en "Testing "
if [ $debug == 1 ]; then
	echo -en "in \e[1;3;36mdebug\e[0m mode"
	if [ $use_valgrind == 1 ]; then
		echo " and using valgrind."
	else
		echo "."
	fi
elif [ $release == 1 ]; then
	echo -e "in \e[1;3;36mrelease\e[0m mode."
fi

################################################################################
# Display where the files will be written

log_file=$storage_dir/$log_file

echo "    Storage directory: '$storage_dir'"
echo "    Log file: '$log_file'"

################################################################################
# Execute the tests

echo "$(date +"%Y/%m/%d.%T")    Started test execution of group '$exe_group'." >> $log_file

if [ $EXECUTE_FROM_GROUP == 1 ]; then
	if [ "$exe_group" == "all" ]; then
		for g in "detail" "generate" "graphs" "io" "linarr" "memory" "numeric" "properties" "utilities"; do
			apply_group $g
		done
	elif [ "$exe_group" == "small-tests" ]; then
		
		small_tests
		
	else
		apply_group $exe_group
	fi
elif [ $EXECUTE_FROM_INPUT == 1 ]; then
	execute_group $input_dir $output_dir 1 1 $storage_dir
fi

echo "$(date +"%Y/%m/%d.%T")    Finished test execution of group '$exe_group'." >> $log_file

echo "A summary of the test has been logged into file '$log_file'"
