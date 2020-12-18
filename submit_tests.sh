#!/bin/bash

mode=$1 # release, debug
info=info_$mode
mkdir -p $info

group=generate
echo $group" - "$mode
sbatch -p short -o $info/"$group"_"$mode".out -e $info/"$group"_"$mode".err ./test.sh --$mode --exe-group=$group --log-file=$info/"$group"_"$mode"_log

group=graphs
echo $group" - "$mode
sbatch -p short -o $info/"$group"_"$mode".out -e $info/"$group"_"$mode".err ./test.sh --$mode --exe-group=$group --log-file=$info/"$group"_"$mode"_log

group=internal
echo $group" - "$mode
sbatch -p short -o $info/"$group"_"$mode".out -e $info/"$group"_"$mode".err ./test.sh --$mode --exe-group=$group --log-file=$info/"$group"_"$mode"_log

group=linarr
echo $group" - "$mode
sbatch -p short -o $info/"$group"_"$mode".out -e $info/"$group"_"$mode".err ./test.sh --$mode --exe-group=$group --log-file=$info/"$group"_"$mode"_log

group=memory
echo $group" - "$mode
sbatch -p short -o $info/"$group"_"$mode".out -e $info/"$group"_"$mode".err ./test.sh --$mode --exe-group=$group --log-file=$info/"$group"_"$mode"_log

group=numeric
echo $group" - "$mode
sbatch -p short -o $info/"$group"_"$mode".out -e $info/"$group"_"$mode".err ./test.sh --$mode --exe-group=$group --log-file=$info/"$group"_"$mode"_log

group=properties
echo $group" - "$mode
sbatch -p short -o $info/"$group"_"$mode".out -e $info/"$group"_"$mode".err ./test.sh --$mode --exe-group=$group --log-file=$info/"$group"_"$mode"_log

group=utilities
echo $group" - "$mode
sbatch -p short -o $info/"$group"_"$mode".out -e $info/"$group"_"$mode".err ./test.sh --$mode --exe-group=$group --log-file=$info/"$group"_"$mode"_log

