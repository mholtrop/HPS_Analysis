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
#include "lcio.h"

//#include "IO/LCReader.h"
#include "EVENT/LCCollection.h"
#include "IMPL/LCTOOLS.h"
//#include "EVENT/CalorimeterHit.h"
#include "IMPL/SimCalorimeterHitImpl.h"
//#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"

using namespace std ;
using namespace lcio ;

class SimAna : public TNamed {

public:
  // Create a reader. If you need to jump around in the file add as argument: LCReader::directAccess
  LCReader *lc_reader = nullptr; //! LCIO reader handle. NOT streamed by ROOT.
  LCEvent  *evt = nullptr;       //! LCIO event handle, NOT streamed by ROOT.

  int fCounter_Freq=10000;
  // Histograms -- set each to NULL in Init and define in SlaveBegin.
  // Note: These nullptr are needed so that the class can be read in from file.
  
  TH1F *N_EcalHits=nullptr;
  TH1F *N_EcalHits_E=nullptr;
  TH1F *N_MCParticle=nullptr;
  TH1F *N_TrackerHits=nullptr;
  TH1F *N_TrackerHitsEcal=nullptr;
  
  // Ecal Info
  TH1F *Ecal_Hit_Energy=nullptr;
  TH1F *Ecal_Hit_CE=nullptr;
  TH1F *Ecal_Hit_Energy_sum=nullptr;
  TH2F *Ecal_Hits_Loc=nullptr;
  TH2F *Ecal_Hits_Loc_E=nullptr;
  TH1F *Ecal_Hit_CNT  = nullptr;
  TH1F *Ecal_Hit_Time = nullptr;
  TH1F *Ecal_Hit_TimeAve= nullptr;
  TH1F *Ecal_Hit_TimeRel= nullptr;

  TH1F *Ecal_Hit_CNT_ecut  = nullptr;
  TH1F *Ecal_Hit_Time_ecut = nullptr;
  TH1F *Ecal_Hit_TimeAve_ecut= nullptr;
  TH1F *Ecal_Hit_TimeRel_ecut= nullptr;

  
  
  // Tracker Info
  TH1F *Tracker_Hit_Energy=nullptr;
  TH1F *Tracker_Hit_Px=nullptr;
  TH1F *Tracker_Hit_Py=nullptr;
  TH1F *Tracker_Hit_Pz=nullptr;
  TH1F *Tracker_Hit_PathLength=nullptr;
  

public:
  SimAna(void);
  SimAna(string lcio_file_name,bool direct_access=false);
  SimAna(string lcio_file_name,const char *name,bool direct_access=false);
  virtual ~SimAna();
  // ===== Initializations =====
  void Init(const string file, bool direct_access=false);
  void OpenLCIO(const string file);

  // ====  Event Processing ====
  void  FillHistos(void);
  bool  Next(void);
  int   Run(int nevt=0);
  bool  ShowNext(int opt=0);
  
  // ==== Information  =====
  int   GetNEvents(){
    return(lc_reader->getNumberOfEvents());
  }
  bool  GetEvent(int evtnum=0);
  bool  GetEvent(int runnum,int evtnum);
  int   GetRunNumber(void);
  int   GetEventNumber(void);

  int  getN(string name);
  // For ease of use...
  int  getNEcalHits(void){
    return( getN("EcalHits"));
  };
  int  getNMCParticle(void){
    return( getN("MCParticle"));
  };
  int  getNTrackerHits(void){
    return( getN("TrackerHits"));
  };
  int  getNTrackerHitsEcal(void){
    return( getN("TrackerHitsEcal"));
  };
  
  // ==== ECAL =====
  //EVENT::CalorimeterHit *GetEcalHit(int n,LCCollection *col=nullptr){
  IMPL::SimCalorimeterHitImpl *GetEcalHit(int n,LCCollection *col=nullptr){
    // Get the N-th ecal hit from the collection.
    if(col==nullptr) col=static_cast<LCCollection *>(evt->getCollection("EcalHits"));
    return((IMPL::SimCalorimeterHitImpl *)col->getElementAt(n));
  }

  //EVENT::CalorimeterHit *Cast_to_CalorimeterHit(EVENT::SimCalorimeterHit *hit){
  IMPL::SimCalorimeterHitImpl *Cast_to_CalorimeterHit(EVENT::SimCalorimeterHit *hit){
    LCObject *oo = (LCObject *)hit;
   // return( (EVENT::CalorimeterHit *)oo);
    return( (IMPL::SimCalorimeterHitImpl *) oo);
  }
  
  int GetEcalHit_ix(IMPL::SimCalorimeterHitImpl *calorimeter_hit){
    uint64_t id0 = (((uint64_t)calorimeter_hit->getCellID1()<<32)) + calorimeter_hit->getCellID0();
    // ECAL hit bit field: "system:6,layer:2,ix:-8,iy:-6"
    int ix= (int)((id0& 0x00FF00LL)>> 8);
    return( (ix&0x80?ix-0x100:ix )); // Use the high bit as sign. If set, then ix = ix - (iy_max+1)
  }
  int GetEcalHit_iy(IMPL::SimCalorimeterHitImpl *calorimeter_hit){
    uint64_t id0 = (((uint64_t)calorimeter_hit->getCellID1()<<32)) + calorimeter_hit->getCellID0();
    // ECAL hit bit field: "system:6,layer:2,ix:-8,iy:-6"
    int iy= (int)((id0& 0x3F0000LL)>> 16);
    return( (iy&0x20?iy-0x40:iy) ); // Use the high bit as sign. If set, then iy = iy - (iy_max+1)
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
  TH2F *EcalHitMap(void);
  void FancyPlot(TH2F *histo,int opt);
  
  virtual Int_t Version() const { return 1; }
  ClassDef(SimAna,1);
};

#endif
