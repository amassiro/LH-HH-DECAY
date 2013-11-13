// main11.cc is a part of the PYTHIA event generator.
// Copyright (C) 2012 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. 
// It illustrates how Les Houches Event File input can be used in Pythia8.
// It uses the ttsample.lhe input file, the latter only with 100 events.


#include <fstream>
#include "Pythia8/Pythia.h"
// #include "Pythia8/Pythia8ToHepMC.h"
// #include "HepMC/GenEvent.h"
// #include "HepMC/IO_GenEvent.h"


using namespace Pythia8; 
int main() {

  // path to LHE folder
  //unsigned const nfiles=4;
  string path= "/tmp/amassiro/";

  //for(unsigned ifile=0; ifile<8; ifile++){
   unsigned ifile=0;

    string namefile_in=path + "";
    //if(ifile==0) namefile_in += "250.lhe";
    if(ifile==0) namefile_in += "GluGluToHHTo2B2Tau_M-125_8TeV_madgraph_v2.lhe";
    if(ifile==1) namefile_in += "500.lhe";
    if(ifile==2) namefile_in += "700.lhe";
    //if(ifile==3) namefile_in += "750.lhe";
    if(ifile==3) namefile_in += "1000.lhe";
    if(ifile==4) namefile_in += "1500.lhe";
    if(ifile==5) namefile_in += "2000.lhe";
    if(ifile==6) namefile_in += "2500.lhe";
    if(ifile==7) namefile_in += "3000.lhe";
    string namefile_out=namefile_in + ".pythia";
// 		namefile_in = "test-MR410.lhe.gz";
//		namefile_in = "ttbar.lhe";
		namefile_out = "/tmp/amassiro/test-MR410_out.lhe";

    cout<<"\n namefile_in = "<<namefile_in<<endl;
    cout<<"\n namefile_out = "<<namefile_out<<endl;

    
    // output file
    // we want to store the list of all final state particles
    ofstream out_pythia;
    // Highest precision required for jet clustering
    out_pythia.precision(15);

    // Generator. We here stick with default values, but changes
    // could be inserted with readString or readFile.          
    Pythia pythia;                            
    
    // Initialize Les Houches Event File run. List initialization information.
    pythia.readString("Beams:frameType = 4"); 
     // the analysis program
    string sfile = "Beams:LHEF ="+namefile_in;
    pythia.readString(sfile.c_str()); 
    out_pythia.open(namefile_out.c_str());
 
    
    // Interface for conversion from Pythia8::Event to HepMC event. 
//     HepMC::Pythia8ToHepMC ToHepMC;
    
    
    
  // Allow for possibility of a few faulty events.
  int nAbort = 2;
  int iAbort = 0;

  //  pythia.settings.listAll();
  //exit(-10);

  
  // Settings
//	pythia.readString("Tune:preferLHAPDF = off"); // possibly temporary
  pythia.readString("PartonLevel:MI = off"); // Off multiple interactions 
  pythia.readString("PartonLevel:ISR = off");  // Shower on
  	pythia.readString("SpaceShower:QEDshowerByQ = off");  // QED off on ISR / quarks irradiate photons
  	pythia.readString("SpaceShower:QEDshowerByL = off");  // QED off on ISR / leptons irradiate photons

  pythia.readString("PartonLevel:FSR = on");  // Shower on
  	pythia.readString("TimeShower:QEDshowerByQ = off");  // QED off on ISR / quarks irradiate photons
  	pythia.readString("TimeShower:QEDshowerByL = off");  // QED off on ISR / leptons irradiate photons  
  	pythia.readString("TimeShower:QEDshowerByGamma = off");  // Allow photons to branch into lepton or quark pairs  

  pythia.readString("HadronLevel:all = off"); // Of hadronization
  pythia.readString("PromptPhoton:all = off"); // Of for the group of all prompt photon processes (is already of by default)
//  pythia.readString("MultipartonInteractions:Kfactor   (default = 1.0; minimum = 0.5; maximum = 4.0) = off"); // Multiply all cross sections by this fix factor.

  // b quarks do not decays
//  pythia.readString("5:mayDecay = no");
//  pythia.readString("-5:mayDecay = no");

  // gammas do not decays
  pythia.readString("22:mayDecay = no");
  
  // Initialization
  pythia.init();      

	// Create an LHAup object that can access relevant information in pythia.
	LHAupFromPYTHIA8 myLHA(&pythia.process, &pythia.info);
	// Open a file on which LHEF events should be stored, and write header.
	myLHA.openLHEF("test.lhef");
	// Store initialization info in the LHAup object.
	myLHA.setInit();
	// Write out this initialization info on the file.
	myLHA.initLHEF();
	

  // Begin event loop; generate until none left in input file.     
  for (int iEvent = 0; iEvent < 1000; ++iEvent) {

    cout<<"\n ievent = "<<iEvent<<"\n"<<endl;

    // Generate events, and check whether generation failed.
    if (!pythia.next()) {

      // If failure because reached end of file then exit event loop.
      if (pythia.info.atEndOfFile()) break; 

      // First few failures write off as "acceptable" errors, then quit.
      if (++iAbort < nAbort) continue;
      break;
    }

    // Construct new empty HepMC event and fill it.
    // Units will be as chosen for HepMC build; but can be changed
    // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)  
//     HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
//     ToHepMC.fill_next_event( pythia, hepmcevt );
//     delete hepmcevt;
    
    // Acess event record
    cout<<"Number of particles = "<<pythia.event.size()<<endl;
    
    vector<int> pID;
    vector<double> px;
    vector<double> py;
    vector<double> pz;
    vector<double> E;
    vector<int> mother;
    vector<int> code;

    // Some checks on the event record
    // Check for example that at least we have two bs and two bbars

    for (int i = 0; i < pythia.event.size(); i++){
      int particle_id = pythia.event[i].id();
      int particle_status = pythia.event[i].status();
      int particle_mother = pythia.event[i].mother1();
      // save only final state particles
      if(particle_status>0){
//	cout<<i<<" "<<particle_id<<" "<<particle_mother<<endl;
	double ppx= pythia.event[i].px();
	double ppy= pythia.event[i].py();
	double ppz= pythia.event[i].pz();
	double EE= pythia.event[i].e();
	//cout<<px<<" "<<py<<" "<<pz<<" "<<E<<endl;
	pID.push_back(particle_id);
	px.push_back(ppx);
	py.push_back(ppy);
	pz.push_back(ppz);
	E.push_back(EE);
	mother.push_back(particle_mother);
	code.push_back(particle_id);
      }
    } 

    // Save into file
    out_pythia<<"#"<<endl;
    cout<<"Number of final state particles = "<<E.size()<<"\n"<<endl;
    out_pythia<<E.size()<<endl;

    /*
    for(unsigned i=0;i<E.size();i++){
      // First of all write the b quarks
      if(code.at(i)==5 && code.at(i+1)==-5 && mother.at(i)==mother.at(i+1)  ){
	out_pythia<<px.at(i)<<" "<<py.at(i)<<" "<<pz.at(i)<<" "<<E.at(i)<<" "<<endl;
	out_pythia<<px.at(i+1)<<" "<<py.at(i+1)<<" "<<pz.at(i+1)<<" "<<E.at(i+1)<<" "<<endl;
      }
    }
    // Now all the other particles
    for(unsigned i=0;i<E.size();i++){
      // First of all write the b quarks
      if(fabs(code.at(i))<5 || code.at(i)==21){
	out_pythia<<px.at(i)<<" "<<py.at(i)<<" "<<pz.at(i)<<" "<<E.at(i)<<" "<<endl;
      }
      }
    */
     for(unsigned i=0;i<E.size();i++){
       out_pythia<<pID.at(i)<<" "<<px.at(i)<<" "<<py.at(i)<<" "<<pz.at(i)<<" "<<E.at(i)<<" "<<endl;
     }
  // Store event info in the LHAup object.
	myLHA.setEvent();
	// Write out this event info on the file.
	// With optional argument (verbose =) false the file is smaller.
	myLHA.eventLHEF();

  // End of event loop.        
  }                           

  out_pythia.close();

  // Give statistics. Print histogram.
  pythia.stat();

	// Update the cross section info based on Monte Carlo integration during run.
	myLHA.updateSigma();
	// Write endtag. Overwrite initialization info with new cross sections.
	myLHA.closeLHEF(true);

  //delete pythia;

 // } //for unsigned

  // Done.                           
  return 0;
}
