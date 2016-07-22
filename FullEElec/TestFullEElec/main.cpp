//
//  main.cpp
//  TestFullEElec
//
//  Created by Maurik Holtrop on 7/11/16.
//  Copyright © 2016 Maurik. All rights reserved.
//

#include <iostream>
#include "FullEElec.h"

int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "TestFullEElec Code start...\n";
  
  TChain *ch = new TChain("HPS_Event");
  ch->Add("/data/HPS/engrun2015/pass6/dst/hps_*.root");
  cout << "Number of events = " << ch->GetEntries() << endl;
  
  FullEElec *pz=new FullEElec(ch);
  pz->SlaveBegin((TTree *)pz);
  pz->Run();
  
  return 0;
}
