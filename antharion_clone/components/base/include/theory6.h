#pragma once
#include <theory.h>

class IntegralCalculus : public Theory
{
public:
  auto multiplicator(big rho) const -> big override
  {
    return std::pow(rho, big(0.196))/50;
  }
private:
  auto setInitialConditions() -> void override { q = 0; r = 1; C = 0;}

  auto getIntegral() -> big
  {
    return getTotalMultiplier() * (std::pow(c1, big(1.15)) * c2 * q * r
      +   0.5 * c3 * r * q * q
      + 1./3. * c4 * r * q * q * q
      +   0.5 * c5 * r * q * r);
  }

  auto bought (Currency currency, big usedMoney) -> void override
  {
    C = getIntegral() - getCurrency(currency) + usedMoney;
  }

  void tick(big dt) override
  {
    q += 1.5 * dt * q1 * q2;
    r += 1.5 * dt * r1 * r2 / 1000;
    getCurrency(Currency::Rho) = getIntegral() - C;
  }

  auto getVariables(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &c4, &c5, &q1, &q2, &r1, &r2};
  }
protected:
  big r = 1, q = 0, C = 0;

  Variable q1{std::nullopt, 3    , 15  , true , false};
  Variable q2{2           , 100  , 500 , false, false};
  Variable r1{std::nullopt, 1e5  , 1e25, false, false};
  Variable r2{2           , 1e10 , 1e30, false, false};
  Variable c1{std::nullopt, 2    , 10  , false,  true};
  Variable c2{2           , 5    , 100 , false, false};
  Variable c3{std::nullopt, 1.255, 1e7 , false, false};
  Variable c4{2           , 5e5  , 1e25, false, false};
  Variable c5{2           , 3.9  , 15  , false, false};
};

class IntegralCalculusNoC34 : public IntegralCalculus
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c5, &q1, &q2, &r1, &r2};
  }
};
class IntegralCalculusNoC34d : public IntegralCalculusNoC34
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
};
class IntegralCalculusNoC1234 : public IntegralCalculus
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c5, &q1, &q2, &r1, &r2};
  }
};
class IntegralCalculusNoC1234d : public IntegralCalculusNoC1234
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
};

class IntegralCalculusNoC345 : public IntegralCalculus
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &q1, &q2, &r1, &r2};
  }
};
class IntegralCalculusNoC345d : public IntegralCalculusNoC345
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
};
