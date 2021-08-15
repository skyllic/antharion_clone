#pragma once
#include <theory.h>

class LinearAlgebra : public Theory
{
public:
  LinearAlgebra() : Theory(3)
  {
    variablesChanged();
  }

  auto multiplicator(big rho) const -> big override
  {
    return 3 * std::pow(rho, big(0.147));
  }

private:
  auto variablesChanged() -> void override
  {
    auto b1 = std::pow(this->b1, big(1.1));
    auto b2 = std::pow(this->b2, big(1.1));
    auto b3 = std::pow(this->b3, big(1.1));
    mDRho1 = getTotalMultiplier() * (b1 * c11 + b2 * c12 + b3 * c13);
    mDRho2 = getTotalMultiplier() * (b1 * c21 + b2 * c22 + b3 * c23);
    mDRho3 = getTotalMultiplier() * (b1 * c31 + b2 * c32 + b3 * c33);
  }

  auto setInitialConditions() -> void override
  {
    variablesChanged();
  }

  void tick(big dt) override
  {
    getCurrency(Currency::Rho)  += dt * mDRho1;
    getCurrency(Currency::Rho2) += dt * mDRho2;
    getCurrency(Currency::Rho3) += dt * mDRho3;
  }

  auto getVariables(Currency currency) -> std::vector<Variable*> override
  {
    auto const variablesRho  = std::vector<Variable*>{&b1, &c11, &c21, &c31};
    auto const variablesRho2 = std::vector<Variable*>{&b2, &c12, &c22, &c32};
    auto const variablesRho3 = std::vector<Variable*>{&b3, &c13, &c23, &c33};
    switch (currency)
    {
    case(Currency::Rho)  : return variablesRho;
    case(Currency::Rho2) : return variablesRho2;
    case(Currency::Rho3) : return variablesRho3;
    }
    return {};
  }

protected:

  Variable b1 {std::nullopt, 1.18099, 10,   true,  false};
  Variable c11{2           , 6.3496,  20,   false, false};
  Variable c21{2           , 18.8343, 500,  false, false};
  Variable c31{2           , 1248.27, 1e4,  false, false};

  Variable b2 {std::nullopt, 1.308,   10,   false, false};
  Variable c12{2           , 2.74,    10,   false, false};
  Variable c22{2           , 3.65,    1e5,  false, false};
  Variable c32{2           , 6.81774, 1000, false, false};

  Variable b3 {std::nullopt, 1.675,   3000, false, false};
  Variable c13{2           , 1.965,   1000, false, false};
  Variable c23{2           , 2.27,    1e5,  false, false};
  Variable c33{2           , 2.98,    1e5,  false, false};


  big mDRho1 = 0;
  big mDRho2 = 0;
  big mDRho3 = 0;
};

class LinearAlgebraNoC11C21C33 : public LinearAlgebra
{
  auto getVariablesToBuy(Currency currency) -> std::vector<Variable*> override
  {
    switch (currency)
    {
    case(Currency::Rho)  : return std::vector<Variable*>{&b1,             &c31};
    case(Currency::Rho2) : return std::vector<Variable*>{&b2, &c12, &c22, &c32};
    case(Currency::Rho3) : return std::vector<Variable*>{&b3, &c13, &c23      };
    }
    return {};
  }
};

class LinearAlgebraNoC11C13C21C33 : public LinearAlgebra
{
  auto getVariablesToBuy(Currency currency) -> std::vector<Variable*> override
  {
    switch (currency)
    {
    case(Currency::Rho)  : return std::vector<Variable*>{&b1,             &c31};
    case(Currency::Rho2) : return std::vector<Variable*>{&b2, &c12, &c22, &c32};
    case(Currency::Rho3) : return std::vector<Variable*>{&b3,       &c23      };
    }
    return {};
  }
};

class LinearAlgebraNoC11C21C33d : public LinearAlgebraNoC11C21C33
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
};

class LinearAlgebraNoC11C13C21C33d : public LinearAlgebraNoC11C13C21C33
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
};

class LinearAlgebraB123C31C12C23 : public LinearAlgebra
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}

  auto getVariablesToBuy(Currency currency) -> std::vector<Variable*> override
  {
    switch (currency)
    {
    case(Currency::Rho)  : return std::vector<Variable*>{&b1,             &c31};
    case(Currency::Rho2) : return std::vector<Variable*>{&b2, &c12            };
    case(Currency::Rho3) : return std::vector<Variable*>{&b3,       &c23      };
    }
    return {};
  }
};

