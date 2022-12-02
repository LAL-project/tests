#!/bin/bash

function show_usage {
	echo "USAGE"
	echo "====="
	echo ""
	echo "Mandatory parameters:"
	echo "    --build=debug,release"
	echo "        What to run: debug build, or release build"
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
	show_usage
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

info=info_$build
valgrind_param=""
use_valgrind=0
memory=4GB

if [ ! -z $analyzer ]; then
	
	if [ "$analyzer" == "valgrind" ]; then
		# valgrind analyzer can only be used in debug compilations
		if [ "$build" != "debug" ]; then
			echo -e "\e[1;4;31mError:\e[0m Cannot use valgrind in a non-debug build"
			exit
		fi
		
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
echo "    analyzer: '$analyzer'"
echo "    job memory: '$memory'"
echo "    job queue: '$queue'"
echo "    use valgrind: '$use_valgrind'"
echo "        valgrind param: '$valgrind_param'"

mkdir -p $info

for group in detail generate graphs linarr memory numeric properties utilities; do
	echo $group" - "$build
	sbatch --mem=$memory --job-name=tests_"$build" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$build --exe-group=$group --log-file="$group"_log --storage-dir=$info
done
