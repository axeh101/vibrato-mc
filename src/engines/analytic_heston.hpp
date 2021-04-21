#ifndef ANALYTIC_HESTON_HPP
#define ANALYTIC_HESTON_HPP

#include <cmath>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/heston.hpp"

template <typename D>
class AnalyticalHeston: public PricingEngine<D>
{

};


#endif  // ANALYTIC_HESTON_HPP
