{
  // gSystem->Load("Release/libPiZero.dylib"))
  // gStyle->SetOptStat(0);
  TProof *proof=TProof::Open("lite://");
  proof->Exec("gSystem->Load(\"/Users/maurik/Documents/Analysis/HPS/Release/libPiZero.dylib\")");
  TChain *ch = new TChain("HPS_Event");
  ch->SetProof(proof);
  ch->Add("/data/HPS/engrun2015/pass6/dst/hps_*.root");
  cout << "Number of entries in chain = " << ch->GetEntries() << endl;
  PiZero *pz=new PiZero();
  
}

