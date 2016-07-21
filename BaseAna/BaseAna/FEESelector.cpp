/*!
 * \class  FEESelector
 * \brief  The base analysis class for hps-dst based analysis projects.
 * \author Maurik Holtrop (maurik@physics.unh.edu)
 * \date   2016
 *
 * This is a simple selector class. Initialize it with a pointer to a BaseAna class or "this" for the
 * Process() method in a BaseAna derived class. Then the Select() method will return true if the event
 * passed the selection criteria.
 */

#include "FEESelector.h"

ClassImp(FEESelector);

///
/// Standard constuctor.
/// This constructor should *always* be called with a BasaAna derived pointer.
///
/// Default parameters will be set with Set_Engineering2015().
///
FEESelector::FEESelector(BaseAna *ba): base(ba){
  Set_Engineering2015();
};



///
/// Set the defaults for the Engineering run 2015 for clean (calibration) FEE.
/// These are the default values used for this class.
///
void FEESelector::Set_Engineering2015(void){
  hitcut = 0;                  // default is not to require a min hit
  min_seed_hit_energy = 0.4;   // Default for clean FEE for 1.05 GeV data
  min_cluster_energy  = 0.6;   // Default for clean FEE for
  min_seed_cluster_ratio = 0.4;
  min_time  = 30.;
  max_time  = 70.;
};

