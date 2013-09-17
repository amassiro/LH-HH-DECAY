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
      <li>cp LH-HH-DECAY/SubmitDecayAndHadronize.py     CMSSW_5_3_9_patch1/src/</li>
      <li>cp LH-HH-DECAY/DecayAndHadronize.sh  CMSSW_5_3_9_patch1/src/DecayAndHadronize.sh</li>
    </ul>
  </li>
  <li>what are the files you just copied:
    <ul>
      <li>POWHEG_PYTHIA6_Tauola_HH_BLA_lnulnu_withTau_TEMPLATETeV_cff_py_GEN.py  :    generic config file for di-Higgs decay. BLA stands for Higgs decay channels, bbWW or bbgg </li>
      <li>SubmitDecayAndHadronize.py   a python script that creates bash scripts calling DecayAndHadronize.sh to submit to batch system</li>
      <li>DecayAndHadronize.sh  the actaual script that performs the decay and hadronization</li>
    </ul>
  </li>
  <li>run
    <ul>
      <li> To run on batch system:</li>
      <li>   ./SubmitDecayAndHadronize.py    MAXITER    CASTORFOLDER                                  MODEL    ENERGY     CSI   </li>
      <li>   ./SubmitDecayAndHadronize.py     1    /castor/cern.ch/user/a/amassiro/HHLHE-SO5-11Sep      SO5      13         10  </li>
    </ul>
  </li>
  <li>Results are saved on a castor folder defined in DecayAndHadronize.sh
    <ul>
      <li>/castor/cern.ch/user/a/amassiro/HH-WWbb</li>
    </ul>
  </li>
</ul>
