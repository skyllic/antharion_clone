#pragma once
#include <theory.h>

auto calculateReccurenceDRho(big multiplier, big q1, big q2, big c1, big c2, big c3, big c4, big rho) -> big
{
  return multiplier * q1 * q2 * (std::pow(c1, big(1.15)) * c2 * (1 + std::log(rho+1)/100) + c3 * std::pow(rho, big(0.2)) + c4 * std::pow(rho, big(0.3)));
}

class RecurrenceRelations : public Theory
{
public:
  auto multiplicator(big rho) const -> big override
  {
    return std::pow(rho, big(0.164))/3;
  }

protected:
  auto calculateDRho() const
  {
    return calculateReccurenceDRho(getTotalMultiplier(), q1,q2,c1,c2,c3,c4, getCurrency(Currency::Rho));
  }

  void tick(big dt) override
  {
    getCurrency(Currency::Rho) += dt * calculateDRho();
  }

  auto getVariables(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&q1, &q2, &c1, &c2, &c3, &c4};
  }

protected:
  Variable q1{std::nullopt, 2,                  5,    true,  false};
  Variable q2{2           , 10,                 100,  false, false};
  Variable c1{std::nullopt, 2,                  15,   false, true };
  Variable c2{2           , 10,                 3000, false, false};
  Variable c3{10          , std::pow(10., 4.5), 1e4,  false, false};
  Variable c4{10          , 1e8,                1e10, false, false};
};

class RecurrenceRelationsC3 : public RecurrenceRelations
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c3, &q1, &q2};
  }
};

class RecurrenceRelationsC4 : public RecurrenceRelations
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c4, &q1, &q2};
  }
};

class RecurrenceRelationsC34 : public RecurrenceRelations
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c3, &c4, &q1, &q2};
  }
};

class RecurrenceRelationsAI : public RecurrenceRelations
{
  auto nextVariableToBuy() -> NextBuy override
  {
    auto const& rho = getCurrency(Currency::Rho);
    auto const m = getTotalMultiplier();

    auto const variables = getVariables(Currency::Rho);
    auto const dRho = calculateReccurenceDRho(m, q1, q2, c1, c2, c3, c4, rho);

    auto options = std::array<std::pair<big, Variable*>, 6>{};
    std::transform(variables.begin(), variables.end(), options.begin(), [=](Variable* var) -> std::pair<big, Variable*>
    {
      if (rho < var->cost())
        return {};
      big modifiedDRho = calculateReccurenceDRho(
                           m,
                           &q1 == var? q1.nextValue() : q1,
                           &q2 == var? q2.nextValue() : q2,
                           &c1 == var? c1.nextValue() : c1,
                           &c2 == var? c2.nextValue() : c2,
                           &c3 == var? c3.nextValue() : c3,
                           &c4 == var? c4.nextValue() : c4,
                           rho - var->cost());
      return {(modifiedDRho - dRho)/var->cost(), var};
    });

    if (q1.cost() + q2.cost() * std::pow(q1 / q1.nextValue(), big(1.15)) > q1.cost() * (q2 / q2.nextValue()) + q2.cost())
      options[0] = {};

    auto const it = std::max_element(options.begin(), options.end());
    if (it->second && it->first > 0)
      return {{{Currency::Rho, {it->second, it->second->cost()}}}};
    return {};
  }
  auto isManual() const -> bool override { return true;}

};
