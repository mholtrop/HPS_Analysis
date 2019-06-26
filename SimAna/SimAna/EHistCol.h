//
//  ElectronHists.h
//  SimAna
//
//  Created by Maurik Holtrop on 5/27/19.
// 
//

#ifndef EHistCol_h
#define EHistCol_h

#include "SimAna.h"

class EHistCol: public TObject {
  
public:
  double threshold;
  SimAna *sa;
  
  vector<TH1 *> hists;
  
  TH1F *N_TrackerHits=nullptr;
  
  TH1F *N_MCParticle = nullptr;
  TH1F *E_PrimaryElec=nullptr;
  TH2F *Loc_PrimaryElec=nullptr;
  TH2F *ThPh_PrimaryElec=nullptr;
  
  // Ecal Info
  TH1F *N_EcalHits = nullptr;
  TH1F *Ecal_Hit_Energy=nullptr;
  TH2F *Ecal_Hits_Loc=nullptr;
  TH1F *Ecal_Seed_Energy=nullptr;
  TH2F *Ecal_Seed_Loc= nullptr;
  TH2F *Ecal_Seed_Locx_vs_Primx=nullptr;
  TH2F *Ecal_Seed_Locy_vs_Primy=nullptr;
  TH1F *Ecal_GTP_Energy = nullptr;
  TH1F *Ecal_Total_Energy=nullptr;
  
  TH2F *E_Seed_E_Ecal= nullptr;
  TH2F *E_Prim_E_Ecal= nullptr;
  TH2F *E_Prim_E_Seed= nullptr;
  
  
  // Hodoscope
  TH1F *N_HodoHits = nullptr;
  TH1F *Hodo_Hit_Energy = nullptr;
  TH1F *Hodo_Hit_CNT = nullptr;

  
public:
  EHistCol(){};
  EHistCol(SimAna *sa,double thresh=0.);
  virtual void InitHists(double max_E);
  virtual void FillHistos(void);
  virtual void Histadd(TH1F *);
  virtual void Histadd(TH2F *);
  void Print(void);
  
  ClassDef(EHistCol,1);
};

#endif 
