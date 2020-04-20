
#include <gtest/gtest.h>

#include "formula.h"
#include "parse_util.h"
#include "formula_util.h"
#include "trace.h"

using namespace HyperPLTL;
using namespace std;

PHyperProp property1NextPlusOperator() {
  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("x");
  std::string formula("(G+ (X+ (EQ x)))");
  return parse_formula(formula, varmap);
}


TEST(PropertyLibTest, ValidTraceNextPlusOperator_Test1) {
  PHyperProp property = property1NextPlusOperator();

  PTrace trace1(new Trace(0, 1));
  PTrace trace2(new Trace(0, 1));
  TraceList tracelist({trace1, trace2});

  bool result = false;
  unsigned traceLength = rand() % 20 + 20;

  unsigned xvalue = 0;
  for (size_t cycle = 0; cycle < traceLength; ++cycle) {
    xvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(0, cycle, xvalue);
    trace2->updateTermValue(0, cycle, xvalue);
  }

  result = evaluateTraces(property, tracelist);

  EXPECT_TRUE(result);
}

TEST(PropertyLibTest, InvalidTraceNextPlusOperator_Test1) {
  PHyperProp property = property1NextPlusOperator();

  PTrace trace1(new Trace(0, 1));
  PTrace trace2(new Trace(0, 1));
  TraceList tracelist({trace1, trace2});

  bool result = false;
  unsigned traceLength = rand() % 20 + 20;
  size_t cycle = 0;

  unsigned xvalue = 0;
  for (; cycle < traceLength; ++cycle) {
    xvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(0, cycle, xvalue);
    trace2->updateTermValue(0, cycle, xvalue);
  }

  trace1->updateTermValue(0, cycle, xvalue);
  trace2->updateTermValue(0, cycle, !xvalue);
  
  result = evaluateTraces(property, tracelist);

  EXPECT_FALSE(result);
}