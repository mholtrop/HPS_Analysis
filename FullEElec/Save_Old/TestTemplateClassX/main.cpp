//
//  main.cpp
//  TestTemplateClassX
//
//  Created by Maurik Holtrop on 7/11/16.
//  Copyright © 2016 Maurik. All rights reserved.
//

#include <iostream>
#include "TemplateClassX.h"

int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "TestTemplateClassX Code start...\n";
  
  TChain *ch = new TChain("HPS_Event");
  ch->Add("/data/HPS/engrun2015/pass6/dst/hps_*.root");
  cout << "Number of events = " << ch->GetEntries() << endl;
  
  TemplateClassX *pz=new TemplateClassX(ch);
  pz->SlaveBegin((TTree *)pz);
  pz->Run();
  
  return 0;
}
