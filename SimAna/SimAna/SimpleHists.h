//
//  SimpleHists.hpp
//  SimAna
//
//  Created by Maurik Holtrop on 5/27/19.
//  Copyright © 2019 Maurik. All rights reserved.
//

#ifndef SimpleHists_h
#define SimpleHists_h

#include "SimAna.h"

using namespace std ;
using namespace lcio ;

class SimpleHists: public SimAna {
  // Simple example extension of SimAna to make some basic plots
  //
public:
  // Histograms -- set each to NULL in Init and define in SlaveBegin.
  // Note: These nullptr are needed so that the class can be read in from file.

  TH1F *N_EcalHits=nullptr;
  TH1F *N_EcalHits_E=nullptr;
  TH1F *N_MCParticle=nullptr;
  TH1F *E_MCParticle=nullptr;
  TH1F *E_MCParticle2=nullptr;
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
  
  // Hodoscope
  TH1F *N_HodoHits = nullptr;
  TH1F *Hodo_Hit_Energy = nullptr;
  
  // Tracker Info
  TH1F *Tracker_Hit_Energy=nullptr;
  TH1F *Tracker_Hit_Px=nullptr;
  TH1F *Tracker_Hit_Py=nullptr;
  TH1F *Tracker_Hit_Pz=nullptr;
  TH1F *Tracker_Hit_PathLength=nullptr;
  
public:
  SimpleHists(void){};
  SimpleHists(const string &lcio_file_name,bool direct_access=false,double max_E=4.7);
    
  virtual void InitHists(double max_E);
  virtual void FillHistos(void);
  ClassDef(SimpleHists,1);
};

#endif /* SimpleHists_hpp */
