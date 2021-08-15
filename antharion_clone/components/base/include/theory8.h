#pragma once
#include <theory.h>

class ChaosTheory : public Theory
{
public:
  auto multiplicator(big rho) const -> big override
  {
    return std::pow(rho, big(0.15));
  }

protected:
  static auto calculateDrho(big multiplier, big c1, big c2, big c3, big c4, big c5, big vx, big vy, big vz)
  {
    return multiplier * c1 * c2 / 100 * std::sqrt(std::pow(c3, 1.15) * vx * vx + std::pow(c4, 1.15) * vy * vy + std::pow(c5, 1.15) * vz * vz);
  }

  virtual auto chaosDt() -> big = 0;

  virtual auto derivative(big x, big y, big z) -> std::tuple<big, big, big>
  {
    auto vx = -500 * (y + z);
    auto vy =  500 * x + 50 * y;
    auto vz =   50  + 500 * z * (x-14);

    return {vx, vy, vz};
  }



  void tickAttractor()
  {
    auto dt = chaosDt();
    auto [vx, vy, vz] = derivative(x,y,z);
    auto [vx2, vy2, vz2] = derivative(
        x + 0.5 * dt * vx,
        y + 0.5 * dt * vy,
        z + 0.5 * dt * vz);
    x += dt * vx2;
    y += dt * vy2;
    z += dt * vz2;
  }

  void tick(big dt) override
  {
    tickAttractor();
    auto [vx, vy, vz] = derivative(x,y,z);
    getCurrency(Currency::Rho) += dt * calculateDrho(getTotalMultiplier(), c1, c2, c3, c4, c5, vx, vy, vz);
  }


  auto getVariables(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &c4, &c4, &c5};
  }

protected:
  big x,y,z;

  Variable c1{std::nullopt, 1.5172,  10,            true,  false};
  Variable c2{2           , 64,      20,            false, false};
  Variable c3{3           , std::pow(3, 1.15), 100, false, false};
  Variable c4{5           , std::pow(5, 1.15), 100, false, false};
  Variable c5{7           , std::pow(7, 1.15), 100, false, false};
};

class ChaosTheoryRoessler : public ChaosTheory
{
  virtual auto chaosDt()  -> big override {return 0.00014;}

  virtual auto derivative(big x, big y, big z) -> std::tuple<big, big, big> override
  {
    auto vx = -500 * (y + z);
    auto vy =  500 * x + 50 * y;
    auto vz =   50  + 500 * z * (x-14);

    return {vx, vy, vz};
  }
  void setInitialConditions() override { x =-6; y = 15, z = 0;}
};
class ChaosTheoryRoessler3 : public ChaosTheoryRoessler
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3};
  }
};

class ChaosTheoryRoessler4 : public ChaosTheoryRoessler
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c4};
  }
};

class ChaosTheoryRoessler5 : public ChaosTheoryRoessler
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c5};
  }
};

class ChaosTheoryRoessler34 : public ChaosTheoryRoessler
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &c4};
  }
};

class ChaosTheoryRoessler35 : public ChaosTheoryRoessler
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &c5};
  }
};

class ChaosTheoryRoessler45 : public ChaosTheoryRoessler
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c4, &c5};
  }
};


class ChaosTheoryRoesslerD : public ChaosTheoryRoessler
{
  NextBuy nextVariableToBuy() override {return nextVariableToBuyConsideringPower();}
  auto isManual() const -> bool override { return true;}
};

class ChaosTheoryRoessler3d : public ChaosTheoryRoesslerD
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3};
  }
};

class ChaosTheoryRoessler4d : public ChaosTheoryRoesslerD
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c4};
  }
};

class ChaosTheoryRoessler5d : public ChaosTheoryRoesslerD
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c5};
  }
};

class ChaosTheoryRoessler34d : public ChaosTheoryRoesslerD
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &c4};
  }
};

class ChaosTheoryRoessler35d : public ChaosTheoryRoesslerD
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c3, &c5};
  }
};

class ChaosTheoryRoessler45d : public ChaosTheoryRoesslerD
{
  auto getVariablesToBuy(Currency) -> std::vector<Variable*> override
  {
    return std::vector<Variable*>{&c1, &c2, &c4, &c5};
  }
};



class ChaosTheoryChen : public ChaosTheory
{
  virtual auto chaosDt() -> big override {return 0.002;}

  virtual auto derivative(big x, big y, big z) -> std::tuple<big, big, big> override
  {
    auto vx = 400 * (y - x);
    auto vy = -120*x-10*x*z+280*y;
    auto vz = 10 * x*y-30*z;

    return {vx, vy, vz};
  }
  void setInitialConditions() override { x =-10.6; y = 4.4, z = 28.6;}
};

class ChaosTheoryLorentz : public ChaosTheory
{
  virtual auto chaosDt() -> big override {return 0.02;}

  virtual auto derivative(big x, big y, big z) -> std::tuple<big, big, big> override
  {
    auto vx = 10 * (y - x);
    auto vy = x * (28 - z) - y;
    auto vz = x*y - 8 * z / 3;

    return {vx, vy, vz};
  }
  void setInitialConditions() override { x =-6; y = -8, z = 26;}
};
