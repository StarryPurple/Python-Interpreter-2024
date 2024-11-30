#include "Evalvisitor.h"

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) override {
  for(auto stmt_ctx: ctx->stmt())
    visit(stmt_ctx);
  // temporarily nothing to return
  return nullptr;
}
std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) override {

}
std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) override {

}
std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {

}
std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) override {

}
std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) override {
  if(auto simple_stmt_ctx = ctx->simple_stmt(); simple_stmt_ctx != nullptr)
    visit(simple_stmt_ctx);
  else if(auto compound_stmt_ctx = ctx->compound_stmt(); compound_stmt_ctx != nullptr) // redundant
    visit(compound_stmt_ctx);
  // temporarily nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
  visit(ctx->small_stmt());
  // temporarily nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
  if(auto expr_stmt_ctx = ctx->expr_stmt(); expr_stmt_ctx != nullptr)
    visit(expr_stmt_ctx);
  else if(auto flow_stmt_ctx = ctx->flow_stmt(); flow_stmt_ctx != nullptr)
    visit(flow_stmt_ctx);
  // temporarily nothing to return.
  return nullptr;
}
std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {

}
std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) override {

}
std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {

}
std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {

}
std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {

}
std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {

}
std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {

}
std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {

}
std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {

}
std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) override {

}
std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) override {

}
std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) override {

}
std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) override {

}
std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) override {

}
std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) override {

}
std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) override {

}
std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {

}
std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) override {

}
std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) override {

}
std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) override {

}
std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) override {

}
std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {

}
std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) override {

}
std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) override {

}
std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) override {

}
std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) override {

}
std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) override {

}
std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) override {

}