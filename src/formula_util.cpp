#include "formula.h"
#include "trace.h"

bool evaluateTraces(HyperPLTL::PHyperProp formula, TraceList const& traces) {

  bool result = false;
  long tracelen = traces[0]->length();
  for (long id = tracelen - 1; id >= 0; --id) {
    result = formula->eval(id, traces);
  }

  return result;
}
