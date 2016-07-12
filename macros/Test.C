{
  //  gSystem->Load("Release/libPiZero.dylib");
  gStyle->SetOptStat(0);
  //  TFile f("/data/HPS/engrun2015/pass6/dst/hps_005772.10_dst_R3.8.root");
  TFile f("test.root");
  TTree *t=(TTree *)f.Get("HPS_Event");
  PiZero *pz=new PiZero(t);
  int nevt=0;
  pz->GetEntry(nevt);
  //  TH2F *ehit = new TH2F(pz->EcalHitMap());
  //  TH2F *ha;
  TCanvas *c1=new TCanvas("c1","c1",1000,800);
  TCanvas *c2=new TCanvas("c2","c2",1000,800);
  TH2F *hits;
}

void ShowNext(){
  c1->cd();
  pz->GetEntry(++nevt);
  pz->Print("all");
  pz->DrawEcal();
  c2->cd();
  hits=pz->EcalHitMap();
  hits->Draw("lego2");
}
