
#include <gtest/gtest.h>

#include "ast.h"
#include "formula.h"
#include "parse_util.h"
#include "parser.h"

std::string prune_whitespaces(std::string const& str) {
  std::string newstr = "";
  for (const auto c : str)
    if (c != ' ') newstr.push_back(c);

  return newstr;
}

TEST(PropertyLibTest, Parse_NOT) {
  std::string prop = "(NOT (EQ x))";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}

TEST(PropertyLibTest, Parse_AND) {
  std::string prop = "(AND (EQ x11) (EQ y12a))";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}

TEST(PropertyLibTest, Parse_OR) {
  std::string prop = "(OR (EQ x11) (G+ (EQ y12a)) )";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}

TEST(PropertyLibTest, Parse_IMPLIES) {
  std::string prop = "(IMPLIES (OR (EQ ab12) (EQ x11)) (EQ y12a))";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}

TEST(PropertyLibTest, Parse_TraceSelect) {
  std::string prop = "(F- (IMPLIES good.1 good.2))";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}

TEST(PropertyLibTest, Parse_TestFailure_InvalidVarName) {
  std::string prop = "(NOT (EQ 1x))";  // invalid identifier name

  EXPECT_EXIT(HyperPLTL::parse_and_regen_string(prop), ::testing::ExitedWithCode(1),
              PARSE_ERR_MSG);
}

TEST(PropertyLibTest, Parse_TestFailure_ParenMisMatch) {
  std::string prop = "(NOT (EQ xy)";

  EXPECT_EXIT(HyperPLTL::parse_and_regen_string(prop), ::testing::ExitedWithCode(1),
              PARSE_ERR_MSG);
}

TEST(PropertyLibTest, Parse_TestFailure_OnlyParens) {
  std::string prop = "( )";

  EXPECT_EXIT(HyperPLTL::parse_and_regen_string(prop), ::testing::ExitedWithCode(1),
              PARSE_ERR_MSG);
}

TEST(PropertyLibTest, Parse_TestFailure_VarWithOutEQ) {
  std::string prop = "(OR ab (EQ xy)";

  EXPECT_EXIT(HyperPLTL::parse_and_regen_string(prop), ::testing::ExitedWithCode(1),
              PARSE_ERR_MSG);
}

TEST(PropertyLibTest, Parse_TestFailure_KeywordAsVarName) {
  std::string prop = "(OR ab (EQ AND)";

  EXPECT_EXIT(HyperPLTL::parse_and_regen_string(prop), ::testing::ExitedWithCode(1),
              PARSE_ERR_MSG);
}

TEST(PropertyLibTest, Parse_ComplexProperty) {
  std::string prop =
      "(G+ (IMPLIES (AND (NOT (EQ ab)) (EQ bc)) (X+ (OR (EQ cd) (EQ de)))))";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}

TEST(PropertyLibTest, Parse_TestFailure_ComplexProperty) {
  std::string prop =
      "(G- (IMPLIES (AND (NOT (EQ ab)) (EQ bc)) (X- (OR (EQ cd) (EQ de)))) (EQ ef))";

  EXPECT_EXIT(HyperPLTL::parse_and_regen_string(prop), ::testing::ExitedWithCode(1),
              PARSE_ERR_MSG);
}

TEST(PropertyLibTest, Parse_TermVarArray) {
  std::string prop = "(IMPLIES (EQ ab) (EQ bytes) )";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}

TEST(PropertyLibTest, ParserVarMap) {
  std::string prop1 = "(G+ (IMPLIES (EQ idTwo) (EQ idOne)))";

  HyperPLTL::PVarMap varmap = std::make_shared<HyperPLTL::VarMap>();
  varmap->addIntVar("idOne");
  varmap->addIntVar("idTwo");
  varmap->addIntVar("idThree");

  auto hpltl1 = parse_formula(prop1, varmap);

  std::string prop2 = "(IMPLIES (G+ (AND (EQ idThree) (EQ idOne))) (EQ idTwo))";
  auto hpltl2 = parse_formula(prop2, varmap);

  // TODO : write a function or lamda to create a pairwise comparision

  EXPECT_EQ(hpltl1->getVarId("idOne"), hpltl2->getVarId("idOne"));
  EXPECT_EQ(hpltl1->getVarId("idTwo"), hpltl2->getVarId("idTwo"));
}

TEST(PropertyLibTest, ParseFormulaWithUnderscore) {

  std::string prop = "( G+ ( IMPLIES ( EQ aes_reg_state_next )( EQ aes_reg_start ) ) )";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}

TEST(PropertyLibTest, ParseFormulaWithMultiInputAND) {

  std::string prop = "(G+ ( AND (EQ one) (EQ two) (EQ three) (EQ four) (EQ five_done)))";
  std::string origPropPruned = prune_whitespaces(prop);
  std::string regenStrPruned = prune_whitespaces(HyperPLTL::parse_and_regen_string(prop));

  EXPECT_EQ(origPropPruned, regenStrPruned);
}
