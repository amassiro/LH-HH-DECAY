#!/bin/env python

import os
import subprocess
import sys

max               = int(sys.argv[1])
folder            = sys.argv[2]
model             = sys.argv[3]
energy            = sys.argv[4]
csi               = sys.argv[5]
destinationfolder = sys.argv[6]


wd     = os.getcwd()
queue  = '8nh'
#queue  = '1nh' # -> not enough !?!?
#queue  = '8nm' #--> not enough time!
newfol = 'makeDecay_{ENERGY}'.format(ENERGY=energy)
os.system('mkdir -p {PWD}/{NEW}'.format(PWD=wd,NEW=newfol))



if len(sys.argv) == 1:
   print "uff, not enough information"
   sys.exit(0)

for i in range(max) :
  command = '#!/bin/bash\n\
              cd {PWD}\n\
              ./DecayAndHadronize.sh {ITER} {FOLDER} {MODEL} {ENERGY} {CSI} {DESTINATIONFOLDER}\n \
             '.format(PWD=wd, ITER=str(i), FOLDER=folder, MODEL=model, ENERGY=energy, CSI=csi, DESTINATIONFOLDER=destinationfolder).replace('  ','')

  fname = '/'.join([wd,newfol,'sub_'+str(i)+'_'+model+'_'+energy+'_'+csi+'.sh'])
  f1    = open(fname, 'w+')
  print >> f1 , command
  os.system('chmod 755 {FILE}'.format(FILE=fname)) 
  f1.close()

for i in range(max) :
  fname = '/'.join([wd,newfol,'sub_'+str(i)+'_'+model+'_'+energy+'_'+csi+'.sh'])
  subprocess.Popen(['bsub -q {QUEUE} < {FILE}'.format(QUEUE=queue,FILE=fname)], stdout=subprocess.PIPE, shell=True)
