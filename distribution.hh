#ifndef DISTRIBUTION_HH
#define DISTRIBUTION_HH

enum class DistType { PARETO, EXPONENTIAL, BIMODAL };

class Distribution
{
public:
  virtual double sample( void ) = 0;
};

#endif
