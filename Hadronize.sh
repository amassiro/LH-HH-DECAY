# |/bin/sh
# hadronize with pythia8

InputFile=$1
OutputFile=$2
Model=$3


echo ">>> prepare local folder"
cd /tmp/
TESTFOLDER="testDecay_"$Model
mkdir /tmp/$TESTFOLDER
cd /tmp/$TESTFOLDER


echo ">>>  hadronize"
/afs/cern.ch/user/a/amassiro/work/Generation/HH/Pythia8/LH-HH-DECAY/pythia8180/examples/main98.exe $InputFile   $OutputFile

/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select mkdir /eos/cms/store/user/amassiro/HH/Background/$Model/
/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select cp    /tmp/$TESTFOLDER/$OutputFile   /eos/cms/store/user/amassiro/HH/Background/$Model/

echo ">>> clean up a little"
rm -r /tmp/$TESTFOLDER/

