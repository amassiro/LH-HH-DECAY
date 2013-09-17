# |/bin/sh
# decay and hadronize with cmssw

NUMBERSEED=$1

CASTORFOLDERNAME=$2

MODEL=$3

ENERGY=$4

CSI=$5



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
# MODELFOLDERTGZ="My"$MODEL"_"$CSI"_"$NUMBERSEED"_"$ENERGY".tgz"

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


echo ">>>  modify H -> 135, to make it properly decay"

NAMELHEMODIFIED=$MODEL"at"$ENERGY"TeV_unweighted_events_modified.lhe"
awk '/\ \ 25\ \ /&&v++%2{sub(/\ \ 25\ \ \ \ 1/, "\ \ 35\ \ \ \ 1")}{print}' $NAMEFILELHE  >  /tmp/$TESTFOLDER/$NAMELHEMODIFIED


echo ">>>  decay the Higgs and hadronize"
head  /tmp/$TESTFOLDER/$NAMELHEMODIFIED
echo "..."
tail  /tmp/$TESTFOLDER/$NAMELHEMODIFIED

cd /afs/cern.ch/work/a/amassiro/Generation/CMSSW_5_3_9_patch1/src/
cmsenv
eval `scramv1 runtime -sh`


echo ">>>  prepare input"
cmsDriver.py MCDBtoEDM --conditions START53_V7C::All -s NONE --eventcontent RAWSIM --datatier GEN --filein file:/tmp/$TESTFOLDER/$NAMELHEMODIFIED   --fileout /tmp/$TESTFOLDER/$NAMEEDM  -n -1

echo ">>>  prepare cfg file for decay"

CFGFILE="POWHEG_PYTHIA6_Tauola_HH_bbWW_lnulnu_withTau_TEMPLATETeV_cff_py_GEN.py"
NEWCFGFILE="POWHEG_PYTHIA6_Tauola_HH_bbWW_lnulnu_withTau_"$ENERGY"_TeV_"$MODEL"_"$NUMBERSEED"_"$CSI"_cff_py_GEN.py"


cat $CFGFILE | sed -e s%TEMPLATEENERGY%$ENERGY%g | sed -e s%TEMPLATEINPUTFILE%/tmp/$TESTFOLDER/$NAMEEDM%g | sed -e s%TEMPLATEOUTPUTFILE%/tmp/$TESTFOLDER/$NAMEGEN%g > /tmp/$TESTFOLDER/$NEWCFGFILE

cat /tmp/$TESTFOLDER/$NEWCFGFILE

echo ">>> decay and hadronize"

cmsRun /tmp/$TESTFOLDER/$NEWCFGFILE


echo ">>> copy result"
# cp /tmp/amassiro/$NAMEGEN /afs/cern.ch/work/a/amassiro/Generation/HH/DATA/
# cp /tmp/amassiro/$NAMEGEN /afs/cern.ch/user/a/amassiro/DATA/
# eoscms cp /tmp/amassiro/$NAMEGEN   /store/caf/user/amassiro/TEST/
rfcp   /tmp/$TESTFOLDER/$NAMEGEN   /castor/cern.ch/user/a/amassiro/HH-WWbb

echo ">>> clean up a little"
rm -r /tmp/$TESTFOLDER/

