#ifndef FLOW_HH
#define FLOW_HH

class Flow
{
private:
  const unsigned int _flow_id;
  double _remaining_flow_size;
  double _scheduled_departure;
  const double _creation_time;

public:
  Flow( const unsigned int s_flow_id,
        const double & s_flow_size,
        const double & s_departure,
        const double & s_created )
    : _flow_id( s_flow_id ),
      _remaining_flow_size( s_flow_size ),
      _scheduled_departure( s_departure ),
      _creation_time( s_created )
  {}
  void set_departure( const double & s_departure ) { _scheduled_departure = s_departure; }
  double get_departure( void ) const { return _scheduled_departure; }
  double get_remaining_flow_size( void ) const { return _remaining_flow_size; }
  double get_creation_time( void ) const { return _creation_time; }
};

#endif // FLOW_HH
