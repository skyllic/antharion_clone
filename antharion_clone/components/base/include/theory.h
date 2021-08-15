#pragma once

#include <variables.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <fstream>

struct Performance
{
  big rho0;
  big rho1;
  big duration;
  big multiplier;

  auto gain() const { return rho1/rho0; }
  auto rate() const { return 3600 * std::log10(gain())/duration;}
};

std::ostream& operator<<(std::ostream& stream, Performance const&  p)
{
  return stream << std::log10(p.rho0)
         << " " << std::log10(p.rho1)
         << " " << std::log10(p.gain())
         << " " << p.multiplier
         << " " << p.duration
         << " " << p.rate();
}

auto getPerformance(std::function<big(big)> multiplier, big initialRho, big currentRho, big dt) -> Performance
{
  return {initialRho, currentRho, dt, multiplier(currentRho)/multiplier(initialRho)};
}

struct Log
{
  big timeToGetBack = 0;
  std::vector<std::pair<big, big>> rhoWithTime;

  auto peakRhoGainIterator(big initialRho) const
  {
    return std::max_element(rhoWithTime.begin(), rhoWithTime.end(), [&](auto const& a, auto const& b)
    {
      return std::log10(a.second/initialRho)/a.first
           < std::log10(b.second/initialRho)/b.first;
    });
  }
  auto peakRhoGain(std::function<big(big)> multiplier, big initialRho) const -> Performance
  {
    auto it = peakRhoGainIterator(initialRho);
    return getPerformance(multiplier, initialRho, it->second, it->first);
  }

  auto toFile(std::string fileName, std::function<big(big)> multiplier, big initialRho)
  {
    std::ofstream stream(fileName);
    auto lastRho = big(0);
    for (auto const& [time, rho] : rhoWithTime)
    {
      if (lastRho * 1.1 >= rho)
        continue;
      lastRho = rho;
      auto p = getPerformance(multiplier, initialRho, rho, time);
      stream     << time
          << " " << std::log10(rho)
          << " " << std::log10(p.gain())
          << " " << p.rate()
          << " " << p.multiplier << std::endl;
    }
  }
};

enum class Currency
{
  Rho, Rho2, Rho3
};

class Theory
{
public:
  Theory(int numCurrencies = 1)
  {
    if (numCurrencies > 1)
    {
      mCurrencies.push_back(Currency::Rho2); mRho2.emplace(0);
    }
    if (numCurrencies > 2)
    {
      mCurrencies.push_back(Currency::Rho3); mRho3.emplace(0);
    }
  }

  virtual ~Theory() = default;

  using NextBuy = std::map<Currency, std::pair<Variable*, big>>;


  auto getCurrency(Currency currency) -> big&
  {
    switch (currency)
    {
    case (Currency::Rho) : return mRho;
    case (Currency::Rho2) : return mRho2.value();
    case (Currency::Rho3) : return mRho3.value();
    }
    return mRho;
  }
  auto getCurrency(Currency currency) const -> big const&
  {
    switch (currency)
    {
    case (Currency::Rho) : return mRho;
    case (Currency::Rho2) : return mRho2.value();
    case (Currency::Rho3) : return mRho3.value();
    }
    return mRho;
  }
  virtual auto multiplicator(big) const -> big = 0;

  virtual auto isManual() const -> bool {return false;};

  auto stopBuyingAt(big value)
  {
    mStopBuyingAt = value;
  }

  auto setRhoMaxAndPublish(big rho)
  {
    mRhoMax = rho;
    publish();
  }
  auto advance(big duration, big buyInterval, big dt, std::optional<big> bailoutAfter = std::nullopt) -> Log
  {
    Log log;
    int buyCount = 0;

    std::optional<Performance> optimum;

    for (big t = 0; t < duration; t += dt)
    {
      tick(dt);

      mRhoMax = std::max(mRhoMax, mRho);
      if (buyCount * buyInterval < t)
      {
        if (log.timeToGetBack == 0 && multiplicator(mRhoMax) > mMultiplier)
        {
          log.timeToGetBack = t;
        }
        log.rhoWithTime.emplace_back(t + dt, mRhoMax);

        if (bailoutAfter)
        {
          auto current = getPerformance([this](big r){return multiplicator(r);}, mPreviousRhoMax, mRhoMax, t);
          if (!optimum || optimum->rate() < current.rate())
            optimum = current;
          else if (optimum->duration * bailoutAfter.value() + 200 < t)
            return log;
        }

        buyAll();
        buyCount++;
      }
    }
    return log;
  }

  auto setSigma(int sigma) {mSigma = sigma;}

protected:
  virtual auto getVariables     (Currency)          -> std::vector<Variable*> = 0;
  virtual auto getVariablesToBuy(Currency currency) -> std::vector<Variable*> {return getVariables(currency);};
  auto getCurrencies() const -> std::vector<Currency> const&
  {
    return mCurrencies;
  }
  auto getTotalMultiplier () const -> big
  {
    auto result = 1.5 * mMultiplier;
    if (mSigma < 65)
      return result;
    if (mSigma < 75)
      return std::max(big(1),std::pow(big(mSigma) / 20, 1)) * result;
    if (mSigma < 85)
      return std::max(big(1),std::pow(big(mSigma) / 20, 2)) * result;
    return std::max(big(1),std::pow(big(mSigma) / 20, 3)) * result;
  }

  virtual auto bought (Currency currency, big usedMoney) -> void
  {
    getCurrency(currency) -= usedMoney;
  }

  auto buyAll() -> void
  {
    if (mStopBuyingAt && mStopBuyingAt.value() <= mRhoMax)
      return;
    auto boughtSomething = false;
    while (true)
    {
      bool boughtSomethingThisIteration = false;
      if (mNextBuy.empty())
        mNextBuy = nextVariableToBuy();
      if (!mNextBuy.empty())
      {
        for (auto const& [currency, toBuy] : mNextBuy)
        {
          auto const& [var, rho] = toBuy;
          assert(var);
          auto usedMoney = var->buyIfPossible(getCurrency(currency));
          if (usedMoney > 0)
          {
            bought(currency, usedMoney);
            boughtSomething = boughtSomethingThisIteration = true;
          }
        }
      }
      if (!boughtSomethingThisIteration)
        break;;
    }
    if (boughtSomething)
    {
      mNextBuy.clear();
      variablesChanged();
    }
  }

  virtual auto tick(big dt) -> void = 0;
  auto publish() -> void
  {
    mMultiplier = std::max(big(1), multiplicator(mRhoMax));
    mPreviousRhoMax = mRhoMax;
    mRhoMax = 0;
    mNextBuy.clear();
    mStopBuyingAt.reset();
    for (auto& currency : getCurrencies())
      getCurrency(currency) = 0;
    for (auto const& currency : getCurrencies())
      for (auto const& variable : getVariables(currency))
        variable->reset();
    setInitialConditions();
  }
  virtual auto variablesChanged() -> void{};
  virtual NextBuy nextVariableToBuy()
  {
    NextBuy result;
    for (auto const& currency : getCurrencies())
    {
      auto vars = getVariablesToBuy(currency);
      auto it = std::min_element(vars.begin(), vars.end(), [](auto a, auto b)
      {
        return a->cost() < b->cost();
      });
      result[currency] = {*it, (*it)->cost()};
    }
    return result;
  }

  NextBuy nextVariableToBuyConsideringPower()
  {
    NextBuy result;
    for (auto const& currency : getCurrencies())
    {
      auto vars = getVariablesToBuy(currency);
      auto it = std::min_element(vars.begin(), vars.end(), [](auto a, auto b)
      {
        return a->costNormalizedToPower() < b->costNormalizedToPower();
      });
      result[currency] = {*it, (*it)->cost()};
    }
    return result;
  }
  virtual auto setInitialConditions() -> void {};

  int mSigma = 230;
  big mRho = 0;
  std::optional<big> mRho2;
  std::optional<big> mRho3;
  big mRhoMax = 0;
  std::optional<big> mStopBuyingAt;
  big mPreviousRhoMax = 0;
  big mMultiplier = 1;

  std::vector<Currency> mCurrencies{Currency::Rho};

  NextBuy mNextBuy;
};

auto performanceForTheory(Theory& theory, int sigma, big initialRho) -> Performance
{
  theory.setSigma(sigma);
  theory.setRhoMaxAndPublish(initialRho);
  return theory.advance(30 * 24 * 3600, 3, 3, big(3)).peakRhoGain([&](big r){return theory.multiplicator(r);}, initialRho);
}

