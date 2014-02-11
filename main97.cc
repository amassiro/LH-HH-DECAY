// ----------------
// HH decay in AABB
// ----------------


#include <fstream>
#include "Pythia.h"
#include "HepMCInterface.h"

#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"

int main(int argc, char **argv) {

 if (argc < 4) {
  std::cerr << " Not enough input information!" << std::endl;
  std::cerr << "   -  input_file   output_file   decay_file " << std::endl;
  exit (0);
 }

 std::string namefile_in;
 namefile_in = argv[1];

 std::string namefile_out;
 namefile_out = argv[2];

 std::string namefile_decay;
 namefile_decay = argv[3];

 std::cout << " namefile_in    = " << namefile_in    << std::endl;
 std::cout << " namefile_out   = " << namefile_out   << std::endl;
 std::cout << " namefile_decay = " << namefile_decay << std::endl;

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
//  HepMC::I_Pythia8 ToHepMC;

 // Specify file where HepMC events will be stored.
//  HepMC::IO_GenEvent ascii_io(namefile_out.c_str(), std::ios::out);

 // Allow for possibility of a few faulty events.
 int nAbort = 2;
 int iAbort = 0;

//  pythia.settings.listAll();
//  pythia.particleData.listAll();

 // Settings

 pythia.readString("HadronLevel:all = on"); // On hadronization

 // read decay table
 pythia.readString("SLHA:readFrom = 2");

 std::string pythiaInstructionString = "SLHA:file = "+namefile_decay;
 pythia.readString( pythiaInstructionString.c_str()); // input the decay table
 // allow overwrite: only works for products of SM - like decays
 pythia.readString("SLHA:allowUserOverride = on ");

 pythia.readString("25:m0 = 125");
 pythia.readString("35:m0 = 125");

 pythia.readString("24:onMode = off");
 pythia.readString("-24:onMode = off");
 pythia.readString("24:onIfMatch = 12 11"); // e ve
 pythia.readString("24:onIfMatch = 14 13"); // mu numu
 pythia.readString("24:onIfMatch = 15 16"); // tau nutau
 pythia.readString("-24:onIfMatch = 12 -11"); // e ve
 pythia.readString("-24:onIfMatch = 14 -13"); // mu numu
 pythia.readString("-24:onIfMatch = 15 -16"); // tau nutau

 // Initialization
 pythia.init();

 // Create an LHAup object that can access relevant information in pythia.
 Pythia8::LHAupFromPYTHIA8 myLHA(&pythia.process, &pythia.info);

 // Open a file on which LHEF events should be stored, and write header.
 myLHA.openLHEF(namefile_out.c_str()); 
 // Store initialization info in the LHAup object. 
 myLHA.setInit();
 // Write out this initialization info on the file.
 myLHA.initLHEF();

 // Begin event loop; generate until none left in input file.     
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

  // Construct new empty HepMC event and fill it.
  // Units will be as chosen for HepMC build; but can be changed
  // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)  
  //   HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
//   HepMC::GenEvent* hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
//   ToHepMC.fill_next_event( pythia, hepmcevt );

  // Write the HepMC event to file. Done with it.
//   ascii_io << hepmcevt;

//   delete hepmcevt;

  // Store event info in the LHAup object. 
  myLHA.setEvent();

  // Write out this event info on the file. 
  // With optional argument (verbose =) false the file is smaller.
  myLHA.eventLHEF();

  // End of event loop.
 }

 // Write endtag. Overwrite initialization info with new cross sections.
 myLHA.closeLHEF(true);

 //delete pythia;

 return 0;
}
