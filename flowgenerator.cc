#include "flowgenerator.hh"

FlowGenerator::FlowGenerator( const double & mean_on_duration,
                              const double & mean_off_duration,
                              PRNG & s_prng )
  : _start_distribution( 1.0 / mean_off_duration, s_prng ),
    _stop_distribution( 1.0 / mean_on_duration, s_prng )
{}
