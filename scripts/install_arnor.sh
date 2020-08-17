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
mkdir benchmark/results
cp -v build/clustering/benchmark/* benchmark # copy executables
# copy arnor scripts
cp -v scripts/run_sort_arnor.sh benchmark/
cp -v scripts/run_single_arnor.sh benchmark/
cp -v scripts/perf_arnor.sh benchmark/
cp -v scripts/perf_arnor_analyse.sh benchmark/
cp -v scripts/bench_with_complete_stats_arnor.sh benchmark/
cp -v scripts/bench_with_runtime.sh benchmark/
cp -v scripts/run_cluster.sh benchmark/

# clean up
rm -r build

