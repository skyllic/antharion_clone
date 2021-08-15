#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

#include <allTheories.h>
#include <execution>
using File = std::filesystem::path;

struct DataPoint
{
  double rho1;
  double dtau;
  double mult;
  double time;
  double rate;
};

int main(int argc, char** argv)
{
  if (argc != 10)
  {
    std::cerr << "Usage: " << argv[0] << " sigma tau1 tau2 tau3 tau4 tau5 tau6 tau7 tau8\n" << std::endl;
    return -1;
  }
  auto sigma = std::stoi(argv[1]);
  std::vector<unsigned int> levels;
  for (int i = 1; i <= 8; ++i)
  {
    levels.push_back(std::stoi(argv[i+1]));
  }

  std::vector<unsigned int> milestoneMult = {25, 25, 25, 25, 25, 25, 25, 20};
  std::vector<unsigned int> numMilestones = {6,  10,  7,  7,  6,  5,  7, 11};

  for (int i = 0; i < 8; ++i)
    if (levels[i] < milestoneMult[i] * numMilestones[i])
      std::cout << "Warning: T" << i + 1 << " does not have enough milestones" << std::endl;

  auto theories = getTheories();

  std::cout << "        Manual"
                  "       Auto"
                  "      Ratio"
                  " MultManual"
                  "   MultAuto"
                  "       StrategyManual"
                  "         StrategyAuto"
                  " TimeManual"
                  "   TimeAuto"
                  " dtauManual"
                  "   dtauAuto" << std::endl;
  for (unsigned int i = 0; i < 8; ++i)
  {
    std::vector<std::pair<std::string, Performance>> data(theories[i].size());
    std::transform(std::execution::par_unseq, theories[i].begin(), theories[i].end(), data.begin(), [&](std::pair<std::string, TheoryFactory> input) -> std::pair<std::string, Performance>
    {
      return {input.first, performanceForTheory(*input.second(), sigma, std::pow(big(10), big(levels[i])))};
    });

    auto sortByRate = [](auto a, auto b){return a.second.rate() < b.second.rate();};
    auto idealIt = std::max_element(data.begin(), data.end(), sortByRate);

    std::vector<std::pair<std::string, Performance>> autos;
    std::copy_if(data.begin(), data.end(), std::back_inserter(autos), [&](auto input){return !theories[i].at(input.first)()->isManual();});
    auto autoIt = std::max_element(autos.begin(), autos.end(), sortByRate);


    std::cout << "T" << i+1 << ": "
              << std::setw(10) << idealIt->second.rate() << " "
              << std::setw(10) << autoIt->second.rate()  << " "
              << std::setw(10) << idealIt->second.rate()/autoIt->second.rate()  << " "
              << std::setw(10) << idealIt->second.multiplier  << " "
              << std::setw(10) << autoIt->second.multiplier  << " "
              << std::setw(20) << idealIt->first  << " "
              << std::setw(20) << autoIt->first  << " "
              << std::setw(10) << idealIt->second.duration/3600  << " "
              << std::setw(10) << autoIt->second.duration/3600  << " ";
    std::cout.precision(1);
    std::cout << std::setw(10) << std::scientific << idealIt->second.gain() << " "
              << std::setw(10) << autoIt->second.gain()  << std::defaultfloat << " "
              << std::endl;
    std::cout.precision(6);
  }
}
