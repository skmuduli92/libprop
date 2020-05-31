#ifndef __AST_H__
#define __AST_H__

#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <vector>

namespace x3 = boost::spirit::x3;

namespace sexpr::ast {
///////////////////////////////////////////////////////////////////////////
//  The AST
///////////////////////////////////////////////////////////////////////////

struct AndNode;
struct OrNode;
struct ImpNode;
struct NotNode;

struct GPlusNode;
struct XPlusNode;
struct FPlusNode;

struct GMinusNode;
struct XMinusNode;
struct FMinusNode;

struct UNode;

struct EqlNode {
  std::string opname;
  std::string varname;
};

struct TraceSelNode {
  std::string varname;
  unsigned traceid;
};

struct VarNode : x3::variant<EqlNode, TraceSelNode, x3::forward_ast<AndNode>,
                             x3::forward_ast<OrNode>, x3::forward_ast<ImpNode>,
                             x3::forward_ast<NotNode>, x3::forward_ast<GPlusNode>,
                             x3::forward_ast<XPlusNode>, x3::forward_ast<FPlusNode>,
                             x3::forward_ast<GMinusNode>, x3::forward_ast<XMinusNode>,
                             x3::forward_ast<FMinusNode>, x3::forward_ast<UNode> > {
  using base_type::base_type;
  using base_type::operator=;
};

struct NotNode {
  std::string opname;
  VarNode arg;
};

struct UnaryOpNode {
  std::string opname;
  VarNode arg;
};

struct BinaryOpNode {
  std::string opname;
  VarNode leftArg, rightArg;
};

struct AndNode {
  std::string opname;
  std::vector<VarNode> args;
};

struct OrNode {
  std::string opname;
  std::vector<VarNode> args;
};

struct ImpNode : BinaryOpNode {};

struct GPlusNode : UnaryOpNode {};
struct XPlusNode : UnaryOpNode {};
struct FPlusNode : UnaryOpNode {};

struct GMinusNode : UnaryOpNode {};
struct XMinusNode : UnaryOpNode {};
struct FMinusNode : UnaryOpNode {};

struct UNode : BinaryOpNode {};

}  // namespace sexpr::ast

#endif
