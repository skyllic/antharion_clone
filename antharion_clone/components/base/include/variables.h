#pragma once
#include <iostream>
#define DUMP(x) std::cout << #x << ": " << x << std::endl;



#include <types.h>
#include <cmath>
#include <optional>

auto value(int level, bool offset)
{
  return (10 + level % 10) * std::pow(big(2), big(level/10)) - (offset? 9 : 10);
}

auto cost(int level, big a, big b, bool first)
{
  return b * std::pow(a, big(level - first));
}

auto costx10(int level, big a, big b, bool first)
{
  return cost(level, a, b, first)
       + cost(level + 1, a, b, first)
       + cost(level + 2, a, b, first)
       + cost(level + 3, a, b, first)
       + cost(level + 4, a, b, first)
       + cost(level + 5, a, b, first)
       + cost(level + 6, a, b, first)
       + cost(level + 7, a, b, first)
       + cost(level + 8, a, b, first)
       + cost(level + 9, a, b, first);
}

class Variable
{
public:
  Variable(
      std::optional<big> base,
      big a,
      big b,
      bool first, bool offset)
    : mBase(base), mA(a), mB(b), mFirst(first), mOffset(offset)
  {
    if (first)
      increment(1);
    else
      increment(0);
  }

  auto reset()
  {
    *this = Variable(mBase, mA, mB, mFirst, mOffset);
  }

  // Returns the cost of bought Upgrades
  auto buyIfPossible(big rho) -> big
  {
    if (auto c = cost(); rho >= c)
    {
      increment(1);
      return c;
    }
    return 0;
  }

  auto increment(int dlevel) -> void
  {
    mLevel += dlevel;
    if (mBase)
      mValue = std::pow(*mBase, big(mLevel + mOffset));
    else
      mValue = ::value(mLevel, mOffset);
    mCost = mCostx10 = mNextValue = std::nullopt;
  }

  auto nextValue() const -> big
  {
    if (!mNextValue)
    {
      Variable copy(*this);
      copy.increment(1);
      mNextValue = copy;
    }
    return mNextValue.value();
  }

  operator big () const
  {
    return mValue;
  }

  auto cost() const -> big
  {
    if (!mCost)
      mCost = ::cost(mLevel, mA, mB, mFirst);
    return *mCost;
  }

  auto costNormalizedToPower()
  {
    if (!mBase)
      return 10 * cost();
    return cost() / log2(*mBase);
  }
private:
  auto costx10() -> big
  {
    if (!mCostx10)
      mCostx10 = ::costx10(mLevel, mA, mB, mFirst);
    return *mCostx10;
  }


  std::optional<big> mBase;
  big mA, mB;
  bool mFirst, mOffset;

  mutable std::optional<big> mCost;
  mutable std::optional<big> mCostx10;

  mutable std::optional<big> mNextValue;

  int mLevel = 0;
  big mValue = 0;
};

