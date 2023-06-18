#!/bin/bash

function show_usage {
	echo "USAGE"
	echo "====="
	echo ""
	echo "Mandatory parameters:"
	echo "    --build=debug,release"
	echo "        What to run: a debug build, or a release build"
	echo ""
	echo "    --compiler-version=dir"
	echo "        This is used to make get the name of the directory that contains the executable file"
	echo ""
	echo "Optional parameters:"
	echo "    --analyzer=valgrind,address-sanitizer"
	echo "        How to run the test program:"
	echo "        * valgrind: execute test program with valgrind. This can only be"
	echo "        run with --build=debug."
	echo "        * address-sanitizer: indicate that the program was compiled with"
	echo "        -fsanitize=address. This increases maximum memory for each job."
}

# usage
#  usage=0
# build type (debug, release)
#  build=0
# analyzer type (valgrind, address-sanitizer
#  analyzer=0

for i in "$@"; do
	case $i in
		
		--help|-h)
		usage=1
		shift
		;;
		
		--build=*)
		build="${i#*=}"
		shift
		;;

		--compiler-version=*)
		compiler_version="${i#*=}"
		shift
		;;
		
		--analyzer=*)
		analyzer="${i#*=}"
		shift
		;;
		
		*)
		echo -e "\e[1;4;31mError:\e[0m Option $i unknown"
		exit
		;;
	esac
done

if [ ! -z $usage ]; then
	show_usage
	exit
fi

if [ -z $build ]; then
	echo -e "\e[1;4;31mError:\e[0m Build parameter cannot be empty"
	exit
fi

if [ -z $compiler_version ]; then
	echo -e "\e[1;4;31mError:\e[0m Missing compiler version"
	exit
fi

if [ "$build" != "debug" ] && [ "$build" != "release" ]; then
	echo -e "\e[1;4;31mError:\e[0m Wrong value for build: '$build'"
	echo "    Allowed values are: 'debug', 'release'."
	exit
fi

if [ ! -z $analyzer ]; then
	
	if [ "$analyzer" != "valgrind" ] && [ "$analyzer" != "address-sanitizer" ]; then
		echo -e "\e[1;4;31mError:\e[0m Wrong value for analyzer: '$analyzer'"
		echo "    Allowed values are: 'valgrind', 'address-sanitizer'."
		exit
	fi
	
fi

exe_directory=build-$build-$compiler_version
log_directory=info_$exe_directory

valgrind_param=""
use_valgrind=0
memory=4GB

if [ ! -z $analyzer ]; then
	
	if [ "$analyzer" == "valgrind" ]; then
		
		use_valgrind=1
		valgrind_param="--valgrind"
		info="$info"_valgrind
		memory=8GB
		
	elif [ "$analyzer" == "address-sanitizer" ]; then
		# address sanitizer can be used in both debug and release compilations
		memory=8GB
		
	fi
	
fi

queue="long"

echo "Execution information"
echo "    build: '$build'"
echo "    exe_directory: '$exe_directory'"
echo "    log_directory: '$log_directory'"
echo "    analyzer: '$analyzer'"
echo "    job memory: '$memory'"
echo "    job queue: '$queue'"
echo "    use valgrind: '$use_valgrind'"
echo "        valgrind param: '$valgrind_param'"
lal_library_path=$HOME/LAL-dev/latest/linear-arrangement-library/lal-$build-$compiler_version
gcc_library_path=/home/soft/gcc-$compiler_version/lib64

echo "    lal_library_path: $lal_library_path"
echo "    gcc_library_path: $gcc_library_path"

mkdir -p $log_directory

for group in detail generate graphs linarr memory numeric properties utilities; do
	job_name=tests_"$build"_"$group"_"$compiler_version"

	echo $group" - "$build
	echo "    job_name: $job_name"

	sbatch --mem=$memory --job-name=$job_name -p $queue -o $log_directory/job__$group.out -e $log_directory/job__$group.err ./test.sh $valgrind_param --$build --exe-directory=$exe_directory --exe-group=$group --log-file=log_$group --storage-dir=$log_directory --lal_library_path=$lal_library_path --gcc_library_path=$gcc_library_path
done

