// Load required library - Requires ROOT 6.04+

void Proof_Process(long num_proc= 0){
  // gStyle->SetOptStat(0);

  cout << "At top \n";

  TProof *proof=TProof::Open("lite://");
  proof->Exec("gSystem->Load(\"/Users/maurik/Documents/Analysis/HPS_Analysis/Release/libFullEElec.dylib\")");
  TChain *ch = new TChain("HPS_Event");
  ch->SetProof(proof);
  ch->Add("/data/HPS/engrun2015/pass6/dst/hps_00577*.root");
  long tot_entries =  ch->GetEntries();
  if(num_proc <= 0) num_proc = tot_entries;
  cout << "Number of entries in chain = " << tot_entries << " Processing " << num_proc << " entries " << endl;
  FullEElec *el=new FullEElec(ch);
  TStopwatch timer;
  timer.Start();
  ch->Process("FullEElec()","",num_proc);
  timer.Stop();
  timer.Print();
}

