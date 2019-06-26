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
  if(argc<2)
    ch->Add("/data/HPS/data/MC/physrun2016/moller/tmp.root");
  else
    ch->Add(argv[1]);
  
  cout << "Number of events = " << ch->GetEntries() << endl;
  
  FullEElec *pz=new FullEElec(ch);
  pz->SlaveBegin((TTree *)pz);
  pz->fCounter_Freq=1;
  pz->Run();
  
  return 0;
}
