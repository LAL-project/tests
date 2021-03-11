#!/bin/bash

sbatch -p short -c 4 compile_cluster.sh build-release
sbatch -p short -c 4 compile_cluster.sh build-debug
