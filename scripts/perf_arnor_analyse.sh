#!/bin/sh
#$1 auszufuehrendes Programm
#$2 Dokument zum speichern

#allgemeine counter
perf stat -o $2 --append -r 10 $1 > trash.txt

# Energie
perf stat -o $2 --append -r 10 -e power/energy-pkg/,power/energy-ram/  $1 > trash.txt
# isengard  -> power/energy-cores/

# TLB
perf stat -o $2 --append -r 10 -e dTLB-loads,dTLB-load-misses,dTLB-prefetch-misses $1 > trash.txt

# L1
perf stat -o $2 --append -r 10 -e L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores $1 > trash.txt

# L2
perf stat -o $2 --append -r 10 -e l2_rqsts.all_demand_data_rd,l2_rqsts.demand_data_rd_miss,l2_lines_out.useless_hwpf,l2_rqsts.all_pf $1 > trash.txt
# nur isengard -> l2_store_lock_rqsts.all,l2_store_lock_rqsts.miss

# L3
perf stat -o $2 --append -r 10 -e LLC-loads,LLC-load-misses,LLC-stores $1 > trash.txt
# nur isengard -> LLC-prefetches