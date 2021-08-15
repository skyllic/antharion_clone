#pragma once
#include <theory1.h>
#include <theory2.h>
#include <theory3.h>
#include <theory4.h>
#include <theory5.h>
#include <theory6.h>
#include <theory7.h>
#include <theory8.h>

#include <memory>
#include<array>

using TheoryFactory = std::function<std::shared_ptr<Theory>()>;
auto getTheories()
{
  std::array<std::map<std::string, TheoryFactory>, 8> theories;

  theories[0] = {
    {"T1"   , std::make_shared<RecurrenceRelations>},
    {"T1C3" , std::make_shared<RecurrenceRelationsC3>},
    {"T1C4" , std::make_shared<RecurrenceRelationsC4>},
    {"T1C34", std::make_shared<RecurrenceRelationsC34>},
    {"T1AI" , std::make_shared<RecurrenceRelationsAI>},
 };
  theories[1] = {
    {"T2"   , std::make_shared<DifferentialCalculus>}
 };
  theories[2] = {
    {"T3",                 std::make_shared<LinearAlgebra>},
    {"T3NoC11C21C33" ,     std::make_shared<LinearAlgebraNoC11C21C33>},
    {"T3NoC11C21C33d" ,    std::make_shared<LinearAlgebraNoC11C21C33d>},
    {"T3B123C31C12C23",    std::make_shared<LinearAlgebraB123C31C12C23>},
    {"T3NoC11C13C21C33" ,  std::make_shared<LinearAlgebraNoC11C13C21C33>},
    {"T3NoC11C13C21C33d" , std::make_shared<LinearAlgebraNoC11C13C21C33d>},
 };
  theories[3] = {
    {"T4",        std::make_shared<Polynomials>},
    {"T4C3" ,     std::make_shared<PolynomialsC3>},
    {"T4C3d" ,    std::make_shared<PolynomialsC3d>},
    {"T4C123",    std::make_shared<PolynomialsC123>},
    {"T4C123d" ,  std::make_shared<PolynomialsC123d>},
 };
  theories[4] = {
    {"T5",        std::make_shared<LogisticFunction>},
    {"T5IOM",     std::make_shared<LogisticFunctionIOM>},
    {"T5AI" ,     std::make_shared<LogisticFunctionAI>},
    {"T5AI2" ,    std::make_shared<LogisticFunctionAI2>},
    {"T5AI3" ,    std::make_shared<LogisticFunctionAI3>},
    {"T5AI4" ,    std::make_shared<LogisticFunctionAI4>},
    {"T5AI5" ,    std::make_shared<LogisticFunctionAI5>}
 };
  theories[5] = {
    {"T6",           std::make_shared<IntegralCalculus>},
    {"T6NoC34" ,     std::make_shared<IntegralCalculusNoC34>},
    {"T6NoC34d" ,    std::make_shared<IntegralCalculusNoC34d>},
    {"T6NoC345" ,    std::make_shared<IntegralCalculusNoC345>},
    {"T6NoC345d" ,   std::make_shared<IntegralCalculusNoC345d>},
    {"T6NoC1234" ,   std::make_shared<IntegralCalculusNoC1234>},
    {"T6NoC1234d" ,  std::make_shared<IntegralCalculusNoC1234d>}
 };

  theories[6] = {
    {"T7",            std::make_shared<NumericalMethods>},
    {"T7NoDecay12" ,  std::make_shared<NumericalMethodsNoDecay12>},
    {"T7NoDecay1" ,   std::make_shared<NumericalMethodsNoDecay1>},
    {"T7NoDecay2",    std::make_shared<NumericalMethodsNoDecay2>},
    {"T7noc123" ,     std::make_shared<NumericalMethodsNoc123>},
    {"T7noc123d",     std::make_shared<NumericalMethodsNoc123d>}
 };

  theories[7] = {
    {"T8R",     std::make_shared<ChaosTheoryRoessler>},
    {"T8Rd" ,   std::make_shared<ChaosTheoryRoesslerD>},
    {"T8R3" ,   std::make_shared<ChaosTheoryRoessler3>},
    {"T8R3d" ,  std::make_shared<ChaosTheoryRoessler3d>},
    {"T8R4",    std::make_shared<ChaosTheoryRoessler4>},
    {"T8R4d",   std::make_shared<ChaosTheoryRoessler4d>},
    {"T8R5",    std::make_shared<ChaosTheoryRoessler5>},
    {"T8R5d",   std::make_shared<ChaosTheoryRoessler5d>},
    {"T8R34" ,  std::make_shared<ChaosTheoryRoessler34>},
    {"T8R34d" , std::make_shared<ChaosTheoryRoessler34d>},
    {"T8R35" ,  std::make_shared<ChaosTheoryRoessler35>},
    {"T8R35d" , std::make_shared<ChaosTheoryRoessler35d>},
    {"T8R45",   std::make_shared<ChaosTheoryRoessler45>},
    {"T8R45d",  std::make_shared<ChaosTheoryRoessler45d>},
    {"T8C",     std::make_shared<ChaosTheoryChen>},
    {"T8L" ,    std::make_shared<ChaosTheoryLorentz>}
 };
  return theories;
}
