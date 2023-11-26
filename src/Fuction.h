#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include "int2048.h"
#include "Evalvisitor.h"

class EvalVisitor : public Python3ParserBaseVisitor {
public:
  std::any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
    std::string x = ctx->NAME()->getText();
    
  }

  std::any visitParameters(Python3Parser::ParametersContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
    return visitChildren(ctx);
  }

  std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    return visit(ctx->testlist());
  }
};

#endif // PYTHON_INTERPRETER_EVALVISITOR_H
