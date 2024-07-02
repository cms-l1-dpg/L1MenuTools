NAME=$1
GT=$2
# EVENTS=100000
EVENTS=-1

FILENTUPLE=ntuple/2023D_EphZB_run370293_1404.list
FILENANO=ntuple/2023D_EphZB_run370293_L1Nano.list

# FILENTUPLE=/afs/cern.ch/user/r/roward/CMS/CMSSW_14_0_4/src/Ntuples/L1Ntuple.root
# FILENANO=/afs/cern.ch/user/r/roward/CMS/CMSSW_14_0_4/src/Ntuples/l1Ntuple_NANO.root

make -j 8

if [[ $GT == "TRUE"  ]]; then
    echo "Testing GT"
    { time ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l $FILENANO -o testNanoGT_$NAME -b 2452 --doPlotLS --doPrintPU --lowerPUbound 58 --UseuGTDecision --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34 --doNano; } |& tee results/testNanoGT_$NAME.log
    { time ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l $FILENTUPLE -o testNtupleGT_$NAME -b 2452 --doPlotLS --doPrintPU --lowerPUbound 58 --UseuGTDecision --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34; } |& tee results/testNtupleGT_$NAME.log
    git diff --no-index results/testNtupleGT_$NAME.log results/testNanoGT_$NAME.log |& tee results/compareNtupleVsNanoGT_$NAME.diff
else
    echo "Testing Standalone"
    { time ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l $FILENTUPLE -o testNtupleStandalone_$NAME -b 2452 --doPlotLS --doPrintPU --lowerPUbound 58 --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34; } |& tee results/testNtupleStandalone_$NAME.log
    { time ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l $FILENANO -o testNanoStandalone_$NAME -b 2452 --doPlotLS --doPrintPU --lowerPUbound 58 --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34 --doNano; } |& tee results/testNanoStandalone_$NAME.log
    git diff --no-index results/testNtupleStandalone_$NAME.log results/testNanoStandalone_$NAME.log |& tee results/compareNtupleVsNanoStandalone_$NAME.diff
fi
