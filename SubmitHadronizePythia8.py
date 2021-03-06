#!/bin/env python

import os
import subprocess
import sys

model             = sys.argv[1]
InputFolder       = sys.argv[2]
InputFile         = sys.argv[3]
OutputFolder      = sys.argv[4]
OutputFile        = sys.argv[5]

startEntry = 0
endEntry = -1
if len(sys.argv)<=6 :
  startEntry = 0
  endEntry = -1
else :
  startEntry        = sys.argv[6]
  endEntry          = sys.argv[7]


wd     = os.getcwd()
queue  = '1nd'
#queue  = '8nh'
#queue  = '1nh' # -> not enough !?!?
#queue  = '8nm' #--> not enough time!
newfol = 'makeHadronization_{MODEL}'.format(MODEL=model)
os.system('mkdir -p {PWD}/{NEW}'.format(PWD=wd,NEW=newfol))

if len(sys.argv) == 1:
   print "uff, not enough information"
   sys.exit(0)

command = '#!/bin/bash\n\
           cd {PWD}\n\
           ./HadronizePythia8.sh {MODEL} {INPUTFOLDER} {INPUTFILE} {OUTPUTFOLDER} {OUTPUTFILE} {STARTENTRY} {ENDENTRY}\n \
           '.format(PWD=wd, MODEL=model, INPUTFOLDER=InputFolder, INPUTFILE=InputFile, OUTPUTFOLDER=OutputFolder, OUTPUTFILE=OutputFile, STARTENTRY=startEntry, ENDENTRY=endEntry).replace('  ','')

fname = '/'.join([wd,newfol,'sub_'+model+'.sh'])
f1    = open(fname, 'w+')
print >> f1 , command
os.system('chmod 755 {FILE}'.format(FILE=fname)) 
f1.close()

fname = '/'.join([wd,newfol,'sub_'+model+'.sh'])
subprocess.Popen(['bsub -q {QUEUE} < {FILE}'.format(QUEUE=queue,FILE=fname)], stdout=subprocess.PIPE, shell=True)
