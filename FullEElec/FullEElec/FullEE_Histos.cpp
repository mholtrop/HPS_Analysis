/*!
 * \class  FullEE_Histos
 * \brief  Histogram collection to allow for different versions with different cuts.
 * \author Maurik Holtrop (maurik@physics.unh.edu)
 * \date   2016
 *
 */
#include "FullEE_Histos.h"

///
/// Constructor:
///
/// This class needs to know who owns the histograms, because it needs to add them
/// to the fOutput when they are being booked. It also needs to know what directory
/// they need to be put in. A blank name will put them in the root directory.
///
/// @param mother The BaseAna class that contains the histograms.
/// @param name   The name of this collection of histograms = directory where histos are stored.
///
FullEE_Histos::FullEE_Histos(BaseAna *mom,const char *nam){

  mother = mom;
  name = nam;              // Does not check if name already exists in this direcory.
  cluster_energy=nullptr;
  cluster_loc=nullptr;
  track_energy=nullptr;
  track_chi2 = nullptr;
  cluster_track_match_dxdy=nullptr;
  cluster_track_match_dr=nullptr;
  cluster_track_match_de=nullptr;
}

///
/// Book the histograms under the directory provided by name.
///
void FullEE_Histos::Book(void){
  
  TDirectory::TContext ctxt; // Will restore gDirectory to its 'current' value at the end of this scope/function
  TList *fOutput = mother->GetOutputList();
  gDirectory->cd("/");
  TDirectory *hdir=gDirectory->mkdir(name);
  gDirectory->cd(name);

//  TList *hdir = new TList();
//  hdir->SetName(name);
  hdir->Add(cluster_energy = new TH1F("cluster_energy","Cluster Energy",2800,0.,2.8));
  hdir->Add(cluster_loc = new TH2F("cluster_loc","Cluster location",350,-330,370,180,-90,90));
  hdir->Add(track_energy= new TH1F("track_energy","Track energy",2800,0.,2.8));
  hdir->Add(track_chi2  = new TH1F("track_chi2","Track Chi Squared",200,0.,100.));
  hdir->Add(cluster_track_match_dxdy = new TH2F("cluster_track_match_dxdy","Cluster - Track match location, dx ,dy",600,-30,30,600,-30,30));
  hdir->Add(cluster_track_match_dr = new TH1F("cluster_track_match_dr","Cluster - Track match location, dr",500,0,50));
  hdir->Add(cluster_track_match_de = new TH1F("cluster_track_match_de","Cluster - Track match Energy, de",1000,-0.5,0.5));

  fOutput->Add(hdir);
  
  gDirectory->cd("/");
}

///
/// Copy the data, but not the histograms, from one FullEE_Histos object to another.
/// This is useful if it allows you to avoid duplicating the calculations required for Fill().
/// The function checks if this==data, so it won't copy onto itself.
/// @param data  The data to be copied.
/// @return      Boolean true if copy was succesfull.
bool FullEE_Histos::Copy(FullEE_Histos *data){
  if( this == data) return(false);
  
  clus_energy = data->clus_energy;
  trk_energy= data->trk_energy;
  delta_energy= data->delta_energy;
  
  trk_chi2 = data->trk_chi2;
  
  clus_pos = data->clus_pos;
  trk_pos= data->trk_pos;
  
  delta_x = data->delta_x;
  delta_y = data->delta_y;
  delta_r = data->delta_r;
  
  return true;
}
///
/// Fill the histograms with the information found from the HpsParticle *part.
/// It does this by computing the required information and then calling FillHistos()
/// @param part The particle to make the histograms for.
/// @return     Boolean true is histograms filled, false it not.
///
bool FullEE_Histos::Fill(HpsParticle *part){
  
  TRefArray *clus_ref = part->getClusters();
  TRefArray *track_ref= part->getTracks();
  if(clus_ref->GetEntries() != 1 || track_ref->GetEntries() != 1) return(false);
  
  EcalCluster *clus = (EcalCluster *)clus_ref->At(0);
  SvtTrack *track = (SvtTrack *) track_ref->At(0);
  
  trk_chi2  = track->getChi2();
  
  clus_energy =  clus->getEnergy();
  trk_energy = mother->GetAbsMomentum(part);
  
  delta_energy = clus_energy - trk_energy;
  
  clus_pos = clus->getPosition();
  trk_pos = track->getPositionAtEcal();
  
  delta_x = clus_pos[0] - trk_pos[0];
  delta_y = clus_pos[1] - trk_pos[1];
  delta_r = sqrt( delta_x*delta_x + delta_y*delta_y );

  FillHistos(nullptr);
  
  return(true);
}

/// Fill the histograms
/// If data is not nullptr and data != this, then first copy the info
/// from data into this class, then fill the histograms.
///
bool FullEE_Histos::FillHistos(FullEE_Histos *data){

  if(data != nullptr && data != this) Copy(data);
  
  cluster_energy->Fill(clus_energy);
  cluster_loc->Fill(clus_pos[0],clus_pos[1]);
  track_chi2->Fill(trk_chi2);
  cluster_track_match_dxdy->Fill(delta_x,delta_y);
  cluster_track_match_dr->Fill(delta_r);
  cluster_track_match_de->Fill(delta_energy);
  
  return(true);
}

ClassImp(FullEE_Histos);

