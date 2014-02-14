HH decay
=======

copy lhe file:

    cp /afs/cern.ch/work/a/acarvalh/phenoHH/model_LHEfiles/bulk_graviton/Madgraphcg0_0137_all/MGraviton_1000.lhe     /tmp/amassiro/

modify H -> 35, to make it properly decay
    35 > bb
    25 > WW/gg/bb

    awk '/\ \ 25\ \ /&&v++%2{sub(/\ \ 25\ \ \ \ 1/, "\ \ 35\ \ \ \ 1")}{print}' /tmp/amassiro/MGraviton_1000.lhe  >  /tmp/amassiro/MGraviton_1000_H35H25.lhe


decay HH:
(compile and run on slc6)

    cd /afs/cern.ch/user/a/amassiro/work/Generation/HH/Pythia8/LH-HH-DECAY/
    cd pythia8153/examples
    ln -s ../../Makefile .
    ln -s ../../main97.cc .
    make main97
    source config.sh
    ./main97.exe    /tmp/amassiro/MGraviton_1000_H35H25.lhe  /tmp/amassiro/MGraviton_1000_H35H25_WWbb.lhe       ../../HHtoWWbb.txt




All
=======

copy

    cp /afs/cern.ch/work/a/acarvalh/phenoHH/model_LHEfiles/bulk_graviton/Madgraphcg0_0137_all/    /tmp/amassiro/

decay

    cd /afs/cern.ch/user/a/amassiro/work/Generation/HH/Pythia8/LH-HH-DECAY/
    cd pythia8153/examples
    ln -s ../../Makefile .
    ln -s ../../main97.cc .
    make main97
    source config.sh

modify H -> 35, to make it properly decay
 * 35 > bb
 * 25 > WW/gg/bb


    for OUTPUT in $(ls /tmp/amassiro/MGraviton_*.lhe)
     do
       awk '/\ \ 25\ \ /&&v++%2{sub(/\ \ 25\ \ \ \ 1/, "\ \ 35\ \ \ \ 1")}{print}' $OUTPUT  >  $OUTPUT.H35H25.lhe
     done


    ls /tmp/amassiro/MGraviton_*.H35H25.lhe  | tr "/" " " | tr "." " "  | awk '{print "./main97.exe  /tmp/amassiro/"$3".lhe."$5".lhe     /tmp/amassiro/"$3"_ww_lvlv.lhe      ../../HHtoWWbb.txt"}'




