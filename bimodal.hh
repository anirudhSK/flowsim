#ifndef BIMODAL_HH
#define BIMODAL_HH

#include <boost/random/exponential_distribution.hpp>
#include "distribution.hh"
#include "random.hh"

class BiModal : public Distribution
{
private:
  boost::random::uniform_real_distribution<> distribution;

  PRNG & prng;

  double _threshold;

public:
  BiModal( const double & mean, PRNG & s_prng )
   : distribution( 0, 1.0 ),
     prng( s_prng ),
     _threshold( mean / 1000.0 ) {}

  double sample( void ) override { return distribution( prng ) < _threshold ? 1000.0 : 1.0; }
};

#endif  // BIMODAL_HH
