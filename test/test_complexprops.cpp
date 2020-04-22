#include <gtest/gtest.h>

#include "formula.h"
#include "formula_util.h"
#include "parse_util.h"
#include "trace.h"

using namespace HyperPLTL;
using namespace std;

// test to check optimistic or pessimistic implementation of F
TEST(PropertyLibTest, TraceXPessmistic) {
  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("x");
  varmap->addIntVar("y");

  std::string propstr("(F- (IMPLIES (EQ x) (X- (EQ y))))");
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

  bool result = evaluateTraces(property, tracelist);
  EXPECT_FALSE(result);
}
