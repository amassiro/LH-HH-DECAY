// main11.cc is a part of the PYTHIA event generator.
// Copyright (C) 2012 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. 
// It illustrates how Les Houches Event File input can be used in Pythia8.
// It uses the ttsample.lhe input file, the latter only with 100 events.


#include <fstream>
#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC.h"
#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"

int main() {
 
 // path to LHE folder
 //unsigned const nfiles=4;
 std::string path= "/tmp/amassiro/";
 
 
 std::string namefile_in=path + "";
 //  namefile_in += "GluGluToHHTo2B2Tau_M-125_8TeV_madgraph_v2.lhe";
 //  namefile_in += "GluGluToHHTo2B2WToLNuLNu_M-125_14TeV_madgraph_v2.lhe";
 
 namefile_in = "/tmp/amassiro/atEightTeV_events_patched.lhe";
 
 std::string namefile_out;
 namefile_out = "/tmp/amassiro/test-MR410_out.lhe.hepmc";
 
 std::cout<<"\n namefile_in = "<<namefile_in<<std::endl;
 std::cout<<"\n namefile_out = "<<namefile_out<<std::endl;
 
 
 // output file
 // we want to store the list of all final state particles
 std::ofstream out_pythia;
 // Highest precision required for jet clustering
 out_pythia.precision(15);
 
 // Generator. We here stick with default values, but changes
 // could be inserted with readString or readFile.          
 Pythia8::Pythia pythia;                            
 
 // Initialize Les Houches Event File run. List initialization information.
 pythia.readString("Beams:frameType = 4"); 
 // the analysis program
 std::string sfile = "Beams:LHEF ="+namefile_in;
 pythia.readString(sfile.c_str()); 
 
 // Interface for conversion from Pythia8::Event to HepMC event. 
 HepMC::Pythia8ToHepMC ToHepMC;
 
 // Specify file where HepMC events will be stored.
 HepMC::IO_GenEvent ascii_io(namefile_out.c_str(), std::ios::out);
 
 // Allow for possibility of a few faulty events.
 int nAbort = 2;
 int iAbort = 0;
 
 pythia.settings.listAll();
 pythia.particleData.listAll();
 
 // Settings
 
//  pythia.readString("SUSY:all = on"); // Load SUSY  --> don't do it

 pythia.readString("HadronLevel:all = on"); // On hadronization
 
//  pythia.readString("25:m0 = 125");
//  pythia.readString("25:onMode = off");
// //  pythia.readString("25:onIfMatch = 5 -5"); //bb
//  pythia.readString("25:onIfMatch = 24 -24"); //WW
//  
//  pythia.readString("35:m0 = 125");
//  pythia.readString("35:onMode = off");
//  pythia.readString("35:onIfMatch = 5 -5"); //bb
// //  pythia.readString("35:onIfMatch = 24 -24"); //WW
//  
//  pythia.readString("24:onMode = off");
//  pythia.readString("24:onIfMatch = -11 12"); //W>ev
//  pythia.readString("24:onIfMatch = -13 14"); //W>mv
//  pythia.readString("24:onIfMatch = -15 16"); //W>tv
//  
//  pythia.readString("-24:onMode = off");
//  pythia.readString("-24:onIfMatch = 11 -12"); //W>ev
//  pythia.readString("-24:onIfMatch = 13 -14"); //W>mv
//  pythia.readString("-24:onIfMatch = 15 -16"); //W>tv
 
 
 
//  // turn decays
//  pythia.readString("HadronLevel:all = on"); // generic hadronization settings
//  // hadronic Higgs decay
//  pythia.readString("25:onMode = off");
//  pythia.readString("25:onIfMatch = 5 -5"); // b
//  pythia.readString("25:onIfMatch = 4 -4"); // c
//  pythia.readString("25:onIfMatch = 21 21"); // gluon
//  // WW "higgs" decay
//  pythia.readString("35:onMode = off");
//  pythia.readString("35:onIfMatch = 24 -24"); // W
//  // W decay
//  pythia.readString("24:onMode = off");
//  pythia.readString("24:onIfMatch = 12 11"); // e ve
//  pythia.readString("24:onIfMatch = 14 13"); // mu numu
//  
//  pythia.readString("-24:onMode = off");
//  pythia.readString("-24:onIfMatch = 12 -11"); // e ve
//  pythia.readString("-24:onIfMatch = 14 -13"); // mu numu
 
 
 // Initialization
 pythia.init();      
 
 // Create an LHAup object that can access relevant information in pythia.
 Pythia8::LHAupFromPYTHIA8 myLHA(&pythia.process, &pythia.info);
 // Open a file on which LHEF events should be stored, and write header.
 
 // Begin event loop; generate until none left in input file.     
//  for (int iEvent = 0; iEvent < 200; ++iEvent) {
 for (int iEvent = 0; iEvent<1000; ++iEvent) {
   
  if (!(iEvent%100)) std::cout<<" ievent = " << iEvent << std::endl;
  
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
  HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
  ToHepMC.fill_next_event( pythia, hepmcevt );
  
  // Write the HepMC event to file. Done with it.
  ascii_io << hepmcevt;
  
  delete hepmcevt;
  
  
  /*   DEBUG
  
//   std::cout << "Number of particles = " << pythia.event.size() << std::endl;
  
  std::vector<int> pID;
  std::vector<double> px;
  std::vector<double> py;
  std::vector<double> pz;
  std::vector<double> E;
  std::vector<int> mother;
  std::vector<int> code;
  
  // Some checks on the event record
  // Check for example that at least we have two bs and two bbars
  
  for (int i = 0; i < pythia.event.size(); i++){
   int particle_id = pythia.event[i].id();
   int particle_status = pythia.event[i].status();
   int particle_mother = pythia.event[i].mother1();
//    if (abs(particle_id) == 24 ) std::cout << " [" << i << ":" << pythia.event.size() << " particle_status = " << particle_status << " id = " << particle_id << std::endl;
//    if (abs(particle_id) == 5  ) std::cout << " [" << i << ":" << pythia.event.size() << " particle_status = " << particle_status << " id = " << particle_id << std::endl;
   // save only final state particles
   if(particle_status>0){
    //  cout<<i<<" "<<particle_id<<" "<<particle_mother<<endl;
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
  
  for(unsigned i=0;i<E.size();i++){
   // First of all write the W
   if(code.at(i)==24){
//     std::cout << " " << px.at(i) << " " << py.at(i) << " " << pz.at(i) << " " << E.at(i) << " " << std::endl;
//     std::cout << px.at(i+1) << " " << py.at(i+1) << " " << pz.at(i+1) << " " << E.at(i+1) << " " << endl;
   }
  }
  
  //   for(unsigned i=0;i<E.size();i++){
  //    // First of all write the b quarks
  //    if(code.at(i)==5 && code.at(i+1)==-5 && mother.at(i)==mother.at(i+1)  ){
  //     std::cout << " " << px.at(i) << " " << py.at(i) << " " << pz.at(i) << " " << E.at(i) << " " << std::endl;
  //     std::cout << px.at(i+1) << " " << py.at(i+1) << " " << pz.at(i+1) << " " << E.at(i+1) << " " << endl;
  //    }
  //   }
  //   // Now all the other particles
  //   for(unsigned i=0;i<E.size();i++){
  //    // First of all write the b quarks
  //    if(fabs(code.at(i))<5 || code.at(i)==21){
  //     std::cout << px.at(i) << " " << py.at(i) << " " << pz.at(i) << " " << E.at(i) << " " << std::endl;
  //    }
  //   }
  
  */
  
  
  // End of event loop.        
 }                           
 
 // Give statistics. Print histogram.
 pythia.stat();
 
 //delete pythia;
 
 return 0;
}
