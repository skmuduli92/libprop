#include <gtest/gtest.h>
#include <memory>

#include "testutils.h"

TEST(TraceSerializeTest, StoreTraceObject) {

  PTrace trace(new Trace(0, 2));
  PTrace traceP(new Trace(0, 2));

  for (size_t cycle = 0; cycle < 10; ++cycle) {
    unsigned xvalue = rand() % std::numeric_limits<unsigned>::max();
    unsigned yvalue = rand() % std::numeric_limits<unsigned>::max();
    trace->updateTermValue(0, cycle, xvalue);
    trace->updateTermValue(1, cycle, yvalue);

    traceP->updateTermValue(1, cycle, yvalue);
    if (cycle == 5)
      traceP->updateTermValue(0, cycle, !xvalue);
    else
      traceP->updateTermValue(0, cycle, xvalue);
  }

  size_t memsize = TraceSerialize::getByteSize(trace);
  std::shared_ptr<uint8_t[]> memptr(new uint8_t[memsize],
                                    std::default_delete<uint8_t[]>());

  size_t bstorage = TraceSerialize::store(memptr.get(), trace);
  PTrace p = TraceSerialize::load(memptr.get());

  EXPECT_NE(trace, traceP);
  EXPECT_EQ(bstorage, memsize);
  EXPECT_EQ(*p, *trace);
}

TEST(TraceSerializeTest, StoreTraceWithArrayVar) {
  PTrace trace(new Trace(0, 2));

  std::vector<uint32_t> arrval(5);

  for (size_t cycle = 0; cycle < 10; ++cycle) {
    randomizeVecData(arrval);
    trace->updateTermValue(0, cycle, arrval);
    trace->updateTermValue(1, cycle, arrval);
  }

  size_t memsize = TraceSerialize::getByteSize(trace);
  std::shared_ptr<uint8_t[]> memptr(new uint8_t[memsize],
                                    std::default_delete<uint8_t[]>());

  size_t bstorage = TraceSerialize::store(memptr.get(), trace);
  std::cout << "bstorage : " << bstorage << std::endl;
  // PTrace p = TraceSerialize::load(memptr.get());

  // EXPECT_EQ(bstorage, memsize);
  // EXPECT_EQ(*p, *trace);
}
