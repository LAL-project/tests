#!/bin/bash

# build type (debug, release)
build=0
# analyzer type (valgrind, address-sanitizer

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
