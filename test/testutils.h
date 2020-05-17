#ifndef __TEST_UTILS_H
#define __TEST_UTILS_H

#include "ast.h"
#include "formula.h"
#include "formula_util.h"
#include "parse_util.h"
#include "parser.h"
#include "trace.h"

void randomizeVecData(std::vector<uint32_t>& vec);
void resetData(std::vector<uint32_t>& vec);

// guarantees the new random vector is different than
// the current random vector provided as input
void newRandomVecData(std::vector<uint32_t>& vec);

namespace sexpr::ast {

struct HPLTLStringBuilder {
  /////////////////////////////////////////////////////////////////
  // visitor methods to generate formula string back from an AST //
  /////////////////////////////////////////////////////////////////

  using result_t = std::string;
  result_t operator()(EqlNode const& eqlNode) const {
    return "(EQ " + eqlNode.varname + ")";
  }

  result_t operator()(TraceSelNode const& selNode) const {
    return selNode.varname + "." + std::to_string(selNode.traceid) + " ";
  }

  result_t operator()(AndNode const& andNode) const {
    return "(AND " + boost::apply_visitor(*this, andNode.leftArg) +
           boost::apply_visitor(*this, andNode.rightArg) + ")";
  }

  result_t operator()(OrNode const& orNode) const {
    return "(OR " + boost::apply_visitor(*this, orNode.leftArg) +
           boost::apply_visitor(*this, orNode.rightArg) + ")";
  }

  result_t operator()(NotNode const& notNode) const {
    return "(NOT " + boost::apply_visitor(*this, notNode.arg) + ")";
  }

  result_t operator()(ImpNode const& impNode) const {
    return "(IMPLIES " + boost::apply_visitor(*this, impNode.leftArg) +
           boost::apply_visitor(*this, impNode.rightArg) + ")";
  }

  result_t operator()(GPlusNode const& gnode) const {
    return "(G+ " + boost::apply_visitor(*this, gnode.arg) + ")";
  }

  result_t operator()(XPlusNode const& ynode) const {
    return "(X+ " + boost::apply_visitor(*this, ynode.arg) + ")";
  }

  result_t operator()(FPlusNode const& onode) const {
    return "(F+ " + boost::apply_visitor(*this, onode.arg) + ")";
  }

  result_t operator()(GMinusNode const& gnode) const {
    return "(G- " + boost::apply_visitor(*this, gnode.arg) + ")";
  }

  result_t operator()(XMinusNode const& ynode) const {
    return "(X- " + boost::apply_visitor(*this, ynode.arg) + ")";
  }

  result_t operator()(FMinusNode const& onode) const {
    return "(F- " + boost::apply_visitor(*this, onode.arg) + ")";
  }

  result_t operator()(UNode const& snode) const {
    return "(U " + boost::apply_visitor(*this, snode.leftArg) +
           boost::apply_visitor(*this, snode.rightArg) + ")";
  }

  result_t operator()(VarNode const& varNode) const {
    return boost::apply_visitor(*this, varNode);
  }
};

}  // namespace sexpr::ast

namespace HyperPLTL {
std::string parse_and_regen_string(std::string const& str);
}  // namespace HyperPLTL

#endif
