#ifndef PARETO_HH
#define PARETO_HH

#include <boost/random/uniform_real_distribution.hpp>

#include "random.hh"

class Pareto : public Distribution
{
private:
  boost::random::uniform_real_distribution<> distribution;

  PRNG & prng;

  double _shape;
  double _scale;

public:
  /* shape = 2 for convenience */
  /* mean = 2 * scale / ( 2 - 1) = 2 * scale */
  /* scale = mean / 2.0 */ 
  Pareto( const double & mean, PRNG & s_prng )
   : distribution( 0, 1.0 ),
     prng( s_prng ),
     _shape( 2.0 ),
     _scale( mean / 2.0 ) {}
  
  double sample( void ) override { return _scale / pow( distribution( prng ), 1 / _shape ); }
};

#endif
