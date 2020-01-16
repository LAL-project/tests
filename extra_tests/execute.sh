#!/bin/bash -l

DIR=/home/usuaris/lalemany/LAL-tests/extra_tests
python3 $DIR/debug_ulab_free_trees.py $1 > $DIR/out-$1
