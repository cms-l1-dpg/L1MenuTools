#!/bin/bash
filelist=`cat run_323940_ratetable.list`
echo "${filelist}"
#filelist="/eos/uscms/store/user/aldas/L1Menu2018/Apr28run_323940_nanodst_main_seeds_inclusive/your_ntuple_323940_0_Default.txt"
./fileread run_323940 ${filelist}
#filelist2="aloke"
#echo $filelist2
