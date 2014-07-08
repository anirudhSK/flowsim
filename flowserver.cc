#include <cassert>
#include <iostream>
#include <limits>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/tail_quantile.hpp>

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
    auto completed_flow = _flow_queue.front();
    assert( tickno == completed_flow.get_departure() );
    _fcts.push_back( tickno - completed_flow.get_creation_time() );
    _flow_queue.pop();
  }
}

double FlowServer::next_event_time( const double & tickno __attribute__ ((unused)) ) const
{
  return _flow_queue.empty() ? numeric_limits<double>::max() :  _flow_queue.front().get_departure();
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
