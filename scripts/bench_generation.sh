#!/bin/sh

echo "Generateion meassurements" > benchmark.txt

echo "Size:0" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 1 > gb.txt

echo "Size:1" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 2 > gb.txt

echo "Size:2" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 3 > gb.txt

echo "Size:3" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 4 > gb.txt

echo "Size:4" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 5 > gb.txt

echo "Size:5" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 6 > gb.txt

echo "Size:6" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 7 > gb.txt

echo "Size:7" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 8 > gb.txt

echo "Size:8" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 9 > gb.txt

echo "Size:9" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 10 > gb.txt

echo "Size:10" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 11 > gb.txt

echo "Size:11" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 12 > gb.txt

echo "Size:12" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 13 > gb.txt

echo "Size:13" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 14 > gb.txt

echo "Size:14" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 15 > gb.txt


echo "Runtime Performance:" >> benchmark.txt
./generateN_time 1 >> benchmark.txt
./generateN_time 2 >> benchmark.txt
./generateN_time 3 >> benchmark.txt
./generateN_time 4 >> benchmark.txt
./generateN_time 5 >> benchmark.txt
./generateN_time 6 >> benchmark.txt
./generateN_time 7 >> benchmark.txt
./generateN_time 8 >> benchmark.txt
./generateN_time 9 >> benchmark.txt
./generateN_time 10 >> benchmark.txt
./generateN_time 11 >> benchmark.txt
./generateN_time 12 >> benchmark.txt
./generateN_time 13 >> benchmark.txt
./generateN_time 14 >> benchmark.txt
./generateN_time 15 >> benchmark.txt