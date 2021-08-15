#pragma once
#include <theory.h>

class LogisticFunction : public Theory
{
public:
  auto multiplicator(big rho) const -> big override
  {
    return std::pow(rho, big(0.159));
  }
protected:
  auto limit() const
  {
    return c2 * std::pow(c3, big(1.1));
  }

  void tick(big dt) override
  {
    q = std::clamp(q + dt * c1 / c2 * q * (std::pow(c3, big(1.0)) - q/c2), big(0), limit());

    getCurrency(Currency::Rho) += getTotalMultiplier() * dt * std::pow(q1, big(1.05)) * q2 * q;
  }

  void setInitialConditions() override
  {
    q = 1;
  }

  auto getVariables(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&q1, &q2, &c1, &c2, &c3};
  }
protected:
  big q = 1;

  Variable q1{std::nullopt, 1.61328,  10,   true,  false};
  Variable q2{2           , 64,       15,   false, false};
  Variable c1{std::nullopt, 1.18099,  1e6,  false, true };
  Variable c2{2           , 4.53725,  75,   false, false};
  Variable c3{2           , 88550700, 1000, false, false};

};

class LogisticFunctionAI : public LogisticFunction
{
public:
  NextBuy nextVariableToBuy() override
  {
    auto q1c = q1.cost();
    auto q2c = q2.cost();
    auto c1c = c1.cost();
    auto c2c = c2.cost();
    auto c3c = c3.cost();

    auto const rho = getCurrency(Currency::Rho);

    if (rho > c3c) { return NextBuy{{Currency::Rho, {&c3, c3c}}}; }
    if (rho > q2c) { return NextBuy{{Currency::Rho, {&q2, q2c}}}; }
    if (rho > c2c && q > 2./3. * limit())
    {
      return NextBuy{{Currency::Rho, {&c2, c2c}}};
    }
    else if (std::min({q2c, c2c, c3c}) > 10 * q1c) { return NextBuy{{Currency::Rho, {&q1, q1c}}}; }
    if (std::min({q2c, c2c, c3c}) > 10 * c1c) { return NextBuy{{Currency::Rho, {&c1, c1c}}}; }
    return {};
  }
  auto isManual() const -> bool override { return true;}
};

class LogisticFunctionIOM : public LogisticFunction
{
public:
  NextBuy nextVariableToBuy() override
  {
    auto q1c = q1.cost();
    auto q2c = q2.cost();
    auto c1c = c1.cost();
    auto c2c = c2.cost();
    auto c3c = c3.cost();

    auto const rho = getCurrency(Currency::Rho);

    if (rho > c3c) return NextBuy{{Currency::Rho, {&c3, c3c}}};
    if (rho > q2c) return NextBuy{{Currency::Rho, {&q2, q2c}}};
    if (q > 2./3. * limit() && rho > c2c) return NextBuy{{Currency::Rho, {&c2, c2c}}};
    if (q < 3./4. * limit() && rho > c1c) return NextBuy{{Currency::Rho, {&c1, c1c}}};
    if (std::min({q2c, c2c, c3c}) > 20/3. * q1c) return NextBuy{{Currency::Rho, {&q1, q1c}}};
    return {};
  }
  auto isManual() const -> bool override { return true;}
};

class LogisticFunctionAI2 : public LogisticFunction
{
public:
  NextBuy nextVariableToBuy() override
  {
    auto q1c = q1.cost();
    auto q2c = q2.cost();
    auto c1c = c1.cost();
    auto c2c = c2.cost();
    auto c3c = c3.cost();

    auto const rho = getCurrency(Currency::Rho);

    if (rho > c3c) { return NextBuy{{Currency::Rho, {&c3, c3c}}}; }
    if (rho > q2c) { return NextBuy{{Currency::Rho, {&q2, q2c}}}; }
    if (q > 2./3. * limit())
    {
      if (rho > c2c)
        return NextBuy{{Currency::Rho, {&c2, c2c}}};
    }
    else if (std::min({q2c, c2c, c3c}) > 10 * q1c) { return NextBuy{{Currency::Rho, {&q1, q1c}}}; }
    if (std::min({q2c, c2c, c3c}) > 10 * c1c) { return NextBuy{{Currency::Rho, {&c1, c1c}}}; }
    return {};
  }
  auto isManual() const -> bool override { return true;}
};

class LogisticFunctionAI3 : public LogisticFunction
{
public:
  NextBuy nextVariableToBuy() override
  {
    NextBuy result;
    std::map<Variable*, big> options;

    options[&q1] = q1.costNormalizedToPower();
    options[&q2] = q2.costNormalizedToPower();
    options[&c3] = c3.costNormalizedToPower();
    if (q > 2./3. * limit())
      options[&c2] = c2.costNormalizedToPower();
    else
      options[&c1] = 10 * c1.costNormalizedToPower();

    auto it = std::min_element(options.begin(), options.end(), [](auto a, auto b)
    {
      return a.second < b.second;
    });
    return NextBuy{{Currency::Rho, {it->first, it->first->cost()}}};
  }
  auto isManual() const -> bool override { return true;}
};

class LogisticFunctionAI4 : public LogisticFunction
{
public:
  NextBuy nextVariableToBuy() override
  {
    NextBuy result;
    std::map<Variable*, big> options;

    options[&q1] = q1.costNormalizedToPower();
    options[&q2] = q2.costNormalizedToPower();
    options[&c3] = c3.costNormalizedToPower();
    if (q > 2./3. * limit())
      options[&c2] = c2.costNormalizedToPower();
    else if (q < 0.62 * limit())
      options[&c1] = 0.1 * c1.costNormalizedToPower();

    auto it = std::min_element(options.begin(), options.end(), [](auto a, auto b)
    {
      return a.second < b.second;
    });
    return NextBuy{{Currency::Rho, {it->first, it->first->cost()}}};
  }
  auto isManual() const -> bool override { return true;}
};

class LogisticFunctionAI5 : public LogisticFunction
{
public:
  NextBuy nextVariableToBuy() override
  {
    NextBuy result;
    std::map<Variable*, big> options;

    options[&q1] = q1.costNormalizedToPower()/1.5;
    options[&q2] = q2.costNormalizedToPower();
    options[&c3] = c3.costNormalizedToPower();
    if (q > 2./3. * limit())
      options[&c2] = c2.costNormalizedToPower();
    else
      options[&c1] = 10 * c1.costNormalizedToPower();

    auto it = std::min_element(options.begin(), options.end(), [](auto a, auto b)
    {
      return a.second < b.second;
    });
    return NextBuy{{Currency::Rho, {it->first, it->first->cost()}}};
  }
  auto isManual() const -> bool override { return true;}
};

