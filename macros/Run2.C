{
  if(gSystem->Load("Release/libPiZero.dylib")){
    cout << "Could not load the PiZero library?\n";
  }
  // gStyle->SetOptStat(0);
  TChain *ch = new TChain("HPS_Event");
  ch->Add("/data/HPS/engrun2015/pass6/dst/hps_*.root");
  cout << "Number of entries in chain = " << ch->GetEntries() << endl;
  PiZero *pz=new PiZero(ch);
  pz->SlaveBegin(ch);
  pz->Run();
  TCanvas *c1=new TCanvas("c1","c1",1000,800);
  TCanvas *c2=new TCanvas("c2","c2",1000,800);
  c1->cd();
  pz->pizero_mass->Draw();
  c2->cd();
  pz->pizero_theta->Draw();
}

