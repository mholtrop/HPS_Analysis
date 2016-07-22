/*
 *  TemplateClassX.hpp
 *  TemplateClassX
 *
 *  Created by Maurik Holtrop on 5/20/16.
 *  Copyright © 2016 Maurik. All rights reserved.
 *
 */

// TODO:
//       * Create copy constuctor?
//

#ifndef TemplateClassX_h
#define TemplateClassX_h

#include <iostream>
#include <ctime>

using namespace std;

#include <TROOT.h>
#include <TProofServ.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

#include "BaseAna.h"
#include "FEESelector.h"

class TemplateClassX : public BaseAna {
public:
  
  int fCounter_Freq=10000;
  // Histograms -- set each to NULL in Init and define in SlaveBegin.
  
  TH1F *fee_energy;
  
  FEESelector *FEE;
  
  
public:
  TemplateClassX(TTree *tree=NULL);
  virtual ~TemplateClassX();
  virtual Int_t Version() const { return 1; }

  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
// virtual void    Init(TTree *tree); // No need to override.
  virtual Bool_t  Process(Long64_t entry);
//  virtual void    SlaveTerminate(); // No need to override.
//  virtual void    Terminate();      // No need to override.
  
//  virtual void    Print(Option_t *option="");
  
  ClassDef(TemplateClassX,0);
};

#endif
