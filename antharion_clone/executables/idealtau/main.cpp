#include <allTheories.h>
#include <parallel/algorithm>

#include <fstream>
#include <functional>
#include <numeric>
#include <execution>

auto sigma(std::array<big, 8> taus) -> int
{
  return 10 * std::accumulate(taus.begin(), taus.end(), 0, [](big a, big b)
  {
    return a + std::log10(b);
  }) / 200;
}
auto getTable(std::string filename, bool autoOnly)
{
  auto theories = getTheories();

  std::array<big, 8> taus = {1,1,1,1,1,1,1,1};

  struct LogEntry
  {
    int sigma;
    std::array<big, 8> taus;
  };

  std::vector<LogEntry> log;

  using Candidate = std::tuple<int, unsigned int, big, std::string>;

  std::map<Candidate, Performance> allCandidates;

  int sigma = 0;
  int t = 0;

  std::ofstream file{filename};

  while (sigma < 250)
  {
    sigma = ::sigma(taus);
    DUMP(sigma);
    std::vector<std::pair<Candidate, Performance>> candidates;
    for (unsigned int i = 0; i < theories.size(); ++i)
    {
      for (auto const& [id, theoryFactory] : theories[i])
      {
        auto theory = theoryFactory();
        if (!theory->isManual() || !autoOnly)
        {
          auto candidate = Candidate{sigma, i, taus[i], id};
          if (allCandidates.count(candidate) == 0)
            allCandidates[candidate] = performanceForTheory(*theory, sigma, taus[i]);
          candidates.push_back(*allCandidates.find(candidate));
        }
      }
    }
    auto it = std::max_element(candidates.begin(), candidates.end(), [](auto a, auto b){
      return a.second.rate() < b.second.rate();
    });
    taus[std::get<1>(it->first)]*=it->second.gain();
    t += it->second.duration;
    log.push_back({sigma, taus});

    file << sigma << " ";
    for (auto const& tau : taus)
      file << std::log10(tau) << " ";
    file << " " << t/3600 << std::endl;
  }
}

int main(int, char**)
{
  getTable("idealTausOld.dat", false);
  getTable("autoTausOld.dat", true);
}
