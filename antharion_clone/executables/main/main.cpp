#include <gtest/gtest.h>
#include <theory1.h>
#include <theory2.h>
#include <theory3.h>
#include <theory4.h>
#include <theory5.h>
#include <theory6.h>
#include <theory7.h>
#include <theory8.h>

#include <parallel/algorithm>

#include <fstream>
#include <functional>
#include <numeric>
#include <execution>
#include <allTheories.h>

TEST(Value, works)
{
  EXPECT_NEAR(std::log2(value(1542, false)), std::log2(2.740e47), 0.001);
  EXPECT_NEAR(std::log2(value(1528, true)), std::log2(1.028e47), 0.001);
}

auto TestTheory(TheoryFactory factory, std::string fileName, int maxRho = 1000, int drho = 1)
{
  DUMP(fileName);
  auto maxT = 3*24*3600;
  std::vector<std::pair<big, std::pair<Performance, Performance>>> graphs;
  for (int i = 0; i < maxRho; i += drho)
    graphs.emplace_back(std::pow(big(10), i), std::pair<Performance, Performance>{});

  std::for_each(std::execution::par_unseq, graphs.begin(), graphs.end(), [maxT, factory](auto& graph)
  {
    auto theory = factory();
    theory->setRhoMaxAndPublish(graph.first);
    auto p1 = theory->advance(maxT, 3, 3).peakRhoGain([&](big r){return theory->multiplicator(r);}, graph.first);
    auto theory2 = factory();
    theory2->setRhoMaxAndPublish(graph.first);
    theory2->stopBuyingAt(p1.rho1);
    auto p2 = theory2->advance(maxT, 3, 3).peakRhoGain([&](big r){return theory2->multiplicator(r);}, graph.first);
    DUMP(p1);
    DUMP(p2);
    DUMP(p2.rho1/p1.rho1);
    graph.second = {p1, p2};
  });

  std::ofstream file (fileName + ".dat");
  std::ofstream filePush (fileName + "_push.dat");

  for (auto const& pair : graphs)
  {
    if (pair.second.first.duration >= 0.9 * maxT)
      break;
    file << pair.second.first << std::endl;
  }
  for (auto const& pair : graphs)
  {
    if (pair.second.second.duration >= 0.9 * maxT)
      break;
    filePush << pair.second.second << std::endl;
  }
}

TEST(Theory1, works)
{
  auto theories = getTheories();
  for (auto const& [name, theory] : theories[0])
    TestTheory(theory, name);
}

TEST(Theory2, works)
{
  auto theories = getTheories();
  for (auto const& [name, theory] : theories[1])
    TestTheory(theory, name);
}

TEST(Theory3, works)
{
  auto theories = getTheories();
  for (auto const& [name, theory] : theories[2])
    TestTheory(theory, name);
}

TEST(Theory4, works)
{
  auto theories = getTheories();
  for (auto const& [name, theory] : theories[3])
    TestTheory(theory, name);
}

TEST(Theory5, works)
{
  auto theories = getTheories();
  for (auto const& [name, theory] : theories[4])
    TestTheory(theory, name);
}

TEST(Theory6, works)
{
  auto theories = getTheories();
  for (auto const& [name, theory] : theories[5])
    TestTheory(theory, name);
}

TEST(Theory7, works)
{
  auto theories = getTheories();
  for (auto const& [name, theory] : theories[6])
    TestTheory(theory, name);
}

TEST(Theory8, works)
{
  auto theories = getTheories();
  for (auto const& [name, theory] : theories[7])
    TestTheory(theory, name);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
