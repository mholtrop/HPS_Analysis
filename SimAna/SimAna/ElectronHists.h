//
//  ElectronHists.h
//  SimAna
//
//  Created by Maurik Holtrop on 5/27/19.
// 
//

#ifndef ElectronHists_h
#define ElectronHists_h

#include "SimAna.h"
#include "EHistCol.h"

using namespace std ;
using namespace lcio ;


class ElectronHists: public SimAna {
  // Collection of EHistCol plots
  
public:
  vector<EHistCol *> hist_store;

public:
  ElectronHists(){ };
  ElectronHists(string lcio_file_name,bool direct_access=false, double max_E=5.0);
  
  virtual void InitHists(double max_E);
  virtual void FillHistos(void);
  virtual vector<EHistCol *> *GetHistColVec(void){
    return(&hist_store);
  }
  virtual EHistCol *GetHistCol(int i){
    return(hist_store[i]);
  }
  ClassDef(ElectronHists,1);
};

#endif /* ElectronHists.h */
