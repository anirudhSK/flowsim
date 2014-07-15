#include "util.hh"
#include "flowgenerator.hh"

FlowGenerator::FlowGenerator( const double & mean_on_duration,
                              const double & mean_off_duration,
                              const DistType & dist_type,
                              PRNG & s_prng )
  : _start_distribution( get_dist( dist_type, mean_off_duration, s_prng ) ),
    _stop_distribution( get_dist( dist_type, mean_on_duration, s_prng ) ),
    _next_flow_start( _start_distribution->sample() )
{}
