#include "util.hh"

std::unique_ptr<Distribution> get_dist( const DistType & dist_type, const double & mean, PRNG & prng )
{
  if ( dist_type == DistType::PARETO ) {
    return static_cast<std::unique_ptr<Distribution>>(new Pareto( mean, prng ));
  } else if ( dist_type == DistType::EXPONENTIAL ) {
    return static_cast<std::unique_ptr<Distribution>>(new Exponential( mean, prng ));
  } else {
    return static_cast<std::unique_ptr<Distribution>>(new BiModal( prng ));
  }
} 
