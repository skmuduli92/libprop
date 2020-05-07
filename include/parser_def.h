#ifndef __PARSER_DEF_H__
#define __PARSER_DEF_H__

#include "ast.h"
#include "ast_adapted.h"
#include "parser.h"

namespace sexpr {

namespace grammar {

///////////////////////////////////////////////////////////////////////////////
//  Grammar
///////////////////////////////////////////////////////////////////////////////

x3::rule<class IdExpr, std::string> idexpr("idexpr");
x3::rule<class VarExpr, ast::VarNode> varexpr("varexpr");

x3::rule<class OrExpr, ast::OrNode> orexpr("orexpr");
x3::rule<class AndExpr, ast::AndNode> andexpr("andexpr");
x3::rule<class ImpExpr, ast::ImpNode> impexpr("impexpr");
x3::rule<class NotExpr, ast::NotNode> notexpr("notexpr");
x3::rule<class EqlExpr, ast::EqlNode> eqlexpr("eqlexpr");
x3::rule<class TraceSelExpr, ast::TraceSelNode> selexpr("selexpr");

x3::rule<class GExpr, ast::GPlusNode> gplusexpr("gplusexpr");
x3::rule<class YExpr, ast::XPlusNode> xplusexpr("xplusexpr");
x3::rule<class OExpr, ast::FPlusNode> fplusexpr("fplusexpr");

x3::rule<class GExpr, ast::GMinusNode> gminusexpr("gminusexpr");
x3::rule<class YExpr, ast::XMinusNode> xminusexpr("xminusexpr");
x3::rule<class OExpr, ast::FMinusNode> fminusexpr("fminusexpr");

x3::rule<class SExpr, ast::UNode> uexpr("uexpr");

x3::rule<class TermExpr, ast::VarNode> termexpr("termexpr");

auto const andstr = x3::string("AND");
auto const orstr = x3::string("OR");
auto const impstr = x3::string("IMPLIES");
auto const notstr = x3::string("NOT");

auto const eqstr = x3::string("EQ");

auto const gstrplus = x3::string("G+");
auto const xstrplus = x3::string("X+");
auto const fstrplus = x3::string("F+");

auto const gstrminus = x3::string("G-");
auto const xstrminus = x3::string("X-");
auto const fstrminus = x3::string("F-");

auto const ustr = x3::string("U");

auto const keywords = andstr | orstr | notstr | impstr | gstrplus | xstrplus | fstrplus |
                      ustr | eqstr | gstrminus | xstrminus | fstrminus;

auto const idexpr_def =
    (x3::lexeme[x3::alpha >> *(x3::alnum | x3::char_('_'))]) - keywords;

// TODO : create another heirarchy to separate optimisitc and pessimistic temporal
// operators
auto const varexpr_def = '(' >>
                         (notexpr | andexpr | orexpr | impexpr | gplusexpr | xplusexpr |
                          fplusexpr | uexpr | xminusexpr | fminusexpr | gminusexpr) >>
                         ')';

auto const orexpr_def = orstr >> termexpr >> termexpr;
auto const andexpr_def = andstr >> termexpr >> termexpr;
auto const impexpr_def = impstr >> termexpr >> termexpr;
auto const notexpr_def = notstr >> termexpr;

auto const gplusexpr_def = gstrplus >> termexpr;
auto const xplusexpr_def = xstrplus >> termexpr;
auto const fplusexpr_def = fstrplus >> termexpr;

auto const gminusexpr_def = gstrminus >> termexpr;
auto const xminusexpr_def = xstrminus >> termexpr;
auto const fminusexpr_def = fstrminus >> termexpr;

auto const uexpr_def = ustr >> termexpr >> termexpr;

auto const termexpr_def = varexpr | ('(' >> eqlexpr >> ')') | selexpr;
auto const eqlexpr_def = eqstr >> idexpr;
auto const selexpr_def = idexpr >> '.' >> x3::uint_;

BOOST_SPIRIT_DEFINE(idexpr, varexpr, notexpr, andexpr, orexpr, impexpr);
BOOST_SPIRIT_DEFINE(gplusexpr, xplusexpr, fplusexpr, uexpr, termexpr);
BOOST_SPIRIT_DEFINE(gminusexpr, xminusexpr, fminusexpr)
BOOST_SPIRIT_DEFINE(eqlexpr, selexpr);
}  // namespace grammar

grammar::varexpr_t parser() { return grammar::varexpr; }

}  // namespace sexpr

#endif
