# |/bin/sh
# decay and hadronize with cmssw

NUMBERSEED=$1

CASTORFOLDERNAME=$2

MODEL=$3

ENERGY=$4

CSI=$5

DESTINATIONFOLDER=$6



# LHE before "higgs" transformation
# fixed!

# LHE after "higgs" transformation
NAMELHEMODIFIED=$MODEL"_"$NUMBERSEED"_"$CSI"_"$ENERGY"_unweighted_events_modified.lhe"
# EDM level file
NAMEEDM=$MODEL"_"$NUMBERSEED"_"$CSI"_"$ENERGY"_EDM.root"
# GEN level file
NAMEGEN=$MODEL"_"$NUMBERSEED"_"$CSI"_"$ENERGY"_GEN.root"




echo ">>> prepare local folder"
cd /tmp/
TESTFOLDER="testDecay_"$ENERGY"_"$NUMBERSEED"_"$CSI
MODELFOLDER="My"$MODEL"_"$CSI
MODELFOLDERTGZ=$MODELFOLDER"_"$NUMBERSEED"_"$ENERGY".tgz"

mkdir /tmp/$TESTFOLDER


echo ">>> copy from castor"
rfcp $CASTORFOLDERNAME/$MODELFOLDERTGZ ./$TESTFOLDER


echo ">>> untar model"
cd /tmp/$TESTFOLDER
tar -xf $MODELFOLDERTGZ


echo ">>> go to Events folder and gunzip"
ls
cd $MODELFOLDER
cd Events
ls

# MODEL="SO5" ---> fast fix due to bug! 0 <-> O

NAMEFILELHEGZ=$MODEL"at"$ENERGY"TeV_unweighted_events.lhe.gz"
gzip -d $NAMEFILELHEGZ
ls

NAMEFILELHE=$MODEL"at"$ENERGY"TeV_unweighted_events.lhe"


echo ">>>  modify H -> 35, to make it properly decay"
echo ">>>       35 > bb"
echo ">>>       25 > WW/gg/bb"

NAMELHEMODIFIED=$MODEL"at"$ENERGY"TeV_unweighted_events_modified.lhe"
awk '/\ \ 25\ \ /&&v++%2{sub(/\ \ 25\ \ \ \ 1/, "\ \ 35\ \ \ \ 1")}{print}' $NAMEFILELHE  >  /tmp/$TESTFOLDER/$NAMELHEMODIFIED


echo ">>>  decay the Higgs and hadronize"
head  /tmp/$TESTFOLDER/$NAMELHEMODIFIED
echo "..."
tail  /tmp/$TESTFOLDER/$NAMELHEMODIFIED

cd /afs/cern.ch/user/a/amassiro/work/Generation/HH/Pythia8/LH-HH-DECAY/pythia8153/examples/
source config.sh
case ${DESTINATIONFOLDER} in
    "WWbb" )
        ./main99.exe    /tmp/$TESTFOLDER/$NAMELHEMODIFIED     /tmp/$TESTFOLDER/$NAMEGEN   ../../HHtoWWbb.txt
        ;;
    "ggbb" )
        ./main99.exe    /tmp/$TESTFOLDER/$NAMELHEMODIFIED     /tmp/$TESTFOLDER/$NAMEGEN   ../../HHtoggbb.txt
        ;;
esac

cd -



echo ">>> copy result"
/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select cp  /tmp/$TESTFOLDER/$NAMEGEN   /eos/cms/store/user/amassiro/HH/VBF/$DESTINATIONFOLDER/


echo ">>> clean up a little"
rm -rf /tmp/$TESTFOLDER/

