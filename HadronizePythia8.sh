# |/bin/sh
# hadronize with pythia8


Model=$1

InputFolder=$2
InputFile=$3

OutputFolder=$4
OutputFile=$5



echo ">>> prepare local folder"
cd /tmp/
TESTFOLDER="testDecay_"$Model
mkdir /tmp/$TESTFOLDER
cd /tmp/$TESTFOLDER

/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select cp  $InputFolder/$InputFile    ./$InputFile

echo ">>>  hadronize"
cd /afs/cern.ch/user/a/amassiro/work/Generation/HH/Pythia8/LH-HH-DECAY/pythia8180/examples/
./main98.exe    /tmp/$TESTFOLDER/$InputFile   /tmp/$TESTFOLDER/$OutputFile
cd -

/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select mkdir /eos/cms/store/user/amassiro/HH/Background/$Model/
/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select cp    /tmp/$TESTFOLDER/$OutputFile   $OutputFolder/$Model/

echo ">>> clean up a little"
rm -r /tmp/$TESTFOLDER/

