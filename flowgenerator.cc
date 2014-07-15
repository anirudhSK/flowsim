#include "exponential.hh"
#include "pareto.hh"
#include "flowgenerator.hh"

FlowGenerator::FlowGenerator( const double & mean_on_duration,
                              const double & mean_off_duration,
                              const DistType & dist_type,
                              PRNG & s_prng )
  : _start_distribution( dist_type == DistType::PARETO ? static_cast<std::unique_ptr<Distribution>>( new Pareto( mean_off_duration, s_prng ) )
                                                       : static_cast<std::unique_ptr<Distribution>>( new Exponential( mean_off_duration, s_prng ) ) ),
    _stop_distribution( dist_type == DistType::PARETO ? static_cast<std::unique_ptr<Distribution>>( new Pareto( mean_on_duration, s_prng ) )
                                                      : static_cast<std::unique_ptr<Distribution>>( new Exponential( mean_on_duration, s_prng ) ) ),
    _next_flow_start( _start_distribution->sample() )
{}
