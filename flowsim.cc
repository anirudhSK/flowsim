#include <iostream>
#include <cstdlib>

#include "random.hh"
#include "flowgenerator.hh"
#include "flowgenerator-templates.cc"

using namespace std;

int main( int argc, const char* argv[] )
{
  if ( argc < 3 ) {
    cout << "Usage: ./flowsim mean_on_duration mean_off_duration " << endl;
    exit( EXIT_SUCCESS );
  }

  double mean_on_duration = stod( argv[ 1 ] );
  double mean_off_duration = stod( argv[ 2 ] );
  FlowGenerator flow_generator ( mean_on_duration, mean_off_duration, global_PRNG() );
  double tickno = 0;
  while ( tickno < 1000000 ) {
    tickno = min( flow_generator.next_event_time( tickno ), 1000000000.0 ); // , flow_server.next_event_time() );
    int var = 5;
    flow_generator.tick( var, tickno );
//    flow_server.tick( tickno );
  }
//  flow_server.output_stats();
}
