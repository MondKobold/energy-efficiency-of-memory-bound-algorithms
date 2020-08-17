#!/bin/sh
#$1 benchmark
#$2 runtime
#$3 output

echo "#### $1 ####" > $3

echo "===== 8 MiB =====" >> $3
sh perf_isengard.sh "$1 10" $3

echo "===== 16 MiB =====" >> $3
sh perf_isengard.sh "$1 11" $3

echo "===== 32 MiB =====" >> $3
sh perf_isengard.sh "$1 12" $3

echo "#### Runtime ####" >> $3
echo "===== 8 MiB =====" >> $3
$2 10 >> $3

echo "===== 16 MiB =====" >> $3
$2 11 >> $3

echo "===== 32 MiB =====" >> $3
$2 13 >> $3