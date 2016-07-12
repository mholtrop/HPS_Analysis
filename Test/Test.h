//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 11 16:32:24 2016 by ROOT version 6.07/06
// from TTree HPS_Event/HPS event tree
// found on file: /Volumes/Thunder1b/data/HPS/engrun2015/pass6/dst/hps_005772.11_dst_3.6.1.root
//////////////////////////////////////////////////////////

#ifndef Test_h
#define Test_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

#include "HpsEvent.h"

// Headers needed by this particular selector

class Test : public TSelector {
public :
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   HpsEvent       *event;        // Event container, filled by tree.
   TBranch        *b_event;      // Branch for event.

   long           evt_count;

   Test(TTree * /*tree*/ =0) { }
   virtual ~Test() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(Test,0);

};

#endif

#ifdef Test_cxx
void Test::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

  fChain = tree;
  b_event = fChain->GetBranch("Event");
  b_event->SetAddress(&event);

  cout << "Init done, set Tree and Branch" << endl;
}

Bool_t Test::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

  if(!fChain) cout << "Notify() -- Does not have a chain! \n";
  return kTRUE;
}

#endif // #ifdef Test_cxx
