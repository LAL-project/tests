#!/bin/bash

./make_makefiles_cluster.sh
sbatch -p short -c 8 compile_cluster.sh build-release
sbatch -p short -c 8 compile_cluster.sh build-debug
