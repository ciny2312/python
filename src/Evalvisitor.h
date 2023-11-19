#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H


#include "Python3ParserBaseVisitor.h"


class EvalVisitor : public Python3ParserBaseVisitor {
// TODO: override all methods of Python3ParserBaseVisitor
  std::any visitFile_input(Python3Parser::File_inputContext *ctx) override {
	auto array=ctx->stmt();
	for(auto x : array){
		visit(x);
	}
	return 1;
  }
  std::any visitStmt(Python3Parser::StmtContext *ctx) override {
	auto x1=ctx->simple_stmt();
	if(x1!=nullptr){
	  visit(x1);
	}
	auto x2=ctx->compound_stmt();
	if(x2!=nullptr){
	  visit(x2);
	}
	return 1;
  }

  std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
	auto x=ctx->small_stmt();
	visit(x);
	return 1;
  }

  std::any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
	auto x1=ctx->expr_stmt();
	if(x1!=nullptr){
		visit(x1);
	}
	auto x2=ctx->flow_stmt();
	if(x2!=nullptr){
		visit(x2);
	}
	return 1;
  }
  std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
/*	auto array=ctx->testlist();
	auto au=ctx->augassign;
	for(auto x : array){
		visit(x);
	}*/
    return visitChildren(ctx);
  }
  std::any visitAugassign(Python3Parser::AugassignContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    return visitChildren(ctx);
  }
};


#endif//PYTHON_INTERPRETER_EVALVISITOR_H
