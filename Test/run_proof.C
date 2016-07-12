{
  gSystem->Load("/Users/maurik/Documents/Analysis/HPS/Release/libPiZero.dylib");
  gSystem->Load("/Users/maurik/Documents/Analysis/HPS/Release/libPiZero.dylib");
  gSystem->AddIncludePath("-I\"/Users/maurik/Documents/Analysis/HPS/hps-dst/include/hps_event\" ");
  gROOT->LoadMacro("Test.C++");
  TChain *ch=new TChain("HPS_Event");
  ch->Add("/data/HPS/engrun2015/pass6/dst/hps_005772.10_dst_R3.8.root");
  ch->Add("/data/HPS/engrun2015/pass6/dst/hps_005772.11_dst_R3.8.root");
  TProof *pl = TProof::Open("lite://");
  gProof->Exec("gSystem->Load(\"/Users/maurik/Documents/Analysis/HPS/Release/libPiZero.dylib\")");
  gProof->Exec("gSystem->AddIncludePath(\"-I\\\"/Users/maurik/Documents/Analysis/HPS/hps-dst/include/hps_event\\\"\")");
  gProof->Exec("gSystem->AddIncludePath(\"-I\\\"/Users/maurik/Documents/Analysis/HPS/Test\\\"\")");
  gProof->Exec("gSystem->Load(\"/Users/maurik/Documents/Analysis/HPS/Test/Test_C.so\")");
  ch->SetProof(gProof);
  ch->Process("Test()");
}
