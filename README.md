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
      <li>cp LH-HH-DECAY/POWHEG_PYTHIA6_Tauola_HH_*_TEMPLATETeV_cff_py_GEN.py  CMSSW_5_3_9_patch1/src/</li>
      <li>cp LH-HH-DECAY/SubmitDecayAndHadronize.py     CMSSW_5_3_9_patch1/src/</li>
      <li>cp LH-HH-DECAY/DecayAndHadronize.sh  CMSSW_5_3_9_patch1/src/DecayAndHadronize.sh</li>
    </ul>
  </li>
  <li>what are the files you just copied:
    <ul>
      <li>POWHEG_PYTHIA6_Tauola_HH_BLA_TEMPLATETeV_cff_py_GEN.py  :    generic config file for di-Higgs decay. BLA stands for Higgs decay channels, bbWW_lnulnu_withTau or bbgg </li>
      <li>SubmitDecayAndHadronize.py   a python script that creates bash scripts calling DecayAndHadronize.sh to submit to batch system</li>
      <li>DecayAndHadronize.sh  the actaual script that performs the decay and hadronization</li>
    </ul>
  </li>
  <li>run
    <ul>
      <li> To run on batch system:</li>
      <li>   ./SubmitDecayAndHadronize.py    MAXITER    CASTORFOLDER                                  MODEL    ENERGY     CSI      DESTINATIONFOLDER  </li>
      <li>   ./SubmitDecayAndHadronize.py     1    /castor/cern.ch/user/a/amassiro/HHLHE-SO5-11Sep      SO5      13         10       WWbb </li>
    </ul>
  </li>
  <li>Results are saved on a castor folder defined in DecayAndHadronize.sh
    <ul>
      <li>/castor/cern.ch/user/a/amassiro/HH-WWbb   ---> old version</li>
      <li>eos/cms/store/user/amassiro/HH/VBF/</li>
      <li>in subdirectories defined by "DESTINATIONFOLDER", as WWbb and ggbb</li>
    </ul>
  </li>
</ul>


## Alternatively, use Pythia 8 ##

_be sure not to have CMSSW environment loaded for the following operations, it is screwing up librairies for runtime_

* Get [Pythia8](http://home.thep.lu.se/~torbjorn/Pythia.html) from the website

        wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8180.tgz

* Untar it:

        tar -xvzf pythia8180.tgz

* Compile pythia (works out of the box on lxplus *slc5*, custom makefiles are needed for linking boost librairies [compulsory for `.lhe.gz` format]):

        cd pythia8180/
        rm configure
        ln -s ../configureSlc5 configure
        ./configure --enable-gzip --with-boost=/afs/cern.ch/sw/lcg/external/Boost/1.48.0_python2.6/x86_64-slc5-gcc43-opt --with-zlib=/afs/cern.ch/sw/lcg/external/zlib/1.2.5/x86_64-slc5-gcc43-opt/lib  --lcgplatform=x86_64-slc5-gcc43-opt  --enable-shared  --with-hepmc=/afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc5-gcc43-opt --with-hepmcversion=2.06.08
        make -j 8


* Compile pythia (works out of the box on lxplus *slc6*, custom makefiles are needed for linking boost librairies [compulsory for `.lhe.gz` format]):

        cd pythia8180/
        rm configure
        ln -s ../configure
        ./configure --enable-gzip --with-boost=/afs/cern.ch/sw/lcg/external/Boost/1.48.0_python2.6/x86_64-slc6-gcc46-opt --with-zlib=/afs/cern.ch/sw/lcg/external/zlib/1.2.5/x86_64-slc6-gcc46-opt/lib --lcgplatform=x86_64-slc6-gcc46-opt  --enable-shared  --with-hepmc=/afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt --with-hepmcversion=2.06.08
        make -j 8
        make -j 8


* Go in the examples directory and link our hadronization stuff, and compile it

        cd examples
        ln -s ../../main00.cc .
        make main00
        source config.sh
        ./main00.exe


  HEPMC dump:

  H decay included

        ln -s ../../Makefile
        ln -s ../../main99.cc .
        make main99
        source config.sh
        ./main99.exe

  Only hadronization

        ln -s ../../Makefile
        ln -s ../../main98.cc .
        make main98
        source config.sh
        ./main98.exe   /tmp/amassiro/atEightTeV_events_patched.lhe        /tmp/amassiro/test-MR410_out.lhe.hepmc


* Hadronize it!

        SubmitHadronize.py :  a python script that creates bash scripts calling Hadronize.sh to submit to batch system
        Hadronize.sh  : the actaual script that performs the hadronization

* how to use it:

        python SubmitHadronize.py  model  InputFolder  InputFile  OutputFolder  OutputFile

            model: e.g. ttbar, Wjet, ...
            InputFile: input lhe file
            OutputFile: output hepmc file

  example:

        python SubmitHadronize.py  ttbar     /eos/cms/store/lhe/9007/     TT_CT10_13TeV-powheg.lhe     /eos/cms/store/user/amassiro/HH/Background      TT_CT10_13TeV-powheg.hepmc

* _TO BE CONTINUED_



