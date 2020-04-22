
#include <gtest/gtest.h>

#include "formula.h"
#include "formula_util.h"
#include "parse_util.h"
#include "trace.h"

using namespace HyperPLTL;
using namespace std;

PHyperProp property1NextMinusOperator() {
  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("x");
  std::string formula("(G+ (X- (EQ x)))");
  return parse_formula(formula, varmap);
}
PHyperProp property2NextMinusOperator() {
  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("x");
  std::string formula("(X- (EQ x))");
  return parse_formula(formula, varmap);
}

PHyperProp property3NextMinuxOperator() {
  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("x");
  varmap->addIntVar("y");

  std::string property("(IMPLIES (EQ x) (X- (EQ y)))");
  auto prop = parse_formula(property, varmap);
  return prop;
}

TEST(PropertyLibTest, ValidTraceNextMinusOperator_Test1) {
  PHyperProp property = property1NextMinusOperator();

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

  EXPECT_FALSE(result);
}

TEST(PropertyLibTest, InvalidTraceNextMinusOperator_Test1) {
  PHyperProp property = property1NextMinusOperator();

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

TEST(PropertyLibTest, InvalidTraceNextMinusOperator_TestAtTraceEnd) {
  PHyperProp property = property2NextMinusOperator();

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

  result = property->eval(traceLength - 1, tracelist);
  EXPECT_FALSE(result);
}

TEST(PropertyLibTest, ValidTraceXMinusOperatorWithImpliesOp) {

  // (IMPLIES (EQ x) (X (EQ y)))

  PHyperProp property = property3NextMinuxOperator();
  PTrace trace1(new Trace(0, 2));
  PTrace trace2(new Trace(0, 2));
  TraceList tracelist({trace1, trace2});

  size_t cycle = 0;
  bool result = false;
  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");

  trace1->updateTermValue(xid, cycle, 20);
  trace2->updateTermValue(xid, cycle, 20);
  trace1->updateTermValue(yid, cycle, rand() % std::numeric_limits<unsigned>::max());
  trace2->updateTermValue(yid, cycle, rand() % std::numeric_limits<unsigned>::max());

  cycle = cycle + 1;

  trace1->updateTermValue(xid, cycle, 20);
  trace2->updateTermValue(xid, cycle, 21);
  trace1->updateTermValue(yid, cycle, 10);
  trace2->updateTermValue(yid, cycle, 10);

  result = evaluateTraces(property, tracelist);
  EXPECT_TRUE(result);
}

TEST(PropertyLibTest, InvalidTraceXOperatorWithImpliesOp) {
  PHyperProp property = property3NextMinuxOperator();

  PTrace trace1(new Trace(0, 2));
  PTrace trace2(new Trace(0, 2));
  TraceList tracelist({trace1, trace2});

  size_t cycle = 0;
  bool result = true;
  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");

  unsigned traceLength = rand() % 20 + 20;

  trace1->updateTermValue(xid, cycle, 20);
  trace2->updateTermValue(xid, cycle, 20);
  trace1->updateTermValue(yid, cycle, 12);
  trace2->updateTermValue(yid, cycle, 10);
  cycle = cycle + 1;

  trace1->updateTermValue(xid, cycle, 20);
  trace2->updateTermValue(xid, cycle, 21);
  trace1->updateTermValue(yid, cycle, 11);
  trace2->updateTermValue(yid, cycle, 10);
  cycle = cycle + 1;

  for (; cycle < traceLength; ++cycle) {
    trace1->updateTermValue(xid, cycle, rand() % 100);
    trace2->updateTermValue(xid, cycle, rand() % 100);
    trace1->updateTermValue(yid, cycle, rand() % 100);
    trace2->updateTermValue(yid, cycle, rand() % 100);
  }

  result = evaluateTraces(property, tracelist);
  EXPECT_FALSE(result);
}

TEST(PropertyLibTest, InvalidTraceXMinusWithImpliesOp_VacuouslyTrue) {
  // check the property evaluation value when (eq x) is never true
  // (IMPLIES (EQ x) (X- (EQ y)))
  PHyperProp property = property3NextMinuxOperator();

  PTrace trace1(new Trace(0, 2));
  PTrace trace2(new Trace(0, 2));
  TraceList tracelist({trace1, trace2});

  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");

  bool result = false;
  unsigned traceLength = rand() % 20 + 20;

  size_t cycle = 0;

  for (; cycle < traceLength; ++cycle) {
    unsigned xvalue = rand() % 100;
    // setting 'x' var value
    trace1->updateTermValue(xid, cycle, xvalue);
    trace2->updateTermValue(xid, cycle, !xvalue);
    // setting 'y' var value
    trace1->updateTermValue(yid, cycle, rand() % 100);
    trace2->updateTermValue(yid, cycle, rand() % 100);
  }

  result = evaluateTraces(property, tracelist);
  EXPECT_TRUE(result);
}
