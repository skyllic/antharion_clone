#include <filesystem>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>

using File = std::filesystem::path;

struct DataPoint
{
  double rho1;
  double dtau;
  double mult;
  double time;
  double rate;
};

auto readFile(std::filesystem::path path)
{
  std::ifstream stream (path);
  std::map<unsigned int, DataPoint> result;
  while (!stream.eof())
  {
    unsigned int rho0;
    DataPoint data;
    stream >> rho0 >> data.rho1 >> data.dtau >> data.mult >> data.time >> data.rate;
    if (stream)
      result[rho0] = data;
    else
      break;
  }
  return result;
}

auto makePlotCommand(std::ostream& stream, std::vector<File> files, std::string column)
{
  for (unsigned int i = 0; i < files.size(); ++i)
  {
    if (i == 0)
      stream << "plot[300:*] ";
    auto title = files[i].filename().string();
    std::replace( title.begin(), title.end(), '_', '-');
    stream << files[i] << " using 1:" << column << " w lp ti \"" << title <<  "\"";
    if (i + 1 != files.size())
      stream << ",\\";
    stream << std::endl;
  }
}

auto all (std::array<std::vector<File>, 8> const& files)
{
  std::vector<File> result;
  for (auto const& theory : files)
    result.insert(result.end(), theory.begin(), theory.end());
  return result;
}

auto multiplierPlot(std::string baseName, std::string title, std::vector<File> files)
{
  std::ofstream plotfile (baseName + "_multiplier.gpl");
  plotfile << "#set terminal pngcairo size 1600,1200\n";
  plotfile << "#set output \"" << baseName << "_multiplier.png\"\n";
  plotfile << "set title \"" << title << "\"\n";
  plotfile << "set log y\n";
  makePlotCommand(plotfile, files, "4");
}

auto ratePlot(std::string baseName, std::string title, std::vector<File> files)
{
  std::ofstream plotfile (baseName + "_rate.gpl");
  plotfile << "#set terminal pngcairo size 1600,1200\n";
  plotfile << "#set output \"" << baseName << "_rate.png\"\n";
  plotfile << "set title \"" << title << "\"\n";
  plotfile << "set log y\n";
  makePlotCommand(plotfile, files, "6");
}

auto timePlot(std::string baseName, std::string title, std::vector<File> files)
{
  std::ofstream plotfile (baseName + "_time.gpl");
  plotfile << "#set terminal pngcairo size 1600,1200\n";
  plotfile << "#set output \"" << baseName << "_time.png\"\n";
  plotfile << "set title \"" << title << "\"\n";
  plotfile << "set log y\n";
  makePlotCommand(plotfile, files, "($5/3600)");
}

int main()
{
  std::array<std::vector<File>, 8> files;

  for (auto const& file : std::filesystem::directory_iterator("."))
  {
    if (file.is_regular_file() && file.path().filename().string().size() >= 2 && file.path().filename().c_str()[0] == 'T')
    {

      if (auto c = file.path().filename().c_str()[1]; std::isdigit(c))
        files[c - '1'].push_back(file);
    }
  }

  for (unsigned int i = 0; i < files.size(); ++i)
  {
    multiplierPlot(std::string("Theory") + char('1' + i), std::string("Theory ") + char('1' + i), files[i]);
    ratePlot(std::string("Theory") + char('1' + i), std::string("Theory ") + char('1' + i), files[i]);
    timePlot(std::string("Theory") + char('1' + i), std::string("Theory ") + char('1' + i), files[i]);
  }
  {
    multiplierPlot("TheoryAll", "All theories", all(files));
    ratePlot("TheoryAll", "All theories", all(files));
    timePlot("TheoryAll", "All theories", all(files));
  }
  {
    std::vector<unsigned int> levels = {463,391,445,461,697,696,432,400};
    std::vector<std::pair<File, File>> files
    {
      {"./T1AI.dat"             , "T1C34.dat"},
      {"./T2_push.dat"          , "T2.dat"},
      {"./T3NoC11C13C21C33d.dat", "T3NoC11C13C21C33.dat"},
      {"./T4C3d.dat"            , "T4C3.dat"},
      {"./T5AI4.dat"            , "T5.dat"},
      {"./T6NoC34d.dat"         , "T6NoC34.dat"},
      {"./T7noc123d.dat"        , "T7noc123.dat"},
      {"./T8R34d.dat"           , "T8.dat"}
    };

    std::vector<File> manual;
    for (auto file : files)
      manual.push_back(file.first);

    multiplierPlot("TheoryBest", "Best strategies", manual);
    ratePlot("TheoryBest", "Best strategies", manual);
    timePlot("TheoryBest", "Best strategies", manual);
  }
}
