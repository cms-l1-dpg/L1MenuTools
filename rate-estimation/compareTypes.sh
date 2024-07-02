NAME=$1

git diff --no-index results/testNtupleGT_$NAME.log results/testNanoGT_$NAME.log |& tee results/compareNtupleVsNanoGT_$NAME.diff
git diff --no-index results/testNtupleStandalone_$NAME.log results/testNanoStandalone_$NAME.log |& tee results/compareNtupleVsNanoStandalone_$NAME.diff

# Direct command line examples
# NAME=<name>; git diff --no-index results/testNtupleGT_$NAME.log results/testNanoGT_$NAME.log |& tee results/compareNtupleVsNanoGT_$NAME.diff
# NAME=<name>; git diff --no-index results/testNtupleStandalone_$NAME.log results/testNanoStandalone_$NAME.log |& tee results/compareNtupleVsNanoStandalone_$NAME.diff
