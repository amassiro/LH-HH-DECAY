# |/bin/sh
# decay and hadronize with cmssw

NUMBERSEED=$1

FOLDERNAME=$2

MODEL=$3

ENERGY=$4


NAMEFILELHE=$MODEL"_unweighted_events.lhe.gz"

# before "higgs" transformation
NAMELHE=$MODEL"_unweighted_events.lhe"
# after "higgs" transformation
NAMELHEMODIFIED=$MODEL"_"$NUMBERSEED"_"$FOLDERNAME"_unweighted_events_modified.lhe"
# EDM level file
NAMEEDM=$MODEL"_"$NUMBERSEED"_"$FOLDERNAME"_EDM.root"
# GEN level file
NAMEGEN=$MODEL"_"$NUMBERSEED"_"$FOLDERNAME"_GEN.root"


cp  $FOLDERNAME$NUMBERSEED/Events/$NAMEFILELHE /tmp/amassiro/
cd /tmp/amassiro/
gzip -d $NAMEFILELHE

cd -


### modify H -> 135, to make it properly decay
awk '/\ \ 25\ \ /&&v++%2{sub(/\ \ 25\ \ \ \ 1/, "\ \ 35\ \ \ \ 1")}{print}' /tmp/amassiro/$NAMELHE  > /tmp/amassiro/$NAMELHEMODIFIED

### decay the Higgs and hadronize
cd /afs/cern.ch/work/a/amassiro/Generation/CMSSW_5_3_9_patch1/src/
cmsenv
eval `scramv1 runtime -sh`

# prepare input
cmsDriver.py MCDBtoEDM --conditions START53_V7C::All -s NONE --eventcontent RAWSIM --datatier GEN --filein file:/tmp/amassiro/$NAMELHEMODIFIED   --fileout /tmp/amassiro/$NAMEEDM  -n -1

# prepare cfg file for decay



### decay and hadronize

CFGFILE="POWHEG_PYTHIA6_Tauola_HH_bbWW_lnulnu_withTau_TEMPLATETeV_cff_py_GEN.py"
NEWCFGFILE="POWHEG_PYTHIA6_Tauola_HH_bbWW_lnulnu_withTau_"$ENERGY"_TeV_"$MODEL"_"$NUMBERSEED"_"$FOLDERNAME"_cff_py_GEN.py"


cat $CFGFILE | sed -e s%TEMPLATEENERGY%$ENERGY%g | sed -e s%TEMPLATEINPUTFILE%/tmp/amassiro/$NAMEEDM%g | sed -e s%TEMPLATEOUTPUTFILE%/tmp/amassiro/$NAMEGEN%g > /tmp/amassiro/$NEWCFGFILE

## actually decaying
cmsRun $CFGFILE

## copy result
cp /tmp/amassiro/$NAMEGEN /afs/cern.ch/work/a/amassiro/Generation/HH/DATA/






