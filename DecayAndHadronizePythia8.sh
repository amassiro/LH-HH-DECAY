# |/bin/sh
# decay and hadronize with cmssw

# go in verbose mode to check what's happening
set -x

# Setup eos
alias eos='/afs/cern.ch/project/eos/installation/0.3.15/bin/eos.select'

if [[ -z ${3} ]]
then
    echo "Need at least 3 arguments"
    exit 1
fi

INPUT_FILE=${1}
OUTPUT_FOLDER=${2}
DECAY_FILE=${3}
PYTHIA_FOLDER=/afs/cern.ch/work/o/obondu/LesHouches2013/pythia8153
LH_HH_DECAY_FOLDER=/afs/cern.ch/work/o/obondu/LesHouches2013/LH-HH-DECAY
# For input = file.lhe, the output is file_DECAY.hepmc
OUTPUT_FILE=`basename ${INPUT_FILE} .lhe`
DECAY=`basename ${DECAY_FILE} .txt`
OUTLOG_FILE="${OUTPUT_FILE}_${DECAY}.eo"
OUTPUT_FILE="${OUTPUT_FILE}_${DECAY}.hepmc"

JOBDIR=`basename ${INPUT_FILE} .lhe`
mkdir -p ${JOBDIR}
mkdir -p xmldoc
cd ${JOBDIR}
echo ${PWD}

# Get everything locally
cp ${PYTHIA_FOLDER}/xmldoc/* ../xmldoc/ 
cp ${PYTHIA_FOLDER}/examples/main99.exe .
cp ${INPUT_FILE} .
cp ${LH_HH_DECAY_FOLDER}/${DECAY_FILE} . 
cp ${LH_HH_DECAY_FOLDER}/config.sh .

# Be prepared to decay the Higgses
echo ">>>  modify H -> 35, to make it properly decay"
echo ">>>       35 > bb"
echo ">>>       25 > WW/gg/bb"
INPUT_FILE_READY_FOR_DECAY=`basename ${INPUT_FILE} .lhe`
INPUT_FILE_READY_FOR_DECAY="${INPUT_FILE_READY_FOR_DECAY}_readyForDecay.lhe"
awk '/\ \ 25\ \ /&&v++%2{sub(/\ \ 25\ \ \ \ 1/, "\ \ 35\ \ \ \ 1")}{print}' ${INPUT_FILE}  >  ${INPUT_FILE_READY_FOR_DECAY}

# Get the main99 doing the job
echo ${PWD}
ls -l

source config.sh
./main99.exe `basename ${INPUT_FILE_READY_FOR_DECAY}` ${OUTPUT_FILE} ${DECAY_FILE} 2> ${OUTLOG_FILE} | tee ${OUTLOG_FILE}

echo ${PWD}
ls -l

# Zip and copy back the output and the logs, perform some post-job cleanup
gzip ${OUTPUT_FILE}
echo ${PWD}
ls -l

eos cp ${OUTPUT_FILE} ${OUTPUT_FOLDER}/
eos cp ${OUTLOG_FILE} ${OUTPUT_FOLDER}/
#cp ${OUTPUT_FILE}.gz ${OUTPUT_FOLDER}
#cp ${OUTLOG_FILE} ${OUTPUT_FOLDER}

#rm ${INPUT_FILE_READY_FOR_DECAY}
#rm ${OUTPUT_FILE}.gz ${OUTLOG_FILE}


