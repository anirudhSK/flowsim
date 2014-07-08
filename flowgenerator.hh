#ifndef FLOWGENERATOR_HH
#define FLOWGENERATOR_HH

#include "flow.hh"
#include "exponential.hh"

class FlowGenerator
{
private:
  unsigned int _flow_count {0};
  double _next_flow_start {0.0};
  Exponential _start_distribution, _stop_distribution;

public:
  FlowGenerator( const double & mean_on_duration, const double & mean_off_duration, PRNG & s_prng );
  template <class NextHop>
  void tick( NextHop & next, const double & tickno );
  double next_event_time( const double & tickno __attribute__ ((unused)) ) const { return _next_flow_start; }
};

#endif  // FLOWGENERATOR_HH
