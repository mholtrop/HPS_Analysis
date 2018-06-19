/*
 *  SimAna.hpp
 *  SimAna
 *
 *  Created by Maurik Holtrop on 5/20/16.
 *  Copyright © 2016 Maurik. All rights reserved.
 *
 */

// TODO:
//       * Create copy constuctor?
//

#ifndef SimAna_h
#define SimAna_h

#include <iostream>
#include <ctime>

#include <TROOT.h>
#include <TProofServ.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include "lcio.h"

//#include "IO/LCReader.h"
#include "IMPL/LCTOOLS.h"

using namespace std ;
using namespace lcio ;

class SimAna : public TObject {
public:
  // Create a reader. If you need to jump around in the file add as argument: LCReader::directAccess
  LCReader *lc_reader = nullptr;
  LCEvent  *evt = nullptr;

  int fCounter_Freq=1000;
  // Histograms -- set each to NULL in Init and define in SlaveBegin.

  TH1F *foo;

public:
  SimAna();
  SimAna(string lcio_file_name,bool direct_access=false);
  virtual ~SimAna();
  
  void Init(const string file, bool direct_access=false);
  
  bool  GetEvent(int evtnum=0);
  bool  GetEvent(int runnum,int evtnum);
  int   GetRunNumber(void);
  int   GetEventNumber(void);
  bool  Next(void);
  void OpenLCIO(const string file);
  void Print(int mode=0);
  
  virtual Int_t Version() const { return 1; }
  ClassDef(SimAna,1);
};

#endif
