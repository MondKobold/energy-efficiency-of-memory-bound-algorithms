#!/bin/sh
#$1 benchmark
#$2 runtime
#$3 output

echo "#### $1 ####" > $3
echo "===== 1 GiB =====" >> $3
sh perf_isengard.sh "$1 13" $3

echo "===== 2 GiB =====" >> $3
sh perf_isengard.sh "$1 14" $3

echo "#### Laufzeit ####" > $3
echo "===== 1 GiB =====" >> $3
$2 13 >> $3

echo "===== 2 GiB =====" >> $3
$2 14 >> $3