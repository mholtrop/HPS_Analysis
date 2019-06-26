//
//  SimpleHists.cpp
//  SimAna
//
//  Created by Maurik Holtrop on 5/27/19.
//  Copyright © 2019 Maurik. All rights reserved.
//

#include "SimpleHists.h"

ClassImp(SimpleHists);

SimpleHists::SimpleHists(const string &lcio_file_name,bool direct_access,double max_E): SimAna(lcio_file_name,direct_access,max_E){
    SetName("SimpleHists");
    SetTitle("LCIO output analyzer.");
    InitHists(max_E);
}

void SimpleHists::InitHists(double max_E){
  
  // ====== Book Histograms ========
  cout << "Booking Histograms \n";
  // ====== ECAL
  N_EcalHits = new TH1F("N_EcalHits","Number of ECAL Hits;Number of hits",100,0,99);
  N_EcalHits_E= new TH1F("N_EcalHits_E","Number of ECAL Hits with E>50MeV;Number of hits",100,0,99);
  
  N_MCParticle = new TH1F("N_MCParticle","Number of MC Particles;Number of particles",200,0,199);
  E_MCParticle = new TH1F("E_MCParticle","Energy of MC Particle;E (GeV);count",1000,0.,max_E);
  E_MCParticle2 = new TH1F("E_MCParticle2","Energy of MC Particle;E (GeV);count",1000,0.,0.01);
  N_TrackerHits = new TH1F("N_TrackerHits","Number of Tracker Hits;Number of hits",100,0,99);
  N_TrackerHitsEcal = new TH1F("N_TrackerHitsEcal","Number of Tracker Hits Ecal;Number of hits",100,0,99);
  
  Ecal_Hit_Energy = new TH1F("Ecal_Hit_Energy","Ecal Hit energy;Energy (GeV)",2300,0.,max_E);
  Ecal_Hit_CE     = new TH1F("Ecal_Hit_CE","Ecal Hit Contributor energy;Energy (GeV)",2300,0.,max_E);
  Ecal_Hit_Energy_sum = new TH1F("Ecal_Hit_Energy_sum","Ecal Hit energy sum;Energy (GeV)",2300,0.,max_E);
  
  int ecal_nx=23;
  int ecal_ny=5;
  Ecal_Hits_Loc = new TH2F("Ecal_Hits_Loc","Ecal Hits;x-index;y-index",(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  Ecal_Hits_Loc_E = new TH2F("Ecal_Hits_Loc_E","Ecal Hits with E>50MeV;x-index;y-index",(ecal_nx+1)*2+1,-ecal_nx-1.5,ecal_nx+2-0.5,(ecal_ny+1)*2+1,-ecal_ny-1.5,ecal_ny+1.5);
  
  Ecal_Hit_CNT  = new TH1F("Ecal_Hit_CNT","Num of Ecal hit contributions;N;counts",1000,0.,1000.);
  Ecal_Hit_Time = new TH1F("Ecal_Hit_Time","Ecal hit time;time;counts",1000,-1000.,1000.);
  Ecal_Hit_TimeAve = new TH1F("Ecal_Hit_TimeAve","Ecal hit time average per event;time;counts",1000,-1000.,1000.);
  Ecal_Hit_TimeRel = new TH1F("Ecal_Hit_TimeRel","Ecal hit time relative to average;time;counts",1000,-2000.,1000.);
  
  Ecal_Hit_CNT_ecut  = new TH1F("Ecal_Hit_CNT_ecut","Num of Ecal hit contributions;N;counts",1000,0.,5000.);
  Ecal_Hit_Time_ecut = new TH1F("Ecal_Hit_Time_ecut","Ecal hit time;time;counts",1000,-1000.,1000.);
  Ecal_Hit_TimeAve_ecut = new TH1F("Ecal_Hit_TimeAve_ecut","Ecal hit time average per event;time;counts",1000,-1000.,1000.);
  Ecal_Hit_TimeRel_ecut = new TH1F("Ecal_Hit_TimeRel_ecut","Ecal hit time relative to average;time;counts",1000,-2000.,1000.);
  
  // ====== Hodoscope
  N_HodoHits = new TH1F("N_HodoHits","Number of Hodoscope Hits;Number of Hits",100,0,99);
  Hodo_Hit_Energy = new TH1F("Hodo_Hit_Energy","Energy of Hodoscope Hit;Energy (GeV)",1000,0.,0.05);
  
  // ==== Tracker
  Tracker_Hit_Energy = new TH1F("Tracker_Hit_Energy","SVT Hit Energy Deposited;Energy (GeV)",1000,0.,0.01);
  Tracker_Hit_PathLength= new TH1F("Tracker_Hit_PathLength","Tracker Hit Path Length;L (mm)",1000,0.32,0.33);
  Tracker_Hit_Px = new TH1F("Tracker_Hit_Px","Tracker Hit x Momentum;Px (GeV/c)",1000,0.,0.1);
  Tracker_Hit_Py = new TH1F("Tracker_Hit_Py","Tracker Hit y Momentum;Py (GeV/c)",1000,0.,0.15);
  Tracker_Hit_Pz = new TH1F("Tracker_Hit_Pz","Tracker Hit z Momentum;Pz (GeV/c)",1000,0.,max_E);

}


void SimpleHists::FillHistos(void){
  // Fill all the histograms with the data from the current event.
  // This is where the work is done.
  
  N_EcalHits->Fill(getNEcalHits());
  N_HodoHits->Fill(getNHodoHits());
  N_MCParticle->Fill(getNMCParticle());
  N_TrackerHits->Fill(getNTrackerHits());
  N_TrackerHitsEcal->Fill(getNTrackerHitsEcal());
  
  // ========= MCParticle ==============
  
  if(getNMCParticle()>0){
    IMPL::MCParticleImpl *mcp;
    LCCollection *mc_particles = static_cast<LCCollection *>(evt->getCollection("MCParticle"));
    for(int i=0;i<mc_particles->getNumberOfElements();++i){
      mcp = GetMCParticle(i,mc_particles);
      E_MCParticle->Fill(mcp->getEnergy());
      E_MCParticle2->Fill(mcp->getEnergy());
    }
  }
  
  
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
  //========== Hodoscope Hits ==============
  cnt=0;
  if(getNHodoHits()>0){
    EVENT::SimTrackerHit *hodo_hit;
    LCCollection *hodo_hits=static_cast<LCCollection *>(evt->getCollection("HodoscopeHits"));
    for(int n=0; n<hodo_hits->getNumberOfElements();++n){
      hodo_hit = GetHodoHit(n,hodo_hits);
      Hodo_Hit_Energy->Fill(hodo_hit->getEDep());
    }
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
