/*
 *  PiZero.cpp
 *  PiZero
 *
 *  Created by Maurik Holtrop on 5/20/16.
 *  Copyright © 2016 Maurik. All rights reserved.
 *
 */

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
//
//  Use:
//  The intent is to be able to use this class with PROOF, however, for practicality, it can also be used directly with a TChain (or TTree),
//  or given a file and a subsequent Run().

#include <iostream>
#include "PiZero.h"

ClassImp(PiZero);

PiZero::PiZero(TTree *tree): BaseAna(tree) {
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
  
  photon_count = nullptr;
  photon_energy = nullptr;
  photon_thetaphi = nullptr;
  photon_xy=nullptr;
  
  pizero_mass=nullptr;
  pizero_theta=nullptr;
  
}

PiZero::~PiZero(){
  // Do any cleanup.
  // Careful here. The histograms cannot be cleaned up, because those are owned by PROOF.
  // In general, we don't need to worry too much about any memory leaks here.
}

/* void PiZero::Init(TTree *tree)
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
  if(fDebug & kDebug_Info) cout << "      PiZero::Init(): \n";

  BaseAna::Init(tree);  // Pass along to the base init.
  // Additional local inits done below here. If we don't do anything else,
  // this entire routine is not needed.
  
}
*/
void PiZero::Begin(TTree *tree)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  BaseAna::Begin(tree);
}

void PiZero::SlaveBegin(TTree *tree)
{

  // The SlaveBegin() function is called after the Begin() function when processing a chain.
  // When running with PROOF SlaveBegin() is called on each slave server, but Begin is NOT called on each slave,
  // so SlaveBegin() needs to setup the essentials, i.e. Histograms etc.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  if(fDebug & kDebug_Info)cout << "PiZero::SlaveBegin(): \n";
  BaseAna::SlaveBegin(tree);
  
  TString option = GetOption();
  
  if(tree){
    if(fDebug & kDebug_Info)cout << "      -- Tree name: "<< tree->GetTitle() << " "<< tree->GetName() << endl;
  }
  cout << endl;

  evt_count = 0;
  
  // Initialize the histograms
  cluster_count = new TH1F("cluster_count","Cluster Count",21,0,21);
  fOutput->Add(cluster_count);
  cluster_no_track_count = new TH1F("cluster_no_track_count","No Track Cluster count",21,0,21);
  fOutput->Add(cluster_no_track_count);
  track_count = new TH1F("track_count","Cluster Count",21,0,21);
  fOutput->Add(track_count);
  particle_count = new TH1F("particle_count","Particle Count",21,0,21);
  fOutput->Add(particle_count);
  particle_count_bsc_v0 = new TH1F("particle_count_bsc_v0","Particle Count",21,0,21);
  fOutput->Add(particle_count_bsc_v0);
  particle_count_tc_v0 = new TH1F("particle_count_tc_v0","Particle Count",21,0,21);
  fOutput->Add(particle_count_tc_v0);
  
  neutrals_count = new TH1F("neutrals_count","Neutrals Count",21,0,21);
  fOutput->Add(neutrals_count);

  photon_count = new TH1F("photon_count","Photon Count",21,0,21);
  fOutput->Add(photon_count);

  photon_energy = new TH1F("photon_energy","Photon Energy",300,0.,1.5);
  fOutput->Add(photon_energy);
  
  photon_thetaphi = new TH2F("photon_thetaphi","Photon phi vs theta",100,0.,0.1,100,-TMath::Pi(),+TMath::Pi());
  photon_xy = new TH2F("photon_xy","Photon Y vs X",350,-330,370,180,-90,90);
  
  pizero_mass  = new TH1F("pizero_mass","Two Photon Invariant Mass",600,0.,0.3);
  pizero_theta = new TH1F("pizero_theta","Two Photon Angle",500,0.,0.5);
  
}

Bool_t PiZero::Process(Long64_t entry)
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
  
  if(cluster_count == nullptr){
    cout << "ERROR -- I was not properly initialized. \n";
  }
  
  int n_clusters=event->getNumberOfEcalClusters();
  cluster_count->Fill(n_clusters);
  
  int n_tracks=event->getNumberOfTracks();
  track_count->Fill(n_tracks);
  
  const int n_particles=event->getNumberOfParticles(HpsParticle::FINAL_STATE_PARTICLE);
  particle_count->Fill(n_particles);
  
  const int n_particles_bcs_v0=event->getNumberOfParticles(HpsParticle::BSC_V0_CANDIDATE);
  particle_count_bsc_v0->Fill(n_particles_bcs_v0);
  
  const int n_particles_tc_v0=event->getNumberOfParticles(HpsParticle::TC_V0_CANDIDATE);
  particle_count_tc_v0->Fill(n_particles_tc_v0);
  
  int neutrals=0;
  vector<HpsParticle *> photons;
  for(int np=0;np<n_particles;++np){
    HpsParticle *part = event->getParticle(HpsParticle::FINAL_STATE_PARTICLE,np);
    int PDGtype = part->getPDG();
    if( PDGtype == 22){        // Yup it's a photon -- To Do: More stringent photon tests here?
      photons.push_back(part); // So put it on our list.
    }
    if( part->getCharge() == 0) ++neutrals;
  }
  
  neutrals_count->Fill(neutrals);
  int n_photons=photons.size();
  photon_count->Fill(n_photons);
  
  for(int g1=0;g1< n_photons;++g1){
    photon_energy->Fill(photons[g1]->getEnergy());

    vector<double> mom1=photons[g1]->getMomentum();
    TLorentzVector p4_g1(mom1[0],mom1[1],mom1[2],photons[g1]->getEnergy());
    photon_thetaphi->Fill(p4_g1.Theta(),p4_g1.Phi());
    EcalCluster *clus1 = (EcalCluster *)photons[g1]->getClusters()->At(0);
    vector<double> pos1=clus1->getPosition();
    photon_xy->Fill(pos1[0],pos1[1]);

    for(int g2=g1;g2< n_photons;++g2){
      vector<double> mom2=photons[g2]->getMomentum();
      TLorentzVector p4_g2(mom2[0],mom2[1],mom2[2],photons[g2]->getEnergy());
      // Put two photon tests here.
      double theta = p4_g1.Angle(p4_g2.Vect());

      if(theta>0.001){
        pizero_theta->Fill(theta);
        TLorentzVector p4_pi0 = p4_g1 + p4_g2;
        pizero_mass->Fill(p4_pi0.M());
      }
   
    }
  }
  
  return kTRUE;
}

void PiZero::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  if(fDebug & kDebug_Info) cout << "Slave Terminate \n";
}

void PiZero::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  
  if(fDebug & kDebug_Info) cout << "Terminate \n";
//  cluster_count = dynamic_cast<TH1F*>(fOutput->FindObject("cluster_count"));
//  TCanvas *c1=new TCanvas("c1","C",800,600);
//  cluster_count->Draw();
  TList *list=GetOutputList();
  cout << list->GetEntries() << endl;
  
  // Write the contends to a file, but skip the first three objects.
  if(fDebug & kDebug_Info) cout << "Writing output file\n";
  TFile f("tmp_hists.root","RECREATE");
  for(int i=0;i<list->GetEntries();++i){
    
    list->At(i)->Write();
  }
  f.Close();
}

