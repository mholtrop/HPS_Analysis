//
//  main.cpp
//  Test_BaseAna
//
//  Created by Maurik Holtrop on 2/9/19.
//  Copyright © 2019 Maurik. All rights reserved.
//

#include <iostream>
#include "BaseAna.h"

int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "Test BaseAna starting with file: /data/HPS/data/physrun2016/dst/v0_hps_007800_0.root\n";
  TFile f("/data/HPS/data/physrun2016/dst/v0_hps_007800_0.root");
  TTree *t = (TTree *)f.Get("HPS_Event");
  BaseAna *b=new BaseAna(t);
  b->SetBField(0.5234000000000001);// 2016 data value.
  b->Next(); b->Next(); b->Next();
  b->Print("all");
  b->Next();
  b->Print("all");
  return 0;
}
