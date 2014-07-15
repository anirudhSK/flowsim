#ifndef EXPONENTIAL_HH
#define EXPONENTIAL_HH

#include <boost/random/exponential_distribution.hpp>

#include "distribution.hh"
#include "random.hh"

class Exponential : public Distribution
{
private:
  boost::random::exponential_distribution<> distribution;

  PRNG & prng;

public:
  Exponential( const double & mean, PRNG & s_prng ) : distribution( 1.0 / mean ), prng( s_prng ) {}

  double sample( void ) override { return distribution( prng ); }
};

#endif
