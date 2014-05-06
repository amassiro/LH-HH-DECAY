#!/bin/bash
LHEDIR="/afs/cern.ch/work/a/acarvalh/phenoHH/model_LHEfiles/bulk_graviton/Madgraphcg0_0137_all_13TeV"
STOREDIR="/eos/cms/store/cmst3/user/obondu/HH/signal/13TeV"

for decay in `echo "HHtobbbb.txt HHtoggbb.txt HHtoWWbb.txt"`
do
    echo "decay= ${decay}"
    for file in `ls ${LHEDIR}/*lhe`
    do
        echo "file= ${file}"
        bsub -q 8nh DecayAndHadronizePythia8.sh ${file} ${STOREDIR} ${decay}
    done
done
