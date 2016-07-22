/*!
 * \class  BaseAna
 * \brief  The base analysis class for hps-dst based analysis projects.
 * \author Maurik Holtrop (maurik@physics.unh.edu)
 * \date   2016
 *
 * The following methods are defined in this file:
 *  -  Begin():          called every time a loop on the tree starts,
 *                       a convenient place to create your histograms.
 *  -  SlaveBegin():     called after Begin(), when on PROOF called only on the
 *                       slave servers.
 *  -  Process():        called for each event, in this function you decide what
 *                       to read and fill your histograms.
 *  -  SlaveTerminate(): called at the end of the loop on the tree, when on PROOF
 *                       called only on the slave servers.
 *  -  Terminate():      called at the end of the loop on the tree,
 *                       a convenient place to draw/fit your histograms.
 *
 * ##Use:
 *
 *  The intent is to be able to use this class with PROOF, however, for practicality, it can also be used directly with a TChain (or TTree),
 *  or given a file and a subsequent Run().
*/

#include <iostream>
#include "BaseAna.h"

ClassImp(BaseAna);

BaseAna::BaseAna(TTree *tree): fChain(NULL),fDebug(kDebug_Info+kDebug_Warning+kDebug_Error), is_process(false), event(NULL), b_event(NULL){
  // Constructor. If tree is passed, initialize with that tree.

  if(tree){
    Init(tree);
  }else{
    if(fDebug & kDebug_Warning) cout << "BaseAnna::BaseAna() Constructor called without a tree. Not initialized.\n";
  }
  
//  if (gROOT->IsProofServ()) {
//    if (gProofServ->IsMaster()) {
//      if(fDebug & kDebug_Info) printf("Macro running on master server\n");
//      // single remote init
//    } else {
//      if(fDebug & kDebug_Info)printf("Macro running on %d of %d\n", gProofServ->GetGroupId(),
//             gProofServ->GetGroupSize());
//      // parallel remote init
//    }
//  }else{
//    if(fDebug & kDebug_Info) cout << "Running outside of PROOF\n";
//  }

  output_file_name = "BaseAna_out.root";

}

BaseAna::~BaseAna(){
    // Clean up. Warning!!! Careful here with the PROOF system. Deleting histograms causes errors.
}

void BaseAna::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the reader is initialized.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
 
  if(fDebug & kDebug_Info) cout << "      BaseAna::Init(): \n";
 
  if(!event){
    event = new HpsEvent();
  }
  current_event=0;
  
  if(tree){
    fChain = tree;
    //
    // Determine the type of Chain, and if it is within Proof, since
    // unfortunately, the behavior of GetEntry() must depend on the type of chain.
    //
    if(gROOT->IsProofServ()){
      fChain_type = kIs_TProof;
      if(fDebug & kDebug_Info)   cout << "            -- PROOF type chain. \n";
    }else{
      if(fChain->InheritsFrom(TChain::Class())){
        if(is_process){
          fChain_type=kIs_TChain_Process;
          if(fDebug & kDebug_Info) cout << "            -- CHAIN::Process type chain. \n";
        }else{
          fChain_type=kIs_TChain;
          Notify();
          if(fDebug & kDebug_Info) cout << "            -- CHAIN type chain. \n";
          fChain->GetEntry(0);
        }
      }else{
        fChain_type=kIs_TTree;
        Notify();
        if(fDebug & kDebug_Info) cout << "            -- TREE type chain. \n";
      }
      
    }
   
  }else{
    if(fDebug & kDebug_Error) cout << "**** ERROR **** - init without a chain.\n";
  }
}

Bool_t BaseAna::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  if(fDebug & kDebug_L1) cout << "BaseAna::Notify():\n";
  if( !fChain ) cout << "Notify does not have a chain!!!!!\n";
  else{
    b_event = fChain->GetBranch("Event");
    if(fDebug & kDebug_L2) cout << "Notify(): got branch? " << (b_event!=NULL? "Yes" : "NO" ) << endl;
    
    if(b_event != NULL){
      if(!event){
        if(fDebug & kDebug_Error) cout << "ERROR ***** Notify has no event object.\n";
        return kFALSE;
      }
      b_event->SetAddress(&event);  // This is needed when running through a chain, not really for Proof but does no harm.
      if(fDebug & kDebug_L2) cout << "Notify(): Address set. \n";
      
    }else{
      if(fDebug & kDebug_Error) cout << "**** ERROR **** File in TChain does not contain Event branch: "<< fChain->GetTreeNumber() <<"\n";
      return kFALSE;
    }
  }
  return kTRUE;
}

void BaseAna::Begin(TTree *tree)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  
  TString option = GetOption();
  if(fDebug & kDebug_L1) cout << "BaseAna::Begin(): \n";
  
  if(tree){
    if(!fChain){
      if(fDebug & kDebug_L1) cout << "      -- Begin called with tree and not already initialized, calling Init(tree) with is_process. \n";
      is_process=true;
      Init(tree);  // With Proof, we get instantiated with a tree, with a TChain, we may not.
    }
  }
  cout << endl;
}

void BaseAna::SlaveBegin(TTree */*tree */)
{
  // The SlaveBegin() function is called after the Begin() function when processing a chain.
  // When running with PROOF SlaveBegin() is called on each slave server, but Begin is NOT called on each slave,
  // so SlaveBegin() needs to setup the essentials, i.e. Histograms etc.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();
  if(fDebug & kDebug_L1)cout << "BaseAna::SlaveBegin(): \n";

  if(!event){
    if(fDebug & kDebug_L2)cout << "BaseAna::SlaveBegin(): create a new event object.\n";
    event = new HpsEvent();
  }
}

Int_t BaseAna::GetEntry(Long64_t entry, Int_t getall){
  // Retrieve the entry from the chain.
  // This routine simply gets the entry, it does not process the entry for you. It will check to see if the entry is in the current Tree
  // and if not link to the correct tree and call Notify()
  current_event=entry;
  if(fChain){
    if(fChain_type == kIs_TProof || fChain_type == kIs_TChain_Process ){ // TProof or TChain::Process()
      return (int)(fChain->GetTree() ? fChain->GetTree()->GetEntry(entry, getall) : 0);
    }else if(fChain_type == kIs_TChain){              // Standard Chain, needs to get correct tree first.
       Int_t treeNumInChain = fChain->GetTreeNumber();
      Long64_t centry = fChain->LoadTree(entry);
      if(centry < 0){
        if(fDebug & kDebug_Warning) cout << "Centry was negative. \n";
        return( -1 );
      }
      Int_t currentTreeNumInChain = fChain->GetTreeNumber();
      if (treeNumInChain != currentTreeNumInChain) {
        if(fDebug & kDebug_L1) cout << "Old Tree: "<< treeNumInChain << "  New Tree from the chain: " << currentTreeNumInChain << endl;
        Notify();
      }
      return (int)(fChain->GetTree() ? fChain->GetTree()->GetEntry(centry, getall) : 0);
      
    }else{ // Normal TTree, just get the entry.
      return (int)fChain->GetEntry(entry,getall);
    }
  }
  
  return 0;
}

Bool_t BaseAna::Process(Long64_t entry)
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
  
  cout << "Process: " << entry << " Tree: " << fChain->GetTreeNumber() << endl;
  int stat =GetEntry(entry);
  if(  stat <= 0 ){
    if(fDebug & kDebug_Error){
      cout << "GetEntry("<< entry << ") returned with status "<<  stat << endl;
      printf("i: %9ld  event: %9d  clust: %2d  track: %2d \n", evt_count, event->getEventNumber(), event->getNumberOfEcalClusters(), event->getNumberOfTracks());
    }
    Abort("Bad event");
    return false;
  }
  if( (evt_count++ % 1000 ) == 0) {
    printf("i: %9ld  event: %9d  clust: %2d  track: %2d  tree: %2d\n", evt_count, event->getEventNumber(), event->getNumberOfEcalClusters(), event->getNumberOfTracks(),fChain->GetTreeNumber());
  }
  return(kTRUE);
}


Int_t BaseAna::Next(void){
  // Get the next event and process it.
  return(Process( current_event + 1)); // Note that current_event is updated in GetEntry(), so ++current_event is NOT correct!
}

void BaseAna::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  if(fDebug & kDebug_L1) cout << "BaseAna::SlaveTerminate() \n";
}

void BaseAna::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  
  if(fDebug & kDebug_L1) cout << "BaseAna::Terminate() \n";
  TList *list=GetOutputList();
  cout << list->GetEntries() << endl;
  
  // Write the contends to a file, but skip the first three objects.
  if(fDebug & kDebug_L1) cout << "Writing output file: " << output_file_name << endl;
  TFile file_out(output_file_name.data(),"RECREATE");
  for(int i=0;i<list->GetEntries();++i){
    
    list->At(i)->Write();
  }
  file_out.Close();
  
}

int BaseAna::Run(int nevent){
  // Run through nevents, if nevent <=0 then run until the end of the chain.
  int max_event = fChain->GetEntries();
  if( nevent>0 && nevent+current_event < max_event ) max_event = nevent;
  
  int stat=1;
  while(current_event+1 < max_event && stat>0){
    stat=Next();
  }
  return(current_event);
}

void BaseAna::Print(Option_t *option) const{
  
  char evt_time[71];
  time_t evt_time_n;
  long evt_time_frac;
  if(event == NULL){
    cout <<  "Event is NULL \n";
    return;
  }
  long evt_time_raw = event->getEventTime();
  if( evt_time_raw > 1000000000 ){
    evt_time_n = evt_time_raw/1000000000;
    evt_time_frac =evt_time_raw%1000000000;
  }else{
    evt_time_n = 0;
    evt_time_raw = event->getEventTime();
    evt_time_frac=0;
  }
  
  struct tm *ti = localtime(&evt_time_n);
  strftime(evt_time,49,"%c",ti);
  
  printf("Run: %6d  Event: %7d  time: %ld = %s %ld evt_in_chain: %ld\n",event->getRunNumber(),event->getEventNumber(),evt_time_n,evt_time,evt_time_frac,current_event);
  printf("Trigger");
  if( event->isPulserTrigger() )printf(" pulser ");
  if( event->isSingle0Trigger())printf(" single0 ");
  if( event->isSingle1Trigger())printf(" single1 ");
  if( event->isPair0Trigger() )printf(" pair0 ");
  if( event->isPair1Trigger() )printf(" pair1 ");
  printf("\n");
  printf("SVT: ");
  if(event->isSvtBiasOn() && event->isSvtClosed() && event->isSvtLatencyGood()) printf(" OK \n");
  else printf("NOT OK: Bias %1d Closed: %1d Latency: %1d \n",event->isSvtBiasOn(),event->isSvtClosed(),event->isSvtLatencyGood());
  printf("N_Ecal: %3d  N_SVT: %3d N_Track: %3d \n",event->getNumberOfEcalClusters(),event->getNumberOfSvtHits(),event->getNumberOfTracks());
  printf("N_Part: %3d  UC_V0: %3d BC_V0: %3d  TC_V0: %3d\n",
         event->getNumberOfParticles(HpsParticle::FINAL_STATE_PARTICLE),
         event->getNumberOfParticles(HpsParticle::UC_V0_CANDIDATE),
         event->getNumberOfParticles(HpsParticle::BSC_V0_CANDIDATE),
         event->getNumberOfParticles(HpsParticle::TC_V0_CANDIDATE));
  if(strcmp(option,"all")==0){
    cout << "Particles: \n";
    for(int i=0; i<event->getNumberOfParticles(HpsParticle::FINAL_STATE_PARTICLE); ++i){
      HpsParticle *part= event->getParticle(HpsParticle::FINAL_STATE_PARTICLE, i);
      if(part){
        // Identify which cluster is associated with this particle.
        int cluster_num=-2;
        TRefArray *cl_ref = part->getClusters();
        if(cl_ref->GetEntries()>1) cout << "More than one cluster for this particle!";
        if(cl_ref->GetEntries()>0){
          EcalCluster *p_clus = (EcalCluster *)cl_ref->At(0);
          for(int nc=0;nc<event->getNumberOfEcalClusters();++nc){
            if( event->getEcalCluster(nc) == p_clus){
              cluster_num = nc;
            }
          }
        }
        // Identify which track is associated with this particle.
        int track_num=-1;
        TRefArray *tr_ref = part->getTracks();
        if(tr_ref->GetEntries()>1) cout << "More than one track for this particle!";
        if(tr_ref->GetEntries()>0){
          SvtTrack *p_track = (SvtTrack *)tr_ref->At(0);
          if( part->getType() <= 32){
            for(int nt=0;nt<event->getNumberOfTracks();++nt){
              if( event->getTrack(nt) == p_track){
                track_num = nt;
                break;
              }
            }
          }else{
            for(int ng=0;ng<event->getNumberOfGblTracks();++ng){
              if(event->getGblTrack(ng) == p_track){
                track_num = 100+ng;
                break;
              }
            }
          }
        }
        vector<double> mom=part->getMomentum();
        printf("%2d: PID: %4d (%5.1f) Type: %2d -- E= %6.4f P=(%5.2f,%5.2f,%5.2f) Cluster: %d  Track:%3d\n",i,part->getPDG(),part->getGoodnessOfPID(),part->getType(),part->getEnergy(),mom[0],mom[1],mom[2],cluster_num+1,track_num);
      }
    }
    cout << "Clusters: \n";
    for(int nc=0;nc<event->getNumberOfEcalClusters();++nc){
      EcalCluster *clus = event->getEcalCluster(nc);
      vector<double> pos=clus->getPosition();
      EcalHit *seed=clus->getSeed();
      printf("%2d: Energy: %5.2f L:(%7.2f,%6.2f,%6.1f) T: %6.2f Seed: (%3d,%3d) hits:",nc+1,clus->getEnergy(),pos[0],pos[1],pos[2],clus->getClusterTime(),TrXhit(seed->getXCrystalIndex()),seed->getYCrystalIndex());
      TRefArray *ec_hits = clus->getEcalHits();
      for(int nh=0;nh<ec_hits->GetEntries();++nh){
        EcalHit *hit = (EcalHit *)ec_hits->At(nh);
        printf("(%3d,%3d) ",TrXhit(hit->getXCrystalIndex()),hit->getYCrystalIndex());
      }
      printf("\n");
    }
    cout << "Hits: \n";
    for(int nh=0;nh<event->getNumberOfEcalHits();++nh){
      EcalHit *hit=event->getEcalHit(nh);
      printf("%2d: E:%7.2f  T:%7.2f  (%3d,%3d) \n",nh,hit->getEnergy(),hit->getTime(),TrXhit(hit->getXCrystalIndex()),hit->getYCrystalIndex());
    }
    cout << "Tracks: \n";
    for(int nt=0;nt<event->getNumberOfTracks();++nt){
      SvtTrack *trk = event->getTrack(nt);
      vector<double> pos=trk->getPositionAtEcal();
      vector<double> mom=trk->getMomentum();
      printf("%2d: Ch:%2d Type: %2d X2: %6.2f P:(%5.2f,%5.2f,%5.2f) L:(%5.2f,%5.2f,%5.2f) T: %5.3f \n",nt,trk->getCharge(),trk->getType(),trk->getChi2(),mom[0],mom[1],mom[2],pos[0],pos[1],pos[2],trk->getTrackTime());
    }
    cout << "GBL Tracks: \n";
    for(int ng=0;ng<event->getNumberOfGblTracks();++ng){
      GblTrack *gtrk = event->getGblTrack(ng);
      vector<double> pos=gtrk->getPositionAtEcal();
      vector<double> mom=gtrk->getMomentum();
      printf("%2d: Ch:%2d Type: %2d X2: %6.2f P:(%5.2f,%5.2f,%5.2f) L:(%5.2f,%5.2f,%5.2f) T: %5.3f \n",ng,gtrk->getCharge(),gtrk->getType(),gtrk->getChi2(),mom[0],mom[1],mom[2],pos[0],pos[1],pos[2],gtrk->getTrackTime());
    }
    
  }
}

HpsParticle    *BaseAna::GetParticle(int n){
  // Returns the n-th FINAL_STATE_PARTICLE from the current event.
  if(n >= event->getNumberOfParticles(HpsParticle::FINAL_STATE_PARTICLE)){
    cout << "Asking for particle that is not in list. ";
    return(nullptr);
  }else{
    return event->getParticle(HpsParticle::FINAL_STATE_PARTICLE,n);
  }
}

double BaseAna::GetAbsMomentum(HpsParticle *part){
  // Return the absolute of the momentum for the particle.
  vector<double> mom = part->getMomentum();
  return( sqrt( mom[0]*mom[0] + mom[1]*mom[1] + mom[2]*mom[2] ));
}

double BaseAna::GetAbsMomentum(int n){
  HpsParticle *part = GetParticle(n);
  if(part) return GetAbsMomentum(part);
  else return -999;
}

void BaseAna::DrawEcal(int n){
  // Draw a representation of the ECAL with he hits for the current event.
  // Argument n=2 sets the line width.
  //
  TH2F *hit_map = EcalHitMap();
  TH2F *clusters =ClusterMap();
  
  FancyPlot(hit_map,0);
  clusters->SetLineWidth(n);
  clusters->Draw("box same");
  clusters->Draw("text same)");
  
}

TH2F *BaseAna::EcalHitMap(void){
  // Return a 2D histogram showing the distribution of ECal hits for this event.
  // Use:
  // ... // define ana
  // TH2F my_hitmap(ana->EcalHitMap());
  // my_hitmap.Draw()
  //
  TH2F *hit_map =(TH2F *)gROOT->FindObject("ecal_hit_map");
  if(hit_map == nullptr){
    hit_map =  new TH2F("ecal_hit_map","Ecal Hit Map",(ecal_nx+1)*2,-ecal_nx-0.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  }else{
    hit_map->Reset();
  }
  
  for(int nhit=0;nhit<event->getNumberOfEcalHits();++nhit){
    EcalHit *hit=event->getEcalHit(nhit);
    int xhit=hit->getXCrystalIndex();
    int yhit=hit->getYCrystalIndex();
//    xhit = (xhit>=0?xhit:xhit+1);
    hit_map->Fill(TrXhit(xhit),yhit,hit->getEnergy());
  }
  return(hit_map);

}

TH2F *BaseAna::ClusterMap(int n_cl){
  // Return a 2D histogram showing the distribution of ECal clusters for this event.
  // Use:
  // ... // define ana
  // TH2F my_hitmap(ana->EcalHitMap());
  // my_hitmap.Draw()
  //

  TH2F *cluster_map =(TH2F *)gROOT->FindObject("cluster_hit_map");
  if(cluster_map == nullptr){
    cluster_map =  new TH2F("cluster_map","Ecal Hit Map",(ecal_nx+1)*2,-ecal_nx-0.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  }else{
    cluster_map->Reset();
  }

  if(n_cl > event->getNumberOfEcalClusters()) n_cl=0;
  string name("Cluster Map");
  if(n_cl>0) name.append(" ").append(to_string(n_cl));
  cluster_map->SetName(name.data());
  
  for(int nc=(n_cl==0?0:n_cl-1);nc<(n_cl!=0?n_cl:event->getNumberOfEcalClusters());++nc){
    if(gDebug ) cout << "Histo for ECAL Cluster" << nc << endl;
    EcalCluster *clus=event->getEcalCluster(nc);
    // EcalHit *seed = clus->getSeed();
    TRefArray *r_hits = clus->getEcalHits();
    for(int nh=0;nh<r_hits->GetEntries();++nh){
      EcalHit *hit=(EcalHit *)r_hits->At(nh);
      int xhit=hit->getXCrystalIndex();
      int yhit=hit->getYCrystalIndex();
//      xhit = (xhit>=0?trxhit:xhit+1);
      cluster_map->Fill(TrXhit(xhit),yhit,float(nc+1));
    }
  }

  cluster_map->SetMaximum(1.);
  cluster_map->SetLineColor(kRed);
  return(cluster_map);
}

void BaseAna::FancyPlot(TH2F *histo,int opt){
  //
  // Fancy plot of the Calorimeter.  For the TEST RUN. (NO LEAD GLASS)
  //
  //this defines the position of the top right region of big boxes, others will fall in by symmetry
  int ecal_x_first=1;
  int ecal_nx_first=0;
  int ecal_y_first=1;
  int ecal_ny_first=-1;
  
  int ecal_nx=23;
  int ecal_ny=5;
  
  TAxis *xax=histo->GetXaxis();
  TAxis *yax=histo->GetYaxis();
    
  TH2F *ones_lb;
  
  if (!gROOT->FindObject(TString(histo->GetName()).Append("_oneslb"))){//if this one exists all the rest probably exist too
    ones_lb=new TH2F(TString(histo->GetName()).Append("_oneslb"),"oneslb",(ecal_nx+1)*2,-ecal_nx-0.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  }
  else {
    ones_lb= (TH2F*) gROOT->FindObject(TString(histo->GetName()).Append("_oneslb"));
    ones_lb->Clear();
  }
  
  if(! opt& 0x4 ) histo->SetMaximum();
  if(histo->GetMaximum() < 1){
    histo->SetMaximum(1.1);
  }
  
  float SetMax=histo->GetMaximum();
  if(SetMax<1.1)SetMax=1.1;
  
  xax=ones_lb->GetXaxis();
  yax=ones_lb->GetYaxis();
  
  //this chunk of code just puts the grid in the right place
  for (int i=0;i<ecal_nx;i++){
    for (int j=0;j<ecal_ny;j++){
      
      ones_lb->SetBinContent(xax->FindBin(ecal_x_first+i),yax->FindBin(ecal_y_first+j),1);
      ones_lb->SetBinContent(xax->FindBin(ecal_x_first+i),yax->FindBin(ecal_ny_first-j),1);
      if(j==0 && i>0 && i<10){
        ;
      }else{
        ones_lb->SetBinContent(xax->FindBin(ecal_nx_first-i),yax->FindBin(ecal_ny_first-j),1);
        ones_lb->SetBinContent(xax->FindBin(ecal_nx_first-i),yax->FindBin(ecal_y_first+j),1);
      }
    }
  }
  
  ones_lb->Scale(SetMax);//scale them so the boxes are big enough
  
  //draw stuff
  
  if (opt & 0x2){
    
    if(opt&0x1){
      histo->Draw("Acol");
    }else{
      histo->Draw("Acolz");
    }
    ones_lb->Draw("boxsame");
    
    double xtal_size = 6.665;
    double xlow = ( xax->GetFirst() -1 + xax->GetXmin() );
    double xhi    ( xax->GetLast() - xax->GetFirst() + 1 + xax->GetXmin());
    double ylow = ( yax->GetFirst() -1 + yax->GetXmin() );
    double yhi  = ( yax->GetLast() - yax->GetFirst() + 1 + yax->GetXmin());
    
    double xlow2 = xtal_size * (xlow-1);
    double xhi2  = xtal_size * (xhi -1);
    double ylow2 = xtal_size * ylow;
    double yhi2  = xtal_size * yhi;
    
    TGaxis *lefta = new TGaxis(gPad->GetUxmin(),gPad->GetUymin(),gPad->GetUxmin(),gPad->GetUymax(),ylow2,yhi2,50510);
    TGaxis *bota  = new TGaxis(gPad->GetUxmin(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymin(),xlow2,xhi2,50510);
    lefta->SetName("leftaxis");
    lefta->SetTitle("y [mm]");
    bota->SetName("bottomaxis");
    bota->SetTitle("x [mm]");
    lefta->Draw();
    bota->Draw();
    
    if(opt & 0x4){
      TGaxis *righta = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),ylow,yhi,510,"");
      TGaxis *topa   = new TGaxis(gPad->GetUxmin(),gPad->GetUymax(),gPad->GetUxmax(),gPad->GetUymax(),xlow,xhi,510,"-");
      righta->SetName("rightaxis");
      righta->SetTitle("y crystal index");
      topa->SetName("topaxis");
      topa->SetTitle("x crystal index");
      //      righta->Draw();
      topa->Draw();
    }
  }else{
    xax->SetTitle("x crystal index");
    yax->SetTitle("y crystal index");
    if(opt&0x1){
      histo->Draw("col");
    }else{
      histo->Draw("colz");
    }
    ones_lb->Draw("boxsame");
  }
}

