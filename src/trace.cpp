
#include <cstring>
#include <iomanip>
#include <iostream>

#include "trace.h"

// utilities to sirielize and deserielize Trace objects
//

// == class VarTrace

// == class Trace
//

//   std::vector<VarTrace<bool>> propositions;
//
//   std::vector<VarTrace<ValueType>> variables;
//
//   unsigned numProps;
//   unsigned numVars; // scan for types of each numvars
//                     // throw exception for arrayvar (not supported now)
//
//   unsigned lastcycles; // size = 1 + lastcycle
//   - storing the data in uncompressed format for now we can add compressed feature late
//
//   - storing the propositions, its bit easy since they are boolean data
//   $
//    for prop in propositions
//      for t in [0, lastcycle]
//        bstream << prop[t]
//
//    for intvar in numvars
//      for t in [0, lastcycle]
//        bstream << intvar[t]
//

void TraceSerialize::store(PTrace trace) {

  //
  // two parts of the data to be stored header part contains the total block size
  // including the header. Multiple data is stored in contiguous order
  //
  //                                    (new block to store)
  // ,,,,,,,,,,,,,,,,,,,,,,,,,,,,, ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  // |block-1 size | block-1 data| | block-2 size | block-2 data|
  // ''''''''''''''''''''''''''''' ''''''''''''''''''''''''''''''
  //                             | |
  //   no. of bytes till now     v v current_addr
  //                  <bytecount>

  uint8_t* currdest = destaddr + bytecount;

  if (trace->numProps() != 0) {
    // TODO :
    std::cerr << "Error : serialization is not supported for propositions\n";
    exit(1);
  }

  // reserving some bytes for header
  bytecount += sizeof(size_t);
  currdest += sizeof(size_t);

  // store numprops
  unsigned nprops = trace->numProps();
  memcpy(currdest, &nprops, sizeof(nprops));
  currdest += sizeof(nprops);
  bytecount += sizeof(nprops);

  // store numvars
  unsigned nvars = trace->numVars();
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
    // FIXME:
    // assuming rest of the element in the array are homogenous
    // change logic after issue #2 is pushed:
    // https://github.com/skmuduli92/libprop/issues/2#issue-612886338
    //----------------------------------------------------------------

    size_t vartype = termvar[0].index();
    size_t bcount;

    switch (vartype) {
      case 0:
        // uint32_t type
        bcount = serializeIntVar(termvar);
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

  assert(currdest == destaddr + bytecount);
}

size_t TraceSerialize::serializeIntVar(VarTrace<ValueType>& intvar) {

  size_t dsize = sizeof(uint32_t);
  size_t totalbytes = dsize * (1 + intvar.lastCycle);

  uint8_t* curr_addr = destaddr + bytecount;

  for (size_t tstep = 0; tstep <= intvar.lastCycle; ++tstep) {
    uint32_t data = std::get<uint32_t>(intvar[tstep]);
    memcpy(curr_addr, &data, dsize);
    curr_addr += dsize;
  }

  // TODO : just sending this as an ACK, change function signature
  // later to return -1 on failure.

  return totalbytes;
}

size_t TraceSerialize::getsize(PTrace trace) {

  // header_size + numvars + numprops + lastcycle
  size_t totalsize = 4 * sizeof(uint32_t);

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
