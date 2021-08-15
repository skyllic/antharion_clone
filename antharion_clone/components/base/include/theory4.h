#pragma once
#include <theory.h>

auto calculatePolynomialsDRho(big multiplier, big q, big c1, big c2, big c3, big c4, big c5, big c6) -> big
{
  return multiplier *
      (std::pow(c1, big(1.15)) * c2
     + c3 * q
     + c4 * q*q
     + c5 * q*q*q
     + c6 * q*q*q*q);
}

class Polynomials : public Theory
{
public:
  auto multiplicator(big rho) const -> big override
  {
    return std::pow(rho, big(0.165))/4;
  }
private:
  auto setInitialConditions() -> void override { q = 0; intq1q2 = 0;}

  void tick(big dt) override
  {
    intq1q2 += 1.5 * 2 * 8 * q1 * q2 * dt;
    q = std::sqrt(1+intq1q2) - 1;
    getCurrency(Currency::Rho) += dt * calculatePolynomialsDRho(getTotalMultiplier(), q, c1, c2, c3, c4, c5, c6);
  }

  auto getVariables(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &c4, &c5, &c6, &q1, &q2};
  }
protected:
  big q = 0;
  big intq1q2 = 0;

  Variable c1{std::nullopt, 1.305, 5,    true,  false};
  Variable c2{2           , 3.75,  20,   false, false};
  Variable c3{2           , 2.468, 2000, false, false};
  Variable c4{3           , 4.85,  1e4,  false, false};
  Variable c5{5           , 12.5,  1e8,  false, false};
  Variable c6{10          , 58,    1e10, false, false};
  Variable q1{std::nullopt, 100,   1000, false, false};
  Variable q2{2           , 1000,  1e4,  false, false};

};

class PolynomialsC123 : public Polynomials
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &q1, &q2};
  }
};

class PolynomialsC3 : public Polynomials
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c3, &q1, &q2};
  }
};

class PolynomialsC123d : public PolynomialsC123
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
};

class PolynomialsC3d : public PolynomialsC3
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
};
