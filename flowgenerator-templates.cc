#include <iostream>
#include "flowgenerator.hh"

template <class NextHop>
void FlowGenerator::tick( NextHop & next __attribute__ ((unused)), const double & tickno )
{
  if ( _next_flow_start <= tickno ) {
    assert( _next_flow_start == tickno );
    const double new_flow_length = _stop_distribution.sample();
    next.accept( Flow( _flow_count++,
                       new_flow_length,
                       tickno,
                       tickno ) );
    _next_flow_start += _start_distribution.sample();
  }
}
