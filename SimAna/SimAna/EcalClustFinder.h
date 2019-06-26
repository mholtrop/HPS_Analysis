//
//  EcalClustFinder.h
//  SimAna
//
//  Created by Maurik Holtrop on 6/12/19.
//

#ifndef EcalClustFinder_h
#define EcalClustFinder_h

#include "SimAna.h"
#include "EcalCluster.h"
#include <stdio.h>

class EcalClustFinder: public TObject{
public:
  SimAna *sim_ana;
  map<pair<short,short>,short> hit_map;      // Store ix,iy to hit_index
  vector<EcalCluster *> clusters;
  
public:
  EcalClustFinder(){};
  EcalClustFinder(SimAna *sa): sim_ana(sa){};
  
  ClassDef(EcalClustFinder,1);
};

#endif /* EcalClustFinder_hpp */
