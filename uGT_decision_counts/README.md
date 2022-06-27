## uGT decision

Returns the emulated trigger counts in an ntuple for a given (L1_bit, L1_seed)  map.

### algo_map

Contains a list of the form [L1_bit, "L1_seed_name"], used to return the emulated counts as stored in the ntuple.
Note : Please ensure that the mapping corresponds exactly to the indices and L1 seed names as the emulated menu, to avoid confusion. This is because the L1 ntuple does not store the L1 seed names. Inclusion of the L1 seed names in this script/map is for convenience and readability.

### getEmulatorDecision.py

Script that takes ntuple as input, using the provided algo map to obtain the emulated trigger counts.

The output lists the L1 bit index, L1 seed name, and the triggered counts.

## Usage

python3 getEmulatorDecision.py <L1Ntuple>

<L1Ntuple> : location of the L1Ntuple.root file
