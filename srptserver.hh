#ifndef SRPT_SERVER_HH
#define SRPT_SERVER_HH

#include <vector>

#include "flow.hh"

class SrptServer
{
private:
  std::vector<Flow> _flow_queue {};
  double _link_speed;
  std::vector<double> _fcts {};
  double _fct_sum {0.0};

public:
  SrptServer( const double & s_link_speed ) : _link_speed( s_link_speed ) {}
  void tick( const double & tickno );
  double next_event_time( const double & tickno ) const;
  void accept( const Flow & s_flow, const double & tickno );
  void output_stats( const double & quantile );
};
 
#endif // SRPT_SERVER_HH
