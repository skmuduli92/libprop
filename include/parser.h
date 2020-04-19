
#ifndef __PARSER_H__
#define __PARSER_H__

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>

#include "ast.h"

namespace sexpr {
namespace grammar {

namespace x3 = boost::spirit::x3;

using idexpr_t = x3::rule<class IdExpr, std::string>;
using varexpr_t = x3::rule<class VarExpr, ast::VarNode>;

using orexpr_t = x3::rule<class OrExpr, ast::OrNode>;
using andexpr_t = x3::rule<class AndExpr, ast::AndNode>;
using impexpr_t = x3::rule<class ImpExpr, ast::ImpNode>;
using notexpr_t = x3::rule<class NotExpr, ast::NotNode>;
using eqlexpr_t = x3::rule<class EqlExpr, ast::EqlNode>;
using selexpr_t = x3::rule<class TraceSelExpr, ast::TraceSelNode>;

using gexprplus_t = x3::rule<class GExpr, ast::GPlusNode>;
using xexprplus_t = x3::rule<class YExpr, ast::XPlusNode>;
using fexprplus_t = x3::rule<class OExpr, ast::FPlusNode>;

using gexprminus_t = x3::rule<class GExpr, ast::GMinusNode>;
using xexprminus_t = x3::rule<class YExpr, ast::XMinusNode>;
using fexprminus_t = x3::rule<class OExpr, ast::FMinusNode>;

using uexpr_t = x3::rule<class SExpr, ast::UNode>;

using termexpr_t = x3::rule<class TermExpr, ast::VarNode>;

BOOST_SPIRIT_DECLARE(idexpr_t);
BOOST_SPIRIT_DECLARE(varexpr_t);

BOOST_SPIRIT_DECLARE(orexpr_t);
BOOST_SPIRIT_DECLARE(andexpr_t);
BOOST_SPIRIT_DECLARE(impexpr_t);
BOOST_SPIRIT_DECLARE(notexpr_t);
BOOST_SPIRIT_DECLARE(eqlexpr_t);
BOOST_SPIRIT_DECLARE(selexpr_t);

BOOST_SPIRIT_DECLARE(gexprplus_t);
BOOST_SPIRIT_DECLARE(xexprplus_t);
BOOST_SPIRIT_DECLARE(fexprplus_t);

BOOST_SPIRIT_DECLARE(gexprplus_t);
BOOST_SPIRIT_DECLARE(xexprminus_t);
BOOST_SPIRIT_DECLARE(fexprminus_t);

BOOST_SPIRIT_DECLARE(uexpr_t);

BOOST_SPIRIT_DECLARE(termexpr_t);

}  // namespace grammar

grammar::varexpr_t parser();
}  // namespace sexpr

#endif
