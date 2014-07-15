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

public:
  BiModal( PRNG & s_prng )
   : distribution( 0, 1.0 ),
     prng( s_prng ) {}

  double sample( void ) override { return distribution( prng ) < 0.5 ? 1 : 1000; }
};

#endif  // BIMODAL_HH
