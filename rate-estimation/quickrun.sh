L1NTUPLE=$1
EVENTS=5000

make -j 8

if [[ $L1NTUPLE == "TRUE"  ]]; then
    echo "Testing NTuple"
    ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/bril_pu_run379420.csv -l ntuple/2024C_EphZB_run379420_1404_all.list -o testNtupleGT -b 2748 --UseuGTDecision --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34 | tee results/testNtupleGT.log
    # ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l ntuple/2023D_EphZB_run370293_1404.list -o testNtuple -b 2748 --UseuGTDecision --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34 | tee results/testNtuple.log
    # ./testMenu2016 -m menu/L1Prescales2023.csv -l ntuple/testL1Ntuple.list -o testNtupleArtur -b 2748 --UseUnpackTree --UseuGTDecision --maxEvent 5000 --SelectCol 2p0E34 | tee results/testNtupleArtur.log
else
    ./testMenu2016 -m menu/L1Prescales2023.csv -u menu/EphZB2023_run370293.csv -l ntuple/2023D_EphZB_run370293_L1Nano.list -o testNano -b 2748 --UseuGTDecision --UseUnpackTree --maxEvent $EVENTS --SelectCol 2p0E34 --doNano | tee results/testNano.log
    # ./testMenu2016 -m menu/L1Prescales2023.csv -l ntuple/testL1Nano.list -o testNanoArtur -b 2748 --UseUnpackTree --UseuGTDecision --maxEvent 5000 --SelectCol 2p0E34 --doNano | tee results/testNanoArtur.log
fi
