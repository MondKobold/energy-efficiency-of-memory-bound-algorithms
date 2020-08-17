#!/bin/sh
#runs all sorting algorithms
sh run_single_isengard.sh ./ms_pc ./pf_ms_pc "results/final_ms_pc.txt"
sh run_single_isengard.sh ./ms_co ./pf_ms_co "results/final_ms_co.txt"
sh run_single_isengard.sh ./ms_ip_si ./pf_ms_ip_si "results/final_ms_ip_si.txt"

sh run_single_isengard_gb.sh ./qs_3_d ./pf_qs_3_d "results/final_qs_3_d_gb.txt"
sh run_single_isengard_gb.sh ./qs_shuffle ./pf_qs_shuffle "results/final_qs_shuffle_gb.txt"

sh run_single_isengard_8-32.sh ./qs_rp ./pf_qs_rp "results/final_qs_rp-middle.txt"
