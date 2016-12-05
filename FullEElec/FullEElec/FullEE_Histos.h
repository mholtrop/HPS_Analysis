//
//  FullEE_Histos.hpp
//  FullEElec
//
//  Created by Maurik Holtrop on 7/22/16.
//  Copyright © 2016 Maurik. All rights reserved.
//

#ifndef FullEE_Histos_hpp
#define FullEE_Histos_hpp

#include <iostream>

using namespace std;

#include <TROOT.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TDirectory.h>
#include <TList.h>

#include "BaseAna.h"

class FullEE_Histos : public TObject {

public:
  TString name;
  BaseAna *mother;

  TH1F *cluster_energy;
  TH2F *cluster_loc;
  TH1F *track_chi2;
  TH2F *cluster_track_match_dxdy;
  TH1F *cluster_track_match_dr;
  TH1F *cluster_track_match_de;
  
  // Computed values that may be useful elsewhere to speed up processing.
  double clus_energy;
  double trk_energy;
  double delta_energy;
  
  double trk_chi2;
  
  vector<double> clus_pos;
  vector<double> trk_pos;
  
  double delta_x;
  double delta_y;
  double delta_r;
  
public:
  
  FullEE_Histos(BaseAna *mother,const char *name="" );
  ~FullEE_Histos(){};
  void Book(void);
  bool Copy(FullEE_Histos *data);
  bool Fill(HpsParticle *part);
  bool FillHistos(FullEE_Histos *data);
  
  ClassDef(FullEE_Histos,0);
};

#endif /* FullEE_Histos_hpp */
