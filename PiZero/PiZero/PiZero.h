/*
 *  PiZero.hpp
 *  PiZero
 *
 *  Created by Maurik Holtrop on 5/20/16.
 *  Copyright © 2016 Maurik. All rights reserved.
 *
 */

// TODO:
//       * Create copy constuctor?
//

#ifndef PiZero_h
#define PiZero_h

#include <iostream>
#include <ctime>

using namespace std;

#include <TROOT.h>
#include <TProofServ.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

#include "BaseAna.h"

class PiZero : public BaseAna {
public:
  
  int fCounter_Freq=10000;
  // Histograms -- set each to NULL in Init and define in SlaveBegin.
  
  TH1F *cluster_count;
  TH1F *track_count;
  TH1F *cluster_no_track_count;
  TH1F *particle_count;
  TH1F *particle_count_bsc_v0;
  TH1F *particle_count_tc_v0;
  TH1F *neutrals_count;
  
  TH1F *photon_count;
  TH1F *photon_energy;
  TH2F *photon_thetaphi;
  TH2F *photon_xy;
  
  TH1F *pizero_mass;
  TH1F *pizero_theta;
  
  
  
public:
  PiZero(TTree *tree=NULL);
  virtual ~PiZero();
  virtual Int_t Version() const { return 1; }

  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
// virtual void    Init(TTree *tree); // No need to override.
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SlaveTerminate();
  virtual void    Terminate();
  
//  virtual void    Print(Option_t *option="");
  
  ClassDef(PiZero,0);
};

#endif
