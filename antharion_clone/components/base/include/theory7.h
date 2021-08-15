#pragma once
#include <theory.h>

class NumericalMethods : public Theory
{
public:
  auto multiplicator(big rho) const -> big override
  {
    return std::pow(rho, big(0.152));
  }

protected:
  static auto calculateDrho(big multiplier, big q1, big c1, big c2, big c3, big c4, big c5, big c6, big rho, big rho2)
  {
    return std::make_pair(
      multiplier * q1 * (std::pow(c1, big(1.15)) * c2 + 1.5 * c3 * std::sqrt(rho)  + ((rho  > 0)? std::min(0.5 * c6 * std::sqrt(rho2/rho), 100*rho) : 0)),
      multiplier * q1 * (c4                           + 1.5 * c5 * std::sqrt(rho2) + ((rho2 > 0)? std::min(0.5 * c6 * std::sqrt(rho/rho2), 100*rho2) : 0))
    );
  }

  void tick(big dt) override
  {
    auto drho = calculateDrho(getTotalMultiplier(), q1, c1, c2, c3, c4, c5, c6, getCurrency(Currency::Rho), mRho2);

    getCurrency(Currency::Rho) += dt * drho.first;
    mRho2 += dt * drho.second;
  }


  auto getVariables(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &c4, &c4, &c5, &c6, &q1};
  }

  void setInitialConditions() override {mRho2 = 1;}
protected:
  big mRho2;

  Variable q1{std::nullopt, 1.51572, 500, true,  false};
  Variable c1{std::nullopt, 1.275,   10,  false, true };
  Variable c2{2           , 8,       40,  false, false};
  Variable c3{2           , 63,      1e5, false, false};
  Variable c4{2           , 2.82,    10,  false, false};
  Variable c5{2           , 60,      1e8, false, false};
  Variable c6{2           , 2.81,    100, false, false};
};

class NumericalMethodsNoc123 : public NumericalMethods
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c4, &c4, &c5, &c6, &q1};
  }
};

class NumericalMethodsNoc123d : public NumericalMethodsNoc123
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c4, &c4, &c5, &c6, &q1};
  }
};

class NumericalMethodsAI : public NumericalMethods
{
  virtual auto isAnImprovement(std::pair<big, big> currentDRho, std::pair<big, big> otherDRho) -> bool = 0;
  auto nextVariableToBuy() -> NextBuy override
  {
    auto const& rho = getCurrency(Currency::Rho);
    auto const& rho2 = mRho2;

    auto dRho = calculateDrho(getTotalMultiplier(), q1, c1, c2, c3, c4, c5, c6, rho, rho2);

    if (rho > c3.cost() && isAnImprovement(dRho, calculateDrho(getTotalMultiplier(), q1, c1, c2, c3.nextValue(), c4, c5, c6, rho - c3.cost(), rho2)))
      return {{Currency::Rho, {&c3, c3.cost()}}};
    if (rho > c4.cost() && isAnImprovement(dRho, calculateDrho(getTotalMultiplier(), q1, c1, c2, c3, c4.nextValue(), c5, c6, rho - c4.cost(), rho2)))
      return {{Currency::Rho, {&c4, c4.cost()}}};
    if (rho > c5.cost() && isAnImprovement(dRho, calculateDrho(getTotalMultiplier(), q1, c1, c2, c3, c4, c5.nextValue(), c6, rho - c5.cost(), rho2)))
      return {{Currency::Rho, {&c5, c5.cost()}}};
    if (rho > c6.cost() && isAnImprovement(dRho, calculateDrho(getTotalMultiplier(), q1, c1, c2, c3, c4, c5, c6.nextValue(), rho - c6.cost(), rho2)))
      return {{Currency::Rho, {&c6, c6.cost()}}};
    if (rho > q1.cost() && isAnImprovement(dRho, calculateDrho(getTotalMultiplier(), q1.nextValue(), c1, c2, c3, c4, c5, c6, rho - q1.cost(), rho2)))
      return {{Currency::Rho, {&q1, q1.cost()}}};
    if (rho > c1.cost() && isAnImprovement(dRho, calculateDrho(getTotalMultiplier(), q1, c1.nextValue(), c2, c3, c4, c5, c6, rho - c1.cost(), rho2)))
      return {{Currency::Rho, {&c1, c1.cost()}}};
    if (rho > c2.cost() && isAnImprovement(dRho, calculateDrho(getTotalMultiplier(), q1, c1, c2.nextValue(), c3, c4, c5, c6, rho - c2.cost(), rho2)))
      return {{Currency::Rho, {&c2, c2.cost()}}};
    return {};
  }
  auto isManual() const -> bool override { return true;}
};

class NumericalMethodsNoDecay12 : public NumericalMethodsAI
{
  auto isAnImprovement(std::pair<big, big> currentDRho, std::pair<big, big> otherDRho) -> bool override
  {
    return currentDRho.first <= otherDRho.first && currentDRho.second <= otherDRho.second;
  }
};


class NumericalMethodsNoDecay1 : public NumericalMethodsAI
{
  auto isAnImprovement(std::pair<big, big> currentDRho, std::pair<big, big> otherDRho) -> bool override
  {
    return currentDRho.first <= otherDRho.first;
  }
};

class NumericalMethodsNoDecay2 : public NumericalMethodsAI
{
  auto isAnImprovement(std::pair<big, big> currentDRho, std::pair<big, big> otherDRho) -> bool override
  {
    return currentDRho.second <= otherDRho.second;
  }
};
