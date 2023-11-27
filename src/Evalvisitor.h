#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "Python3ParserBaseVisitor.h"
#include "int2048.h"
//#include "Fuction.h"

class EvalVisitor : public Python3ParserBaseVisitor {
  int scope_num = 0, cur_num = 0, fuc_num = 0;
  std::map<std::string, int> fucTable;
  class Scope {
    std::map<std::string, std::any> varTable;

  public:
    void var_register(std::string x, std::any y) { varTable[x] = y; }
    bool query(std::string x) { return varTable.find(x) != varTable.end(); }
    std::any var_query(std::string x) { return varTable[x]; }
    void clear() { varTable.clear(); }
  } scope[5000];
  class Fuction {
    Python3Parser::SuiteContext *point;

  public:
    std::vector<std::string> var;
    std::vector<std::any> var_val;
    void fuc_register(Python3Parser::SuiteContext *x) { point = x; }
    void fuc_register_var(std::string x) { var.push_back(x); }
    void fuc_register_var_val(std::any x) { var_val.push_back(x); }
    Python3Parser::SuiteContext *into_fuc() { return point; }

  } fuc[100000];
  class nothing {};
  class do_break {};
  class do_continue {};
  class do_return {
  public:
    std::vector<std::any> ans;
  };

public:
  /*	std::any visit(antlr4::tree::ParseTree *tree) override {
                  std::cout << tree->getText() << std::endl;
          return tree->accept(this);
      }
  */

  // TODO: override all methods of Python3ParserBaseVisitor
  std::any visitFile_input(Python3Parser::File_inputContext *ctx) override {
    auto array = ctx->stmt();
    for (auto x : array) {
      visit(x);
    }
    return 1;
  }
  std::any visitStmt(Python3Parser::StmtContext *ctx) override {
    if (ctx->simple_stmt()) {
      return visit(ctx->simple_stmt());
    }
    return visit(ctx->compound_stmt());
  }

  std::any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
    return visit(ctx->small_stmt());
  }

  std::any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
    if (ctx->expr_stmt())
      return visit(ctx->expr_stmt());
    return visit(ctx->flow_stmt());
  }

  std::any add(std::any x, std::any y) {
  //  std::cout<<"add"<<std::endl;
    if (std::any_cast<double>(&x) || std::any_cast<double>(&y)) {
      double ans = 0;
      if (std::any_cast<double>(&x))
        ans += std::any_cast<double>(x);
      else if (std::any_cast<sjtu::int2048>(&x))
        ans += turn_to_double(std::any_cast<sjtu::int2048>(x));
      else
        ans += std::any_cast<bool>(x);
      if (std::any_cast<double>(&y))
        ans += std::any_cast<double>(y);
      else if (std::any_cast<sjtu::int2048>(&y))
        ans += turn_to_double(std::any_cast<sjtu::int2048>(y));
      else
        ans += std::any_cast<bool>(y);
      return ans;
    } else if (std::any_cast<std::string>(&x) &&
               std::any_cast<std::string>(&y)) {
      return std::any_cast<std::string>(x) + std::any_cast<std::string>(y);
    } else {
      sjtu::int2048 ans = 0;
      if (std::any_cast<sjtu::int2048>(&x))
        ans += std::any_cast<sjtu::int2048>(x);
      else
        ans += std::any_cast<bool>(x);
      if (std::any_cast<sjtu::int2048>(&y))
        ans += std::any_cast<sjtu::int2048>(y);
      else
        ans += std::any_cast<bool>(y);
      return ans;
    }
  }
  std::any del(std::any x, std::any y) {
  //  std::cout<<"del"<<std::endl;
    if (std::any_cast<double>(&x) || std::any_cast<double>(&y)) {
      double ans = 0;
      if (std::any_cast<double>(&x))
        ans += std::any_cast<double>(x);
      else if (std::any_cast<sjtu::int2048>(&x))
        ans += turn_to_double(std::any_cast<sjtu::int2048>(x));
      else
        ans += std::any_cast<bool>(x);
      if (std::any_cast<double>(&y))
        ans -= std::any_cast<double>(y);
      else if (std::any_cast<sjtu::int2048>(&y))
        ans -= turn_to_double(std::any_cast<sjtu::int2048>(y));
      else
        ans -= std::any_cast<bool>(y);
      return ans;
    } else {
      sjtu::int2048 ans = 0;
      if (std::any_cast<sjtu::int2048>(&x))
        ans += std::any_cast<sjtu::int2048>(x);
      else
        ans += std::any_cast<bool>(x);
      if (std::any_cast<sjtu::int2048>(&y))
        ans -= std::any_cast<sjtu::int2048>(y);
      else
        ans -= std::any_cast<bool>(y);
      return ans;
    }
  }
  std::any mul(std::any x, std::any y) {
  //  std::cout<<"mul"<<std::endl;
    if (std::any_cast<double>(&x) || std::any_cast<double>(&y)) {
      double ans = 1;
      if (std::any_cast<double>(&x))
        ans *= std::any_cast<double>(x);
      else if (std::any_cast<sjtu::int2048>(&x))
        ans *= turn_to_double(std::any_cast<sjtu::int2048>(x));
      else
        ans *= std::any_cast<bool>(x);
      if (std::any_cast<double>(&y))
        ans *= std::any_cast<double>(y);
      else if (std::any_cast<sjtu::int2048>(&y))
        ans *= turn_to_double(std::any_cast<sjtu::int2048>(y));
      else
        ans *= std::any_cast<bool>(y);
      return ans;
    } else if (std::any_cast<std::string>(&x) ||
               std::any_cast<std::string>(&y)) {
      std::string ans = "", v;
      if (std::any_cast<std::string>(&x)) {
        v = std::any_cast<std::string>(x);
        int num = turn_to_double(std::any_cast<sjtu::int2048>(y));
        //    std::cout<<"here"<<std::endl;
        while (num) {
          if (num &1)
            ans = ans + v;
          v = v + v;
          num >>=1;
        }
      }
      if (std::any_cast<std::string>(&y)) {
        v = std::any_cast<std::string>(y);
        int num = turn_to_double(std::any_cast<sjtu::int2048>(x));
        while (num) {
          if (num &1)
            ans = ans + v;
          v = v + v;
          num /= 2;
        }
      }
      return ans;
    } else {
      sjtu::int2048 ans = 1;
      if (std::any_cast<sjtu::int2048>(&x))
        ans *= std::any_cast<sjtu::int2048>(x);
      else
        ans *= std::any_cast<bool>(x);
      if (std::any_cast<sjtu::int2048>(&y))
        ans *= std::any_cast<sjtu::int2048>(y);
      else
        ans *= std::any_cast<bool>(y);
      return ans;
    }
  }
  std::any div1(std::any x, std::any y) {
  //  std::cout<<"div1"<<std::endl;
    double ans = 0;
    if (std::any_cast<double>(&x))
      ans = std::any_cast<double>(x);
    else if (std::any_cast<sjtu::int2048>(&x))
      ans = turn_to_double(std::any_cast<sjtu::int2048>(x));
    else
      ans = std::any_cast<bool>(x);
    if (std::any_cast<double>(&y))
      ans /= std::any_cast<double>(y);
    else if (std::any_cast<sjtu::int2048>(&y))
      ans /= turn_to_double(std::any_cast<sjtu::int2048>(y));
    else
      ans /= std::any_cast<bool>(y);
    return ans;
  }
  std::any div2(std::any x, std::any y) {
    std::cout<<"div2"<<std::endl;
    sjtu::int2048 ans = 0;
    if (std::any_cast<sjtu::int2048>(&x))
      ans = std::any_cast<sjtu::int2048>(x);
    else
      ans = std::any_cast<bool>(x);
    std::cout<<"div2_half_down"<<std::endl;
    if (std::any_cast<sjtu::int2048>(&y))
      ans /= std::any_cast<sjtu::int2048>(y);
    else
      ans /= std::any_cast<bool>(y);
    std::cout<<"div2_down"<<std::endl;
    return ans;
  }

  std::vector<std::string> get_name(Python3Parser::TestlistContext *ctx) {
    auto array = ctx->test();
    std::vector<std::string> ans;
    for (int i = 0; i < array.size(); i++) {
      ans.push_back(array[i]->getText());
    }
    return ans;
  }
  std::any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
    auto array = ctx->testlist();
    auto au = ctx->augassign();
    if (au == nullptr) {
      auto val =
          std::any_cast<std::vector<std::any>>(visit(array[array.size() - 1]));
      for (int i = array.size() - 2; i >= 0; i--) {
        auto x = get_name(array[i]);
        for (int j = 0; j < val.size(); j++) {
          if (scope[cur_num].query(x[j]))
            scope[cur_num].var_register(x[j], val[j]);
          else if (scope[0].query(x[j]))
            scope[0].var_register(x[j], val[j]);
          else
            scope[cur_num].var_register(x[j], val[j]);
        }
      }
    } else if (au->getText() == "+=") {
      auto val = std::any_cast<std::vector<std::any>>(visit(array[1]));
      auto x = array[0]->getText();
      if (scope[cur_num].query(x)) {
        std::any v = scope[cur_num].var_query(x);
        scope[cur_num].var_register(x, add(v, val[0]));
      } else {
        std::any v = scope[0].var_query(x);
        scope[0].var_register(x, add(v, val[0]));
      }
    } else if (au->getText() == "-=") {
      auto val = std::any_cast<std::vector<std::any>>(visit(array[1]));
      auto x = array[0]->getText();
      if (scope[cur_num].query(x)) {
        std::any v = scope[cur_num].var_query(x);
        scope[cur_num].var_register(x, del(v, val[0]));
      } else {
        std::any v = scope[0].var_query(x);
        scope[0].var_register(x, del(v, val[0]));
      }
    } else if (au->getText() == "*=") {
      auto val = std::any_cast<std::vector<std::any>>(visit(array[1]));
      auto x = array[0]->getText();
      if (scope[cur_num].query(x)) {
        std::any v = scope[cur_num].var_query(x);
        scope[cur_num].var_register(x, mul(v, val[0]));
      } else {
        std::any v = scope[0].var_query(x);
        scope[0].var_register(x, mul(v, val[0]));
      }
      //  std::cout<<x<<std::endl;

    } else if (au->getText() == "/=") {
      auto val = std::any_cast<std::vector<std::any>>(visit(array[1]));
      auto x = array[0]->getText();
      if (scope[cur_num].query(x)) {
        std::any v = scope[cur_num].var_query(x);
        scope[cur_num].var_register(x, div1(v, val[0]));
      } else {
        std::any v = scope[0].var_query(x);
        scope[0].var_register(x, div1(v, val[0]));
      }

    } else if (au->getText() == "//=") {
      auto val = std::any_cast<std::vector<std::any>>(visit(array[1]));
      auto x = array[0]->getText();
      if (scope[cur_num].query(x)) {
        std::any v = scope[cur_num].var_query(x);
        scope[cur_num].var_register(x, div2(v, val[0]));
      } else {
        std::any v = scope[0].var_query(x);
        scope[0].var_register(x, div2(v, val[0]));
      }
    } else if (au->getText() == "%=") {
      auto val = std::any_cast<std::vector<std::any>>(visit(array[1]));
      auto x = array[0]->getText();
      if (scope[cur_num].query(x)) {
        std::any v = scope[cur_num].var_query(x);
        scope[cur_num].var_register(x, del(v, mul(div2(v, val), val[0])));
      } else {
        std::any v = scope[0].var_query(x);
        scope[0].var_register(x, del(v, mul(div2(v, val), val[0])));
      }
    }
    return 1;
  }

  std::any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
    if (ctx->break_stmt())
      return visit(ctx->break_stmt());
    if (ctx->return_stmt())
      return visit(ctx->return_stmt());
    return visit(ctx->continue_stmt());
  }

  std::any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
    do_break x;
    //  std::cout<<"break"<<std::endl;
    return x;
  }

  std::any
  visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
    do_continue x;
    return x;
  }

  std::any
  visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
    if (ctx->if_stmt())
      return visit(ctx->if_stmt());
    if (ctx->while_stmt())
      return visit(ctx->while_stmt());
    return visit(ctx->funcdef());
  }

  std::any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
    auto array = ctx->test();
    auto x = ctx->suite();
    for (int i = 0; i < array.size(); i++) {
      if (turn_to_bool(visit(array[i]))) {
        //  std::cout<<"if"<<std::endl;
        return visit(x[i]);
      }
    }
    if (x.size() > array.size())
      return visit(x[array.size()]);
    return 1;
  }

  std::any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
    while (turn_to_bool(visit(ctx->test()))) {
      std::any x = visit(ctx->suite());
      if (std::any_cast<do_break>(&x)) {
        //  std::cout<<"breakfinal"<<std::endl;
        return 0;
      }
    }
    return 1;
  }

  std::any visitSuite(Python3Parser::SuiteContext *ctx) override {
    if (ctx->simple_stmt()) {
      return visit(ctx->simple_stmt());
    }
    auto array = ctx->stmt();
    for (int i = 0; i < array.size(); i++) {
      std::any x = visit(array[i]);
      if (std::any_cast<do_continue>(&x))
        return x;
      if (std::any_cast<do_break>(&x)) {
        //  std::cout<<"breakhere"<<std::endl;
        return x;
      }
      if (std::any_cast<do_return>(&x)) {
        return x;
      }
    }
    return 1;
  }

  std::any visitTestlist(Python3Parser::TestlistContext *ctx) override {
    auto array = ctx->test();
    std::vector<std::any> ans;
    for (int i = 0; i < array.size(); i++)
      ans.push_back(visit(array[i]));
    //  std::cout<<"****"<<ctx->getText()<<std::endl;
    return ans;
  }

  std::any visitTest(Python3Parser::TestContext *ctx) override {
    //  std::cout<<"*"<<ctx->getText()<<std::endl;
    return visit(ctx->or_test());
    //  std::cout<<"**"<<ctx->getText()<<std::endl;
  }
  bool turn_to_bool(std::any x) {
    bool ans;
    if (std::any_cast<sjtu::int2048>(&x))
      ans = !is_zero(std::any_cast<sjtu::int2048>(x));
    if (std::any_cast<bool>(&x))
      ans = std::any_cast<bool>(x);
    if (std::any_cast<double>(&x))
      ans = std::any_cast<double>(x);
    if (std::any_cast<std::string>(&x)) {
      if (std::any_cast<std::string>(x) == "")
        ans = false;
      else
        ans = true;
    }
    return ans;
  }
  std::any visitOr_test(Python3Parser::Or_testContext *ctx) override {
    bool flag = false;
    auto array = ctx->and_test();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    for (auto x : array) {
      if (flag == true)
        break;
      flag |= turn_to_bool(visit(x));
    }
    return flag;
  }

  std::any visitAnd_test(Python3Parser::And_testContext *ctx) override {
    bool flag = true;
    auto array = ctx->not_test();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    for (auto x : array) {
      if (flag == false)
        break;
      flag &= turn_to_bool(visit(x));
    }
    return flag;
  }

  std::any visitNot_test(Python3Parser::Not_testContext *ctx) override {
    if (ctx->not_test()) {
      return !turn_to_bool(visit(ctx->not_test()));
    }
    return visit(ctx->comparison());
  }
  double get_val(std::any x) {
    if (std::any_cast<sjtu::int2048>(&x)) {
      return turn_to_double(std::any_cast<sjtu::int2048>(x));
    }
    if (std::any_cast<double>(&x)) {
      return std::any_cast<double>(x);
    }
    if (std::any_cast<std::string>(&x)){
      return string_to_double(std::any_cast<std::string>(x));
    }
    return std::any_cast<bool>(x);
  }
  bool can_not_to_number(std::string x) {
    int num = 0;
    if (x[0] == '-')
      x = x.substr(1);
    for (int i = 0; i < x.length(); i++) {
      if (x[i] == '.') {
        num++;
        if (num == 2)
          return false;
      } else if (x[i] < '0' || x[i] > '9')
        return false;
    }
    return true;
  }
  std::any visitComparison(Python3Parser::ComparisonContext *ctx) override {
    auto array = ctx->arith_expr();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    auto op = ctx->comp_op();
    bool flag = true;
    std::vector<std::any> ans;
    for (int i = 0; i < array.size(); i++) {
      ans.push_back(visit(array[i]));
    }
    //  std::cout<<ctx->getText()<<std::endl;
    for (int i = 1; i < array.size(); i++) {
      if (flag == false)
        break;
      if (std::any_cast<std::string>(&ans[i - 1]) ||
          std::any_cast<std::string>(&ans[i])) {
        if (std::any_cast<std::string>(&ans[i - 1]) &&
            std::any_cast<std::string>(&ans[i])) {
          if (op[i - 1]->getText() == "==")
            flag &= (std::any_cast<std::string>(ans[i - 1]) ==
                     std::any_cast<std::string>(ans[i]));
          else if (op[i - 1]->getText() == "<=")
            flag &= (std::any_cast<std::string>(ans[i - 1]) <=
                     std::any_cast<std::string>(ans[i]));
          else if (op[i - 1]->getText() == ">=")
            flag &= (std::any_cast<std::string>(ans[i - 1]) >=
                     std::any_cast<std::string>(ans[i]));
          else if (op[i - 1]->getText() == "<")
            flag &= (std::any_cast<std::string>(ans[i - 1]) <
                     std::any_cast<std::string>(ans[i]));
          else if (op[i - 1]->getText() == ">")
            flag &= (std::any_cast<std::string>(ans[i - 1]) >
                     std::any_cast<std::string>(ans[i]));
          else
            flag &= (std::any_cast<std::string>(ans[i - 1]) !=
                     std::any_cast<std::string>(ans[i]));
          continue;
        } else if (!can_not_to_number(std::any_cast<std::string>(ans[i - 1]))){
          flag = false;
          continue;
        }
        else if (!can_not_to_number(std::any_cast<std::string>(ans[i]))){
          flag = false;
          continue;
        }
      }
      if(std::any_cast<sjtu::int2048>(&ans[i - 1]) ||
          std::any_cast<sjtu::int2048>(&ans[i])){
            if(std::any_cast<sjtu::int2048>(&ans[i - 1]) && std::any_cast<sjtu::int2048>(&ans[i])){
              if (op[i - 1]->getText() == "==")
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) ==
                        std::any_cast<sjtu::int2048>(ans[i]));
              else if (op[i - 1]->getText() == "<=")
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) <=
                        std::any_cast<sjtu::int2048>(ans[i]));
              else if (op[i - 1]->getText() == ">=")
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) >=
                        std::any_cast<sjtu::int2048>(ans[i]));
              else if (op[i - 1]->getText() == "<")
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) <
                        std::any_cast<sjtu::int2048>(ans[i]));
              else if (op[i - 1]->getText() == ">")
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) >
                        std::any_cast<sjtu::int2048>(ans[i]));
              else
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) !=
                        std::any_cast<sjtu::int2048>(ans[i]));
              continue;
            }
            if(std::any_cast<sjtu::int2048>(&ans[i - 1])){
              if(std::any_cast<double>(&ans[i])){
                sjtu::int2048 val(std::any_cast<double>(ans[i]));
                if (op[i - 1]->getText() == "==")
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) == val);
                else if (op[i - 1]->getText() == "<=")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) <=val);
                else if (op[i - 1]->getText() == ">=")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) >=val);
                else if (op[i - 1]->getText() == "<")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) <val);
                else if (op[i - 1]->getText() == ">")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) >val);
                else
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) !=val);
              }
              if(std::any_cast<std::string>(&ans[i])){
                sjtu::int2048 val;
                if(std::any_cast<std::string>(ans[i]).length()<=300)
                  val=sjtu::int2048(string_to_double(std::any_cast<std::string>(ans[i])));
                else val=sjtu::int2048(std::any_cast<std::string>(ans[i]));
                if (op[i - 1]->getText() == "==")
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) == val);
                else if (op[i - 1]->getText() == "<=")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) <=val);
                else if (op[i - 1]->getText() == ">=")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) >=val);
                else if (op[i - 1]->getText() == "<")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) <val);
                else if (op[i - 1]->getText() == ">")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) >val);
                else
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) !=val);
              }
              if(std::any_cast<bool>(&ans[i])){
                sjtu::int2048 val(std::any_cast<bool>(ans[i]));
                if (op[i - 1]->getText() == "==")
                flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) == val);
                else if (op[i - 1]->getText() == "<=")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) <=val);
                else if (op[i - 1]->getText() == ">=")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) >=val);
                else if (op[i - 1]->getText() == "<")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) <val);
                else if (op[i - 1]->getText() == ">")
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) >val);
                else
                  flag &= (std::any_cast<sjtu::int2048>(ans[i - 1]) !=val);
              }
              continue;
            }
            if(std::any_cast<sjtu::int2048>(&ans[i])){
              if(std::any_cast<double>(&ans[i-1])){
                sjtu::int2048 val(std::any_cast<double>(ans[i-1]));
                if (op[i - 1]->getText() == "==")
                flag &= (val==std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == "<=")
                flag &= (val<=std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == ">=")
                flag &= (val>=std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == "<")
                flag &= (val<std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == ">")
                flag &= (val>std::any_cast<sjtu::int2048>(ans[i]));
                else
                flag &= (val!=std::any_cast<sjtu::int2048>(ans[i]));
              }
              if(std::any_cast<std::string>(&ans[i])){
                sjtu::int2048 val;
                if(std::any_cast<std::string>(ans[i]).length()<=300)
                  val=sjtu::int2048(string_to_double(std::any_cast<std::string>(ans[i])));
                else val=sjtu::int2048(std::any_cast<std::string>(ans[i]));
                if (op[i - 1]->getText() == "==")
                flag &= (val==std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == "<=")
                flag &= (val<=std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == ">=")
                flag &= (val>=std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == "<")
                flag &= (val<std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == ">")
                flag &= (val>std::any_cast<sjtu::int2048>(ans[i]));
                else
                flag &= (val!=std::any_cast<sjtu::int2048>(ans[i]));
              }
              if(std::any_cast<bool>(&ans[i-1])){
                sjtu::int2048 val(std::any_cast<bool>(ans[i-1]));
                if (op[i - 1]->getText() == "==")
                flag &= (val==std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == "<=")
                flag &= (val<=std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == ">=")
                flag &= (val>=std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == "<")
                flag &= (val<std::any_cast<sjtu::int2048>(ans[i]));
                else if (op[i - 1]->getText() == ">")
                flag &= (val>std::any_cast<sjtu::int2048>(ans[i]));
                else
                flag &= (val!=std::any_cast<sjtu::int2048>(ans[i]));
              }
              continue;
            }
          }
      if (op[i - 1]->getText() == "==") {
        if (fabs(get_val(ans[i - 1]) - get_val(ans[i])) > 1e-9)
          flag = false;
      } else if (op[i - 1]->getText() == "<=") {
        if (get_val(ans[i - 1]) > get_val(ans[i]) + 1e-9)
          flag = false;
      } else if (op[i - 1]->getText() == ">=") {
        if (get_val(ans[i - 1]) < get_val(ans[i]) - 1e-9)
          flag = false;
      } else if (op[i - 1]->getText() == "<") {
        if (get_val(ans[i - 1]) > get_val(ans[i]) - 1e-9)
          flag = false;
      } else if (op[i - 1]->getText() == ">") {
        if (get_val(ans[i - 1]) < get_val(ans[i]) + 1e-9)
          flag = false;
      } else {
        if (fabs(get_val(ans[i - 1]) - get_val(ans[i])) <= 1e-9)
          flag = false;
      }
    }
    return flag;
  }

  std::any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
    auto array = ctx->term();
    auto op = ctx->addorsub_op();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    std::any ans = visit(array[0]);
    for (int i = 1; i < array.size(); i++) {
      if (op[i - 1]->getText() == "+") {
        ans = add(ans, visit(array[i]));
      } else {
        ans = del(ans, visit(array[i]));
      }
    }
    return ans;
    /*  if (std::any_cast<std::string *>(ans)) {
        std::string val = std::any_cast<std::string>(ans);
        for (int i = 1; i < array.size(); i++) {
          val = val + std::any_cast<std::string>(visit(array[i]));
        }
        return val;
      }
      std::vector<std::any> v;
      bool flag = 0;
      for (int i = 0; i < array.size(); i++) {
        v[i] = visit(array[i]);
        if (std::any_cast<double *>(v[i]))
          flag = 1;
      }
      if (flag) {
        double val;
        if (std::any_cast<int *>(v[0]))
          val = std::any_cast<int>(v[0]);
        if (std::any_cast<bool *>(v[0]))
          val = std::any_cast<bool>(v[0]);
        if (std::any_cast<double *>(v[0]))
          val = std::any_cast<double>(v[0]);
        for (int i = 1; i < array.size(); i++) {
          if (op[i-1]->getText() == "+") {
            if (std::any_cast<int *>(v[i]))
              val += std::any_cast<int>(v[i]);
            if (std::any_cast<bool *>(v[i]))
              val += std::any_cast<bool>(v[i]);
            if (std::any_cast<double *>(v[i]))
              val += std::any_cast<double>(v[i]);
          } else {
            if (std::any_cast<int *>(v[i]))
              val -= std::any_cast<int>(v[i]);
            if (std::any_cast<bool *>(v[i]))
              val -= std::any_cast<bool>(v[i]);
            if (std::any_cast<double *>(v[i]))
              val -= std::any_cast<double>(v[i]);
          }
        }
        return val;
      }
      int val = 0;
      if (std::any_cast<int *>(v[0]))
        val = std::any_cast<int>(v[0]);
      if (std::any_cast<bool *>(v[0]))
        val = std::any_cast<bool>(v[0]);
      for (int i = 1; i < array.size(); i++) {
        if (op[i-1]->getText() == "+") {
          if (std::any_cast<int *>(v[i]))
            val += std::any_cast<int>(v[i]);
          if (std::any_cast<bool *>(v[i]))
            val += std::any_cast<bool>(v[i]);
        } else {
          if (std::any_cast<int *>(v[i]))
            val -= std::any_cast<int>(v[i]);
          if (std::any_cast<bool *>(v[i]))
            val -= std::any_cast<bool>(v[i]);
        }
      }
      return val;*/
  }

  std::any visitTerm(Python3Parser::TermContext *ctx) override {
    auto array = ctx->factor();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    auto op = ctx->muldivmod_op();
    std::any ans = visit(array[0]);
    for (int i = 1; i < array.size(); i++) {
      if (op[i - 1]->getText() == "*")
        ans = mul(ans, visit(array[i]));
      else if (op[i - 1]->getText() == "/")
        ans = div1(ans, visit(array[i]));
      else if (op[i - 1]->getText() == "//")
        ans = div2(ans, visit(array[i]));
      else {
        std::any val = visit(array[i]);
        ans = del(ans, mul(div2(ans, val), val));
      }
    }
    return ans;
  }

  std::any visitFactor(Python3Parser::FactorContext *ctx) override {
    if (ctx->factor()) {
      if (ctx->ADD())
        return visit(ctx->factor());
      else {
        std::any x = visit(ctx->factor());
        if (std::any_cast<sjtu::int2048>(&x))
          return -std::any_cast<sjtu::int2048>(x);
        if (std::any_cast<double>(&x))
          return -std::any_cast<double>(x);
      }
    }
    return visit(ctx->atom_expr());
  }

  std::string double_to_string(double x){
    int val=x;
    std::string ans="";
    while(val){
      char c='0'+val%10;
      ans=c+ans;
      val/=10;
    }
    double v=x-val;
    if(fabs(v)<=1e-9) return ans;
    ans+=".";
    while(fabs(v)>1e-9){
      v*=10;val=v;
      char c='0'+val;
      ans+=c;
      v-=val;
    }
    return ans;
  }

  std::any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
    if (ctx->trailer()) {
      std::string fucname = ctx->atom()->getText();
      auto x = visit(ctx->trailer());
      if (std::any_cast<std::vector<std::any>>(&x)) {
        auto array = std::any_cast<std::vector<std::any>>(x);
        if (fucname == "print") {
          //    std::cout<<"BOOL"<<array.size()<<std::endl;
          for (int i = 0; i < array.size(); i++) {
            if (std::any_cast<sjtu::int2048>(&array[i])) {
              std::cout << std::any_cast<sjtu::int2048>(array[i]);
              if(i!=array.size()-1) std::cout << ' ';
            }
            if (std::any_cast<double>(&array[i])){
              printf("%.6lf", std::any_cast<double>(array[i]));
              if(i!=array.size()-1) std::cout << ' ';
            }
            if (std::any_cast<std::string>(&array[i])) {
              std::cout << std::any_cast<std::string>(array[i]);
              if(i!=array.size()-1) std::cout << ' ';
            }

            if (std::any_cast<bool>(&array[i])) {
              if (std::any_cast<bool>(array[i]) == true)
                std::cout << "True";
              else
                std::cout << "False";
              if(i!=array.size()-1) std::cout << ' ';
            }
            if (std::any_cast<nothing>(&array[i])) {
              std::cout << "None" ;
              if(i!=array.size()-1) std::cout << ' ';
            }
          }
          printf("\n");
          return 1;
        }
        if (fucname == "int") {
          if (array.size() > 1)
            std::cerr << "(int)more than one" << std::endl;
          sjtu::int2048 ans;
          if (std::any_cast<sjtu::int2048>(&array[0]))
            ans = std::any_cast<sjtu::int2048>(array[0]);
          else if (std::any_cast<bool>(&array[0]))
            ans = std::any_cast<bool>(array[0]);
          else if (std::any_cast<double>(&array[0]))
            ans = std::any_cast<double>(array[0]);
          else
            ans = sjtu::int2048(std::any_cast<std::string>(array[0]));
          return ans;
        }
        if (fucname == "float") {
          if (array.size() > 1)
            std::cerr << "(double)more than one" << std::endl;
          double ans;
          if (std::any_cast<sjtu::int2048>(&array[0]))
            ans = turn_to_double(std::any_cast<sjtu::int2048>(array[0]));
          else if (std::any_cast<bool>(&array[0]))
            ans = std::any_cast<bool>(array[0]);
          else if (std::any_cast<double>(&array[0]))
            ans = std::any_cast<double>(array[0]);
          else
            ans = string_to_double(std::any_cast<std::string>(array[0]));
          return ans;
        }
        if (fucname == "str") {
          if (array.size() > 1)
            std::cerr << "(str)more than one" << std::endl;
          std::string ans;
          if (std::any_cast<sjtu::int2048>(&array[0])){
            ans = turn_to_str(std::any_cast<sjtu::int2048>(array[0]));
            //do some thing
          }
          if (std::any_cast<bool>(&array[0])){
            if(std::any_cast<bool>(array[0])==true)
              ans="True";
            else ans="False";
          }
          if (std::any_cast<double>(&array[0])){
            ans = double_to_string(std::any_cast<double>(array[0]));
          }
          return ans;
        }
        if (fucname == "bool") {
          if (array.size() > 1)
            std::cerr << "(bool)more than one" << std::endl;
          bool ans;
          if (std::any_cast<sjtu::int2048>(&array[0]))
            ans = is_zero(std::any_cast<sjtu::int2048>(array[0]));
          if (std::any_cast<bool>(&array[0]))
            ans = std::any_cast<bool>(array[0]);
          if (std::any_cast<double>(&array[0]))
            ans = std::any_cast<double>(array[0]);
          if (std::any_cast<std::string>(&array[0])) {
            if (std::any_cast<std::string>(array[0]) == "")
              ans = false;
            else
              ans = true;
          }
          return ans;
        }
      }
      if(fucname=="print") return 0;
  //    std::cout<<ctx->getText()<<' '<<fucname<<std::endl;
      int num = fucTable[fucname];
      cur_num++;
      int ex = fuc[num].var.size() - fuc[num].var_val.size();
      for (int i = 0; i < fuc[num].var_val.size(); i++) {
        scope[cur_num].var_register(fuc[num].var[i + ex], fuc[num].var_val[i]);
      }
      if (std::any_cast<std::vector<std::any>>(&x)) {
        auto array = std::any_cast<std::vector<std::any>>(x);
        for (int i = 0; i < array.size(); i++) {
          scope[cur_num].var_register(fuc[num].var[i], array[i]);
        }
      }
      auto re = visit(fuc[num].into_fuc());
      scope[cur_num].clear();
      cur_num--;
      if(std::any_cast<do_return>(&re))
        return std::any_cast<do_return>(re).ans;
      return 0;
    }
    return visit(ctx->atom());
  }

  std::any visitTrailer(Python3Parser::TrailerContext *ctx) override {
    if (ctx->arglist())
      return visit(ctx->arglist());
    return 0;
  }

  double string_to_double(std::string s) {
    double ans = 0, bit = 0;
    int f = 1;
    if (s[0] == '-')
      s = s.substr(1), f = -f;
    for (int i = 0; i < s.length(); i++) {
      bit *= 10;
      if (s[i] == '.')
        bit = 1;
      else
        ans = ans * 10 + s[i] - '0';
    }
    return ans * f / bit;
  }
  std::string get_string(std::string x) {
    std::string ans = x.substr(1, x.size() - 2);
    return ans;
  }
  bool is_double(std::string s){
    for(int i=0;i<s.length();i++){
      if(s[i]=='.') return true;
    }
    return false;
  }
  std::any visitAtom(Python3Parser::AtomContext *ctx) override {
    if (ctx->NUMBER()) {
      if(is_double(ctx->NUMBER()->getText())) return string_to_double(ctx->NUMBER()->getText());
      return sjtu::int2048(ctx->NUMBER()->getText());
    }
    if (ctx->NAME()) {
      std::string x = ctx->NAME()->getText();
      if (scope[cur_num].query(x))
        return scope[cur_num].var_query(x);
      return scope[0].var_query(x);
    }
    if (ctx->FALSE()) {
      //  std::cout<<"FALSE"<<std::endl;
      return false;
    }
    if (ctx->TRUE()) {
      return true;
    }
    if (ctx->STRING(0)) {
      std::string ans = "";
      auto array = ctx->STRING();
      for (int i = 0; i < array.size(); i++) {
        //  std::cout<<"STRING: "<<std::endl;
        ans = ans + get_string(array[i]->getText());
        //  std::cout<<"STRING_END: "<<std::endl;
      }
      return ans;
    }
    if (ctx->test()) {
      return visit(ctx->test());
    }
    EvalVisitor::nothing x;
    return x;
  }

  std::any visitArglist(Python3Parser::ArglistContext *ctx) override {
    auto array = ctx->argument();
    std::vector<std::any> ans;
    for (int i = 0; i < array.size(); i++) {
      ans.push_back(visit(array[i]));
    }
    return ans;
  }

  std::any visitArgument(Python3Parser::ArgumentContext *ctx) override {
    auto array = ctx->test();
    if (array.size() == 1) {
      return visit(array[0]);
    }
    return visit(array[1]);
  }

  std::any visitFuncdef(Python3Parser::FuncdefContext *) override;
  std::any visitParameters(Python3Parser::ParametersContext *) override;
  std::any visitTypedargslist(Python3Parser::TypedargslistContext *) override;
  std::any visitTfpdef(Python3Parser::TfpdefContext *) override;
  std::any visitReturn_stmt(Python3Parser::Return_stmtContext *) override;
/*
  std::any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {
  //  std::cout<<ctx->getText()<<std::endl;
    std::string x = ctx->NAME()->getText();
    fuc_num++;
    fucTable[x]=fuc_num;
    fuc[fuc_num].fuc_register(ctx->suite());
    fuc[fuc_num].var.clear();
    fuc[fuc_num].var_val.clear();
    visit(ctx->parameters());
    return 1;
  }

  std::any visitParameters(Python3Parser::ParametersContext *ctx) override {
    if(ctx->typedargslist())
      return visit(ctx->typedargslist());
    nothing x;
    return x;
  }

  std::any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
    auto array = ctx ->tfpdef();
    auto val = ctx -> test();
    for(int i=0;i< array.size();i++){
      fuc[fuc_num].fuc_register_var(std::any_cast<std::string>(visit(array[i])));
    }
    for(int i=0;i<val.size();i++){
      fuc[fuc_num].fuc_register_var_val(visit(val[i]));
    }
    return 1;
  }

  std::any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
    return ctx->NAME()->getText();
  }

  std::any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
    do_return x;
    if(ctx->testlist()){
      x.ans=std::any_cast<std::vector<std::any> >(visit(ctx->testlist()));
    }
    return x;
  }*/
};

#endif // PYTHON_INTERPRETER_EVALVISITOR_H
