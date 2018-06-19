/*\class SimAna
 *\author Maurik Holtrop
 *\brief  This class is a template to create new BaseAna derived classes.
 *\date   2018-06-18
 */
#include "SimAna.h"

ClassImp(SimAna);

SimAna::SimAna(){

}

SimAna::SimAna(string lcio_file_name, bool direct_access){
  Init(lcio_file_name,direct_access);
}


// Standard destructor.
SimAna::~SimAna(){
  // Do any cleanup.
}

void SimAna::Init(const string lcio_file_name, bool direct_access){
// Initialize with file for reading, and setup histograms.
  if(direct_access==true){
    lc_reader=LCFactory::getInstance()->createLCReader(LCReader::directAccess);
  }else{
    lc_reader=LCFactory::getInstance()->createLCReader();
  }
  OpenLCIO(lcio_file_name);
}

bool SimAna::GetEvent(int evtnum){
  // Read event evtnum from file. This assumes the file was opened for DirectAccess.
  // The run used will the current run, or the first run in the file.
  int thisrun;
  if(evt==nullptr){
    EVENT::IntVec runs;
    lc_reader->getRuns(runs);
    thisrun=runs.at(0);
  }else{
    thisrun=evt->getRunNumber();
  }
  
  evt = lc_reader->readEvent(thisrun,evtnum);
  if(evt == nullptr){
    return false;
  }else{
    return true;
  }
}

bool SimAna::GetEvent(int runnum,int evtnum){
  // Read event evtnum from file. This assumes the file was opened for DirectAccess.
  // The run used will the current run, or the first run in the file.
  evt = lc_reader->readEvent(runnum,evtnum);
  if(evt == nullptr){
    return false;
  }else{
    return true;
  }
}


int SimAna::GetRunNumber(void){
  // Return the current run number
  if(evt) return(evt->getRunNumber());
  else    return(-1);
}

int SimAna::GetEventNumber(void){
  // Return the current event number
  if(evt) return(evt->getEventNumber());
  else return(-1);
}
bool SimAna::Next(void){
// Read the next event from the LCIO file.
  evt = lc_reader->readNextEvent();
  if(evt == nullptr){
    return false;
  }else{
    return true;
  }
}

void SimAna::OpenLCIO(const string file){
// Open an LCIO file for processing
  lc_reader->open(file);
}

void SimAna::Print(int mode){
// Print information about the file and/or the event

  if(evt == nullptr) return;
  LCTOOLS::dumpEvent(evt);
  
}
