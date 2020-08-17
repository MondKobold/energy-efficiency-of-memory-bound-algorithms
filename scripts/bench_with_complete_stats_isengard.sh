#!/bin/sh
#$1 aufruf
#$2 ausgabe Datei
echo "#### $1 ####" > $2

echo "===== 1 KiB =====" >> $2
sh perf_isengard.sh "$1 0" $2

echo "===== 16 KiB =====" >> $2
sh perf_isengard.sh "$1 1" $2

echo "===== 32 KiB =====" >> $2
sh perf_isengard.sh "$1 2" $2

echo "===== 64 KiB =====" >> $2
sh perf_isengard.sh "$1 3" $2

echo "===== 128 KiB =====" >> $2
sh perf_isengard.sh "$1 4" $2

echo "===== 256 KiB =====" >> $2
sh perf_isengard.sh "$1 5" $2

echo "===== 512 KiB =====" >> $2
sh perf_isengard.sh "$1 6" $2

echo "===== 1 MiB =====" >> $2
sh perf_isengard.sh "$1 7" $2

echo "===== 2 MiB =====" >> $2
sh perf_isengard.sh "$1 8" $2

echo "===== 4 MiB =====" >> $2
sh perf_isengard.sh "$1 9" $2

echo "===== 8 MiB =====" >> $2
sh perf_isengard.sh "$1 10" $2

echo "===== 16 MiB =====" >> $2
sh perf_isengard.sh "$1 11" $2

echo "===== 32 MiB =====" >> $2
sh perf_isengard.sh "$1 12" $2

echo "===== 1 GiB =====" >> $2
sh perf_isengard.sh "$1 13" $2

echo "===== 2 GiB =====" >> $2
sh perf_isengard.sh "$1 14" $2