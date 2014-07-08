#include <iostream>
#include <cstdlib>

#include "random.hh"
#include "srptserver.hh"
#include "flowgenerator.hh"
#include "flowgenerator-templates.cc"

using namespace std;

int main( int argc, const char* argv[] )
{
  if ( argc < 6 ) {
    cout << "Usage: ./flowsim mean_on_duration (packets)  mean_off_duration (ms)  link_speed (packets/ms) total_ticks (ms)  quantile (0--1) " << endl;
    exit( EXIT_SUCCESS );
  }

  const double mean_on_duration = stod( argv[ 1 ] );
  const double mean_off_duration = stod( argv[ 2 ] );
  const double link_speed = stod( argv[ 3 ] );
  const double total_ticks = stod( argv[ 4 ] );
  const double quantile = stod( argv[ 5 ] );

  FlowGenerator flow_generator ( mean_on_duration, mean_off_duration, global_PRNG() );
  SrptServer flow_server( link_speed );
  double tickno = 0;
  while ( tickno < total_ticks ) {
    tickno = min( flow_generator.next_event_time( tickno ),
                  flow_server.next_event_time( tickno ) );
    assert( tickno > 0 );
    flow_generator.tick( flow_server, tickno );
    flow_server.tick( tickno );
  }
  flow_server.output_stats( quantile );
}
