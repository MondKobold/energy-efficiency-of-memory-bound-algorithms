#!/bin/sh
# create build directory
mkdir build
cd build

# build executeables
cmake ../
make

cd ..
rm -r --force benchmark
mkdir benchmark
mkdir results
cp -v build/clustering/benchmark/* benchmark # copy executables
# copy isengard scripts
cp -v scripts/perf_isengard.sh benchmark/
cp -v scripts/bench_with_complete_stats_isengard.sh benchmark/
cp -v scripts/bench_with_runtime.sh benchmark/
cp -v scripts/run_sort_isengard.sh benchmark/
cp -v scripts/run_single_isengard.sh benchmark/
cp -v scripts/run_single_isengard_gb.sh benchmark/
cp -v scripts/run_single_isengard_8-32.sh benchmark/
cp -v scripts/perf_isengard_analyse.sh benchmark/
cp -v scripts/run_cluster.sh benchmark/

# clean up
rm -r build
