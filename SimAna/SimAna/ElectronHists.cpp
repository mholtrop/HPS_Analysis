//
//  ElectronHists.cpp
//  SimAna
//
//  Created by Maurik Holtrop on 5/27/19.
//
//

#include "ElectronHists.h"

ClassImp(ElectronHists);

ElectronHists::ElectronHists(string lcio_file_name,bool direct_access, double max_E): SimAna(lcio_file_name,direct_access,max_E){
  SetName("ElectronHists");
  SetTitle("LCIO output analyzer.");
  InitHists(max_E);
}

void ElectronHists::InitHists(double Max_E){
//Initialize histogram collections.
  Max_Energy=Max_E;
  auto dir=gDirectory->mkdir("Uncut");
  dir->cd();
  hist_store.push_back(new EHistCol(this,0.));
  dir=gDirectory->mkdir("CutSeed100");
  dir->cd();
  hist_store.push_back(new EHistCol(this,0.1));
  dir=gDirectory->mkdir("CutSeed200");
  dir->cd();
  hist_store.push_back(new EHistCol(this,0.2));
}

void ElectronHists::FillHistos(){
  for(EHistCol *hists : hist_store){
    hists->FillHistos();
  }
}
