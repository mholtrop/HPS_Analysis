//
//  TriggerStudy.cpp
//  SimAna
//
//  Created by Maurik Holtrop on 6/12/19.
// 
//

#include "TriggerStudy.h"

ClassImp(TriggerStudy);

TriggerStudy::TriggerStudy(string lcio_file_name,bool direct_access, double max_E): SimAna(lcio_file_name,direct_access,max_E){
  SetName("TriggerStudy");
  SetTitle("LCIO analyzer for Trigger Studies");
  InitHists(max_E);
}

void TriggerStudy::InitHists(double Max_E){
  //Initialize histogram collections.
  Max_Energy=Max_E;
  auto dir=gDirectory->mkdir("Uncut");
  dir->cd();
  hist_store.push_back(new TrigHistCol(this,0.));
  dir=gDirectory->mkdir("CutSeed100");
  dir->cd();
  hist_store.push_back(new TrigHistCol(this,0.1));
  dir=gDirectory->mkdir("CutSeed200");
  dir->cd();
  hist_store.push_back(new TrigHistCol(this,0.2));
}

void TriggerStudy::FillHistos(){
  for(TrigHistCol *hists : hist_store){
    hists->FillHistos();
  }
}
