#include <cassert>
#include <limits>

#include <iostream>
#include <algorithm>

#include "srptserver.hh"

using namespace std;
void SrptServer::accept( const Flow & s_flow, const double & tickno )
{
  if ( _flow_queue.empty() ) {
    Flow current_flow( s_flow );
    current_flow.set_begin_service( tickno );
    _flow_queue.push_back( current_flow );
    std::push_heap( _flow_queue.begin(), _flow_queue.end(), Flow::greater_than );
  } else {
    /* Update remaining_flow_size on top element,
       this is ok because we only ever decrease it. */
    const Flow head_flow = _flow_queue.front();

    /* somebody should have set the service time for this flow */
    assert( head_flow.get_begin_service() > 0 );

    /* current tickno should be larger than get_begin_service() */
    assert( tickno > head_flow.get_begin_service() );

    /* Update remaining flow size on head */
    const double rem_flow_size = head_flow.get_remaining_flow_size() -
                           ( ( tickno - head_flow.get_begin_service() ) * _link_speed  );
    assert( rem_flow_size > 0.0 );
    assert( rem_flow_size < head_flow.get_remaining_flow_size() );

    /* Now we have a smaller flow, in case it gets prempted by s_flow */
    _flow_queue.front().set_remaining_flow_size( rem_flow_size );
    _flow_queue.front().set_begin_service( tickno );

    /* Insert  s_flow treating it as a heap */
    _flow_queue.push_back( s_flow );
    std::push_heap( _flow_queue.begin(), _flow_queue.end(), Flow::greater_than );
    _flow_queue.front().set_begin_service( tickno );
  }
}

void SrptServer::tick( const double & tickno )
{
  if ( next_event_time( tickno ) <= tickno ) {
    assert( not _flow_queue.empty() );
    assert( tickno == next_event_time( tickno ) );
    /* only way I can get here is if I have to deque a packet */
    auto completed_flow = _flow_queue.front();
    _fcts.push_back( tickno - completed_flow.get_creation_time() );
    _fct_sum += ( tickno - completed_flow.get_creation_time() );
    std::pop_heap( _flow_queue.begin(), _flow_queue.end(), Flow::greater_than );
    _flow_queue.pop_back();
    /* Mark beginning of service time
       of next flow in case it gets pre-empted */
    if ( not _flow_queue.empty() ) {
      _flow_queue.front().set_begin_service( tickno );
    }
  }
}

double SrptServer::next_event_time( const double & tickno __attribute__ ((unused)) ) const
{
  return _flow_queue.empty() ? numeric_limits<double>::max() : _flow_queue.front().get_begin_service()
                                                             + _flow_queue.front().get_remaining_flow_size() / _link_speed ;
}

void SrptServer::output_stats( const double & quantile __attribute__ ((unused)) )
{
  cout << " Mean FCT " << _fct_sum / _fcts.size() << endl ;
  //cout << quantile << " quantile " << " TODO, still need to implement " << endl;
}
