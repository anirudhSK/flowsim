#ifndef FLOW_HH
#define FLOW_HH

#include <cassert>

class Flow
{
private:
  unsigned int _flow_id;
  double _remaining_flow_size;
  double _creation_time;
  double _begin_service {-1};

public:
  Flow( const unsigned int s_flow_id,
        const double & s_flow_size,
        const double & s_created )
    : _flow_id( s_flow_id ),
      _remaining_flow_size( s_flow_size ),
      _creation_time( s_created )
  { assert( _remaining_flow_size >= 0 ); /* Decimal imprecisions might give you a flow of size 0 */ }
  static bool greater_than( const Flow & lhs, const Flow & rhs ) {
    return    lhs._remaining_flow_size == rhs._remaining_flow_size
            ? lhs._creation_time > rhs._creation_time
            : lhs._remaining_flow_size > rhs._remaining_flow_size; }
  void set_begin_service( const double & s_begin_service ) { _begin_service = s_begin_service; }
  double get_begin_service( void ) const { return _begin_service; }
  double get_remaining_flow_size( void ) const { return _remaining_flow_size; }
  void set_remaining_flow_size( const double & s_flow_size ) { _remaining_flow_size = s_flow_size; }
  double get_creation_time( void ) const { return _creation_time; }
};

#endif // FLOW_HH
