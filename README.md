LH-HH-DECAY
===========

Make di-H events decay and hadronize in CMSSW




<ul>
  <li>get git repo
    <ul>
      <li>git clone git@github.com:amassiro/LH-HH-DECAY.git</li>
    </ul>
  </li>
  <li>copy into CMSSWrelease the cfg file
    <ul>
      <li>cp LH-HH-DECAY/POWHEG_PYTHIA6_Tauola_HH_bbWW_lnulnu_withTau_TEMPLATETeV_cff_py_GEN.py  CMSSW_5_3_9_patch1/src/POWHEG_PYTHIA6_Tauola_HH_bbWW_lnulnu_withTau_TEMPLATETeV_cff_py_GEN.py</li>
    </ul>
  </li>
  <li>get where LHE events are saved (MG style)
    <ul>
      <li>cd /afs/cern.ch/work/a/amassiro/Generation/HH/MG</li>
    </ul>
  </li>
  <li>copy script here
    <ul>
      <li>cp /afs/cern.ch/work/a/amassiro/Generation/LH-HH-DECAY/DecayAndHadronize.sh ./</li>
    </ul>
  </li>
  <li>run
    <ul>
      <li>####                                                                     seed       folder             model     energy</li>
      <li>for ((  i = 0 ;  i \< 10;  i++  ))  do bsub -q 8nh "DecayAndHadronize.sh   $i    MyS05at13TeV_10k_    SO5at13TeV    13"; done</li>
    </ul>
  </li>
</ul>
