# |/bin/sh
# hadronize with pythia8


Model=$1

InputFolder=$2
InputFile=$3

OutputFolder=$4
OutputFile=$5

startEntry=$6
endEntry=$7

echo "- Model        = "$Model
echo "- InputFolder  = "$InputFolder
echo "- InputFile    = "$InputFile
echo "- OutputFolder = "$OutputFolder
echo "- OutputFile   = "$OutputFile
echo "- startEntry   = "$startEntry
echo "- endEntry     = "$endEntry


echo ">>> prepare local folder"
cd /tmp/
TESTFOLDER="testDecay_"$Model
echo "         "$TESTFOLDER
mkdir /tmp/$TESTFOLDER
cd /tmp/$TESTFOLDER

/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select cp  $InputFolder/$InputFile    ./$InputFile

echo ">>>  hadronize"
cd /afs/cern.ch/user/a/amassiro/work/Generation/HH/Pythia8/LH-HH-DECAY/pythia8153/examples/
source config.sh
./main98.exe    /tmp/$TESTFOLDER/$InputFile   /tmp/$TESTFOLDER/$OutputFile    13    $startEntry    $endEntry
cd -

/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select mkdir /eos/cms/store/user/amassiro/HH/Background/$Model/
/afs/cern.ch/project/eos/installation/0.2.31/bin/eos.select cp    /tmp/$TESTFOLDER/$OutputFile   $OutputFolder/$Model/

echo ">>> clean up a little"
rm -rf /tmp/$TESTFOLDER/

