/*
 *  FullEElec.hpp
 *  FullEElec
 *
 *  Created by Maurik Holtrop on 5/20/16.
 *  Copyright © 2016 Maurik. All rights reserved.
 *
 */

// TODO:
//       * Create copy constuctor?
//

#ifndef FullEElec_h
#define FullEElec_h

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
#include "FEESelector.h"

class FullEElec : public BaseAna {
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
  TH1F *electron_count;
  TH1F *positron_count;

  TH1F *cluster_energy;
  TH1F *cluster_e_energy;
  TH1F *cluster_e_matched_energy;
  TH1F *cluster_fee_energy;
  TH1F *cluster_fee_matched_energy;
  
  TH2F *cluster_loc;

  TH2F *cluster_e_loc;
  TH2F *cluster_e_matched_loc;
  
  TH2F *cluster_fee_loc;
  TH2F *cluster_fee_matched_loc;
  
  TH2F *cluster_track_match_xy;
  TH1F *cluster_track_match_dr;
  TH1F *cluster_track_match_de;

  TH2F *cluster_e_track_match_xy;
  TH1F *cluster_e_track_match_dr;
  TH1F *cluster_e_track_match_de;

  TH2F *cluster_e_matched_track_match_xy;
  TH1F *cluster_e_matched_track_match_dr;
  TH1F *cluster_e_matched_track_match_de;
  
  TH2F *cluster_fee_track_match_xy;
  TH1F *cluster_fee_track_match_dr;
  TH1F *cluster_fee_track_match_de;

  TH2F *cluster_fee_matched_track_match_xy;
  TH1F *cluster_fee_matched_track_match_dr;
  TH1F *cluster_fee_matched_track_match_de;

  TH1F *photon_count;
  TH1F *photon_energy;
  TH2F *photon_thetaphi;
  TH2F *photon_xy;
  
  FEESelector *FEE;
  
  
public:
  FullEElec(TTree *tree=NULL);
  virtual ~FullEElec();
  virtual Int_t Version() const { return 1; }

  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
// virtual void    Init(TTree *tree); // No need to override.
  virtual Bool_t  Process(Long64_t entry);
//  virtual void    SlaveTerminate(); // No need to override.
//  virtual void    Terminate();      // No need to override.
  
//  virtual void    Print(Option_t *option="");
  
  ClassDef(FullEElec,0);
};

#endif
