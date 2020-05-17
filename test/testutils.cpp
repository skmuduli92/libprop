#include "testutils.h"

namespace HyperPLTL {

std::string parse_and_regen_string(std::string const& str) {
  typedef std::string::const_iterator iterator_type;
  typedef sexpr::ast::VarNode SExprAst;

  auto grammar = sexpr::parser();
  SExprAst exprAst;

  iterator_type iter = str.begin();
  iterator_type end = str.end();

  boost::spirit::x3::ascii::space_type space;
  bool r = phrase_parse(iter, end, grammar, space, exprAst);

  if (!r || iter != end) {
    std::cerr << "Error : Parsing failed\n";
    exit(1);
  }

  sexpr::ast::HPLTLStringBuilder strbuilder;
  return strbuilder(exprAst);
}

}  // namespace HyperPLTL

void randomizeVecData(std::vector<uint32_t>& vec) {
  for (uint32_t& data : vec) {
    data = rand() % std::numeric_limits<uint32_t>::max();
  }
}

void resetData(std::vector<uint32_t>& vec) {
  for (uint32_t& data : vec) {
    data = 0;
  }
}

// guarantees the new random vector is different than
// the current random vector provided as input
void newRandomVecData(std::vector<uint32_t>& vec) {
  std::vector<uint32_t> oldVecd(vec);
  while (oldVecd == vec) {
    for (uint32_t& data : vec) {
      data = rand() % std::numeric_limits<uint32_t>::max();
    }
  }
}
