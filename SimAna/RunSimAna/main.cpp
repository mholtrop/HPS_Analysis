//
//  main.cpp
//  TestSimAna
//
//  Created by Maurik Holtrop on 7/11/16.
//  Copyright © 2016 Maurik. All rights reserved.
//

#include <iostream>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#include "TFile.h"
#pragma clang diagnostic pop

#include "SimAna.h"
#include "SimpleHists.h"
#include "ElectronHists.h"

int main(int argc, const char * argv[]) {
  // insert code here...
  string file;
  if(argc<2){
    file="/data/HPS/Analysis/electrons*.slcio";
  }else{
    file=argv[1];
  }
// We do not yet have <filesystem> available. !!
  string file_out = file;
  size_t loc= file_out.find(".slcio");
  if(loc>1){
    file_out.replace(loc,file_out.size()-loc,".root");
  }else{
    file_out.append(".root");
  }
  std::cout << "SimAna Code start, analyzing: " << file << endl;
  std::cout << "Output histograms to " << file_out << endl;
  TFile *f=new TFile(file_out.c_str(),"RECREATE");
  // auto *sa = new SimAna(file);
  // auto *sa = new SimpleHists(file);
  auto *sa=new ElectronHists(file);

  sa->fCounter_Freq=1;
  sa->Run();
  //sa->Write();
  //f->Write();
  //f->Close();
  //sa->Write();
  f->ls();
  f->Write();
  f->Close();
  
  return 0;
}
