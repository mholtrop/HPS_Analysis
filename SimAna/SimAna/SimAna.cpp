/*\class SimAna
 *\author Maurik Holtrop
 *\brief  This class is a template to create new BaseAna derived classes.
 *\date   2018-06-18
 */
#include "SimAna.h"

ClassImp(SimAna);

SimAna::SimAna(){
  SetName("SimAna");
    SetTitle("LCIO output analyzer.");
 }

SimAna::SimAna(string lcio_file_name, bool direct_access){
  SetName("SimAna");
  SetTitle("LCIO output analyzer.");
  Init(lcio_file_name,direct_access);
}

SimAna::SimAna(string lcio_file_name, const char* name, bool direct_access){
  SetName(name);
  SetTitle("LCIO output analyzer.");
  Init(lcio_file_name,direct_access);
}

// Standard destructor.
SimAna::~SimAna(){
  // Do any cleanup.
}

void SimAna::Init(const string lcio_file_name, bool direct_access){
// Initialize with file for reading, and setup histograms.
  if(direct_access==true){
    lc_reader=LCFactory::getInstance()->createLCReader(LCReader::directAccess);
  }else{
    lc_reader=LCFactory::getInstance()->createLCReader();
  }
  OpenLCIO(lcio_file_name);
  
  // ====== Book Histograms ========
  N_EcalHits = new TH1F("N_EcalHits","Number of ECAL Hits",100,0,99);
  N_EcalHits->SetXTitle("Number of hits");
  N_EcalHits_E= new TH1F("N_EcalHits_E","Number of ECAL Hits with E>50MeV",100,0,99);
  N_EcalHits_E->SetXTitle("Number of hits");

  N_MCParticle = new TH1F("N_MCParticle","Number of MC Particles",200,0,199);
  N_MCParticle->SetXTitle("Number of particles");
  N_TrackerHits = new TH1F("N_TrackerHits","Number of Tracker Hits",100,0,99);
  N_TrackerHits->SetXTitle("Number of hits");
  N_TrackerHitsEcal = new TH1F("N_TrackerHitsEcal","Number of Tracker Hits Ecal",100,0,99);
  N_TrackerHitsEcal->SetXTitle("Number of hits");
  
  Ecal_Hit_Energy = new TH1F("Ecal_Hit_Energy","Ecal Hit energy",2300,0.,2.3);
  Ecal_Hit_Energy->SetXTitle("Energy (GeV)");
  Ecal_Hit_CE     = new TH1F("Ecal_Hit_CE","Ecal Hit Contributor energy",2300,0.,2.3);
  Ecal_Hit_CE->SetXTitle("Energy (GeV)");
  Ecal_Hit_Energy_sum = new TH1F("Ecal_Hit_Energy_sum","Ecal Hit energy sum",2300,0.,2.3);
  Ecal_Hit_Energy_sum->SetXTitle("Energy (GeV)");
  
  int ecal_nx=23;
  int ecal_ny=5;
  Ecal_Hits_Loc = new TH2F("Ecal_Hits_Loc","Ecal Hits",(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  Ecal_Hits_Loc->SetXTitle("x-index");
  Ecal_Hits_Loc->SetYTitle("y-index");
  Ecal_Hits_Loc_E = new TH2F("Ecal_Hits_Loc_E","Ecal Hits with E>50MeV",(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  Ecal_Hits_Loc_E->SetXTitle("x-index");
  Ecal_Hits_Loc_E->SetYTitle("y-index");
  
  Ecal_Hit_CNT  = new TH1F("Ecal_Hit_CNT","Num of Ecal hit contributions;N;counts",1000,0.,1000.);
  Ecal_Hit_Time = new TH1F("Ecal_Hit_Time","Ecal hit time;time;counts",1000,-1000.,1000.);
  Ecal_Hit_TimeAve = new TH1F("Ecal_Hit_TimeAve","Ecal hit time average per event;time;counts",1000,-1000.,1000.);
  Ecal_Hit_TimeRel = new TH1F("Ecal_Hit_TimeRel","Ecal hit time relative to average;time;counts",1000,-2000.,1000.);

  Ecal_Hit_CNT_ecut  = new TH1F("Ecal_Hit_CNT_ecut","Num of Ecal hit contributions;N;counts",1000,0.,5000.);
  Ecal_Hit_Time_ecut = new TH1F("Ecal_Hit_Time_ecut","Ecal hit time;time;counts",1000,-1000.,1000.);
  Ecal_Hit_TimeAve_ecut = new TH1F("Ecal_Hit_TimeAve_ecut","Ecal hit time average per event;time;counts",1000,-1000.,1000.);
  Ecal_Hit_TimeRel_ecut = new TH1F("Ecal_Hit_TimeRel_ecut","Ecal hit time relative to average;time;counts",1000,-2000.,1000.);

  
  // ==== Tracker
  Tracker_Hit_Energy = new TH1F("Tracker_Hit_Energy","SVT Hit Energy Deposited",1000,0.,0.01);
  Tracker_Hit_Energy->SetXTitle("Energy (GeV)");
  Tracker_Hit_PathLength= new TH1F("Tracker_Hit_PathLength","Tracker Hit Path Length",1000,0.32,0.33);
  Tracker_Hit_PathLength->SetXTitle("L (mm)");
  Tracker_Hit_Px = new TH1F("Tracker_Hit_Px","Tracker Hit x Momentum",1000,0.,0.1);
  Tracker_Hit_Px->SetXTitle("Px (GeV/c)");
  Tracker_Hit_Py = new TH1F("Tracker_Hit_Py","Tracker Hit y Momentum",1000,0.,0.15);
  Tracker_Hit_Py->SetXTitle("Py (GeV/c)");
  Tracker_Hit_Pz = new TH1F("Tracker_Hit_Pz","Tracker Hit z Momentum",1000,0.,2.3);
  Tracker_Hit_Pz->SetXTitle("Pz (GeV/c)");

}

int SimAna::Run(int nevt){
  // Run through nevt events of this file. If nevt<=0 run through all.
  // For each event call FillHistos()
  int count=0;
  if(nevt<=0){
    nevt=GetNEvents();
  }
  for(int i=0;i<nevt && true; ++i){
    if(Next()){
      if( count%fCounter_Freq == 0){
        printf("i: %12d  Event: %12d \n",i,evt->getEventNumber());
      };
      count++;
      FillHistos();
    }
  }
  return(count);
}

void SimAna::FillHistos(void){
  // Fill all the histograms with the data from the current event.
  // This is where the work is done.
  
  N_EcalHits->Fill(getNEcalHits());
  N_MCParticle->Fill(getNMCParticle());
  N_TrackerHits->Fill(getNTrackerHits());
  N_TrackerHitsEcal->Fill(getNTrackerHitsEcal());
  
  //========== ECAL Hits ==============
  int cnt=0;
  if(getNEcalHits()>0){
    IMPL::SimCalorimeterHitImpl *calorimeter_hit;
    LCCollection *ecal_hits=static_cast<LCCollection *>(evt->getCollection("EcalHits"));
    double energy_sum=0;
    for(int n=0; n<ecal_hits->getNumberOfElements();++n){
      calorimeter_hit = GetEcalHit(n,ecal_hits);
      Ecal_Hit_Energy->Fill(calorimeter_hit->getEnergy());
      energy_sum+=calorimeter_hit->getEnergy();
      Ecal_Hits_Loc->Fill(GetEcalHit_ix(calorimeter_hit),GetEcalHit_iy(calorimeter_hit));
      if(calorimeter_hit->getEnergy()>0.050){
        cnt++;
        Ecal_Hits_Loc_E->Fill(GetEcalHit_ix(calorimeter_hit),GetEcalHit_iy(calorimeter_hit));
      }
      int num_elem=calorimeter_hit->getNMCContributions();
      Ecal_Hit_CNT->Fill(num_elem);
      double time_sum=0;
      double time_sum_ecut=0;

      int count_e_cut=0;
      for (int nt=0; nt<num_elem;++nt){
        double cal_energy = calorimeter_hit->getEnergyCont(nt);
        double cal_time=calorimeter_hit->getTimeCont(nt);
        Ecal_Hit_CE->Fill(cal_energy);
        Ecal_Hit_Time->Fill(cal_time);
        if(cal_energy>0.001){
          count_e_cut++;
          Ecal_Hit_Time_ecut->Fill(cal_time);
          time_sum_ecut += cal_time;
        }
        time_sum += cal_time;
      }
      
      if(num_elem>0){
        double time_ave = time_sum/num_elem;
        Ecal_Hit_TimeAve->Fill(time_ave);
        for (int nt=0; nt<num_elem;++nt){
          double cal_time=calorimeter_hit->getTimeCont(nt);
          double rel_time = cal_time - time_ave;
          Ecal_Hit_TimeRel->Fill(rel_time);
        }
      }else{
        std::cout << "Error: not hit time, but there are hits. Nhits = " << num_elem << std::endl;
        Ecal_Hit_TimeAve->Fill(time_sum);
      }
      
      
      
      Ecal_Hit_CNT_ecut->Fill(count_e_cut);
      if(count_e_cut>0){
        double time_ave_ecut = time_sum_ecut/count_e_cut;
        Ecal_Hit_TimeAve_ecut->Fill(time_ave_ecut);
        for (int nt=0; nt<num_elem;++nt){
          if(calorimeter_hit->getEnergyCont(nt)>0.001){
            double cal_time=calorimeter_hit->getTimeCont(nt);
            double rel_time = cal_time - time_ave_ecut;
            Ecal_Hit_TimeRel_ecut->Fill(rel_time);
          }
        }
      }
    }
    Ecal_Hit_Energy_sum->Fill(energy_sum);
    
    N_EcalHits_E->Fill(cnt);
  }
  
//  //===== Tracker Hits =====
  if(getNTrackerHits()>0){
    EVENT::SimTrackerHit *tracker_hit;
    LCCollection *tracker_hits=static_cast<LCCollection *>(evt->getCollection("TrackerHits"));
    for(int n=0; n<tracker_hits->getNumberOfElements();++n){
      tracker_hit = GetSimTrackerHit(n,tracker_hits);
      float SVT_hit_energy=tracker_hit->getEDep();
      Tracker_Hit_Energy->Fill(SVT_hit_energy);
      const float *momentum=tracker_hit->getMomentum();
      Tracker_Hit_Px->Fill(momentum[0]);
      Tracker_Hit_Py->Fill(momentum[1]);
      Tracker_Hit_Pz->Fill(momentum[2]);
      Tracker_Hit_PathLength->Fill(tracker_hit->getPathLength());
    }
  }
  
}

bool SimAna::GetEvent(int evtnum){
  // Read event evtnum from file. This assumes the file was opened for DirectAccess.
  // The run used will the current run, or the first run in the file.
  int thisrun;
  if(evt==nullptr){
    EVENT::IntVec runs;
    lc_reader->getRuns(runs);
    thisrun=runs.at(0);
  }else{
    thisrun=evt->getRunNumber();
  }
  
  evt = lc_reader->readEvent(thisrun,evtnum);
  if(evt == nullptr){
    return false;
  }else{
    return true;
  }
}

bool SimAna::GetEvent(int runnum,int evtnum){
  // Read event evtnum from file. This assumes the file was opened for DirectAccess.
  // The run used will the current run, or the first run in the file.
  evt = lc_reader->readEvent(runnum,evtnum);
  if(evt == nullptr){
    return false;
  }else{
    return true;
  }
}

int SimAna::GetRunNumber(void){
  // Return the current run number
  if(evt) return(evt->getRunNumber());
  else    return(-1);
}

int SimAna::GetEventNumber(void){
  // Return the current event number
  if(evt) return(evt->getEventNumber());
  else return(-1);
}

bool SimAna::Next(void){
// Read the next event from the LCIO file.
  evt = lc_reader->readNextEvent();
  if(evt == nullptr){
    return false;
  }else{
    return true;
  }
}

void SimAna::OpenLCIO(const string file){
// Open an LCIO file for processing
  if( lc_reader != nullptr){
    lc_reader->open(file);
  }else{
    Init(file,false);
  }
}

int SimAna::getN(string name){
// Return the number of "" Hits in the current event.
  try{
    LCCollection *hits=static_cast<LCCollection *>(evt->getCollection(name));
    if(hits){
      int nhits=hits->getNumberOfElements();
      return(nhits);
    }else{
      return(0);
    }
  }catch(EVENT::DataNotAvailableException e){
    return(0);
  }
}

void SimAna::PrintParticle(EVENT::MCParticle *part,map<EVENT::MCParticle *,int> MC_ptoi,int level){
// For recursive printing of the particle tree.
  MCParticleVec parents=part->getParents();
  MCParticleVec daughters=part->getDaughters();
  int pdg=part->getPDG();
  int k = MC_ptoi[part];
  string space(level*4,' ');
  string out = space+"-> [%2i] PDG=(%4i) E=%8.4f GeV\n";
  printf(out.c_str(),k,pdg,part->getEnergy());
  float esum=0;
  for(int l=0;l<daughters.size();++l){
    MCParticle *d=daughters.at(l);
    PrintParticle(d,MC_ptoi,level+1);
    esum += d->getEnergy();
  }
  if(daughters.size()){
    out=space+"-----------> sum: %9.4f GeV \n";
    printf(out.c_str(),esum);
  }
}


void SimAna::Print(int mode){
// Print information about the file and/or the event

  if(evt == nullptr) return;
  
  LCCollection *mcp=nullptr;
  map<MCParticle *,int> MC_ptoi;
  
  if(mode & 0x1) LCTOOLS::dumpEvent(evt);
  if(mode & 0x2) LCTOOLS::dumpEventDetailed(evt);
  
  if(mode & 0x4 || mode & 0x8){
    try{
      mcp=static_cast<LCCollection *>(evt->getCollection("MCParticle"));
      if(mcp){
        int n=mcp->getNumberOfElements();
        map<int,int> MC_mom_daughter;
        for(int k=0;k<n;++k){
          MCParticle* part =  dynamic_cast<MCParticle*>( mcp->getElementAt( k ) ) ;
          MC_ptoi[part]=k;
        }
       }
    }catch(EVENT::DataNotAvailableException e){
      cout << "No MC Particles in event ???" << endl;
    }
  }
  
  if(mode & 0x4 && mcp!=nullptr){
    printf("======= MCParticles [%3i] =======\n",mcp->getNumberOfElements());
    for(int k=0;k<mcp->getNumberOfElements();++k){
      MCParticle* part =  dynamic_cast<MCParticle*>( mcp->getElementAt( k ) ) ;
      MCParticleVec parents=part->getParents();
      if( parents.size() == 0){                     // no parent, so this is a top particle.
        PrintParticle(part,MC_ptoi,0);
      }
    }
  }
  
  if(mode & 0x8){
    LCCollection* sch = evt->getCollection("EcalHits") ;
    if( sch->getTypeName() != LCIO::SIMCALORIMETERHIT ){
      cout << "These are not SimCalorimeterHits but " << sch->getTypeName() << endl;
      return;
    }
    // LCTOOLS::printSimCalorimeterHits(sch);
    printf("i  (ix,iy) Energy  [NMCP] \n");
    for(int i=0;i<sch->getNumberOfElements();++i){
      EVENT::SimCalorimeterHit *hit=dynamic_cast<EVENT::SimCalorimeterHit *>(sch->getElementAt(i));
      float energy=hit->getEnergy();
      int ix=GetEcalHit_ix(Cast_to_CalorimeterHit(hit));
      int iy=GetEcalHit_iy(Cast_to_CalorimeterHit(hit));
      int n_mcp=hit->getNMCContributions();
      printf("%2i (%2d,%2d) %8.5f [%2d] \n",i,ix,iy,energy,n_mcp);
      float en_sum=0;
      for(int j=0;j<n_mcp;++j){
        MCParticle *pc=hit->getParticleCont(j);
        int id=pc->id();
        int pci = MC_ptoi[pc];
        int pdg = pc->getPDG();
        float en= pc->getEnergy();
        float tim=pc->getTime();
        printf("-- Contrib[%2i] = %9i [%3i] PDG:(%4i) E=%8.5f GeV t=%11.9f \n",j,id,pci,pdg,en,tim);
        en_sum += en;
      }
      printf("------------------- en sum: %8.5f GeV \n",en_sum);
      float esum=0;
      for(int j=0;j<n_mcp;j++){
        float pE=hit->getEnergyCont(j);
        int pdg=hit->getPDGCont(j);
        float tim=hit->getTimeCont(j);
        printf("---  [%3i] = PDG:(%4i) dE=%8.4f GeV t=%11.9f\n",j,pdg,pE,tim);
        esum+=pE;
      }
      printf("------------- sum: %9.4f GeV \n",esum);
    }
    
  }
}

bool SimAna::ShowNext(int opt){
  Next();
  TH2F *hits=EcalHitMap();
  FancyPlot(hits,opt);
  return(true);
}


TH2F *SimAna::EcalHitMap(void){
  // Return a 2D histogram showing the distribution of ECal hits for this event.
  // Use:
  // ... // define ana
  // TH2F my_hitmap(ana->EcalHitMap());
  // my_hitmap.Draw()
  //
  // Mostly a copy of the code from BaseAna, but we don't close the ECAL x-gap.
  int ecal_nx=23;
  int ecal_ny=5;
  
  TH2F *hit_map =(TH2F *)gROOT->FindObject("ecal_hit_map");
  if(hit_map == nullptr){
    hit_map =  new TH2F("ecal_hit_map","Ecal Hit Map",(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  }else{
    hit_map->Reset();
  }
  
  for(int nhit=0;nhit<getNEcalHits();++nhit){
    IMPL::SimCalorimeterHitImpl *hit= GetEcalHit(nhit);
    int xhit=GetEcalHit_ix(hit);
    int yhit=GetEcalHit_iy(hit);
    //   xhit = (xhit>=0?xhit:xhit+1);
    hit_map->Fill(xhit,yhit,hit->getEnergy());
  }
  return(hit_map);
}


void SimAna::FancyPlot(TH2F *histo,int opt){
  //
  // Fancy plot of the Calorimeter.
  // Mostly a copy of the code from BaseAna.
  //
  // opt:
  //    bit0 = 0x1  - If set do not draw the z index.
  //    bit1 = 0x2  - If set, draw x-axis in mm.
  //    bit2 = 0x4  - If set, draw x-axis in both mm and index
  //    bit3 = 0x8  - If set, do not mess with Maximum()
  //
  //this defines the position of the top right region of big boxes, others will fall in by symmetry
  int ecal_x_first=1;
  int ecal_nx_first=-1;
  int ecal_y_first=1;
  int ecal_ny_first=-1;
  
  int ecal_nx=23;
  int ecal_ny=5;
  
  TAxis *xax=histo->GetXaxis();
  TAxis *yax=histo->GetYaxis();
  
  TH2F *ones_lb;
  
  if (!gROOT->FindObject(TString(histo->GetName()).Append("_oneslb"))){//if this one exists all the rest probably exist too
    ones_lb=new TH2F(TString(histo->GetName()).Append("_oneslb"),"oneslb",(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  }
  else {
    ones_lb= (TH2F*) gROOT->FindObject(TString(histo->GetName()).Append("_oneslb"));
    ones_lb->Clear();
  }
  
  if(!(opt& 0x8) ){
    histo->SetMaximum();
    if(histo->GetMaximum() < 1){
      histo->SetMaximum(1.1);
    }
  }
  
  float SetMax=histo->GetMaximum();
  
  xax=ones_lb->GetXaxis();
  yax=ones_lb->GetYaxis();
  
  //this chunk of code just puts the grid in the right place
  for (int i=0;i<ecal_nx;i++){
    for (int j=0;j<ecal_ny;j++){
      
      ones_lb->SetBinContent(xax->FindBin(ecal_x_first+i),yax->FindBin(ecal_y_first+j),1.);
      ones_lb->SetBinContent(xax->FindBin(ecal_x_first+i),yax->FindBin(ecal_ny_first-j),1.);
      if(j==0 && i>0 && i<10){
        ;
      }else{
        ones_lb->SetBinContent(xax->FindBin(ecal_nx_first-i),yax->FindBin(ecal_ny_first-j),1.);
        ones_lb->SetBinContent(xax->FindBin(ecal_nx_first-i),yax->FindBin(ecal_y_first+j),1.);
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

