#include <cassert>
#include <iostream>
#include <limits>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/tail_quantile.hpp>

#include "flowserver.hh"

using namespace std;

void FlowServer::accept( const Flow & s_flow, const double & tickno )
{
  Flow new_flow( s_flow );
  if ( _flow_queue.empty() ) {
    /* Set beginning of service to current time */
    new_flow.set_begin_service( tickno );
  }
  _flow_queue.emplace( new_flow );
}

void FlowServer::tick( const double & tickno )
{
  if ( next_event_time( tickno ) <= tickno ) {
    assert( not _flow_queue.empty() );
    assert( tickno == next_event_time( tickno ) );
    auto completed_flow = _flow_queue.front();
    _fcts.push_back( tickno - completed_flow.get_creation_time() );
    _fct_sum += ( tickno - completed_flow.get_creation_time() );
    _flow_queue.pop();
    if ( not _flow_queue.empty() ) {
      _flow_queue.front().set_begin_service( tickno );
    }
  }
}

double FlowServer::next_event_time( const double & tickno __attribute__ ((unused)) ) const
{
  return _flow_queue.empty() ? numeric_limits<double>::max() : _flow_queue.front().get_begin_service()
                                                             + _flow_queue.front().get_remaining_flow_size() / _link_speed ;
}

void FlowServer::output_stats( const double & quantile )
{
  boost::accumulators::accumulator_set< double,
                                        boost::accumulators::stats
                                        <boost::accumulators::tag::tail_quantile
                                        <boost::accumulators::right> > > fct_acc
                                      ( boost::accumulators::tag::tail<boost::accumulators::right>::cache_size = _fcts.size() );
  for_each( _fcts.begin(), _fcts.end(), [&] ( const double & val ) { fct_acc( val ); } );
  cout << " Total of " << _fcts.size() << " flows " << endl;
  cout << quantile << " quantile " <<  boost::accumulators::quantile( fct_acc, boost::accumulators::quantile_probability = quantile ) << endl;
}
