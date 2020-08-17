#!/bin/sh

echo $1 > benchmark.txt

echo "Size:10" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 10 > gb.txt

echo "Size:11" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 11 > gb.txt

echo "Size:12" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 12 > gb.txt

echo "Size:13" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 13 > gb.txt

echo "Size:14" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append ./sortN 14 > gb.txt


echo "Runtime Performance:" >> benchmark.txt
./perf_time 15 ns >> benchmark.txt