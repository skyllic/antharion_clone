#include <allTheories.h>
#include <parallel/algorithm>

#include <fstream>
#include <functional>
#include <numeric>

template<class T>
auto flatten(T const& t)
{
  std::vector<typename T::value_type::value_type> result;
  for (auto const& a : t)
    for (auto const& b : a)
      result.push_back(b);
  return result;
}

int main(int argc, char** argv)
{
  if (argc != 5)
    return -1;

  auto strategy = std::string(argv[1]);
  auto sigma = atoi(argv[2]);
  auto initialRho = std::pow(big(10), big(atoi(argv[3])));
  auto filename = std::string(argv[4]);

  auto theories = flatten(getTheories());

  auto it = std::find_if(theories.begin(), theories.end(), [strategy](auto pair){return pair.first == strategy;});
  if (it == theories.end())
    return -2;

  auto theory = it->second();
  theory->setSigma(sigma);
  theory->setRhoMaxAndPublish(initialRho);
  auto log = theory->advance(3600*24*30, 0.6, 0.1);
  log.toFile(filename, [theory](auto r){return theory->multiplicator(r);}, initialRho);
  return 0;
}
