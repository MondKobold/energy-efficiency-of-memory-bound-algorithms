#!/bin/sh

echo $1 > benchmark.txt

echo "Size:0" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 0 > gb.txt

echo "Size:1" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 1 > gb.txt

echo "Size:2" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 2 > gb.txt

echo "Size:3" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 3 > gb.txt

echo "Size:4" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 4 > gb.txt

echo "Size:5" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 5 > gb.txt

echo "Size:6" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 6 > gb.txt

echo "Size:7" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 7 > gb.txt

echo "Size:8" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 8 > gb.txt

echo "Size:9" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 9 > gb.txt

echo "Size:10" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 10 > gb.txt

echo "Size:11" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 11 > gb.txt

echo "Size:12" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 12 > gb.txt

echo "Size:13" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 13 > gb.txt

echo "Size:14" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./sortN 14 > gb.txt


echo "Runtime Performance:" >> benchmark.txt
./perf_time 15 ns >> benchmark.txt