/*\class FullEElec
 *\author Maurik Holtrop
 *\brief  This class is a template to create new BaseAna derived classes.
 *\date   2016
 *
 * This class will analyze the full energy electrons. It invokes the FEESelector class from the BaseAna library
 * to select a full energy electron, then produces a number of diagnostic histograms.
 * 
 * Selection of FEE are based on Holly Szumila-Vance selector in the HPS JAVA code.
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
#include "FullEElec.h"

ClassImp(FullEElec);

FullEElec::FullEElec(TTree *tree): BaseAna(tree) {
  // Constructor. If tree is passed, initialize with that tree.

// If Init() does more than the base class stuff, we need to call it here. See comments for Init().
//  if(tree){
//    Init(tree);
//  }else{
//    if(fDebug & kDebug_Warning) cout << "Constructor called without a tree. Not initialized.\n";
//  }

  // Zero all the histogram pointers. This makes PROOF happy.
  cluster_count=nullptr;
  track_count=nullptr;
  cluster_no_track_count=nullptr;
  particle_count=nullptr;
  particle_count_bsc_v0=nullptr;
  particle_count_tc_v0=nullptr;
  neutrals_count=nullptr;
  electron_count=nullptr;
  positron_count=nullptr;
  
  photon_count = nullptr;
  photon_energy = nullptr;
  photon_thetaphi = nullptr;
  photon_xy=nullptr;
    
  FEE = new FEESelector(this);
  
  fee_hists.push_back(clus               = new FullEE_Histos(this,"clus"));
  fee_hists.push_back(clus_matched_e     = new FullEE_Histos(this,"clus_matched_e"));
  fee_hists.push_back(clus_matched_r     = new FullEE_Histos(this,"clus_matched_r"));
  fee_hists.push_back(clus_e             = new FullEE_Histos(this,"clus_e"));
  fee_hists.push_back(clus_e_matched_e   = new FullEE_Histos(this,"clus_e_matched_e"));
  fee_hists.push_back(clus_e_matched_r   = new FullEE_Histos(this,"clus_e__matched_r"));
  fee_hists.push_back(clus_fee           = new FullEE_Histos(this,"clus_fee"));
  fee_hists.push_back(clus_fee_matched_e = new FullEE_Histos(this,"clus_fee_matched_e"));
  fee_hists.push_back(clus_fee_matched_r = new FullEE_Histos(this,"clus_fee_matched_r"));
  
  SetOutputFileName("FullEElec_out.root");
  
}

FullEElec::~FullEElec(){
  // Do any cleanup.
  // Careful here. The histograms cannot be cleaned up, because those are owned by PROOF.
  // In general, we don't need to worry too much about any memory leaks here.
}

/* void FullEElec::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the reader is initialized.
  // This is done in the base class BaseAna.
  // The routine here can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  //
  // Note: Since Init() is by neccessity a virtual method, we get into trouble
  // if we depend on the correct Init() being called from the constructor.
  // Carefull here! Read Effective C++, 3rd edition, pages 48-52
  if(fDebug & kDebug_Info) cout << "      FullEElec::Init(): \n";

  BaseAna::Init(tree);  // Pass along to the base init.
  // Additional local inits done below here. If we don't do anything else,
  // this entire routine is not needed.
  
}
*/
void FullEElec::Begin(TTree *tree)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  BaseAna::Begin(tree);
}

void FullEElec::SlaveBegin(TTree *tree)
{

  // The SlaveBegin() function is called after the Begin() function when processing a chain.
  // When running with PROOF SlaveBegin() is called on each slave server, but Begin is NOT called on each slave,
  // so SlaveBegin() needs to setup the essentials, i.e. Histograms etc.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  if(fDebug & kDebug_Info)cout << "FullEElec::SlaveBegin(): \n";
  BaseAna::SlaveBegin(tree);
  
  TString option = GetOption();
  
  if(tree){
    if(fDebug & kDebug_Info)cout << "      -- Tree name: "<< tree->GetTitle() << " "<< tree->GetName() << endl;
  }
  cout << endl;

  evt_count = 0;
  
  // Initialize the histograms
  
  fOutput->Add(cluster_count = new TH1F("cluster_count","Cluster Count",21,0,21));
  fOutput->Add(cluster_no_track_count = new TH1F("cluster_no_track_count","No Track Cluster count",21,0,21));
  fOutput->Add(track_count = new TH1F("track_count","Cluster Count",21,0,21));
  fOutput->Add(particle_count = new TH1F("particle_count","Particle Count",21,0,21));
  fOutput->Add(particle_count_bsc_v0 = new TH1F("particle_count_bsc_v0","Particle Count",21,0,21));

  fOutput->Add(particle_count_tc_v0 = new TH1F("particle_count_tc_v0","Particle Count",21,0,21));
  fOutput->Add(neutrals_count = new TH1F("neutrals_count","Neutrals Count",21,0,21));
  fOutput->Add(electron_count = new TH1F("electron_count","Electron Count",21,0,21));
  fOutput->Add(positron_count = new TH1F("positron_count","Positron Count",21,0,21));

  
  fOutput->Add(photon_count = new TH1F("photon_count","Photon Count",21,0,21));
  fOutput->Add(photon_energy = new TH1F("photon_energy","Photon Energy",300,0.,1.5));
  fOutput->Add(photon_thetaphi = new TH2F("photon_thetaphi","Photon phi vs theta",100,0.,0.1,100,-TMath::Pi(),+TMath::Pi()));
  fOutput->Add(photon_xy = new TH2F("photon_xy","Photon Y vs X",350,-330,370,180,-90,90));
  
  for(auto hist_col:fee_hists){
    hist_col->Book();
  }

}

Bool_t FullEElec::Process(Long64_t entry)
{
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
  // The return value is currently not used.
  
  // fReader.SetEntry(entry);

  // cout << "Process: " << entry << " Tree: " << fChain->GetTreeNumber() << endl;
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
  
  
  if(cluster_count == nullptr){
    cout << "ERROR -- I was not properly initialized. \n";
  }
  
  int n_clusters=event->getNumberOfEcalClusters();
  cluster_count->Fill(n_clusters);

//  for( int iclus=0;iclus < n_clusters; ++iclus){
//    EcalCluster *clus= event->getEcalCluster(iclus);
//    cluster_energy->Fill(clus->getEnergy());
//    vector<double> pos = clus->getPosition();
//    cluster_loc->Fill(pos[0],pos[1]);
//  }
  
  int n_tracks=event->getNumberOfTracks();
  track_count->Fill(n_tracks);
  
  const int n_particles=event->getNumberOfParticles(HpsParticle::FINAL_STATE_PARTICLE);
  particle_count->Fill(n_particles);
  
  const int n_particles_bcs_v0=event->getNumberOfParticles(HpsParticle::BSC_V0_CANDIDATE);
  particle_count_bsc_v0->Fill(n_particles_bcs_v0);
  
  const int n_particles_tc_v0=event->getNumberOfParticles(HpsParticle::TC_V0_CANDIDATE);
  particle_count_tc_v0->Fill(n_particles_tc_v0);
  
  int n_neutrals=0;
  int n_electrons=0;
  int n_positrons=0;
  
  for(int np=0;np<n_particles;++np){
    HpsParticle *part = GetParticle(np);   //event->getParticle(HpsParticle::FINAL_STATE_PARTICLE,np);
    
    if( part->getCharge() == 0) ++n_neutrals;
    
    int PDGtype = part->getPDG();
    if( PDGtype == 22){        // Yup it's a photon -- To Do: More stringent photon tests here?
      ++n_neutrals;
    }else if (PDGtype == 11){
      if(part->getType() <32) continue;  // Skip the particles that are not GBL re-fit. Avoids duplicates.
      
      ++n_electrons;
      
      if(clus->Fill(part)){
        
        if( abs(clus->delta_energy) < 0.1 ){
          clus_matched_e->FillHistos(clus);
        }
        if( clus->delta_r < 10 ){
          clus_matched_r->FillHistos(clus);
        }
        
        TRefArray *track_ref= part->getTracks();
        SvtTrack *track = (SvtTrack *) track_ref->At(0);
        double chi2 = track->getChi2();
        
        if(chi2 < 10.){
          if(clus_fee->FillHistos(clus)){
            
            if( abs(clus_fee->delta_energy) < 0.1 ){
              clus_matched_e->FillHistos(clus_fee);
            }
            if( clus_fee->delta_r < 10 ){
              clus_matched_r->FillHistos(clus_fee);
            }
          }
          
          if(FEE->Select(part)){
            if(clus_fee->FillHistos(clus)){
              
              if( abs(clus_fee->delta_energy) < 0.1 ){
                clus_matched_e->FillHistos(clus_fee);
              }
              if( clus_fee->delta_r < 10 ){
                clus_matched_r->FillHistos(clus_fee);
              }
            }
          }
        }
      }
    }else if (PDGtype == -11){
      ++n_positrons;
    }
  }
  
  neutrals_count->Fill(n_neutrals);
  electron_count->Fill(n_electrons);
  positron_count->Fill(n_positrons);
  
  return kTRUE;
}
