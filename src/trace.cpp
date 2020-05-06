
#include <cstring>
#include <iomanip>
#include <iostream>

#include "trace.h"

uint32_t TraceSerialize::store(uint8_t* dest, PTrace trace) {

  //
  // two parts of the data to be stored header part contains the total block size
  // including the header.
  //
  // ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  // |   block-1 header   | block-1 data  |
  // ''''''''''''''''''''''''''''''''''''''
  //  <---- store returns total size ---->
  //

  uint8_t* currdest = dest;
  uint32_t bytecount = 0;

  if (trace->numProps() != 0) {
    // TODO :
    std::cerr << "Error : serialization is not supported for propositions\n";
    exit(1);
  }

  // reserving some bytes for header
  bytecount += sizeof(bytecount);
  currdest += sizeof(bytecount);

  auto nprops = trace->numProps();
  auto nvars = trace->numVars();

  // store numprops
  memcpy(currdest, &nprops, sizeof(nprops));
  currdest += sizeof(nprops);
  bytecount += sizeof(nprops);

  // store numvars
  memcpy(currdest, &nvars, sizeof(nvars));
  currdest += sizeof(nvars);
  bytecount += sizeof(nvars);

  // store lastCycle value
  memcpy(currdest, &(trace->lastCycle), sizeof(trace->lastCycle));
  currdest += sizeof(trace->lastCycle);
  bytecount += sizeof(trace->lastCycle);

  // for (auto& propvar : trace->propositions) {
  //   // TODO : to be implemented
  // }

  for (auto& termvar : trace->variables) {

    //----------------------------------------------------------------
    // TODO:
    // assuming rest of the element in the array are homogenous
    // change logic after issue #2 is pushed:
    // https://github.com/skmuduli92/libprop/issues/2#issue-612886338
    //----------------------------------------------------------------

    size_t vartype = termvar[0].index();
    size_t bcount = 0;

    switch (vartype) {
      case 0:
        // uint32_t type
        bcount = serializeIntVar(currdest, termvar);
        bytecount += bcount;
        currdest += bcount;
        break;

      case 1:
        // std::vector<uint32_t>
        // TODO : needs to implement
        std::cerr << "Error : TermArrayVar is not supported yet\n";
        exit(1);
        break;

      default:
        assert(false);
        break;
    }
  }

  // storing the header
  memcpy(dest, &bytecount, sizeof(bytecount));

  assert(currdest == dest + bytecount);
  return bytecount;
}

PTrace TraceSerialize::load(uint8_t* memloc) {

  // PTrace trace(new Trace(nprops, nvars));

  uint8_t* currsrc = memloc;

  uint32_t blocksize;
  memcpy(&blocksize, currsrc, sizeof(blocksize));
  currsrc += sizeof(blocksize);

  uint32_t nprops;
  uint32_t nvars;
  uint32_t lastTStep;

  memcpy(&nprops, currsrc, sizeof(nprops));
  currsrc += sizeof(nprops);

  memcpy(&nvars, currsrc, sizeof(nvars));
  currsrc += sizeof(nvars);

  memcpy(&lastTStep, currsrc, sizeof(lastTStep));
  currsrc += sizeof(lastTStep);

  // create trace object
  PTrace trace(new Trace(nprops, nvars));
  uint32_t value;

  for (size_t varid = 0; varid < nvars; ++varid) {
    for (size_t tstep = 0; tstep <= lastTStep; ++tstep) {
      memcpy(&value, currsrc, sizeof(value));
      currsrc += sizeof(value);
      trace->updateTermValue(varid, tstep, value);
    }
  }

  return trace;
}

size_t TraceSerialize::serializeIntVar(uint8_t* dest, VarTrace<ValueType>& intvar) {

  size_t dsize = sizeof(uint32_t);
  size_t totalbytes = dsize * (1 + intvar.lastCycle);

  uint8_t* curr_addr = dest;

  for (size_t tstep = 0; tstep <= intvar.lastCycle; ++tstep) {
    uint32_t data = std::get<uint32_t>(intvar[tstep]);
    memcpy(curr_addr, &data, dsize);
    curr_addr += dsize;
  }

  // TODO : just sending this as an ACK, change function signature
  // later to return -1 on failure.

  return totalbytes;
}

uint32_t TraceSerialize::byteStorage(PTrace trace) {

  // header_size + numvars + numprops + lastcycle
  auto nprops = trace->numProps();
  auto nvars = trace->numVars();
  size_t totalsize =
      sizeof(uint32_t) + sizeof(nprops) + sizeof(nvars) + sizeof(trace->lastCycle);

  // TODO : compute size of the prop vars

  // computing size of termvars

  for (auto& termvar : trace->variables) {
    size_t vartype = termvar[0].index();

    switch (vartype) {
      case 0:
        // uint32_t type
        totalsize += (1 + termvar.lastCycle) * sizeof(uint32_t);
        break;

      case 1:
        // std::vector<uint32_t>
        // TODO : needs to implement
        std::cerr << "Error : TermArrayVar is not supported yet\n";
        exit(1);
        break;

      default:
        assert(false);
        break;
    }
  }

  return totalsize;
}
