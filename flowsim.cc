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
  if ( argc < 6 ) {
    cout << "Usage: ./flowsim mean_on_duration (packets)  mean_off_duration (ms)  link_speed (packets/ms) total_ticks (ms)  sch_type [quantile (0 -- 1)] " << endl;
    exit( EXIT_SUCCESS );
  }

  const double mean_on_duration = stod( argv[ 1 ] );
  const double mean_off_duration = stod( argv[ 2 ] );
  const double link_speed = stod( argv[ 3 ] );
  const double total_ticks = stod( argv[ 4 ] );
  const string sch_type ( argv[ 5 ] );
  const double quantile = ( argc >= 7 ) ? stod( argv[ 6 ] ) : 0.0;

  FlowGenerator flow_generator ( mean_on_duration, mean_off_duration, global_PRNG() );
  if ( sch_type == "srpt" ) {
    SrptServer srpt_server( link_speed );
    double tickno = 0;
    while ( tickno < total_ticks ) {
      tickno = min( flow_generator.next_event_time( tickno ),
                    srpt_server.next_event_time( tickno ) );
      assert( tickno > 0 );
      flow_generator.tick( srpt_server,  tickno );
      srpt_server.tick( tickno );
    }
    ( quantile != 0 ) ? srpt_server.output_stats( quantile ) : srpt_server.output_average();
  } else {
    FlowServer fcfs_server( link_speed );
    double tickno = 0;
    while ( tickno < total_ticks ) {
      tickno = min( flow_generator.next_event_time( tickno ),
                    fcfs_server.next_event_time( tickno ) );
      assert( tickno > 0 );
      flow_generator.tick( fcfs_server,  tickno );
      fcfs_server.tick( tickno );
    }
    ( quantile != 0 ) ? fcfs_server.output_stats( quantile ) : fcfs_server.output_average();
  }
  cout << " Scheduled a total of " << flow_generator.flow_count() << " flows " << endl;
}
