//
//  main.cpp
//  TestSimAna
//
//  Created by Maurik Holtrop on 7/11/16.
//  Copyright © 2016 Maurik. All rights reserved.
//

#include <iostream>
#include "TFile.h"
#include "SimAna.h"

int main(int argc, const char * argv[]) {
  // insert code here...
  string file;
  if(argc<2){
    file="/data/HPS/SimCompare/slic_electrons.slcio";
  }else{
    file=argv[1];
  }
// We do not yet have <filesystem> available. !!
  string file_out = file;
  int loc= file_out.find(".slcio");
  if(loc>1){
    file_out.replace(loc,file_out.size()-loc,".root");
  }else{
    file_out.append(".root");
  }
  std::cout << "SimAna Code start, analyzing: " << file << endl;
  std::cout << "Output histograms to " << file_out << endl;
  TFile *f=new TFile(file_out.c_str(),"RECREATE");
  SimAna *sa=new SimAna(file);
  sa->fCounter_Freq=1000;
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
