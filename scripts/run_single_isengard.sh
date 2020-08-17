#!/bin/sh
#$1 benchmark
#$2 runtime
#$3 output

sh bench_with_complete_stats_isengard.sh $1 $3
sh bench_with_runtime.sh $2 $3