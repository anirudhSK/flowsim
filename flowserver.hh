#ifndef FLOWSERVER_HH
#define FLOWSERVER_HH

#include <queue>
#include "flow.hh"

class FlowServer
{
private:
  std::queue<Flow> _flow_queue {};
  double _link_speed;
  std::vector<double> _fcts {};

public:
  FlowServer( const double & s_link_speed ) : _link_speed( s_link_speed ) {}
  void tick( const double & tickno );
  double next_event_time( const double & tickno ) const;
  void accept( const Flow & s_flow, const double & tickno );
  void output_stats( const double & quantile );
};

#endif  // FLOWSERVER_HH
