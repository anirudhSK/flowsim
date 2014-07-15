#ifndef UTIL_HH
#define UTIL_HH

#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <memory>
#include "distribution.hh"
#include "exponential.hh"
#include "pareto.hh"
#include "bimodal.hh"

std::unique_ptr<Distribution> get_dist( const DistType & dist_type, const double & mean, PRNG & prng );

template <class T>
void dump_to_file( const T & protobuf, const std::string & file_name )
{
  assert( file_name != "" );
  char of[ 128 ];
  snprintf( of, 128, "%s", file_name.c_str() );
  fprintf( stderr, "Writing to \"%s\"... \n", of );
  int fd = open( of, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR );
  if ( fd < 0 ) {
    perror( ( "open " + file_name ).c_str() );
    exit( 1 );
  }
  if ( not protobuf.SerializeToFileDescriptor( fd ) ) {
    fprintf( stderr, "Could not serialize protobuf.\n" );
    exit( 1 );
  }

  if ( close( fd ) < 0 ) {
    perror( "close" );
    exit( 1 );
  }
}

template <class T>
T parse_from_file( const std::string & file_name )
{
  assert( file_name != "" );
  int fd = open( file_name.c_str(), O_RDONLY );
  if ( fd < 0 ) {
    perror( ( "open " + file_name ).c_str() );
    exit( 1 );
  }

  T protobuf;;
  if ( not protobuf.ParseFromFileDescriptor( fd ) ) {
    fprintf( stderr, "Could not parse %s.\n", file_name.c_str() );
    exit( 1 );
  }

  if ( close( fd ) < 0 ) {
    perror( "close" );
    exit( 1 );
  }


  return protobuf;
}

#endif // UTIL_HH
