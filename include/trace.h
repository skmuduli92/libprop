#ifndef __TRACE_H_DEFINED__
#define __TRACE_H_DEFINED__

#include <algorithm>
#include <cassert>
#include <memory>
#include <variant>
#include <vector>

class Trace;
typedef std::shared_ptr<Trace> PTrace;

template <class T>
struct VarTrace {
  /// DataPoint stores the value of a signal at a particular time index.
  using DataPoint = std::pair<uint32_t, T>;
  std::vector<DataPoint> datapoints;

  /// time when the last addition was performed.
  uint32_t lastCycle;

 public:
  VarTrace() : lastCycle(0) {}

  /**
   * updateValue(t, value) must be called every cycle.
   * It will insert into the vector if needed.
   */
  void updateValue(uint32_t time, const T& v) {
    if (datapoints.size() == 0) {
      assert(time == 0);
      datapoints.push_back(DataPoint(time, v));
    } else {
      // must update a time index only once.
      assert(time > lastCycle);

      // find the last index into the array.
      size_t last = datapoints.size() - 1;
      assert(last <= time);

      // check if we need to add.
      if (datapoints[last].second != v) {
        datapoints.push_back(std::pair(time, v));
      }  // else nothing to do.
    }

    lastCycle = time;
  }

  /** Extends the trace to the specified number of cycles. */
  void extendToCycle(uint32_t cycle) {
    assert (cycle >= lastCycle);
    lastCycle = cycle;
  }

  /// Return the element at a particular index.
  const T operator[](uint32_t cycle) {
    assert(datapoints.size() > 0);

    auto lowerCmp = [](const std::pair<unsigned int, T>& cv, const unsigned int v) {
      return cv.first < v;
    };
    auto upperCmp = [](const unsigned int v, const std::pair<unsigned int, T>& cv) {
      return v < cv.first;
    };

    auto lower = std::lower_bound(datapoints.begin(), datapoints.end(), cycle, lowerCmp);
    assert((lower - datapoints.begin()) >= 0);
    auto upper = std::upper_bound(datapoints.begin(), datapoints.end(), cycle, upperCmp);

    if (lower == upper)
      return (lower - 1)->second;
    else
      return lower->second;
  }

  size_t size() const { return datapoints.size(); }
};

using ValueType = std::variant<uint32_t, std::vector<uint32_t>>;

class Trace {
  /** A vector of traces for each propositional variable. */
  std::vector<VarTrace<bool>> propositions;

  /** A vector of traces for each term variable. */
  std::vector<VarTrace<ValueType>> variables;

  /** The last valid time cycle in this trace. */
  unsigned lastCycle;

 public:
  /** Create a trace capable of storing numVars variables and
      numProps propositions. */
  Trace(unsigned numProps, unsigned numVars)
      : propositions(numProps), variables(numVars), lastCycle(0) {}

  /** Return the number of propositional variables in the trace. */
  unsigned numProps() const { return propositions.size(); }

  /** Return the number of term (numeric) variables in the trace. */
  unsigned numVars() const { return variables.size(); }

  /** Update the value of variable i at time cycle. */
  void updateTermValue(unsigned i, uint32_t cycle, ValueType value) {
    assert(i < variables.size());
    if (lastCycle < cycle) {
      lastCycle = cycle;
    }
    variables[i].updateValue(cycle, value);
  }


  /** Update the value of proposition i at time cycle. */
  void updatePropValue(unsigned i, uint32_t cycle, bool value) {
    assert(i < propositions.size());
    if (lastCycle < cycle) {
      lastCycle = cycle;
    }
    propositions[i].updateValue(cycle, value);
  }

  /** Return the value of variable i at time cycle. */
  ValueType termValueAt(unsigned i, uint32_t cycle) {
    assert(i < variables.size());
    return variables[i][cycle];
  }


  /** Return the value of a proposition i at time cycle. */
  bool propValueAt(unsigned i, uint32_t cycle) {
    assert(i < propositions.size());
    return propositions[i][cycle];
  }

  void extendToCycle(uint32_t cycle) {
    assert (cycle >= lastCycle);
    lastCycle = cycle;
    for (auto p : propositions) { p.extendToCycle(cycle); }
    for (auto v : variables) { v.extendToCycle(cycle); }
  }

  /// get trace length (un-compressed)
  size_t length(void) { return lastCycle; }
};

typedef std::vector<PTrace> TraceList;
#endif
