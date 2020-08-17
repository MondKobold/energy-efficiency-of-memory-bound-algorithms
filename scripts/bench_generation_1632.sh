#!/bin/sh

echo "Generateion meassurements" > benchmark.txt

echo "Size:11" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 12 > gb.txt

echo "Size:12" >> benchmark.txt
perf stat -e power/energy-pkg/ -o benchmark.txt --append -r 10 ./generateN 13 > gb.txt


echo "Runtime Performance:" >> benchmark.txt
./generateN_time 12 >> benchmark.txt
./generateN_time 13 >> benchmark.txt
