#!/bin/bash
filelist=`cat run_323940_ratetable.list`
filelist2=`cat run_324077_ratetable.list`
filelist3=`cat run_325022_ratetable.list`
#echo "${filelist}"
./fileread run_323940 ${filelist}
./fileread run_324077 ${filelist2}
./fileread run_325022 ${filelist3}
