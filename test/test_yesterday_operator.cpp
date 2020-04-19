#include <gtest/gtest.h>

#include "formula.h"
#include "parse_util.h"
#include "trace.h"

using namespace HyperPLTL;
using namespace std;

PHyperProp propertyXOperator() {
  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("x");
  varmap->addIntVar("y");

  std::string property("(IMPLIES (EQ x) (X (EQ y)))");
  auto prop = parse_formula(property, varmap);
  return prop;
}

TEST(PropertyLibTest, SimpleProperty_XOperator) {

  PVarMap varmap = std::make_shared<VarMap>();
  unsigned xid = varmap->addIntVar("x");

  std::string prop = "(X (EQ x))";
  auto property = parse_formula(prop, varmap);

  PTrace trace1(new Trace(0, 1));
  PTrace trace2(new Trace(0, 1));
  TraceList tracelist({trace1, trace2});

  srand(time(NULL));
  size_t cycle = 0;
  bool result = false;

  trace1->updateTermValue(xid, cycle, rand() % std::numeric_limits<unsigned>::max());
  trace2->updateTermValue(xid, cycle, rand() % std::numeric_limits<unsigned>::max());
  cycle++;

  trace1->updateTermValue(xid, cycle, 0);
  trace2->updateTermValue(xid, cycle, 0);
  cycle++;

  size_t numiter = 20 + (rand() % 20);
  for (; cycle < numiter; ++cycle) {
    trace1->updateTermValue(xid, cycle, rand() % std::numeric_limits<unsigned>::max());
    trace2->updateTermValue(xid, cycle, rand() % std::numeric_limits<unsigned>::max());
  }

  for (int cyc = cycle - 1; cyc >= 0; --cyc) result = property->eval(cyc, tracelist);

  ASSERT_TRUE(result);
}

TEST(PropertyLibTest, ValidTraceXOperator) {

  // (IMPLIES (X (EQ x)) (EQ y))

  PHyperProp property = propertyXOperator();
  PTrace trace1(new Trace(0, 2));
  PTrace trace2(new Trace(0, 2));
  TraceList tracelist({trace1, trace2});

  size_t cycle = 0;
  bool result = true;
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

  for (int cid = cycle; cid >= 0; --cid) {
    result = property->eval(cycle, tracelist);
  }

  EXPECT_TRUE(result);
}

TEST(PropertyLibTest, InvalidTraceXOperator) {
  PHyperProp property = propertyXOperator();

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

  for (int cid = cycle - 1; cid >= 0; --cid) {
    result = property->eval(cid, tracelist);
  }

  EXPECT_FALSE(result);
}

// test to check optimistic or pessimistic implementation of F
TEST(PropertyLibTest, TraceXPessmistic) {
  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("x");
  varmap->addIntVar("y");

  std::string propstr("(F (IMPLIES (EQ x) (X (EQ y))))");
  auto property = parse_formula(propstr, varmap);

  PTrace trace1(new Trace(0, 2));
  PTrace trace2(new Trace(0, 2));
  TraceList tracelist({trace1, trace2});

  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");

  unsigned traceLength = rand() % 20 + 20;

  unsigned xvalue = 0;
  unsigned yvalue = 0;

  for (size_t cid = 0; cid < traceLength - 1; ++cid) {
    xvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(xid, cid, xvalue);
    trace2->updateTermValue(xid, cid, xvalue);
    yvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(yid, cid, yvalue);
    trace2->updateTermValue(yid, cid, !yvalue);
  }

  // xvalue = rand() % std::numeric_limits<unsigned>::max();
  // trace1->updateTermValue(xid, traceLength - 1, xvalue);
  // trace2->updateTermValue(xid, traceLength - 1, xvalue);
  // yvalue = rand() % std::numeric_limits<unsigned>::max();
  // trace1->updateTermValue(yid, traceLength - 1, yvalue);
  // trace2->updateTermValue(yid, traceLength - 1, yvalue);

  bool result = true;

  EXPECT_FALSE(result);
}

TEST(PropertyLibTest, InvalidTraceYeqXNeverTrue) {
  // check the property evaluation value when (Y (eq x) is never true

  PHyperProp property = propertyXOperator();

  PTrace trace1(new Trace(0, 2));
  PTrace trace2(new Trace(0, 2));
  TraceList tracelist({trace1, trace2});

  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");

  bool result = true;
  unsigned traceLength = rand() % 20 + 20;

  size_t cycle = 0;

  for (; cycle < traceLength; ++cycle) {
    unsigned xvalue = rand() % 100;
    // setting 'x' var value
    trace1->updateTermValue(xid, cycle, xvalue);
    xvalue = (xvalue == 50) ? 51 : (100 - xvalue);
    trace2->updateTermValue(xid, cycle, xvalue);
    // setting 'y' var value
    trace1->updateTermValue(yid, cycle, rand() % 100);
    trace2->updateTermValue(yid, cycle, rand() % 100);
  }

  for (int cid = cycle - 1; cid >= 0; --cid) {
    result = property->eval(cycle, tracelist);
  }

  EXPECT_TRUE(result);
}
