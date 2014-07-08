#include <cassert>
#include <iostream>
#include <limits>

#include "flowserver.hh"

using namespace std;

void FlowServer::accept( const Flow & s_flow )
{
  double current_departure =  _flow_queue.empty() ? s_flow.get_creation_time() :  _flow_queue.back().get_departure();
  Flow new_flow( s_flow );
  new_flow.set_departure( current_departure + new_flow.get_remaining_flow_size() / _link_speed );
  _flow_queue.emplace( new_flow );
}

void FlowServer::tick( const double & tickno )
{
  if ( next_event_time( tickno ) <= tickno ) {
    assert( not _flow_queue.empty() );
    assert( tickno == next_event_time( tickno ) );
    _flow_queue.pop();
  }
}

double FlowServer::next_event_time( const double & tickno __attribute__ ((unused)) ) const
{
  return _flow_queue.empty() ? numeric_limits<double>::max() :  _flow_queue.front().get_departure();
}
