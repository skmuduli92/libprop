#include <gtest/gtest.h>

#include "ast.h"
#include "formula.h"
#include "parse_util.h"
#include "parser.h"
#include "trace.h"

#include <memory>

TEST(TraceSerializeTest, StoreTraceObject) {

  PTrace trace(new Trace(0, 2));

  for (size_t cycle = 0; cycle < 10; ++cycle) {
    unsigned xvalue = rand() % std::numeric_limits<unsigned>::max();
    unsigned yvalue = rand() % std::numeric_limits<unsigned>::max();
    trace->updateTermValue(0, cycle, xvalue);
    trace->updateTermValue(1, cycle, yvalue);
  }

  size_t memsize = TraceSerialize::byteStorage(trace);
  std::cout << "total size of trace : " << memsize << std::endl;

  std::shared_ptr<uint8_t[]> memptr(new uint8_t[memsize],
                                    std::default_delete<uint8_t[]>());

  size_t bstorage = TraceSerialize::store(memptr.get(), trace);

  EXPECT_EQ(bstorage, memsize);
}
