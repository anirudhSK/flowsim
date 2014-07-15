#ifndef FLOWGENERATOR_HH
#define FLOWGENERATOR_HH

#include <memory>
#include "flow.hh"
#include "random.hh"
#include "distribution.hh"

class FlowGenerator
{
private:
  unsigned int _flow_count {0};
  std::unique_ptr< Distribution > _start_distribution {nullptr};
  std::unique_ptr< Distribution > _stop_distribution {nullptr};
  double _next_flow_start;

public:
  FlowGenerator( const double & mean_on_duration,
                 const double & mean_off_duration,
                 const DistType & dist_type,
                 PRNG & s_prng );
  template <class NextHop>
  void tick( NextHop & next, const double & tickno );
  double next_event_time( const double & tickno __attribute__ ((unused)) ) const { return _next_flow_start; }
  unsigned int flow_count( void ) const { return _flow_count; }
};

#endif  // FLOWGENERATOR_HH
