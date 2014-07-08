#include <cassert>
#include <limits>

#include <iostream>
#include <algorithm>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/tail_quantile.hpp>

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

    /* current tickno should be at least as large as get_begin_service() */
    assert( tickno >= head_flow.get_begin_service() );

    /* Update remaining flow size on head */
    const double rem_flow_size = head_flow.get_remaining_flow_size() -
                           ( ( tickno - head_flow.get_begin_service() ) * _link_speed  );
//    assert( rem_flow_size >= 0.0 );
    assert( rem_flow_size <= head_flow.get_remaining_flow_size() );

    /* Now we have a smaller flow, in case it gets prempted by s_flow */
    if ( rem_flow_size > 0 ) {
      _flow_queue.front().set_remaining_flow_size( rem_flow_size );
      _flow_queue.front().set_begin_service( tickno );
    } else {
      _fcts.push_back( tickno - head_flow.get_creation_time() );
      _fct_sum += ( tickno - head_flow.get_creation_time() );
      std::pop_heap( _flow_queue.begin(), _flow_queue.end(), Flow::greater_than );
      _flow_queue.pop_back();
    }

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
  boost::accumulators::accumulator_set< double,
                                        boost::accumulators::stats
                                        <boost::accumulators::tag::tail_quantile
                                        <boost::accumulators::right> > > fct_acc
                                      ( boost::accumulators::tag::tail<boost::accumulators::right>::cache_size = _fcts.size() );
  for_each( _fcts.begin(), _fcts.end(), [&] ( const double & val ) { fct_acc( val ); } );
  cout << " Total of " << _fcts.size() << " flows " << endl;
  cout << quantile << " quantile " <<  boost::accumulators::quantile( fct_acc, boost::accumulators::quantile_probability = quantile ) << endl;}
