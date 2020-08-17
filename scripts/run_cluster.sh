#!/bin/sh
#runs all clustering stuff

echo "#### Clustering ####" > clustering.txt

echo "#### DBSCAN ####" >> clustering.txt
#perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./dbscan128 >> clustering.txt
perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./dbscan16 >> clustering.txt
#perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./dbscan128o >> clustering.txt
perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./dbscan16o >> clustering.txt

echo "#### KMEANS ####" >> clustering.txt
#perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./kmeans128 >> clustering.txt
perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./kmeans16 >> clustering.txt
#perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./kmeans128o >> clustering.txt
perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./kmeans16o >> clustering.txt

echo "#### Generating ####" >> clustering.txt
#perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./gen128KB >> clustering.txt
perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./gen16MB >> clustering.txt
#perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./gen128KB_big >> clustering.txt
perf stat -o clustering.txt --append -r 4 -e power/energy-pkg/,power/energy-ram/  ./gen16MB_big >> clustering.txt
