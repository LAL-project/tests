#!/bin/bash

idx=$1

for i in `seq 1 9`; do
	echo "Test finished without apparent errors." > test-$idx.000$i
done

for i in `seq 10 20`; do
	echo "Test finished without apparent errors." > test-$idx.00$i
done
for i in `seq 30 10 90`; do
	echo "Test finished without apparent errors." > test-$idx.00$i
done

for i in `seq 100 10 200`; do
	echo "Test finished without apparent errors." > test-$idx.0$i
done
for i in `seq 300 100 900`; do
	echo "Test finished without apparent errors." > test-$idx.0$i
done

echo "Test finished without apparent errors." > test-$idx.1000
