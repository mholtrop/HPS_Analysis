/*\class TemplateClassX
 *\author Maurik Holtrop
 *\brief  This class is a template to create new BaseAna derived classes.
 *\date   2016
 *
 *
 *
 * The following methods are defined in this file:
 *  -  Begin():        called every time a loop on the tree starts,
 *                    a convenient place to create your histograms.
 *  -  SlaveBegin():   called after Begin(), when on PROOF called only on the
 *                    slave servers.
 *  -  Process():      called for each event, in this function you decide what
 *                    to read and fill your histograms.
 *  -  SlaveTerminate: called at the end of the loop on the tree, when on PROOF
 *                    called only on the slave servers.
 *  -  Terminate():    called at the end of the loop on the tree,
 *                    a convenient place to draw/fit your histograms.
 *
 *
 *  ##Use:
 *  The intent is to be able to use this class with PROOF, however, for practicality, it can also be used directly with a TChain (or TTree),
 *  or given a file and a subsequent Run().
 */
#include <iostream>
#include "TemplateClassX.h"

ClassImp(TemplateClassX);

// Constructor. If tree is passed, initialize with that tree.
TemplateClassX::TemplateClassX(TTree *tree): BaseAna(tree) {
// If Init() does more than the base class stuff, we need to call it here. See comments for Init().
//  if(tree){
//    Init(tree);
//  }else{
//    if(fDebug & kDebug_Warning) cout << "Constructor called without a tree. Not initialized.\n";
//  }

// Zero all the histogram pointers. This makes PROOF happy.

  fee_energy=nullptr;
 
  FEE = new FEESelector(this);
  SetOutputFileName("TemplateClassX_out.root");  // Set the output file name, so we can recognize it.
}

// Standard destructor.
TemplateClassX::~TemplateClassX(){
  // Do any cleanup.
  // Careful here. The histograms cannot be cleaned up, because those are owned by PROOF.
  // In general, we don't need to worry too much about any memory leaks here.
}


// The Begin() function is called at the start of the query.
// When running with PROOF Begin() is only called on the client.
// The tree argument is deprecated (on PROOF 0 is passed).
void TemplateClassX::Begin(TTree *tree)
{
  BaseAna::Begin(tree);
}

// The SlaveBegin() function is called after the Begin() function when processing a chain.
// When running with PROOF SlaveBegin() is called on each slave server, but Begin is NOT called on each slave,
// so SlaveBegin() needs to setup the essentials, i.e. Histograms etc.
// The tree argument is deprecated (on PROOF 0 is passed).
void TemplateClassX::SlaveBegin(TTree *tree)
{
  if(fDebug & kDebug_Info)cout << "TemplateClassX::SlaveBegin(): \n";
  BaseAna::SlaveBegin(tree);
  
  TString option = GetOption();
  
  if(tree){
    if(fDebug & kDebug_Info)cout << "      -- Tree name: "<< tree->GetTitle() << " "<< tree->GetName() << endl;
  }
  cout << endl;

  evt_count = 0;
  
  // Initialize the histograms
  // Add each histogram to the output list.
  fOutput->Add(fee_energy = new TH1F("fee_energy","Cluter Energy for FEE e-",1500,0.,1.5));
  
}

// The Process() function is called for each entry in the tree (or possibly
// keyed object in the case of PROOF) to be processed. The entry argument
// specifies which entry in the currently loaded tree is to be processed.
// When processing keyed objects with PROOF, the object is already loaded
// and is available via the fObject pointer.
//
// This function should contain the \"body\" of the analysis. It can contain
// simple or elaborate selection criteria, run algorithms on the data
// of the event and typically fill histograms.
//
// The processing can be stopped by calling Abort().
//
// Use fStatus to set the return value of TTree::Process().
//
Bool_t TemplateClassX::Process(Long64_t entry)
{
  int stat =GetEntry(entry);
  if(  stat <= 0 ){
    if(fDebug & kDebug_Error){
      cout << "GetEntry("<< entry << ") returned with status "<<  stat << endl;
      printf("i: %9ld  event: %9d  clust: %2d  track: %2d \n", evt_count, event->getEventNumber(), event->getNumberOfEcalClusters(), event->getNumberOfTracks());
    }
    Abort("Bad event");
    return false;
  }
  if( (evt_count++ % fCounter_Freq ) == 0) {
    printf("i: %9ld  event: %9d  clust: %2d  track: %2d  tree: %2d\n", evt_count, event->getEventNumber(), event->getNumberOfEcalClusters(), event->getNumberOfTracks(),fChain->GetTreeNumber());
  }
  
  if( ! (event->isSvtBiasOn() && event->isSvtClosed() && event->isSvtLatencyGood()) ){
    if(fDebug & kDebug_L1) cout << "Event skipped for bad SVT status \n";
    return(kTRUE);
  }
  
  if(fee_energy == nullptr){
    cout << "ERROR -- I was not properly initialized. \n";
  }
  
  
  vector<HpsParticle *> photons;
  for(int np=0;np<event->getNumberOfParticles(HpsParticle::FINAL_STATE_PARTICLE);++np){
    HpsParticle *part = GetParticle(np);   //event->getParticle(HpsParticle::FINAL_STATE_PARTICLE,np);
    int PDGtype = part->getPDG();
    if (PDGtype == 11){
      if(part->getType() <32) continue;  // Skip the particles that are not GBL re-fit. Avoids duplicates.
      if(FEE->Select(part)){
        fee_energy->Fill(part->getEnergy());
      }
    }
  }
  return kTRUE;
}
