#ifndef __AST_ADAPTED_H__
#define __AST_ADAPTED_H__

#include <boost/fusion/include/adapt_struct.hpp>
#include "ast.h"

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::AndNode, opname, leftArg, rightArg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::OrNode, opname, leftArg, rightArg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::ImpNode, opname, leftArg, rightArg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::NotNode, opname, arg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::GPlusNode, opname, arg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::XPlusNode, opname, arg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::FPlusNode, opname, arg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::GMinusNode, opname, arg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::XMinusNode, opname, arg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::FMinusNode, opname, arg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::UNode, opname, leftArg, rightArg);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::EqlNode, opname, varname);

BOOST_FUSION_ADAPT_STRUCT(sexpr::ast::TraceSelNode, varname, traceid);

#endif
