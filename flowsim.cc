#include <iostream>
#include <cstdlib>

#include "random.hh"
#include "srptserver.hh"
#include "flowserver.hh"
#include "flowgenerator.hh"
#include "flowgenerator-templates.cc"

using namespace std;

int main( int argc, const char* argv[] )
{
  if ( argc < 5 ) {
    cout << "Usage: ./flowsim lambda(0--1)  total_ticks (ms)  quantile (0--1) sch_type " << endl;
    exit( EXIT_SUCCESS );
  }

  const double lambda = stod( argv[ 1 ] );
  assert( lambda > 0 and lambda < 1 );
  const double total_ticks = stod( argv[ 2 ] );
  const double quantile = stod( argv[ 3 ] );
  const string sch_type ( argv[ 4 ] );

  FlowGenerator flow_generator ( 1.0, 1.0 / lambda, global_PRNG() );
  if ( sch_type == "srpt" ) {
    SrptServer srpt_server( 1.0 );
    double tickno = 0;
    while ( tickno < total_ticks ) {
      tickno = min( flow_generator.next_event_time( tickno ),
                    srpt_server.next_event_time( tickno ) );
      assert( tickno > 0 );
      flow_generator.tick( srpt_server,  tickno );
      srpt_server.tick( tickno );
    }
    srpt_server.output_stats( quantile );
  } else {
    FlowServer fcfs_server( 1.0 );
    double tickno = 0;
    while ( tickno < total_ticks ) {
      tickno = min( flow_generator.next_event_time( tickno ),
                    fcfs_server.next_event_time( tickno ) );
      assert( tickno > 0 );
      flow_generator.tick( fcfs_server,  tickno );
      fcfs_server.tick( tickno );
    }
    fcfs_server.output_stats( quantile );
  }
  cout << " Scheduled a total of " << flow_generator.flow_count() << " flows " << endl;
}
