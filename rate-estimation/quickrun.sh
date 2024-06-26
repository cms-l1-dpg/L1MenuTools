NAME=$1
L1NTUPLE=$2
EVENTS=5000

make -j 8

if [[ $L1NTUPLE == "TRUE"  ]]; then
    echo "Testing NTuple"
    { time ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/bril_pu_run379420.csv -l ntuple/2024C_EphZB_run379420_1404_all.list -o testNtupleGT_$NAME -b 2748 --UseuGTDecision --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34; } |& tee results/testNtupleGT_$NAME.log
    { time ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/bril_pu_run379420.csv -l ntuple/2024C_EphZB_run379420_1404_all.list -o testNtupleStandalone_$NAME -b 2748 --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34; } |& tee results/testNtupleStandalone_$NAME.log
    # ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l ntuple/2023D_EphZB_run370293_1404.list -o testNtuple -b 2748 --UseuGTDecision --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34 | tee results/testNtuple.log
    # ./testMenu2016 -m menu/L1Prescales2023.csv -l ntuple/testL1Ntuple.list -o testNtupleArtur -b 2748 --UseUnpackTree --UseuGTDecision --maxEvent 5000 --SelectCol 2p0E34 | tee results/testNtupleArtur.log
    git diff --no-index results/testNtupleGT_$NAME.log results/testNtupleStandalone_$NAME.log |& tee results/testNtuple_$NAME.diff
else
    { time ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l ntuple/2023D_EphZB_run370293_L1Nano.list -o testNanoGT_$NAME -b 2748 --UseuGTDecision --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34 --doNano; } |& tee results/testNanoGT_$NAME.log
    { time ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l ntuple/2023D_EphZB_run370293_L1Nano.list -o testNanoStandalone_$NAME -b 2748 --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34 --doNano; } |& tee results/testNanoStandalone_$NAME.log
    # ./testMenu2016 -m menu/L1Prescales2023.csv -l ntuple/testL1Nano.list -o testNanoArtur -b 2748 --UseUnpackTree --UseuGTDecision --maxEvent 5000 --SelectCol 2p0E34 --doNano | tee results/testNanoArtur.log
    git diff --no-index results/testNanoGT_$NAME.log results/testNanoStandalone_$NAME.log |& tee results/testNano_$NAME.diff
fi
