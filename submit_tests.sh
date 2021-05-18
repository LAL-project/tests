#!/bin/bash

mode=$1 # release, debug
if [ "$mode" != "release" ] && [ "$mode" != "debug" ]; then
	echo "Wrong mode of execution: $mode"
	echo "    ./submit_tests.sh release/debug"
	exit
fi

info=info_$mode
valgrind_param=""
use_valgrind=0
memory=2GB

if [ ! -z $2 ]; then
	if [ "$mode" == "debug" ] && [ "$2" == "valgrind" ]; then
		use_valgrind=1
		valgrind_param="--valgrind"
		info="$info"_valgrind
		memory=4GB
	fi
fi

queue="long"

mkdir -p $info

group=generate
echo $group" - "$mode
sbatch --mem=$memory --job-name=tests_"$mode" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$mode --exe-group=$group --log-file="$group"_log --storage-dir=$info

group=graphs
echo $group" - "$mode
sbatch --mem=$memory --job-name=tests_"$mode" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$mode --exe-group=$group --log-file="$group"_log --storage-dir=$info

group=internal
echo $group" - "$mode
sbatch --mem=$memory --job-name=tests_"$mode" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$mode --exe-group=$group --log-file="$group"_log --storage-dir=$info

group=linarr
echo $group" - "$mode
sbatch --mem=$memory --job-name=tests_"$mode" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$mode --exe-group=$group --log-file="$group"_log --storage-dir=$info

group=memory
echo $group" - "$mode
sbatch --mem=$memory --job-name=tests_"$mode" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$mode --exe-group=$group --log-file="$group"_log --storage-dir=$info

group=numeric
echo $group" - "$mode
sbatch --mem=$memory --job-name=tests_"$mode" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$mode --exe-group=$group --log-file="$group"_log --storage-dir=$info

group=properties
echo $group" - "$mode
sbatch --mem=$memory --job-name=tests_"$mode" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$mode --exe-group=$group --log-file="$group"_log --storage-dir=$info

group=utilities
echo $group" - "$mode
sbatch --mem=$memory --job-name=tests_"$mode" -p $queue -o $info/"$group".out -e $info/"$group".err ./test.sh $valgrind_param --$mode --exe-group=$group --log-file="$group"_log --storage-dir=$info
