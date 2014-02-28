LH-HH-DECAY
===========

Make di-H events decay and hadronize in CMSSW

 * get git repo

        git clone git@github.com:amassiro/LH-HH-DECAY.git

 * copy into CMSSWrelease the cfg file

        cp LH-HH-DECAY/POWHEG_PYTHIA6_Tauola_HH_*_TEMPLATETeV_cff_py_GEN.py  CMSSW_5_3_9_patch1/src/
        cp LH-HH-DECAY/SubmitDecayAndHadronize.py     CMSSW_5_3_9_patch1/src/
        cp LH-HH-DECAY/DecayAndHadronize.sh  CMSSW_5_3_9_patch1/src/DecayAndHadronize.sh<


 * what are the files you just copied:

        POWHEG_PYTHIA6_Tauola_HH_BLA_TEMPLATETeV_cff_py_GEN.py  :    generic config file for di-Higgs decay. BLA stands for Higgs decay channels, bbWW_lnulnu_withTau or bbgg
        SubmitDecayAndHadronize.py   a python script that creates bash scripts calling DecayAndHadronize.sh to submit to batch system
        DecayAndHadronize.sh  the actaual script that performs the decay and hadronization

 * run
      To run on batch system:

        ./SubmitDecayAndHadronize.py    MAXITER    CASTORFOLDER                                  MODEL    ENERGY     CSI      DESTINATIONFOLDER
        ./SubmitDecayAndHadronize.py     1    /castor/cern.ch/user/a/amassiro/HHLHE-SO5-11Sep      SO5      13         10       WWbb

 * Results are saved on a castor folder defined in DecayAndHadronize.sh

        /castor/cern.ch/user/a/amassiro/HH-WWbb   ---> old version
        eos/cms/store/user/amassiro/HH/VBF/
        in subdirectories defined by "DESTINATIONFOLDER", as WWbb and ggbb


## Alternatively, use Pythia 8 ##

_be sure not to have CMSSW environment loaded for the following operations, it is screwing up librairies for runtime_

* Get [Pythia8](http://home.thep.lu.se/~torbjorn/Pythia.html) from the website

        NOT wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8180.tgz
        NOT wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8176.tgz
        wget http://home.thep.lu.se/~torbjorn/pythia8/pythia8153.tgz   -> as in CMS

* Untar it:

        tar -xvzf pythia8153.tgz     -> as in CMS


* Compile pythia (works out of the box on lxplus *slc6*, custom makefiles are needed for linking boost librairies [compulsory for `.lhe.gz` format]):

        cd pythia8153/
        mv configure old_configure; ln -s ../LH-HH-DECAY/configure .
        ./configure --enable-gzip  --with-hepmcversion=2.06.08
        make -j 8

* Go in the examples directory and link our hadronization stuff, and compile it

        cd examples
        ./configure
        ln -s ../../LH-HH-DECAY/main99.cc .
        mv Makefile old_Makefile; ln -s ../../LH-HH-DECAY/Makefile .
        make main99


# LHE parton analysis:

 * Pythia decay of HH signal without hadronization.
 * LHE output
 * main97.cc

        cd /afs/cern.ch/user/a/amassiro/work/Generation/HH/Pythia8/LH-HH-DECAY/
        cd pythia8153/examples
        ln -s ../../Makefile .
        ln -s ../../main97.cc .
        make main97
        source config.sh
        ./main97.exe    ../../atEightTeV_events_TEST_H35H25.lhe    output.lhe       ../../HHtoWWbb.txt

but first:

    modify H -> 35, to make it properly decay
    35 > bb
    25 > WW/gg/bb

    awk '/\ \ 25\ \ /&&v++%2{sub(/\ \ 25\ \ \ \ 1/, "\ \ 35\ \ \ \ 1")}{print}' ../../atEightTeV_events_TEST.lhe  >  ../../atEightTeV_events_TEST_H35H25.lhe



 * Pythia decay of WW into lvlv without hadronization.
 * LHE output
 * main96.cc


        cd /afs/cern.ch/user/a/amassiro/work/Generation/HH/Pythia8/LH-HH-DECAY/
        cd pythia8153/examples
        ln -s ../../Makefile .
        ln -s ../../main96.cc .
        make main96
        source config.sh
        cp /afs/cern.ch/user/a/amassiro/public/xLHTF/hhwwbb/wwbbjj/unweighted_events.lhe /tmp/amassiro/
        ./main96.exe   /tmp/amassiro/unweighted_events.lhe    output.lhe

        cp /afs/cern.ch/user/a/amassiro/public/xLHTF/hhwwbb/wwbbjj_ptb20/unweighted_events.lhe  /tmp/amassiro/
       ./main96.exe   /tmp/amassiro/unweighted_events.lhe    /tmp/amassiro/wwbbjj_ptb20_llvv.lhe


        mkdir /afs/cern.ch/user/a/amassiro/public/xLHTF/hhwwbb/wwbbjj_ptb20_lvlv/ 
        cp  /tmp/amassiro/wwbbjj_ptb20_llvv.lhe    /afs/cern.ch/user/a/amassiro/public/xLHTF/hhwwbb/wwbbjj_ptb20_lvlv/


This one:

       21   -1    0    0  504  503  0.00000000000E+00  0.00000000000E+00  0.14459962809E+04  0.14459962809E+04  0.00000000000E+00 0. -1.
        2   -1    0    0  501    0  0.00000000000E+00  0.00000000000E+00 -0.14222108953E+04  0.14222108953E+04  0.00000000000E+00 0. -1.
        6    2    1    2  501    0 -0.18899978949E+03 -0.22288782180E+03 -0.61236892484E+02  0.34482679745E+03  0.17249829480E+03 0.  0.
       -6    2    1    2    0  503 -0.93776545970E+02  0.30000518302E+03 -0.99444680889E+03  0.10567653743E+04  0.17038617983E+03 0.  0.
       24    1    3    3    0    0 -0.13569242764E+03 -0.19821953927E+03  0.85826165828E+01  0.25346459063E+03  0.80419007276E+02 0.  0.
      -24    1    4    4    0    0 -0.12250790057E+03  0.28971568985E+03 -0.87186496770E+03  0.93035429019E+03  0.80419007276E+02 0.  0.
        5    1    3    3  501    0 -0.53307361849E+02 -0.24668282531E+02 -0.69819509066E+02  0.91362206819E+02  0.46999998093E+01 0. -1.
       -5    1    4    4    0  503  0.28731354604E+02  0.10289493172E+02 -0.12258184119E+03  0.12641108415E+03  0.46999998093E+01 0.  1.
       21    1    1    2  504  502  0.49232109154E+02 -0.45324725622E+02  0.12000149182E+04  0.12018793347E+04  0.00000000000E+00 0. -1.
        2    1    1    2  502    0  0.23354422630E+03 -0.31792635598E+02 -0.12054583126E+03  0.26473566959E+03  0.00000000000E+00 0. -1.


transformed into:

       21    -1     0     0   504   503  0.0000000000e+00  0.0000000000e+00  1.4459962809e+03  1.4459962809e+03  0.0000000000e+00 0. 9.
        2    -1     0     0   501     0  0.0000000000e+00  0.0000000000e+00 -1.4222108953e+03  1.4222108953e+03  0.0000000000e+00 0. 9.
        6     2     1     2   501     0 -1.8899978949e+02 -2.2288782180e+02 -6.1236892484e+01  3.4482679745e+02  1.7249829480e+02 0. 9.
       -6     2     1     2     0   503 -9.3776545970e+01  3.0000518302e+02 -9.9444680889e+02  1.0567653743e+03  1.7038617983e+02 0. 9.
       21     1     1     2   504   502  4.9232109154e+01 -4.5324725622e+01  1.2000149182e+03  1.2018793347e+03  0.0000000000e+00 0. 9.
        2     1     1     2   502     0  2.3354422630e+02 -3.1792635598e+01 -1.2054583126e+02  2.6473566959e+02  0.0000000000e+00 0. 9.
       24     2     3     0     0     0 -1.3569242764e+02 -1.9821953927e+02  8.5826165828e+00  2.5346459063e+02  8.0419007276e+01 0. 9.
        5     1     3     0   501     0 -5.3307361849e+01 -2.4668282531e+01 -6.9819509066e+01  9.1362206819e+01  4.6999998093e+00 0. 9.
      -24     2     4     0     0     0 -1.2250790057e+02  2.8971568985e+02 -8.7186496770e+02  9.3035429019e+02  8.0419007276e+01 0. 9.
       -5     1     4     0     0   503  2.8731354604e+01  1.0289493172e+01 -1.2258184119e+02  1.2641108415e+02  4.6999998093e+00 0. 9.
      -11     1     7     0     0     0 -8.9621536517e+01 -8.9378632712e+01 -2.8214490596e+01  1.2967890065e+02  5.1100000000e-04 0. 9.
       12     1     7     0     0     0 -4.6070891151e+01 -1.0884090660e+02  3.6797107180e+01  1.2378569004e+02  0.0000000000e+00 0. 9.
       13     1     9     0     0     0 -5.6471824287e+01  6.4641755894e+01 -3.0573753943e+02  3.1755799112e+02  1.0566000000e-01  8.113505e+05 9.
      -14     1     9     0     0     0 -6.6036076342e+01  2.2507393409e+02 -5.6612742868e+02  6.1279629951e+02  0.0000000000e+00 0. 9.



# HEPMC dump:

 * H decay included "main99"

        ln -s ../../Makefile
        ln -s ../../main99.cc .
        make main99
        source config.sh
        ./main99.exe     input.lhe    output.hepmc       decay.txt

        ./main99.exe   /tmp/amassiro/atEightTeV_events_patched.lhe        /tmp/amassiro/test-MR410_out.lhe.hepmc      ../../HHtobbbb.txt

 * Only hadronization "main98"

        ln -s ../../Makefile
        ln -s ../../main98.cc
        make main98
        source config.sh
        ./main98.exe   input.lhe     output.hepmc

        ./main98.exe   /tmp/amassiro/TT_CT10_13TeV-powheg-reduced.lhe     /tmp/amassiro/test-ttbar.lhe.hepmc
        ./main98.exe   /tmp/amassiro/TT_CT10_13TeV-powheg-reduced.lhe     /tmp/amassiro/test-ttbar.lhe.hepmc   13    0     1000


* Hadronize it!

        SubmitHadronize.py :  a python script that creates bash scripts calling Hadronize.sh to submit to batch system
        Hadronize.sh  : the actaual script that performs the hadronization

        Using CMSSW interface
        SubmitHadronizePythia8.py :  a python script that creates bash scripts calling Hadronize.sh to submit to batch system
        HadronizePythia8.sh  : the actaual script that performs the hadronization

* how to use it:

        python SubmitHadronize.py         model  InputFolder  InputFile  OutputFolder  OutputFile
        python SubmitHadronizePythia8.py  model  InputFolder  InputFile  OutputFolder  OutputFile

            model: e.g. ttbar, Wjet, ...
            InputFile: input lhe file
            OutputFile: output hepmc file

  example:

        python SubmitHadronizePythia8.py  ttbar     /eos/cms/store/lhe/9007/     TT_CT10_13TeV-powheg.lhe     /eos/cms/store/user/amassiro/HH/Background      TT_CT10_13TeV-powheg.hepmc

        python SubmitHadronizePythia8.py  ttbar_1     /eos/cms/store/lhe/9007/     TT_CT10_13TeV-powheg.lhe     /eos/cms/store/user/amassiro/HH/Background      TT_CT10_13TeV-powheg.hepmc      0    5000
        python SubmitHadronizePythia8.py  ttbar_2     /eos/cms/store/lhe/9007/     TT_CT10_13TeV-powheg.lhe     /eos/cms/store/user/amassiro/HH/Background      TT_CT10_13TeV-powheg.hepmc   5000   10000
        python SubmitHadronizePythia8.py  ttbar_3     /eos/cms/store/lhe/9007/     TT_CT10_13TeV-powheg.lhe     /eos/cms/store/user/amassiro/HH/Background      TT_CT10_13TeV-powheg.hepmc  10000   20000
        python SubmitHadronizePythia8.py  ttbar_4     /eos/cms/store/lhe/9007/     TT_CT10_13TeV-powheg.lhe     /eos/cms/store/user/amassiro/HH/Background      TT_CT10_13TeV-powheg.hepmc  20000   30000
        python SubmitHadronizePythia8.py  ttbar_5     /eos/cms/store/lhe/9007/     TT_CT10_13TeV-powheg.lhe     /eos/cms/store/user/amassiro/HH/Background      TT_CT10_13TeV-powheg.hepmc  30000   40000
        python SubmitHadronizePythia8.py  ttbar_6     /eos/cms/store/lhe/9007/     TT_CT10_13TeV-powheg.lhe     /eos/cms/store/user/amassiro/HH/Background      TT_CT10_13TeV-powheg.hepmc  40000   50000


  to run Higgs decay on batch system (NB Center of mass energy is taken from LHE file by Pythia8!):

        ./SubmitDecayAndHadronizePythia8.py    MAXITER    CASTORFOLDER                                  MODEL    ENERGY     CSI      DESTINATIONFOLDER
        ./SubmitDecayAndHadronizePythia8.py     1    /castor/cern.ch/user/a/amassiro/HHLHE-SO5-11Sep      SO5      13         10       WWbb



