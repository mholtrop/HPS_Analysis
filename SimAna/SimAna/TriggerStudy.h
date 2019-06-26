//
//  TriggerStudy.hpp
//  SimAna
//
//  Created by Maurik Holtrop on 6/12/19.
//

#ifndef TriggerStudy_h
#define TriggerStudy_h

#include <stdio.h>
#include "SimAna.h"
#include "TrigHistCol.h"

class TriggerStudy: public SimAna {

public:
  vector<TrigHistCol *> hist_store;

public:
  TriggerStudy(){ };
  TriggerStudy(string lcio_file_name,bool direct_access=false, double max_E=5.0);

  virtual void InitHists(double max_E);
  virtual void FillHistos(void);

  virtual vector<TrigHistCol *> *GetHistColVec(void){
    return(&hist_store);
  }
  virtual TrigHistCol *GetHistCol(int i){
    return(hist_store[i]);
  }
  ClassDef(TriggerStudy,1);

};

#endif /* TriggerStudy_hpp */
