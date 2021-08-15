#pragma once
#include <theory.h>

class DifferentialCalculus : public Theory
{
public:
  DifferentialCalculus(){ setInitialConditions(); }

  auto multiplicator(big rho) const -> big override
  {
    return std::pow(rho, big(0.198))/100;
  }
private:
  auto setInitialConditions() -> void override
  {
    q1 = 0;
    q2 = q3 = q4 = r1 = r2 = r3 = r4 = 1;
  }
  auto calculateDRho()
  {
    return getTotalMultiplier() * std::pow(q1, big(1.15)) * std::pow(r1, big(1.15));
  }

  auto getVariables(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&q1m, &q2m, &q3m, &q4m, &r1m, &r2m, &r3m, &r4m};
  }

  void tick(big dt) override
  {
    r4 += big(1.5) * dt *      r4m;
    r3 += big(1.5) * dt * r4 * r3m;
    r2 += big(1.5) * dt * r3 * r2m;
    r1 += big(1.5) * dt * r2 * r1m;
    q4 += big(1.5) * dt *      q4m;
    q3 += big(1.5) * dt * q4 * q3m;
    q2 += big(1.5) * dt * q3 * q2m;
    q1 += big(1.5) * dt * q2 * q1m;

    getCurrency(Currency::Rho) += dt  * calculateDRho();
  }

protected:
  big r1, r2, r3, r4, q1, q2, q3, q4;

  Variable q1m{std::nullopt, 2, 10  , true,  false};
  Variable q2m{std::nullopt, 2, 5000, false, false};
  Variable q3m{std::nullopt, 3, 3e25, false, false};
  Variable q4m{std::nullopt, 4, 8e50, false, false};
  Variable r1m{std::nullopt, 2, 2e6 , false, false};
  Variable r2m{std::nullopt, 2, 3e9 , false, false};
  Variable r3m{std::nullopt, 3, 4e25, false, false};
  Variable r4m{std::nullopt, 4, 5e50, false, false};
};
