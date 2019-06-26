/*
 *  SimAna.hpp
 *  SimAna
 *
 *  Created by Maurik Holtrop on 5/20/16.
 *  Copyright © 2016 Maurik. All rights reserved.
 *
 */

// TODO:
//       * Create copy constuctor?
//

#ifndef SimAna_h
#define SimAna_h

#include <iostream>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#include <TROOT.h>
#include <TProofServ.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGaxis.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#pragma clang diagnostic pop

#include "lcio.h"

//#include "IO/LCReader.h"
#include "EVENT/LCCollection.h"
#include "IMPL/LCTOOLS.h"
//#include "EVENT/CalorimeterHit.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/SimCalorimeterHitImpl.h"
//#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"

using namespace std ;
using namespace lcio ;

#define Pi 3.14159265358979323846

class SimAna : public TNamed {

public:
  // Create a reader. If you need to jump around in the file add as argument: LCReader::directAccess
  LCReader *lc_reader = nullptr; //! LCIO reader handle. NOT streamed by ROOT.
  LCEvent  *evt = nullptr;       //! LCIO event handle, NOT streamed by ROOT.

  vector<string> filenames;
  int            current_file=0;
  int fCounter_Freq=1000;

  double Max_Energy=5.0;
  
public:
  SimAna(void){};
  SimAna(const string & lcio_file_name,bool direct_access=false, double max_E=4.7);
  
  virtual ~SimAna();
  // ===== Initializations =====
  int  AddLcioFiles(const string& filename);
  void InitLcio(bool direct_access=false);
 
  // ====  Event Processing ====
  virtual bool  Next(void);
  virtual int   Run(int nevt=0);
  bool  GetEvent(int evtnum=0);
  bool  GetEvent(int runnum,int evtnum);

  // ==== Histograms ====
  void InitHists(double max_E=4.7);
  virtual void  FillHistos(void);
  
  // ==== Information  =====
  int   GetNEvents(){
    // Returns the number of events in the current file.
    return(lc_reader->getNumberOfEvents());
  }
  
  int  CountEvents();
  
  int   GetRunNumber(void);
  int   GetEventNumber(void);

  int  getN(string name);
  // For ease of use...
  int  getNEcalHits(void){
    return( getN("EcalHits"));
  };
  int  getNHodoHits(void){
    return( getN("HodoscopeHits"));
  };
  int  getNMCParticle(void){
    return( getN("MCParticle"));
  };
  int  getNTrackerHits(void){
    return( getN("TrackerHits"));
  };
  int  getNTrackerHitsEcal(void){
    return( getN("TrackerHitsECal"));
  };
  
  // ==== MCParticles ====
IMPL::MCParticleImpl *GetMCParticle(int n,LCCollection *col=nullptr){
  if(col==nullptr) col=static_cast<LCCollection *>(evt->getCollection("MCParticle"));
  return( (IMPL::MCParticleImpl *)col->getElementAt(n));
}
  
  // ==== ECAL =====
  IMPL::SimCalorimeterHitImpl *GetEcalHit(int n,LCCollection *col=nullptr){
    // Get the N-th ecal hit from the collection.
    if(col==nullptr) col=static_cast<LCCollection *>(evt->getCollection("EcalHits"));
    return((IMPL::SimCalorimeterHitImpl *)col->getElementAt(n));
  }

  IMPL::SimCalorimeterHitImpl *Cast_to_CalorimeterHit(EVENT::SimCalorimeterHit *hit){
    LCObject *oo = (LCObject *)hit;
    return( (IMPL::SimCalorimeterHitImpl *) oo);
  }
  
  int GetEcalHit_ix(IMPL::SimCalorimeterHitImpl *calorimeter_hit){
    // Return the crystal x index for the hit.
    //
    uint64_t id0 = (((uint64_t)calorimeter_hit->getCellID1()<<32)) + calorimeter_hit->getCellID0();
    // ECAL hit bit field: "system:6,layer:2,ix:-8,iy:-6"
    int ix= (int)((id0& 0x00FF00LL)>> 8);
    return( (ix&0x80?ix-0x100:ix )); // Use the high bit as sign. If set, then ix = ix - (iy_max+1)
  }
  int GetEcalHit_iy(IMPL::SimCalorimeterHitImpl *calorimeter_hit){
    // Return the crystal y index for the hit.
    //
    uint64_t id0 = (((uint64_t)calorimeter_hit->getCellID1()<<32)) + calorimeter_hit->getCellID0();
    // ECAL hit bit field: "system:6,layer:2,ix:-8,iy:-6"
    int iy= (int)((id0& 0x3F0000LL)>> 16);
    return( (iy&0x20?iy-0x40:iy) ); // Use the high bit as sign. If set, then iy = iy - (iy_max+1)
  }

  
  
  // ==== Hodoscope =====
  EVENT::SimTrackerHit *GetHodoHit(int n,LCCollection *col=nullptr){
    // Get the N-th hodoscope hit from the collection.
    if(col==nullptr) col=static_cast<LCCollection *>(evt->getCollection("HodoscopeHits"));
    return((EVENT::SimTrackerHit *)col->getElementAt(n));
  }
  
  //========= Tracker =========
  
  EVENT::SimTrackerHit *GetSimTrackerHit(int n,LCCollection *col=nullptr){
    // Get the N-th SimTrackerHit
    if(col==nullptr) col=static_cast<LCCollection *>(evt->getCollection("TrackerHits"));
    return((EVENT::SimTrackerHit *)col->getElementAt(n));
  }

  //=======================================================
  
  // For recursively printing the partice tree.
  void PrintParticle(EVENT::MCParticle *part,map<EVENT::MCParticle *,int> MC_ptoi,int level=0);
  // Print information about this event. Mode=0x1 - collection table, 0x2 - detailed full event dump,  0x3 MCParticle tree, 0x4 Hit Tree.
  void Print(int mode=0);
 
  inline int TrXhit(const int x) const{ return (x>=0?x:x+1); };
  
  virtual Int_t Version() const { return 1; }
  ClassDef(SimAna,1);
};

// Convenient Global Utility Functions
// TODO: Split this off into its own header and source files.

TH2F *EcalHitMap(SimAna *sa);
void EcalFancyPlot(TH2F *histo,int opt=0);



#endif
