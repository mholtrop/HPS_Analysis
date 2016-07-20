// Load required library - Requires ROOT 6.04+

void Run_Job(void){
  // gStyle->SetOptStat(0);

  cout << "At top \n";

  TProof *proof=TProof::Open("lite://");
  proof->Exec("gSystem->Load(\"/net/home/maurik/Analysis/HPS_Analysis/lib/libPiZero.so\")");
  TChain *ch = new TChain("HPS_Event");
  ch->SetProof(proof);
  ch->Add("/data/HPS/engrun2015/pass6/dst/hps_00577*.root");
  cout << "Number of entries in chain = " << ch->GetEntries() << endl;
  PiZero *pz=new PiZero();
  TStopwatch timer;
  timer.Start();
  ch->Process("PiZero()");
  timer.Stop();
  timer.Print();
}

