#include <iostream>
#include <cstdlib>

#include "random.hh"
#include "flowserver.hh"
#include "flowgenerator.hh"
#include "flowgenerator-templates.cc"

using namespace std;

int main( int argc, const char* argv[] )
{
  if ( argc < 4 ) {
    cout << "Usage: ./flowsim mean_on_duration (packets)  mean_off_duration (ms)  link_speed (packets/ms)  " << endl;
    exit( EXIT_SUCCESS );
  }

  double mean_on_duration = stod( argv[ 1 ] );
  double mean_off_duration = stod( argv[ 2 ] );
  double link_speed = stod( argv[ 3 ] );

  FlowGenerator flow_generator ( mean_on_duration, mean_off_duration, global_PRNG() );
  FlowServer flow_server( link_speed );
  double tickno = 0;
  while ( tickno < 1000000 ) {
    tickno = min( flow_generator.next_event_time( tickno ),
                  flow_server.next_event_time( tickno ) );
    flow_generator.tick( flow_server, tickno );
    flow_server.tick( tickno );
  }
  //flow_server.output_stats();
}
