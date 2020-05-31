#include <gtest/gtest.h>

#include "formula.h"
#include "formula_util.h"
#include "parse_util.h"
#include "trace.h"

using namespace HyperPLTL;
using namespace std;

TEST(PropertyLibTest, MultiInputAndOperatorValidTrace) {

  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("w");
  varmap->addIntVar("x");
  varmap->addIntVar("y");
  varmap->addIntVar("z");

  std::string propstr("(G+ ( AND (EQ w) (EQ x) (EQ  y) (EQ z) ) )");
  auto property = parse_formula(propstr, varmap);

  PTrace trace1(new Trace(0, 4));
  PTrace trace2(new Trace(0, 4));
  TraceList tracelist({trace1, trace2});

  unsigned wid = property->getVarId("w");
  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");
  unsigned zid = property->getVarId("z");

  unsigned traceLength = rand() % 20 + 20;

  unsigned tvalue = 0;

  for (size_t cid = 0; cid < traceLength - 1; ++cid) {

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(wid, cid, tvalue);
    trace2->updateTermValue(wid, cid, tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(xid, cid, tvalue);
    trace2->updateTermValue(xid, cid, tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(yid, cid, tvalue);
    trace2->updateTermValue(yid, cid, tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(zid, cid, tvalue);
    trace2->updateTermValue(zid, cid, tvalue);
  }

  bool result = evaluateTraces(property, tracelist);
  EXPECT_TRUE(result);
}

TEST(PropertyLibTest, MultiInputOrOperatorValidTrace) {

  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("w");
  varmap->addIntVar("x");
  varmap->addIntVar("y");
  varmap->addIntVar("z");

  std::string propstr("(G+ ( OR (EQ w) (EQ x) (EQ  y) (EQ z) ) )");
  auto property = parse_formula(propstr, varmap);

  PTrace trace1(new Trace(0, 4));
  PTrace trace2(new Trace(0, 4));
  TraceList tracelist({trace1, trace2});

  unsigned wid = property->getVarId("w");
  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");
  unsigned zid = property->getVarId("z");

  unsigned traceLength = rand() % 20 + 20;

  unsigned tvalue = 0;

  for (size_t cid = 0; cid < traceLength - 1; ++cid) {

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(wid, cid, tvalue);
    trace2->updateTermValue(wid, cid, !tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(xid, cid, tvalue);
    trace2->updateTermValue(xid, cid, !tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(yid, cid, tvalue);
    trace2->updateTermValue(yid, cid, tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(zid, cid, !tvalue);
    trace2->updateTermValue(zid, cid, tvalue);
  }

  bool result = evaluateTraces(property, tracelist);
  EXPECT_TRUE(result);
}

TEST(PropertyLibTest, MultiInputAndOperatorInvalidTrace) {

  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("w");
  varmap->addIntVar("x");
  varmap->addIntVar("y");
  varmap->addIntVar("z");

  std::string propstr("(G+ ( AND (EQ w) (EQ x) (EQ  y) (EQ z)))");
  auto property = parse_formula(propstr, varmap);

  PTrace trace1(new Trace(0, 4));
  PTrace trace2(new Trace(0, 4));
  TraceList tracelist({trace1, trace2});

  unsigned wid = property->getVarId("w");
  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");
  unsigned zid = property->getVarId("z");

  unsigned traceLength = rand() % 20 + 20;

  unsigned tvalue = 0;

  for (size_t cid = 0; cid < traceLength - 1; ++cid) {

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(wid, cid, tvalue);
    trace2->updateTermValue(wid, cid, tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(xid, cid, tvalue);
    trace2->updateTermValue(xid, cid, !tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(yid, cid, tvalue);
    trace2->updateTermValue(yid, cid, tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(zid, cid, tvalue);
    trace2->updateTermValue(zid, cid, tvalue);
  }

  bool result = evaluateTraces(property, tracelist);
  EXPECT_FALSE(result);
}

TEST(PropertyLibTest, MultiInputOrOperatorInvalidTrace) {

  PVarMap varmap = std::make_shared<VarMap>();
  varmap->addIntVar("w");
  varmap->addIntVar("x");
  varmap->addIntVar("y");
  varmap->addIntVar("z");

  std::string propstr("(G+ ( OR (EQ w) (EQ x) (EQ  y) (EQ z)))");
  auto property = parse_formula(propstr, varmap);

  PTrace trace1(new Trace(0, 4));
  PTrace trace2(new Trace(0, 4));
  TraceList tracelist({trace1, trace2});

  unsigned wid = property->getVarId("w");
  unsigned xid = property->getVarId("x");
  unsigned yid = property->getVarId("y");
  unsigned zid = property->getVarId("z");

  unsigned traceLength = rand() % 20 + 20;

  unsigned tvalue = 0;

  for (size_t cid = 0; cid < traceLength - 1; ++cid) {

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(wid, cid, tvalue);
    trace2->updateTermValue(wid, cid, !tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(xid, cid, tvalue);
    trace2->updateTermValue(xid, cid, !tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(yid, cid, tvalue);
    trace2->updateTermValue(yid, cid, !tvalue);

    tvalue = rand() % std::numeric_limits<unsigned>::max();
    trace1->updateTermValue(zid, cid, tvalue);
    trace2->updateTermValue(zid, cid, !tvalue);
  }

  bool result = evaluateTraces(property, tracelist);
  EXPECT_FALSE(result);
}
