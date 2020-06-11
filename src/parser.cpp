

#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <numeric>
#include <string>

#include "formula.h"
#include "parse_util.h"
#include "parser_def.h"

namespace x3 = boost::spirit::x3;

namespace sexpr::grammar {

using iterator_type = std::string::const_iterator;
using context_type = x3::phrase_parse_context<x3::ascii::space_type>::type;
BOOST_SPIRIT_INSTANTIATE(varexpr_t, iterator_type, context_type);

}  // namespace sexpr::grammar

namespace sexpr::ast {

///////////////////////////////////////////////////////////////////////////////
//  AST visitor methods
///////////////////////////////////////////////////////////////////////////////

struct HPLTLBuilder {
  using result_t = HyperPLTL::PHyperProp;

  HyperPLTL::PVarMap varmap;
  HPLTLBuilder(HyperPLTL::PVarMap inputmap) { varmap = inputmap; }

  result_t operator()(EqlNode const& eqlNode) const {
    // adding identifier to varmap
    unsigned varid = varmap->getVarIndex(eqlNode.varname);

    HyperPLTL::PTerm newvar;
    switch (varmap->getVarType(eqlNode.varname)) {
      using namespace HyperPLTL;
      case VarType::INT_VAR:
        newvar = std::make_shared<TermVar>(varmap, varid);
        break;
      case VarType::ARRAY_VAR:
        newvar = std::make_shared<TermArrayVar>(varmap, varid);
        break;
      default:
        assert(0);
        break;
    }

    result_t eq(new HyperPLTL::Equal(varmap, newvar));
    return eq;
  }

  result_t operator()(TraceSelNode const& selNode) const {
    unsigned varid = varmap->getPropIndex(selNode.varname);
    HyperPLTL::PTraceProp newvar(new HyperPLTL::PropVar(varmap, varid));
    result_t sel(new HyperPLTL::TraceSelect(varmap, selNode.traceid, newvar));
    return sel;
  }

  result_t operator()(AndNode const& andNode) const {
    std::vector<result_t> andInputVec;
    
    for (VarNode const& vnode : andNode.args) {
      andInputVec.push_back(boost::apply_visitor(*this, vnode));
    }

    result_t andP(new HyperPLTL::And(varmap, andInputVec));
    return andP;
  }

  result_t operator()(OrNode const& orNode) const {
    std::vector<result_t> orInputVec;
    
    for (VarNode const& vnode : orNode.args) {
      orInputVec.push_back(boost::apply_visitor(*this, vnode));
    }

    result_t orP(new HyperPLTL::Or(varmap, orInputVec));
    return orP;
  }

  result_t operator()(NotNode const& notNode) const {
    result_t argP = boost::apply_visitor(*this, notNode.arg);
    result_t notP(new HyperPLTL::Not(varmap, argP));
    return notP;
  }

  result_t operator()(ImpNode const& impNode) const {
    result_t leftP = boost::apply_visitor(*this, impNode.leftArg);
    result_t rightP = boost::apply_visitor(*this, impNode.rightArg);
    result_t imp(new HyperPLTL::Implies(varmap, leftP, rightP));
    return imp;
  }

  result_t operator()(GPlusNode const& gnode) const {
    result_t argP = boost::apply_visitor(*this, gnode.arg);
    result_t gplus(new HyperPLTL::AlwaysPlus(varmap, argP));
    return gplus;
  }

  result_t operator()(GMinusNode const& gnode) const {
    result_t argP = boost::apply_visitor(*this, gnode.arg);
    result_t gminus(new HyperPLTL::AlwaysMinus(varmap, argP));
    return gminus;
  }

  result_t operator()(XPlusNode const& ynode) const {
    result_t argP = boost::apply_visitor(*this, ynode.arg);
    result_t xplus(new HyperPLTL::NextPlus(varmap, argP));
    return xplus;
  }

  result_t operator()(XMinusNode const& ynode) const {
    result_t argP = boost::apply_visitor(*this, ynode.arg);
    result_t xminus(new HyperPLTL::NextMinus(varmap, argP));
    return xminus;
  }

  result_t operator()(FPlusNode const& onode) const {
    result_t argP = boost::apply_visitor(*this, onode.arg);
    result_t future(new HyperPLTL::FuturePlus(varmap, argP));
    return future;
  }

  result_t operator()(FMinusNode const& onode) const {
    result_t argP = boost::apply_visitor(*this, onode.arg);
    result_t future(new HyperPLTL::FutureMinus(varmap, argP));
    return future;
  }

  result_t operator()(UNode const& snode) const {
    result_t leftP = boost::apply_visitor(*this, snode.leftArg);
    result_t rightP = boost::apply_visitor(*this, snode.rightArg);
    result_t until(new HyperPLTL::Since(varmap, leftP, rightP));
    return until;
  }

  result_t operator()(VarNode const& varNode) const {
    return boost::apply_visitor(*this, varNode);
  }
};

}  // namespace sexpr::ast

///////////////////////////////////////////////////////////////////////////////
//  Parse Formula (Main Program)
///////////////////////////////////////////////////////////////////////////////

namespace HyperPLTL {

PHyperProp parse_formula(std::string const& str, PVarMap varmap) {
  typedef std::string::const_iterator iterator_type;
  typedef sexpr::ast::VarNode SExprAst;

  assert(varmap->empty() == false);
  auto grammar = sexpr::parser();
  SExprAst exprAst;

  iterator_type iter = str.begin();
  iterator_type end = str.end();
  boost::spirit::x3::ascii::space_type space;
  bool r = phrase_parse(iter, end, grammar, space, exprAst);

  if (!r || iter != end) {
    std::cerr << PARSE_ERR_MSG << std::endl;
    exit(1);
  }

  sexpr::ast::HPLTLBuilder propbuilder(varmap);
  PHyperProp prop = propbuilder(exprAst);
  return prop;
}

}  // namespace HyperPLTL
