// main11.cc is a part of the PYTHIA event generator.
// Copyright (C) 2012 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program. 
// It illustrates how Les Houches Event File input can be used in Pythia8.
// It uses the ttsample.lhe input file, the latter only with 100 events.


#include <fstream>
// #include "Pythia8/Pythia.h"
// #include "Pythia8/Pythia8ToHepMC.h"

#include "Pythia.h"
#include "HepMCInterface.h"

#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"

int main(int argc, char **argv) {

 if (argc < 3) {
  std::cerr << " Not enough input information!" << std::endl;
  exit (0);
 }

 std::string namefile_in;
 namefile_in = argv[1];

 std::string namefile_out;
 namefile_out = argv[2];

 std::cout << " namefile_in  = " << namefile_in  << std::endl;
 std::cout << " namefile_out = " << namefile_out << std::endl;

 int energy = 13;
 if (argc >= 4) energy = atoi(argv[3]);

 int startEntry = 0;
 if (argc >= 5) startEntry = atoi(argv[4]);
 int endEntry = -1;
 if (argc >= 6) endEntry   = atoi(argv[5]);

 std::cout << " energy       = " << energy << std::endl;
 std::cout << " startEntry   = " << startEntry << std::endl;
 std::cout << " endEntry     = " << endEntry << std::endl;

 // output file
 // we want to store the list of all final state particles
 std::ofstream out_pythia;
 // Highest precision required for jet clustering
 out_pythia.precision(15);

 // Generator. We here stick with default values, but changes
 // could be inserted with readString or readFile.
 Pythia8::Pythia pythia;

 //  http://home.thep.lu.se/~torbjorn/pythia81html/BeamParameters.html
//  pythia.readString("Beams:frameType = 1");
//  if (energy == 8)  pythia.readString("Beams:eCM = 8000");   // 8 TeV
//  if (energy == 13) pythia.readString("Beams:eCM = 13000"); // 13 TeV
//  if (energy == 14) pythia.readString("Beams:eCM = 14000");   // 14 TeV

 // Initialize Les Houches Event File run. List initialization information.
 // the analysis program
 pythia.readString("Beams:frameType = 4");
 std::string sfile = "Beams:LHEF ="+namefile_in;
 pythia.readString(sfile.c_str()); 


 // Interface for conversion from Pythia8::Event to HepMC event. 
 HepMC::I_Pythia8 ToHepMC;
//  HepMC::Pythia8ToHepMC ToHepMC;

 // Specify file where HepMC events will be stored.
 HepMC::IO_GenEvent ascii_io(namefile_out.c_str(), std::ios::out);

 // Allow for possibility of a few faulty events.
 int nAbort = 10;
 int iAbort = 0;

//  pythia.settings.listAll();
//  pythia.particleData.listAll();

 // Settings
//  pythia.readString("ProcessLevel:all = off"); // Off event generation -> need on for MPI
 pythia.readString("HadronLevel:all  = on"); // On hadronization


 // Initialization
 pythia.init();

 // Create an LHAup object that can access relevant information in pythia.
 Pythia8::LHAupFromPYTHIA8 myLHA(&pythia.process, &pythia.info);
 // Open a file on which LHEF events should be stored, and write header.

 // Begin event loop; generate until none left in input file.
//  for (int iEvent = 0; iEvent < 200; ++iEvent) {

 for (int iEvent = 0; ; ++iEvent) {

  if (!(iEvent%500)) std::cout<<" ievent = " << iEvent << std::endl;

  // Generate events, and check whether generation failed.
  if (!pythia.next()) {

   // If failure because reached end of file then exit event loop.
   if (pythia.info.atEndOfFile()) break; 

   // First few failures write off as "acceptable" errors, then quit.
   if (++iAbort < nAbort) continue;
   break;
  }

  //---- subrange of events ----
  if (iEvent < startEntry) continue;
  if (endEntry!=-1 && iEvent>=endEntry) break;

  // Construct new empty HepMC event and fill it.
  // Units will be as chosen for HepMC build; but can be changed
  // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)  
//   HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
  HepMC::GenEvent* hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
  ToHepMC.fill_next_event( pythia, hepmcevt );

  // Write the HepMC event to file. Done with it.
  ascii_io << hepmcevt;

  delete hepmcevt;
  // End of event loop.
 }

 // Give statistics. Print histogram.
//  pythia.stat();

 //delete pythia;

 return 0;
}


