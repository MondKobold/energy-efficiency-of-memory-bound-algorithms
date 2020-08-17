#!/bin/sh
#$1 auszufuehrendes Programm
#$2 Dokument zum speichern

# Energie
perf stat -o $2 --append -r 2 -e power/energy-pkg/,power/energy-ram/,power/energy-cores/  $1 > trash.txt
# isengard  -> power/energy-cores/
