#!/bin/bash

./make_makefiles_cluster.sh
sbatch -p short -c 8 compile_cluster.sh build-release
sbatch -p short -c 8 compile_cluster.sh build-debug
sbatch -p short -c 8 compile_cluster.sh build-release-11.3
sbatch -p short -c 8 compile_cluster.sh build-debug-11.3
sbatch -p short -c 8 compile_cluster.sh build-release-12.3
sbatch -p short -c 8 compile_cluster.sh build-debug-12.3
sbatch -p short -c 8 compile_cluster.sh build-release-13.1
sbatch -p short -c 8 compile_cluster.sh build-debug-13.1
