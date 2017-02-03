/*
 *  BaseAna.hpp
 *  BaseAna
 *
 *  Created by Maurik Holtrop on 5/20/16.
 *  Copyright © 2016 Maurik. All rights reserved.
 *
 */

// TODO:
//       * Create copy constuctor?
//

#ifndef BaseAna_h
#define BaseAna_h

#include <iostream>

using namespace std;

#include <TROOT.h>
#include <TProofServ.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGaxis.h>
#include <TCanvas.h>

#include "HpsEvent.h"

class BaseAna : public TSelector {
public:
  TTree  *fChain; //! Chain holding events.
  Int_t   fChain_type; //!  Controls the behavior of GetEntry: 1=TTree, 2= TChain, 3=Proof
  Int_t   fCurrent;      //! Current Tree number is a TChain
  Int_t   fDebug;
  
  bool    is_process;

  HpsEvent *event;   // Stores the HpsEvent
  TBranch  *b_event; // Branch to HpsEvent
  
  long    current_event; // Current event in file
  long    evt_count;     // Current sequence number for event.

  string  output_file_name;
  TFile   *output_file;

  
  enum Debug_codes {
    kDebug_Quiet   = 0x00,
    kDebug_Error   = 0x01,
    kDebug_Warning = 0x02,
    kDebug_Info    = 0x04,
    kDebug_L1      = 0x10,  // Debug levels
    kDebug_L2      = 0x20,
    kDebug_L3      = 0x30,
    kDebug_L4      = 0x40
  };
  
  enum Chain_types {
    kIs_TTree  = 1,
    kIs_TChain = 2,
    kIs_TChain_Process = 3,
    kIs_TProof = 4
  };
  
  static const int ecal_nx=23;
  static const int ecal_ny=5;
  
public:
  BaseAna(TTree *tree=NULL);
  virtual ~BaseAna();
  virtual Int_t Version(void) const { return 1; }

  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree */* tree */);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0);
  virtual Int_t   Next(void);
  virtual void    SetOption(const char *option) { fOption = option; };
  virtual void    SetObject(TObject *obj) { fObject = obj; };
  virtual void    SetInputList(TList *input) { fInput = input; };
  virtual TList  *GetOutputList() const{ return fOutput; };
  virtual void    SlaveTerminate();
  virtual void    Terminate();

// Debug Manipulators:
  virtual void    SetDebugLevel(const int level){ fDebug = level;};
  virtual void    SetDebugLevelBit(const int level){ fDebug = fDebug | level;};
  virtual void    ClearDebugLevelBit(const int level){ fDebug = fDebug & ~level;};
  virtual void    SetDebugLevelVerbose(void){ fDebug = kDebug_Error +kDebug_Warning +kDebug_Info;};
  virtual int     GetDebugLevel(void){ return fDebug; };

  
// Useful extra methods not needed for PROOF
  
  virtual int             Run(int nevent=0);
  virtual void            Print(Option_t *opt="") const;
  virtual HpsEvent       *GetEvent(){return event;};
  virtual HpsParticle    *GetParticle(int n);
  virtual double         GetAbsMomentum(HpsParticle *part);
  virtual double         GetAbsMomentum(int n);
  
  void            SetOutputFileName(const string& outfile){output_file_name=outfile;};
  string          GetOutputFileName(void){return(output_file_name);};
  void            WriteList(TList *ll);

  void            DrawEcal(int n=2);
  TH2F           *EcalHitMap(void);
  TH2F           *ClusterMap(int n_cl=0);
  void            FancyPlot(TH2F *histo,int opt);
  inline int      TrXhit(const int x) const{ return (x>=0?x:x+1); };
  
  ClassDef(BaseAna,0);
};

#endif
