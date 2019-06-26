//
//  ElectronHists.cpp
//  SimAna
//
//  Created by Maurik Holtrop on 5/27/19.
//
//

#include "EHistCol.h"

ClassImp(EHistCol);

EHistCol::EHistCol(SimAna *sim_ana,double thres): sa{sim_ana},threshold(thres){
  InitHists(sa->Max_Energy);
}


void EHistCol::InitHists(double max_E){

  int ecal_nx=23;
  int ecal_ny=5;

  // ====== Book Histograms ========
  // ====== ECAL
  Histadd(N_TrackerHits = new TH1F("N_TrackerHits","Number of Tracker Hits;Number of hits",21,0,20));

  Histadd(N_MCParticle = new TH1F("N_MCParticle","Number of MC Particles;Number of particles;N",200,0,199));
  Histadd(E_PrimaryElec = new TH1F("E_PrimaryElec","Energy of Primary MC Particle;E (GeV);count",1000,0.,max_E));
  Histadd(Loc_PrimaryElec = new TH2F("Loc_PrimaryElec","Location of e-, projected to 150cm.;x (cm);y (cm)",271,-50.,50.,36,0.,12.));
  Histadd(ThPh_PrimaryElec =new TH2F("ThPh_PrimaryElec","Direction of e-, projected;theta (rad);phi (rad)",300,0.,45./150,360,0.,Pi/2));

  Histadd(N_EcalHits = new TH1F("N_EcalHits","Number of ECAL Hits;Number of hits",100,0,99));
  Histadd(Ecal_Hit_Energy = new TH1F("Ecal_Hit_Energy","Ecal Hit energy;Energy (GeV)",2300,0.,max_E));
  Histadd(Ecal_Hits_Loc = new TH2F("Ecal_Hits_Loc","Ecal Hits;x-index;y-index",(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5));
  
  Histadd(Ecal_Seed_Energy  = new TH1F("Ecal_Seed_Energy","Energy for ECal GTP Cluster",2000,0.,max_E));
  Histadd(Ecal_Seed_Loc = new TH2F("Ecal_Seed_Loc","Ecal Hits;x-index;y-index",(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5));
  Histadd(Ecal_Seed_Locx_vs_Primx = new TH2F("Ecal_Seed_Locx_vs_Primx","Ecal Seed x vs Thrown x;x;x-index",101,-50.,50.,(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5));
  Histadd(Ecal_Seed_Locy_vs_Primy = new TH2F("Ecal_Seed_Locy_vs_Primy","Ecal Seed y vs Thrown y;y;y-index",101,-50.,50.,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5));

  Histadd(Ecal_GTP_Energy  = new TH1F("Ecal_GTP_Energy","Energy for ECal GTP Cluster",2000,0.,max_E));
  
  Histadd(Ecal_Total_Energy  = new TH1F("Ecal_Total_Energy","Energy for entire ECal",2000,0.,max_E));
  
  Histadd(E_Seed_E_Ecal = new TH2F("E_Seed_E_Ecal","Ecal energy vs E e-",200,0.,max_E,200,0.,max_E));
  Histadd(E_Prim_E_Ecal = new TH2F("E_Prim_E_Ecal","Ecal energy vs E e-",200,0.,max_E,200,0.,max_E));
  Histadd(E_Prim_E_Seed = new TH2F("E_Prim_E_Seed","Ecal Seed energy vs E e-",200,0.,max_E,200,0.,max_E));
  
  // ====== Hodoscope
  Histadd(N_HodoHits = new TH1F("N_HodoHits","Number of Hodoscope Hits;Number of Hits",100,0,99));
  Histadd(Hodo_Hit_Energy = new TH1F("Hodo_Hit_Energy","Energy of Hodoscope Hit;Energy (GeV)",1000,0.,0.05));
}


void EHistCol::FillHistos(){
  // Fill all the histograms with the data from the current event.
  // This is where the work is done.
  
  double energy_sum=0;
  double max_hit_energy=0;
  int    max_hit_index=-1;

  if(sa->getNEcalHits()>0){
    IMPL::SimCalorimeterHitImpl *calorimeter_hit;
    LCCollection *ecal_hits=static_cast<LCCollection *>(sa->evt->getCollection("EcalHits"));
    int max=ecal_hits->getNumberOfElements();
    for(int n = 0; n<max;++n){
      calorimeter_hit = sa->GetEcalHit(n,ecal_hits);
      energy_sum+=calorimeter_hit->getEnergy();
      if( max_hit_energy<calorimeter_hit->getEnergy()){
        max_hit_energy = calorimeter_hit->getEnergy();
        max_hit_index=n;
      }
    }
  }

  if(max_hit_energy<threshold) return;
  
  N_EcalHits->Fill(sa->getNEcalHits());
  N_HodoHits->Fill(sa->getNHodoHits());
  N_MCParticle->Fill(sa->getNMCParticle());
  N_TrackerHits->Fill(sa->getNTrackerHits());

  // ========= MCParticle ==============
  double x =0;
  double y =0;
  double E_prime_e=0.;
  if(sa->getNMCParticle()>0){
    IMPL::MCParticleImpl *mcp;
    LCCollection *mc_particles = static_cast<LCCollection *>(sa->evt->getCollection("MCParticle"));
    mcp = sa->GetMCParticle(0,mc_particles);  // This should be the primary electron (postiron)
    if(mcp->getPDG() != 11) std::cout << "Primary particle is not an electron! \n";
    E_prime_e = mcp->getEnergy();
    E_PrimaryElec->Fill(E_prime_e);
    const double *mom=mcp->getMomentum();
    double ptot = sqrt(mom[0]*mom[0]+mom[1]*mom[1]+mom[2]*mom[2]);
    double theta = acos(mom[2]/ptot);
    double phi   = atan2(mom[1],mom[0]);
    x  = 150.*tan(theta)*cos(phi);
    y  = 150.*tan(theta)*sin(phi);

    Loc_PrimaryElec->Fill(x,y);
    ThPh_PrimaryElec->Fill(theta,phi);
//    for(int i=0;i<mc_particles->getNumberOfElements();++i){
//      mcp = GetMCParticle(i,mc_particles);
//      E_MCParticle->Fill(mcp->getEnergy());
//      E_MCParticle2->Fill(mcp->getEnergy());
//    }
  }
  
  
  //========== ECAL Hits ==============
  int cnt=0;
  if(sa->getNEcalHits()>0){
    IMPL::SimCalorimeterHitImpl *calorimeter_hit;
    LCCollection *ecal_hits=static_cast<LCCollection *>(sa->evt->getCollection("EcalHits"));
    int max=ecal_hits->getNumberOfElements();
    for(int n = 0; n<max;++n){
      calorimeter_hit = sa->GetEcalHit(n,ecal_hits);
      Ecal_Hit_Energy->Fill(calorimeter_hit->getEnergy());
      Ecal_Hits_Loc->Fill(sa->GetEcalHit_ix(calorimeter_hit),sa->GetEcalHit_iy(calorimeter_hit));
    }

    Ecal_Seed_Energy->Fill(max_hit_energy);
    calorimeter_hit = sa->GetEcalHit(max_hit_index);
    Ecal_Seed_Loc->Fill(sa->GetEcalHit_ix(calorimeter_hit),sa->GetEcalHit_iy(calorimeter_hit));
    Ecal_Seed_Locx_vs_Primx->Fill(x,sa->GetEcalHit_ix(calorimeter_hit));
    Ecal_Seed_Locy_vs_Primy->Fill(y,sa->GetEcalHit_iy(calorimeter_hit));
    Ecal_Total_Energy->Fill(energy_sum);
    
    E_Seed_E_Ecal->Fill(E_prime_e,max_hit_energy);
    E_Prim_E_Ecal->Fill(E_prime_e,energy_sum);
    E_Prim_E_Seed->Fill(E_prime_e,max_hit_energy);
    
  }
  //========== Hodoscope Hits ==============
  cnt=0;
  if(sa->getNHodoHits()>0){
    EVENT::SimTrackerHit *hodo_hit;
    LCCollection *hodo_hits=static_cast<LCCollection *>(sa->evt->getCollection("HodoscopeHits"));
    for(int n=0; n<hodo_hits->getNumberOfElements();++n){
      hodo_hit = sa->GetHodoHit(n,hodo_hits);
      double energy=hodo_hit->getEDep();
      Hodo_Hit_Energy->Fill(energy);
    }
  }
  
}

void EHistCol::Histadd(TH1F *h){
  // Add a 1D hist.
  hists.push_back(static_cast<TH1 *>(h));
}

void EHistCol::Histadd(TH2F *h){
  // Add a 2D hist.
  hists.push_back(static_cast<TH1 *>(h));
}

void EHistCol::Print(void){
  // Print the list of histos.
  for(int i=0; i<hists.size();++i){
      cout<< hists[i]->GetName() << "  " << hists[i]->GetTitle() << endl;
  }
}
