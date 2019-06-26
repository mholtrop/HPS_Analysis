//
//  EcalCluster.h
//  SimAna
//
//  Created by Maurik Holtrop on 6/12/19.
//

#ifndef EcalCluster_h
#define EcalCluster_h

#include "SimAna.h"
#include <stdio.h>

class EcalCluster: public TObject {
  
public:
  SimAna *sim_ana;
  int seed_index;
  vector<short>  hit_indexes;
  
public:
  EcalCluster(){};
  EcalCluster(SimAna *sa): sim_ana(sa) {};
  
  ClassDef(EcalCluster,1);
};


#endif /* EcalCluster_h */
