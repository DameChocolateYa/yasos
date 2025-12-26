#include "parser.hh"
#include "error.hh"
#include "lexer.hh"
#include <cstdlib>
#include <memory>
#include <optional>

bool is_expr_start(Token tok) {
  switch (tok.type) {
  case TokenType::ident:
  case TokenType::int_lit:
  case TokenType::float_lit:
  case TokenType::str_lit:
  case TokenType::deref: // deref p
  case TokenType::ref:   // ref p
  case TokenType::star:
  case TokenType::amp:
  case TokenType::open_paren:
  case TokenType::l_bracket:
  case TokenType::l_key:
    return true;
  default:
    return false;
  }
}

bool is_type(Token tok) {
  switch (tok.type) {
  case TokenType::str_type:
  case TokenType::int_type:
  case TokenType::ident:
  case TokenType::double_type:
  case TokenType::any_type:
  case TokenType::none:
  case TokenType::char_type:
  case TokenType::ptr_type:
    return true;
  default:
    return false;
  }
}

bool is_assignment_op(Token tok) {
  switch (tok.type) {
  case TokenType::eq:       // =
  case TokenType::plus_eq:  // +=
  case TokenType::minus_eq: // -=
  case TokenType::star_eq:  // *=
  case TokenType::slash_eq: // /=
    return true;
  default:
    return false;
  }
}

bool is_unary_op(Token tok) {
  switch (tok.type) {
  case TokenType::bang:
  case TokenType::minus:
    return true;
  default:
    return false;
  }
}

static int get_precedence(TokenType token_type) {
  if (token_type == TokenType::plus || token_type == TokenType::minus)
    return 5;
  else if (token_type == TokenType::star || token_type == TokenType::slash ||
           token_type == TokenType::percent)
    return 6;
  else if (token_type == TokenType::l_arrow || token_type == TokenType::lte ||
           token_type == TokenType::lt || token_type == TokenType::r_arrow ||
           token_type == TokenType::gt || token_type == TokenType::gte)
    return 4;
  else if (token_type == TokenType::eq_eq || token_type == TokenType::bang_eq)
    return 3;
  else if (token_type == TokenType::_and)
    return 2;
  else if (token_type == TokenType::_or)
    return 1;
  else if (token_type == TokenType::bang)
    return 7;
  else
    return 0;
}

static Type get_type_from_tok(Token tok) {
  switch (tok.type) {
  case TokenType::str_type:
    return Type{Type::Kind::Str};
    break;
  case TokenType::char_type:
    return Type{Type::Kind::Char};
  case TokenType::int_type:
    return Type{Type::Kind::Int};
    break;
  case TokenType::double_type:
    return Type{Type::Kind::Float};
    break;
  case TokenType::any_type:
    return Type{Type::Kind::Any};
  case TokenType::ptr_type:
    return Type{Type::Kind::Ptr};
  case TokenType::none:
    return Type{Type::Kind::None};
  case TokenType::ident:
    return Type{Type::Kind::UserDefined, false, tok.value.value()};
    break;
  default:
    return Type{Type::Kind::UserDefined, false, tok.value.value()};
    break;
  }
}

Type Parser::parse_type() {
  Type type;
  std::shared_ptr<Type> pointee = nullptr;
  bool is_ref = false;
  if (peek().has_value() && peek().value().type == TokenType::amp) {
    consume();
    pointee = std::make_shared<Type>(parse_type());
    is_ref = true;
  }
  if (peek().has_value() && is_type(peek().value())) {
    type = get_type_from_tok(peek().value());
  }

  type.is_ref = is_ref;
  type.pointee = pointee;

  return type;
}

std::string Parser::parse_mangled_chain() {
  std::string name;
  std::vector<std::string> names;
  int line = peek().value().line;

  while (peek().has_value()) {
    if (peek(1).has_value() && peek(1).value().type == TokenType::dp && peek(2).has_value() && peek(2).value().type == TokenType::dp) {
      names.push_back(consume().value.value());
    } else break;

    if (peek().has_value() && peek().value().type == TokenType::dp && peek(1).has_value() && peek(1).value().type == TokenType::dp) {
      consume(); consume();
      continue;
    }
    break;
  }

  std::string function_mangled_name;
  for (const auto &name : names) {
    function_mangled_name.append("$MOD" + name);
  }

  if (!peek().has_value() || peek().value().type != TokenType::ident) {
    add_error("Expected identifier after '::'", line);
    return "ERR";
  }

  return consume().value.value() + function_mangled_name;
}

std::optional<NodeExpr> Parser::parse_property_chain(std::optional<NodeExpr> base_expr) {
  // std::optional<NodeExpr> expr = base_expr;
  std::optional<NodeExpr> expr = base_expr;

  while (peek().has_value() && peek().value().type == TokenType::dot) {
    consume(); // consume '.'

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier after '.'",
                peek().has_value() ? peek().value().line : -1);
      return std::nullopt;
    }

    Token property = consume();
    int line = property.line;

    bool is_func = false;
    std::vector<NodeExprPtr> args;

    if (peek().has_value() && peek().value().type == TokenType::open_paren) {
      consume(); // '('
      is_func = true;

      while (peek().has_value() && peek().value().type != TokenType::close_paren) {
        auto arg_expr = parse_expr();
        if (!arg_expr.has_value()) {
          add_error("Invalid expression in function arguments", line);
        } else {
          args.push_back(std::make_shared<NodeExpr>(arg_expr.value()));
        }

        if (peek().has_value() && peek().value().type == TokenType::comma) {
          consume();
        } else {
          break;
        }
      }

      if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
        add_error("Expected ')'", line);
      } else {
        consume(); // ')'
      }
    }

    expr = NodeExpr(NodeExprProperty{.base = std::make_shared<NodeExpr>(*expr),
                                     .property = property,
                                     .is_func = is_func,
                                     .args = args,
                                     .line = line});
  }

  return expr;
}

// Parse the primary expression
bool recursive_expr = false;
std::optional<NodeExpr> Parser::parse_primary_expr() {
  if (peek().has_value() && peek().value().type == TokenType::ident &&
  peek(1).has_value() && peek(1).value().type == TokenType::dp &&
  peek(2).has_value() && peek(2).value().type == TokenType::dp) {
    Token new_tok = {.type = TokenType::ident, .value = parse_mangled_chain(), .line = peek().value().line};
    m_tokens.insert(m_tokens.begin() + m_index, new_tok);
  }

  if (peek().has_value() && is_expr_start(peek().value()) &&
    peek(1).has_value() && peek(1).value().type == TokenType::bang &&
    peek(2).has_value() && peek(2).value().type == TokenType::bang &&
    !recursive_expr) {
    int line = peek().value().line;
    recursive_expr = true;
    auto expr = parse_expr();
    recursive_expr = false;

    if (!expr.has_value()) {
      add_error("Malformed expression", line);
      return std::nullopt;
    }

    consume(); consume();

    return NodeExpr(NodeExprFact {.expr = std::make_shared<NodeExpr>(*expr), .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::pipe &&
      peek(1).has_value() && peek(1).value().type != TokenType::pipe && !recursive_expr) {
      int line = consume().line;

      recursive_expr = true;
      auto expr = parse_expr();
      recursive_expr = false;

      if (!expr.has_value()) {
        add_error("Malformed Expression in abs", line);
        return std::nullopt;
      }

      if (!peek().has_value() || peek().value().type != TokenType::pipe) {
        add_error("Expected '|' to close abs expression", line);
        return std::nullopt;
      } else consume();

      return NodeExpr(NodeExprAbs {.expr = std::make_shared<NodeExpr>(*expr), .line = line});
    } else if (peek().has_value() && is_unary_op(peek().value())) {
    int line = peek().value().line;
    Token op = consume();

    auto rhs_opt = parse_primary_expr();
    if (!rhs_opt.has_value()) {
      add_error("Expected expression after unary op", line);
      return std::nullopt;
    }

    return NodeExpr(NodeExprUnary{
        .op = op, .expr = std::make_shared<NodeExpr>(*rhs_opt), .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::open_paren) {
    int line = peek().value().line;
    consume(); // (

    auto inner_expr = parse_expr();
    if (!inner_expr.has_value()) {
      add_error("Malformed expression", line);
    }
    if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
      add_error("Expected ')'", line);
    }
    consume(); // )

    return inner_expr;
  }  else if (peek().has_value() && peek().value().type == TokenType::_is_def) {
    int line = consume().line;

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier", line);
    }
    Token name = consume();

    return NodeExpr(NodeExprIsDef{.name = name, .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::open_paren &&
             peek(1).has_value() &&
             (peek(1).value().type == TokenType::amp || is_type(peek(1).value()))) {
    int line = consume().line;
    bool is_ref = false;

    if (peek().has_value() && peek().value().type == TokenType::amp) {
      consume();
      is_ref = true;
    }
    Type type = get_type_from_tok(consume());
    type.is_ref = is_ref;

    if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
      add_error("Expected ')'", line);
      return std::nullopt;
    }
    consume();

    auto e = parse_expr();
    if (!e.has_value()) {
      add_error("Malformed expression", line);
      return std::nullopt;
    }

    return NodeExpr(NodeExprCast{
        .expr = std::make_shared<NodeExpr>(*e), .target_type = type, .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::_is_ndef) {
    int line = consume().line;

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier", line);
    }
    Token name = consume();

    return NodeExpr(NodeExprIsNotDef{.name = name, .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::_size_of) {
    int line = consume().line;

    if (!peek().has_value() || !is_type(peek().value())) {
      add_error("Expected type", line);
    }
    Type type = get_type_from_tok(consume());

    return NodeExpr(NodeExprSizeOf{.type = type, .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::ident &&
             peek(1).has_value() && peek(1).value().type == TokenType::open_paren) {
    Token name = consume();
    int line = name.line;
    consume();

    std::vector<NodeExpr> args;

    while (peek().has_value() && peek().value().type != TokenType::close_paren) {
      if (auto e = parse_expr()) {
        args.push_back(e.value());
      } else {
        add_error("Invalid Expression in function call (expression context)", line);
      }

      if (peek().has_value() && peek().value().type == TokenType::comma) {
        consume();
      } else if (peek().has_value() && peek().value().type != TokenType::close_paren) {
        add_error("Expected ',' or ')'", line);
      }
    }

    if (peek().has_value() && peek().value().type == TokenType::close_paren) {
      consume();
    } else {
      add_error("Expected ')' at the end of function call", line);
    }

    std::vector<std::shared_ptr<NodeExpr>> wrapped_args;
    for (auto &arg : args) {
      wrapped_args.push_back(std::make_shared<NodeExpr>(std::move(arg)));
    }
    return NodeExpr(NodeExprCall{name, wrapped_args, line});
  } else if (peek().has_value() && is_expr_start(peek().value()) &&
             peek(1).has_value() && peek(1).value().type == TokenType::dot &&
             !recursive_expr) {
    int line = peek().value().line;
    recursive_expr = true;
    auto e = parse_primary_expr();
    recursive_expr = false;
    if (!e.has_value()) {
      add_error("Malformed expression", line);
    }

    std::optional<NodeExpr> expr = parse_property_chain(e);
    if (!expr.has_value()) {
      add_error("Malformed expression", line);
    }

    return *expr;
  } else if (peek().has_value() && peek().value().type == TokenType::amp &&
                 peek(1).has_value() && peek(1).value().type == TokenType::amp ||
             peek().has_value() && peek().value().type == TokenType::pipe &&
                 peek(1).has_value() && peek(1).value().type == TokenType::pipe) {
    int line = consume().line;
    consume();

    add_error("Neither && nor || are valid operators. Use and / or instead", line,
              ErrType::WrongAndOr);
  } else if (peek().has_value() && peek().value().type == TokenType::ref ||
             peek().value().type == TokenType::amp) {
    int line = peek().value().line;
    consume();

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier", line);
    }
    Token ident = consume();

    return NodeExpr(NodeExprGetPtr{.ident = ident, .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::deref ||
             peek().value().type == TokenType::star) {
    int line = consume().line; // deref
    NodeExpr expr;

    auto e = parse_primary_expr();
    if (!e.has_value()) {
      add_error("Error evaluating expression getting a reference", line);
      return std::nullopt;
    }
    expr = *e;

    return NodeExpr(
        NodeExprDeref{.expr = std::make_shared<NodeExpr>(expr), .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::_new) {
    int line = consume().line;

    if (!peek().has_value()) {
      add_error("Expected identifier after in 'new' declaration", line);
      return std::nullopt;
    }
    Type type = get_type_from_tok(consume());

    return NodeExpr(NodeExprNew{.type = type, .line = line});
  }

  else if (peek().has_value() && peek().value().type == TokenType::_list) {
    int line = peek().value().line;
    consume();
    std::vector<NodeExpr> elements;

    if (!peek().has_value() || peek().value().type != TokenType::l_bracket) {
      add_error("Expected a list\n", line);
    }
    consume();

    while (peek().has_value() && peek().value().type != TokenType::r_bracket) {
      auto e = parse_expr();
      if (!e.has_value()) {
        add_error("Malformed expression in list declaration\n", line);
      }
      elements.push_back(e.value());
      if (!peek().has_value() || (peek().value().type != TokenType::r_bracket &&
                                  peek().value().type != TokenType::comma)) {
        add_error("Expected ',' or '>'\n", line);
      }
      if (peek().value().type == TokenType::comma) {
        consume();
        continue;
      } else
        break;
    }
    consume();

    return NodeExpr(NodeExprList({.elements = elements, .line = line}));
  } else if (peek().has_value() && peek().value().type == TokenType::_nwstruct) {
    int line = peek().value().line;
    consume();

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected name of struct template", line);
    }
    Token struct_template_name = consume();

    std::vector<NodeExprPtr> fields;
    if (peek().has_value() && peek().value().type == TokenType::l_key) {
      consume();
      while (peek().has_value() && peek().value().type != TokenType::r_key) {
        auto e = parse_expr();
        if (!e.has_value()) {
          add_error("Malformed expression in field of struct", line);
        }
        fields.push_back(std::make_shared<NodeExpr>(e.value()));
        if (peek().has_value() && peek().value().type == TokenType::comma)
          consume();
      }
      if (!peek().has_value() || peek().value().type != TokenType::r_key) {
        add_error("Expected '}'", line);
      }
      consume();
    } else if (peek().has_value()) {
      add_error("Expected declaration of struct or at least the end of the expression",
                line);
    }

    return NodeExpr(NodeExprStruct{
        .template_name = struct_template_name, .fields = fields, .line = line});
  } else if (peek().has_value() && is_expr_start(peek().value()) &&
             peek(1).has_value() && peek(1).value().type == TokenType::l_bracket &&
             !recursive_expr) {
    int line = peek().value().line;
    recursive_expr = true;
    auto e = parse_primary_expr();
    recursive_expr = false;
    if (!e.has_value()) {
      add_error("Malformed expression", line);
    }
    NodeExpr list_expr = *e;

    consume();
    e = parse_expr();
    if (!e.has_value()) {
      add_error("Malformed expression", line);
    }

    NodeExprPtr index = std::make_shared<NodeExpr>(e.value());

    if (!peek().has_value() || peek().value().type != TokenType::r_bracket) {
      add_error("Expected ']'", line);
    }
    consume();

    return NodeExpr(
        NodeExprListElement{.list_expr = std::make_shared<NodeExpr>(list_expr),
                            .index = index,
                            .line = line});
  } else if (peek().has_value() && peek().value().type == TokenType::int_lit) {
    return NodeExpr(NodeExprIntLit{consume()});
  } else if (peek().has_value() && peek().value().type == TokenType::ident) {
    return NodeExpr(NodeExprIdent{consume()});
  } else if (peek().has_value() && peek().value().type == TokenType::str_lit) {
    return NodeExpr(NodeExprStrLit{consume()});
  } else if (peek().has_value() && peek().value().type == TokenType::char_lit) {
    return NodeExpr(NodeExprCharLit(consume()));
  } else if (peek().has_value() && peek().value().type == TokenType::float_lit) {
    return NodeExpr(NodeExprFloatLit{consume()});
  } else if (peek().has_value() && peek().value().type == TokenType::none) {
    return NodeExpr(NodeExprNone{});
  } else if (peek().has_value() && peek().value().type == TokenType::_nullptr) {
    consume();
    return NodeExpr(NodeExprNullptr{});
  } else if (peek().has_value() && peek().value().type == TokenType::no_arg) {
    return NodeExpr(NodeExprNoArg{consume()});
  } else if (peek().has_value() && peek().value().type == TokenType::cr) {
    return NodeExpr(NodeExprCR{consume()});
  } else if (peek().has_value() && peek().value().type == TokenType::_true) {
    int line = consume().line;
    return NodeExpr(NodeExprIntLit{
        .int_lit = Token{.type = TokenType::int_lit, .value = "1", .line = line}});
    // return NodeExpr(NodeExprBoolValue{1});
  } else if (peek().has_value() && peek().value().type == TokenType::_false) {
    int line = consume().line;
    return NodeExpr(NodeExprIntLit{
        .int_lit = Token{.type = TokenType::int_lit, .value = "0", .line = line}});
  } else if (peek().has_value() && peek().value().type == TokenType::open_paren) {
    int line = peek().value().line;
    auto inner_expr = parse_expr(0);
    if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
      add_error("Expected ')'", line);
      return std::nullopt;
    }
    consume();
    return inner_expr;
  }

  return {};
}

// Parse complex expressions
std::optional<NodeExpr> Parser::parse_expr(int min_precedence) {
  auto lhs = parse_primary_expr();
  if (!lhs.has_value())
    return std::nullopt;

  while (true) {
    auto op_token_opt = peek();
    if (!op_token_opt.has_value())
      break;
    auto op_token = *op_token_opt;

    if (op_token.type == TokenType::close_paren || op_token.type == TokenType::comma ||
        op_token.type == TokenType::r_bracket || op_token.type == TokenType::r_key ||
        op_token.type == TokenType::l_key || op_token.type == TokenType::dp ||
        op_token.type == TokenType::semi || is_assignment_op(op_token) || 
        op_token.type == TokenType::bang || op_token.type == TokenType::pipe) {
      break;
    }

    int precedence = get_precedence(op_token.type);
    if (precedence < min_precedence)
      break;

    auto op_opt = consume();
    if (!op_opt.value.has_value())
      break;
    auto op = *op_opt.value;

    auto rhs_opt = parse_expr(precedence + 1);
    if (!rhs_opt.has_value())
      break;
    auto rhs = *rhs_opt;

    lhs = NodeExpr(NodeExprBinary{std::make_shared<NodeExpr>(*lhs), op_token,
      std::make_shared<NodeExpr>(rhs), op_token.line});
  }
  return lhs;
}

// Parse statments
bool need_semi = true;
bool in_for = false;

std::optional<NodeStmt> result = std::nullopt;
std::optional<NodeStmt> Parser::parse_stmt() {
  int line = -1;
  if (peek().has_value() && peek().value().type == TokenType::ident &&
  peek(1).has_value() && peek(1).value().type == TokenType::dp &&
  peek(2).has_value() && peek(2).value().type == TokenType::dp) {
    Token new_tok = {.type = TokenType::ident, .value = parse_mangled_chain(), .line = peek().value().line};
    m_tokens.insert(m_tokens.begin() + m_index, new_tok);
  }
  if (peek().has_value())
    line = peek().value().line;

  if (peek().has_value() && peek().value().type == TokenType::ident &&
      peek().value().value == "__asm") {
    consume();
    need_semi = false;

    if (!peek().has_value() || peek().value().type != TokenType::l_key) {
      add_error("Expected '{'", line);
    }
    consume();

    std::vector<std::string> format_instructions;
    std::vector<std::vector<NodeExpr>> exprs;
    while (peek().has_value() && peek().value().type != TokenType::r_key) {
      if (peek().value().type != TokenType::open_paren) {
        add_error("Expected '(' to start asm instruction", line);
      }
      consume();

      std::vector<NodeExpr> current_exprs;
      std::string format_instruction;

      auto e = parse_expr();
      if (!e.has_value() || !std::holds_alternative<NodeExprStrLit>(e->var)) {
        add_error("Expected string expression in asm instruction", line);
      }
      format_instruction =
          std::get<NodeExprStrLit>(e->var).str_lit.value.value_or("nop");

      while (peek().has_value() && peek().value().type != TokenType::close_paren) {
        if (!peek().has_value() || peek().value().type != TokenType::dp) {
          add_error("Expected ':' between asm operators", line);
        }
        consume();
        auto e = parse_primary_expr();
        if (!e.has_value()) {
          add_error("Malformed expression in asm instruction", line);
        }
        current_exprs.push_back(*e);
      }
      if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
        add_error("Expected ')' in asm instruction", line);
      }
      consume();

      format_instructions.push_back(format_instruction);
      exprs.push_back(current_exprs);

      if (peek().has_value() && peek().value().type == TokenType::comma) {
        consume();
      } else if (peek().has_value() && peek().value().type == TokenType::r_key)
        break;
    }

    if (!peek().has_value() || peek().value().type != TokenType::r_key) {
      std::cerr << peek().value().value.value() << "\n";
      add_error("Expected '}'", line);
    }
    consume();

    result = NodeStmt{
        .var = NodeStmtAsmUserWrite{
            .format_instructions = format_instructions, .exprs = exprs, .line = line}};

  } else if (peek().has_value() && peek().value().type == TokenType::str_lit) {
    Token fmt = consume();

    std::vector<NodeExpr> args;

    if (peek().has_value() && peek().value().type == TokenType::comma) {
      consume();
      while (peek().has_value() && peek().value().type != TokenType::semi) {
        auto arg = parse_expr();
        if (!arg.has_value()) {
          add_error("Invalid Expression in Print native stmt", line);
        }
        args.push_back(arg.value());

        if (peek().has_value() && peek().value().type == TokenType::comma) {
          consume();
        } else if (peek().has_value() && peek().value().type != TokenType::semi) {
          add_error("Expected ',' or ';'", line);
        }
      }
    }

    result = NodeStmt(NodeStmtPrint{fmt, args, line});
  } else if (peek().has_value() && peek().value().type == TokenType::ident &&
             peek(1).has_value() && peek(1).value().type == TokenType::dp) {
    Token ident = consume();
    consume();

    need_semi = false;
    result = NodeStmt{.var = NodeStmtLabel{.ident = ident, .line = line}};
  } else if (peek().has_value() &&
             (peek().value().type == TokenType::var ||
              peek().value().type == TokenType::cnst) &&
             peek(1).has_value() && peek(1).value().type == TokenType::ident) {
    int mut = peek().value().type == TokenType::var ? true : false;
    consume();
    Token ident = consume();
    Token type = Token{.type = TokenType::null_tok, .value = "", .line = line};
    NodeStmtVar stmt_var;
    stmt_var.ident = ident;
    stmt_var.is_mutable = mut;
    stmt_var.line = line;

    if (peek().has_value() && peek().value().type == TokenType::dp) {
      consume();
      if (!peek().has_value()) {
        add_error("Expected type after ':'", line);
      }

      stmt_var.type = parse_type();
      consume(); // parse_type does not consum the main tok
      /*bool is_ref = false;
      if (peek().has_value() && peek().value().type == TokenType::amp) {
        consume();
        is_ref = true;
      }
      type = consume();
      stmt_var.type = get_type_from_tok(type);
      stmt_var.type.is_ref = is_ref;*/
    }

    if (!peek().has_value()) {
      add_error("Expected '=' with a value or a ';'", line);
    }
    if (peek().value().type != TokenType::eq) {
      stmt_var.has_initial_value = false;
      if (!mut) {
        add_error("Constant need to be declared", line);
      }

      result = NodeStmt{.var = stmt_var};
    } else {
      consume();

      auto expr = parse_expr();
      if (!expr.has_value()) {
        add_error("Invalid Expression", line);
      }
      stmt_var.expr = expr.value();

      result = NodeStmt{.var = stmt_var};
    }
  } else if (peek().has_value() && peek().value().type == TokenType::_fnc ||
             peek().has_value() && peek().value().type == TokenType::_extern &&
                 peek(1).has_value() && peek(1).value().type == TokenType::_fnc) {
    bool is_pub = false;
    bool is_extern = false;
    if (peek().value().type == TokenType::_extern) {
      is_extern = true;
      consume();
    }
    consume();
    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected name in the function declaration\n", line);
    }
    Token name = consume();
    std::vector<CustomFuncArgs> args;
    std::vector<std::string> absolute_type_name_args;

    bool has_args = false;
    bool vargs = false;
    if (peek().has_value() && peek().value().type == TokenType::open_paren) {
    consume();
    has_args = true;
    while (peek().has_value() && peek().value().type != TokenType::close_paren) {
      if (peek().has_value() && peek().value().type == TokenType::ident) {
        std::string arg_name = consume().value.value();
        Type arg_type;
        Token arg_type_tok;

        if (arg_name == "undefan") {
          // arg_type = Type{Type::Kind::NxtUndefNum};
          vargs = true;
          break;
        } else if (arg_name == "undefa") {
          arg_type = Type{Type::Kind::Any};
        } else {
          if (!peek().has_value()) {
            add_error("Malformed expression\n", line);
          }
          consume();
          if (!peek().has_value()) {
            add_error("Malformed expression\n", line);
          }

          arg_type = parse_type();
          consume();
        }
        args.push_back({.name = arg_name, .arg_type = arg_type});
        absolute_type_name_args.push_back(arg_type_tok.value.value());

        if (peek().has_value() && peek().value().type == TokenType::comma) {
          consume();
          continue;
        } else if (peek().has_value() &&
                   peek().value().type == TokenType::close_paren) {
          break;
        }
      } else if (peek().has_value() && peek().value().type == TokenType::none) {
        consume();
        if (has_args ||
            (peek().has_value() && peek().value().type != TokenType::close_paren)) {
          add_error(
              "None must be a unique and no named argument in function declaration",
              line);
        }
      } else {
        add_error("Malformed expression in function declaration", line);
      }
    }
    }
    if (has_args) {
      if (peek().has_value() && peek().value().type == TokenType::close_paren) consume();
      else {
        add_error("Expected ')'", line);
      }
    }

    Type return_type = Type{Type::Kind::None};
    if (peek().has_value() && peek().value().type == TokenType::minus &&
        peek(1).has_value() && peek(1).value().type == TokenType::r_arrow) {
      int line = peek().value().line;
      consume();
      consume();

      if (!peek().has_value()) {
        add_error("Expected a type for return specification", line);
      }

      return_type = parse_type();
      consume(); // parse_type does not consume main token
    }

    std::vector<NodeStmt> code_branch;
    need_semi = true;
    bool is_defined = false;
    if (peek().has_value() && peek().value().type == TokenType::l_key) {
      consume();
      is_defined = true;
      need_semi = true;
      while (peek().has_value() && peek().value().type != TokenType::r_key) {
        auto stmt = parse_stmt();
        if (!stmt.has_value()) {
          add_warning("Empty block in func", line);
          break;
        }
        code_branch.push_back(stmt.value());
      }
      need_semi = false;

      if (!peek().has_value() || peek().value().type != TokenType::r_key) {
        add_error("Expected '}' to end 'func' block", line);
      }
      consume(); // }

      if (peek().has_value() && peek().value().type == TokenType::_pub) {
        consume();
        is_pub = true;
        need_semi = true;
      }
    } else if (peek().has_value() && peek().value().type == TokenType::_pub) {
      consume();
      is_pub = true;
      need_semi = true;
    }
    result =
        NodeStmt{NodeStmtDefFunc{.name = name,
          .args = args,
          .return_type = return_type,
          .is_defined = is_defined,
          .code_branch = code_branch,
          .is_pub = is_pub,
          .is_extern = is_extern,
          .absolute_type_name_args = absolute_type_name_args,
          .is_vargs = vargs,
          .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::ident &&
             peek(1).has_value() &&
             (peek(1).value().type == TokenType::plusplus ||
              peek(1).value().type == TokenType::minusminus)) {
    Token ident = consume(); // Identifier   ...var1...
    int line = ident.line;
    Token op = consume(); // Operator (+=, -=, *=, /=)

    NodeStmtVar reassignment{.ident = ident,
                             .expr = NodeExpr(NodeExprUnaryIncDec{
                                 .ident = ident, .op_token = op, .line = line})};
    result = NodeStmt{.var = reassignment};
  } else if (peek().has_value() && (peek().value().type == TokenType::plusplus ||
                                    peek().value().type == TokenType::minusminus)) {
    Token op_tok = consume();
    auto e = parse_expr();
    if (!e.has_value()) {
      add_error("Malformed expression in inc/dec statment", line);
    }

    NodeExpr expr = *e;
    switch (op_tok.type) {
    case TokenType::plusplus:
      op_tok = Token{.type = TokenType::plus_eq, .value = "+=", .line = line};
      break;
    case TokenType::minusminus:
      op_tok = Token{.type = TokenType::minus_eq, .value = "-=", .line = line};
      break;
    default:
      add_error("THIS SHOULD BE ALREADY CONTROLLED", line);
      break;
    }
    NodeExpr val = NodeExpr(NodeExprIntLit{
        .int_lit = Token{.type = TokenType::int_lit, .value = "1", .line = line}});

    result =
        NodeStmt{.var = NodeStmtAssign{
                     .target = expr, .op_tok = op_tok, .value = val, .line = line}};
  } else if (peek().has_value() &&
             (peek().value().type == TokenType::plusplus ||
              peek().value().type == TokenType::minusminus) &&
             peek(1).has_value() && peek(1).value().type == TokenType::ident) {
    Token op = consume();
    int line = op.line;
    Token ident = consume();

    NodeStmtVarRe reassignment{.ident = ident,
                               .expr = NodeExpr(NodeExprUnaryIncDec{
                                   .ident = ident, .op_token = op, .line = line})};
    result = NodeStmt{.var = reassignment};
  } else if (peek().has_value() && peek().value().type == TokenType::_if) {
    consume(); // If

    auto condition = parse_expr();
    if (!condition.has_value()) {
      add_error("Invalid expression", line);
    }

    need_semi = true;
    if (!peek().has_value() || peek().value().type != TokenType::l_key) {
      add_error("Expected '{' to start conditional", line);
    }
    consume(); // {

    std::vector<NodeStmt> then_branch;
    while (peek().has_value() && peek().value().type != TokenType::r_key) {
      auto stmt = parse_stmt();
      if (!stmt.has_value()) {
        add_error("Invalid statment in 'if' block", line);
      }
      then_branch.push_back(stmt.value());
    }

    if (!peek().has_value() || peek().value().type != TokenType::r_key) {
      add_error("Expected '}' to end 'if' block", line);
    }
    consume(); // }

    std::vector<NodeExpr> elif_conditions;
    std::vector<std::vector<NodeStmt>> elif_branches;
    std::vector<NodeStmt> else_branch;

    while (peek().has_value()) {
      if (peek()->type == TokenType::_elif) {
        consume(); // elif
        /*if (!peek().has_value() || peek().value().type != TokenType::open_paren) {
          add_error("Expected '(' in elif conditional", line);
          return std::nullopt;
        }
        consume(); // (*/

        auto elif_condition = parse_expr();
        if (!elif_condition.has_value()) {
          add_error("Invalid expression", line);
        }

        /*if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
          add_error("Expected ')' after condition in elif", line);
        }
        consume(); // )*/

        if (!peek().has_value() || peek().value().type != TokenType::l_key) {
          add_error("Expected '{' to start conditional", line);
        }
        consume(); // {

        std::vector<NodeStmt> elif_then_branch;
        while (peek().has_value() && peek().value().type != TokenType::r_key) {
          auto stmt = parse_stmt();
          if (!stmt.has_value()) {
            add_error("Invalid statment in 'elif' block", line);
          }
          elif_then_branch.push_back(stmt.value());
        }

        if (!peek().has_value() || peek().value().type != TokenType::r_key) {
          add_error("Expected '}' to end 'elif' block", line);
        }
        consume(); // }

        elif_conditions.push_back(elif_condition.value_or(NodeExpr(NodeExprNone{})));
        elif_branches.push_back(elif_then_branch);
      } else if (peek()->type == TokenType::_else) {
        consume(); // consume 'else'

        if (!peek().has_value() || peek()->type != TokenType::l_key) {
          add_error("Expected '{' to start 'else' block", line);
        }
        consume(); // consume '{'

        std::vector<NodeStmt> else_block;
        while (peek().has_value() && peek()->type != TokenType::r_key) {
          auto stmt = parse_stmt();
          if (!stmt.has_value()) {
            add_error("Invalid statement in 'else' block", line);
          }
          else_block.push_back(stmt.value());
        }

        if (!peek().has_value() || peek()->type != TokenType::r_key) {
          add_error("Expected '}' to end 'else' block");
        }
        consume(); // consume '}'

        else_branch = else_block;
      }

      if (!peek().has_value() || (peek().value().type != TokenType::_elif &&
                                  peek().value().type != TokenType::_else))
        break;
    }
    NodeStmtIf if_stmt{.condition = condition.value(),
                       .then_branch = then_branch,
                       .elif_conditions = elif_conditions,
                       .elif_branches = elif_branches,
                       .else_branch = else_branch,
                       .line = line};

    need_semi = false;
    result = NodeStmt{.var = if_stmt};
  } else if (peek().has_value() && peek().value().type == TokenType::_while) {
    consume(); // while
    std::vector<NodeStmt> bfw;
    std::vector<NodeStmt> afi;
    /*if (peek().has_value() && peek().value().type == TokenType::l_bracket) {
        consume(); // [
        while (peek().has_value() && peek().value().type != TokenType::r_bracket) {
            auto stmt = parse_stmt();
            if (!stmt.has_value()) {
                std::cerr << "Invalid statment in 'while' inline\n";
                terminate(EXIT_FAILURE);
            }
            bfw.push_back(stmt.value());
        }
        if (!peek().has_value() || peek().value().type != TokenType::r_bracket) {
            std::cerr << "Expected ']'\n";
            terminate(EXIT_FAILURE);
        }
        consume(); // ]
    }*/
    /*if (!peek().has_value() || peek().value().type != TokenType::open_paren) {
        add_error("Expected '('", line);
    }
    consume(); // (*/

    auto e = parse_expr();
    if (!e.has_value()) {
      add_error("Invalid expression", line);
    }
    NodeExpr condition = e.value();

    /*if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
        add_error("Expected ')' after while conditional", line);
    }
    consume(); // )*/

    /*if (peek().has_value() && peek().value().type == TokenType::l_bracket) {
        consume(); // [
        while (peek().has_value() && peek().value().type != TokenType::r_bracket) {
            auto stmt = parse_stmt();
            if (!stmt.has_value()) {
                std::cerr << "Invalid statment in 'while' inline\n";
                terminate(EXIT_FAILURE);
            }
            afi.push_back(stmt.value());
        }
        if (!peek().has_value() || peek().value().type != TokenType::r_bracket) {
            std::cerr << "Expected ']'\n";
            terminate(EXIT_FAILURE);
        }
        consume(); // 
    }*/

    if (!peek().has_value() || peek().value().type != TokenType::l_key) {
      add_error("Expected '{' after while conditional");
    }
    consume(); // {

    std::vector<NodeStmt> then_branch;
    while (peek().has_value() && peek().value().type != TokenType::r_key) {
      need_semi = true;
      auto stmt = parse_stmt();
      if (!stmt.has_value()) {
        add_error("Invalid statment in 'while' block", line);
      }
      then_branch.push_back(stmt.value());
    }

    if (!peek().has_value() || peek().value().type != TokenType::r_key) {
      add_error("Expected '}' after while loop", line);
    }
    consume(); // }

    need_semi = false;
    result = NodeStmt{.var = NodeStmtWhile{.condition = condition,
                                           .then_branch = then_branch,
                                           .bfw = bfw,
                                           .afi = afi,
                                           .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_loop) {
    consume();
    std::vector<NodeStmt> then_branch;
    if (!peek().has_value()) {
      add_error("Expected code block or ';'", line);
      return std::nullopt;
    }

    if (peek().value().type == TokenType::l_key) {
      consume(); // {
      while (peek().has_value() && peek().value().type != TokenType::r_key) {
        auto stmt = parse_stmt();
        if (!stmt.has_value()) {
          add_error("Malformed stmt in loop block", line);
        }
        then_branch.push_back(*stmt);
      }
      if (!peek().has_value() || peek().value().type != TokenType::r_key) {
        add_error("Expected '}' to end loop block", line);
      }
      consume();
    }

    need_semi = false;
    result = NodeStmt{.var = NodeStmtLoop{.then_branch = then_branch, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_for) {
    consume();
    need_semi = false;

    /*if (!peek().has_value() || peek().value().type != TokenType::open_paren) {
      add_error("Expected '(' in for loop", line);
      return std::nullopt;
    }
    consume(); // (*/

    std::vector<NodeStmt> init;
    std::vector<NodeStmt> update;
    NodeExpr condition = NodeExpr(NodeExprBoolValue{.value = 1, .line = line});
    int index_status = 0; // 0 = init, 1 = expression, 2 = update
    while (peek().has_value() && peek().value().type != TokenType::l_key) {
      if (index_status != 2) {
        auto e = parse_expr();
        if (e.has_value() && index_status != 2) {
          condition = *e;
          if ((index_status != 2) && !peek().has_value() ||
              peek().value().type != TokenType::semi) {
            add_error("Expected ';' after expression in for loop", line);
            return std::nullopt;
          }
          consume(); // ;
          index_status = 2;
          continue;
        }
      }
      need_semi = false;
      in_for = true;
      auto stmt = parse_stmt();
      in_for = false;
      if (index_status == 0 &&
          (!peek().has_value() || peek().value().type != TokenType::semi)) {
        add_error("Expected ';' in for instruction", line);
      } else if (index_status == 2 && peek().has_value()) {
        if (peek().value().type != TokenType::semi) {
          if (peek().value().type != TokenType::l_key) {
            add_error("Expected ';' in for instruction", line);
          }
        } else if (peek().value().type == TokenType::semi &&
                   (!peek(1).has_value() || peek(1).value().type == TokenType::l_key)) {
          add_error("Extra ';' at the end of for instruction", line);
        }
        if (peek().value().type == TokenType::semi)
          consume();
      } else if (index_status == 0)
        consume();
      if (!stmt.has_value()) {
        // add_error(std::string("Malformed statment/expression in for loop ") +
        // std::string(index_status == 0 ? "(initial values)" : (index_status == 1 ?
        // "expression" : "(update)")), line); return std::nullopt;
        continue;
      }
      if (index_status == 0)
        init.push_back(*stmt);
      else if (index_status == 2)
        update.push_back(*stmt);
    }
    /*if (!peek().has_value() || peek().value().type != TokenType::close_paren) {
      add_error("Expected ')' after for loop condition", line);
      return std::nullopt;
    }
    consume(); // )*/

    std::vector<NodeStmt> code_branch;
    if (!peek().has_value()) {
      add_error("Expected code block or ';'", line);
      return std::nullopt;
    }

    need_semi = true;
    if (peek().value().type == TokenType::l_key) {
      consume(); // {
      while (peek().has_value() && peek().value().type != TokenType::r_key) {
        auto stmt = parse_stmt();
        if (!stmt.has_value()) {
          add_error("Malformed stmt in loop block", line);
          return std::nullopt;
        }
        code_branch.push_back(*stmt);
      }
      if (!peek().has_value() || peek().value().type != TokenType::r_key) {
        add_error("Expected '}' to end loop block", line);
        return std::nullopt;
      }
      consume();
    } else if (peek().value().type != TokenType::semi) {
      add_error("Expected code block or ';'", line);
      return std::nullopt;
    }

    need_semi = false;
    result = NodeStmt{.var = NodeStmtFor{.init = init,
                                         .condition = condition,
                                         .update = update,
                                         .code_branch = code_branch,
                                         .line = line}};
  }

  else if (peek().has_value() && peek().value().type == TokenType::use &&
           peek(1).has_value() && peek(1).value().type == TokenType::str_lit) {
    int line = peek().value().line;
    consume();
    NodeStmtUhead stmt_import;

    stmt_import.mod_name = consume();
    stmt_import.line = line;

    result = NodeStmt{.var = stmt_import};
  }

  else if (peek().has_value() && peek().value().type == TokenType::use) {
    int line = peek().value().line;
    consume();
    NodeStmtUse stmt_import;

    stmt_import.mod_name = consume();
    stmt_import.line = line;

    if (!peek().has_value() || peek().value().type != TokenType::dp ||
        !peek(1).has_value() || peek(1).value().type != TokenType::dp) {
      add_error("Expected element(s) to import", line);
    }

    consume();
    consume();
    if (!peek().has_value()) {
      add_error("Expected element(s) to import after '::'", line);
    }

    std::vector<std::string> elements;
    if (peek().value().type == TokenType::l_key) {
      consume();
      while (peek().has_value() && peek().value().type != TokenType::r_key) {
        if (!peek().has_value() || peek().value().type != TokenType::ident) {
          add_error("Expected element in import list", line);
          break;
        }
        elements.push_back(consume().value.value());

        if (peek().has_value() && peek().value().type == TokenType::comma) {
          consume();
          continue;
        }
      }
      if (!peek().has_value() || peek().value().type != TokenType::r_key) {
        add_error("Expected '}' to close import list", line);
      }
      consume();
    } else if (peek().value().type == TokenType::ident) {
      elements = {consume().value.value()};
    }

    stmt_import.to_import = elements;

    result = NodeStmt{.var = stmt_import};
  }
  else if (peek().has_value() && peek().value().type == TokenType::import) {
    int line = peek().value().line;
    consume();
    NodeStmtImport stmt_import;

    stmt_import.mod_name = consume();
    stmt_import.line = line;

    result = NodeStmt{.var = stmt_import};
  }

  /*else if (peek().has_value() && peek().value().type == TokenType::use &&
              peek(1).has_value() && peek(1).value().type == TokenType::l_key)
  {
      int line = peek().value().line;
      consume();
      consume();

      NodeStmtUse use;

      while (peek().has_value() && peek().value().type != TokenType::r_key) {
          use.use.push_back(consume());

          if (peek().has_value() && peek().value().type == TokenType::comma) {
              consume();
          } else if (peek().has_value() && peek().value().type != TokenType::r_key) {
              add_error("Expected ',' or '}'", line);
          }
      }
      use.line = line;

      consume();

      if (!peek().has_value() || peek().value().type != TokenType::semi) {
          add_error("Expected ';'", line);
      }
      consume();

      return NodeStmt{.var = use};
  }*/
  else if (peek().has_value() && peek().value().type == TokenType::mkpub) {
    int line = peek().value().line;
    NodeStmtMkpub mkpub;
    mkpub.line = line;
    consume();
    if (peek().has_value() && peek().value().type == TokenType::l_key) {
      consume(); // {
      while (peek().has_value() && peek().value().type != TokenType::r_key) {
        mkpub.functions.push_back(consume()); // func name

        if (peek().has_value() && peek().value().type == TokenType::comma) {
          consume(); // ,
        } else if (peek().has_value() && peek().value().type != TokenType::r_key) {
          add_error("Expected ',' or '}'", line);
        }
      }
      consume();
    } else if (peek().has_value() && peek().value().type == TokenType::ident) {
      mkpub.functions.push_back(consume());
    }

    if (!peek().has_value() || peek().value().type != TokenType::semi) {
      add_error("Expected ';'", line);
    }

    consume();

    result = NodeStmt{.var = mkpub};
  } else if (peek().has_value() && peek().value().type == TokenType::ident &&
             peek(1).has_value() && peek(1).value().type == TokenType::dp) {
    need_semi = false;
    Token name = consume();
    int line = peek().value().line;
    consume();
    result = NodeStmt{.var = NodeStmtDefFunc{.name = name, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::endfn) {
    need_semi = false;
    int line = peek().value().line;
    consume();
    result = NodeStmt{.var = NodeStmtEndfn{.line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::ret) {
    int line = peek().value().line;
    consume(); // ret keyword
    auto e = parse_expr();
    if (!e.has_value()) {
      add_error("Invalid expression in return statment", line);
    }
    NodeExpr expr = e.value();

    result = NodeStmt{.var = NodeStmtRet{.value = expr, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_unload) {
    int line = peek().value().line;
    consume(); // unload
    std::vector<Token> vars;

    if (peek().has_value() && peek().value().type == TokenType::l_key) {
      consume(); // {
      while (peek().has_value() && peek().value().type != TokenType::r_key) {
        if (!peek().has_value() || peek().value().type != TokenType::ident) {
          add_error("Expected a variable in list to unload", line);
        }
        vars.push_back(consume());

        if (peek().has_value() && peek().value().type == TokenType::comma) {
          consume();
          continue;
        } else
          break;
      }
      if (!peek().has_value() || peek().value().type != TokenType::r_key) {
        add_error("Expected '}' or ',' after identifier", line);
      }
      consume();
    } else {
      if (!peek().has_value() || peek().value().type != TokenType::ident) {
        add_error("Expected variable to unload", line);
      }
      vars.push_back(consume());
    }

    result = NodeStmt{.var = NodeStmtUnload{.vars = vars, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_stop) {
    int line = peek().value().line;
    consume(); // stop

    result = NodeStmt{.var = NodeStmtStop{.line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_continue) {
    int line = peek().value().line;
    consume(); // continue

    result = NodeStmt{.var = NodeStmtContinue{.line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_declmod) {
    consume(); // declmod
    int line = peek().value().line;

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected name of module", line);
    }
    Token module_name = consume();

    result = NodeStmt{.var = NodeStmtDeclmod{.module_name = module_name, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_endmod) {
    int line = peek().value().line;
    consume(); // endmod

    result = NodeStmt{.var = NodeStmtEndmod{.line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_umod) {
    consume(); //_umod
    int line = peek().value().line;
    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected module to import", line);
    }
    Token module_name = consume();

    if (!peek().has_value() || peek().value().type != TokenType::semi) {
      add_error("Expected ';'", line);
    }
    consume();

    return NodeStmt{.var = NodeStmtUmod{.module_name = module_name, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_ubeepmod) {
    int line = peek().value().line;
    consume();
    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected global module to import", line);
    }
    Token module_name = consume();

    if (!peek().has_value() || peek().value().type != TokenType::semi) {
      add_error("Expected ';'", line);
    }
    consume();

    return NodeStmt{.var = NodeStmtUbeepmod{.module_name = module_name, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_llibrary) {
    int line = peek().value().line;
    consume();

    /*if (!peek().has_value() || peek().value().type != TokenType::l_arrow) {
        add_error("Expected '<'", line);
    }
    consume();*/

    if (!peek().has_value() || peek().value().type != TokenType::str_lit) {
      add_error("Expected library path", line);
    }
    Token path = consume();

    return NodeStmt{.var = NodeStmtLlibrary{.name = path, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_libpath) {
    int line = peek().value().line;
    consume();

    /*if (!peek().has_value() || peek().value().type != TokenType::l_arrow) {
        add_error("Expected '<'", line);
    }
    consume();*/

    if (!peek().has_value() || peek().value().type != TokenType::str_lit) {
      add_error("Expected library path", line);
    }
    Token path = consume();

    result = NodeStmt{.var = NodeStmtLibpath{.path = path, .line = line}};
  }
  /*else if (peek().has_value() && peek().value().type == TokenType::_sptr) {
    int line = peek().value().line;
    consume();

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier", line);
    }
    Token ident = consume();

    if (!peek().has_value() || peek().value().type != TokenType::eq) {
      add_error("Expected '='", line);
    }
    consume();

    auto e = parse_expr();
    if (!e.has_value()) {
      add_error("Expected a expression\n", line);
    }
    NodeExpr expr = e.value();

    if (!peek().has_value() || peek().value().type != TokenType::semi) {
      add_error("Expected ';'", line);
    }
    consume();

    return NodeStmt{.var = NodeStmtSetPtr{.ident = ident, .expr = expr, .line = line}};
  }*/
  else if (peek().has_value() && peek().value().type == TokenType::_globl) {
    int line = peek().value().line;
    consume();

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier", line);
    }
    Token ident = consume();

    result = NodeStmt{.var = NodeStmtGlobl{.ident = ident, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_header) {
    int line = peek().value().line;
    return NodeStmt{.var = NodeStmtHeader{.line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_leave) {
    int line = peek().value().line;
    consume();

    result = NodeStmt{.var = NodeStmtLeave{.line = line}};
  }
  /*else if (peek().has_value() && peek().value().type == TokenType::ident &&
  peek(1).has_value() && peek(1).value().type == TokenType::l_bracket) { int line =
  peek().value().line; Token ident = consume(); consume();

    if (!peek().has_value()) {
      add_error("Expected expression", line);
    }
    auto e = parse_expr();
    if (!e.has_value()) {
      add_error("Malformed expression", line);
    }

    NodeExpr index = e.value();

    if (!peek().has_value() || peek().value().type != TokenType::r_bracket) {
      add_error("Expected ']'", line);
    }
    consume();

    if (!peek().has_value() && peek().value().type != TokenType::eq) {
      add_error("Expected '='", line);
    }
    consume();

    e = parse_expr();
    if (!e.has_value()) {
      add_error("Malformed expression", line);
    }
    NodeExpr expr = e.value();

    if (!peek().has_value() || peek().value().type != TokenType::semi) {
      add_error("Expected ';'");
    }
    consume();

    return NodeStmt{.var = NodeStmtListElement{.list_name = ident, .index = index, .expr
  = expr, .line = line}};
  }*/
  else if (peek().has_value() && peek().value().type == TokenType::_struct) {
    int line = peek().value().line;
    consume();

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected name to struct", line);
    }
    Token struct_name = consume();

    if (!peek().has_value() || peek().value().type != TokenType::l_key) {
      add_error("Expected '{' in struct definition", line);
    }
    consume();

    std::vector<std::pair<std::string, Type>> fields;
    while (peek().has_value() && peek().value().type != TokenType::r_key) {
      if (!peek().has_value() || peek().value().type != TokenType::ident) {
        add_error("Unknown statment in struct");
      }
      Token name = consume();

      if (!peek().has_value() || peek().value().type != TokenType::dp) {
        add_error("Expected ':'");
      }
      consume();

      const std::string &field_name = name.value.value();
      Type type = parse_type();
      consume();
      fields.push_back({field_name, type});

      if (!peek().has_value() || peek().value().type != TokenType::semi) {
        add_error("Expected ';'", line);
      }
      consume();
    }

    if (!peek().has_value() || peek().value().type != TokenType::r_key) {
      add_error("Expected '}'");
    }
    consume();

    result = NodeStmt{
        .var = NodeStmtStruct{.name = struct_name, .fields = fields, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_def) {
    consume();
    need_semi = true;

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier", line);
    }
    Token ident = consume();
    std::vector<Token> value;

    if (peek().has_value() && peek().value().type != TokenType::semi) {
      while (peek().has_value() && peek().value().type != TokenType::semi) {
        value.push_back(consume());
      }
    }

    result = NodeStmt{.var = NodeStmtDefine{.name = ident, .value = value, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_undef) {
    consume();
    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier", line);
    }
    Token ident = consume();

    result = NodeStmt{.var = NodeStmtUndef{.name = ident, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_pre_if) {
    consume(); // If

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier in $elif conditional", line);
      return std::nullopt;
    }
    Token condition = consume();

    need_semi = true;

    std::vector<NodeStmt> then_branch;
    while (peek().has_value() && peek().value().type != TokenType::_pre_endif &&
           peek().value().type != TokenType::_pre_elif &&
           peek().value().type != TokenType::_pre_else) {
      auto stmt = parse_stmt();
      if (!stmt.has_value()) {
        add_error("Invalid statment in 'if' block", line);
      }
      then_branch.push_back(stmt.value());
    }

    if (!peek().has_value() || (peek().value().type != TokenType::_pre_endif &&
                                peek().value().type != TokenType::_pre_elif &&
                                peek().value().type != TokenType::_pre_else)) {
      add_error("Expected '}' to end 'if' block", line);
    }
    if (peek().value().type == TokenType::_pre_endif)
      consume(); // }

    std::vector<Token> elif_conditions;
    std::vector<std::vector<NodeStmt>> elif_branches;
    std::vector<NodeStmt> else_branch;

    while (peek().has_value()) {
      if (peek()->type == TokenType::_pre_elif) {
        consume(); // elif
        if (!peek().has_value() || peek().value().type != TokenType::ident) {
          add_error("Expected identifier in $elif conditional", line);
          return std::nullopt;
        }
        elif_conditions.push_back(consume());

        std::vector<NodeStmt> elif_then_branch;
        while (peek().has_value() && (peek().value().type != TokenType::_pre_elif &&
                                      peek().value().type != TokenType::_pre_else &&
                                      peek().value().type != TokenType::_pre_endif)) {
          auto stmt = parse_stmt();
          if (!stmt.has_value()) {
            add_error("Invalid statment in 'elif' block", line);
          }
          elif_then_branch.push_back(stmt.value());
        }

        if (!peek().has_value() || (peek().value().type != TokenType::_pre_elif &&
                                    peek().value().type != TokenType::_pre_endif &&
                                    peek().value().type != TokenType::_pre_else)) {
          add_error("Expected '}' to end 'elif' block", line);
        }
        if (peek().value().type == TokenType::_pre_endif)
          consume(); // }

        elif_branches.push_back(elif_then_branch);
      } else if (peek()->type == TokenType::_pre_else) {
        consume(); // consume 'else'

        std::vector<NodeStmt> else_block;
        while (peek().has_value() && peek()->type != TokenType::_pre_endif) {
          auto stmt = parse_stmt();
          if (!stmt.has_value()) {
            add_error("Invalid statement in 'else' block", line);
          }
          else_block.push_back(stmt.value());
        }

        if (!peek().has_value() || peek()->type != TokenType::_pre_endif) {
          add_error("Expected 'endif' to end 'else' block");
        }
        consume(); // consume '}'

        else_branch = else_block;
      }

      if (!peek().has_value() || (peek().value().type != TokenType::_pre_elif &&
                                  peek().value().type != TokenType::_pre_else))
        break;
    }
    NodeStmtPreprocessorCond if_stmt{.condition = condition,
                                     .then_branch = then_branch,
                                     .elif_conditions = elif_conditions,
                                     .elif_branches = elif_branches,
                                     .else_branch = else_branch,
                                     .line = line};

    need_semi = false;
    result = NodeStmt{.var = if_stmt};
  } else if (peek().has_value() && peek().value().type == TokenType::_pre_error) {
    consume();
    std::string msg_error = "CUSTOM ERROR WITH NO MESSAGE";
    need_semi = false;

    if (peek().has_value() && peek().value().type == TokenType::str_lit) {
      msg_error = consume().value.value_or(msg_error);
    }

    result = NodeStmt{.var = NodeStmtPreError{.err_msg = msg_error, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_pre_warning) {
    consume();
    std::string msg_warning = "CUSTOM WARNING WITH NO MESSAGE";
    need_semi = false;

    if (peek().has_value() && peek().value().type == TokenType::str_lit) {
      msg_warning = consume().value.value_or(msg_warning);
    }

    result = NodeStmt{.var = NodeStmtPreWarning{.warn_msg = msg_warning, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::_goto) {
    consume();
    Token ident;

    if (!peek().has_value() || peek().value().type != TokenType::ident) {
      add_error("Expected identifier in 'goto' instruction", line);
    } else
      ident = consume();

    result = NodeStmt{.var = NodeStmtGoto{.ident = ident, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::l_key) {
    consume();
    std::vector<NodeStmt> code_branch;

    while (peek().has_value() && peek().value().type != TokenType::r_key) {
      auto maybe_stmt = parse_stmt();
      if (!maybe_stmt.has_value()) {
        add_error("Malfored instruction", line);
        continue;
      }
      code_branch.push_back(*maybe_stmt);
    }

    if (!peek().has_value() || peek().value().type != TokenType::r_key) {
      add_error("Expected '}", line);
    } else
      consume();

    need_semi = false;
    result = NodeStmt{.var = NodeStmtScope{.code_branch = code_branch, .line = line}};
  } else if (peek().has_value() && peek().value().type == TokenType::ident &&
           peek(1).has_value() && peek(1).value().type == TokenType::open_paren ||
          peek(1).has_value() && peek(1).value().type == TokenType::semi) {
    Token name = consume();
    int line = name.line;

    std::vector<NodeExpr> args;
    bool has_args = false;

    if (peek().has_value() && peek().value().type == TokenType::open_paren) {
    consume();
    has_args = true;
    while (peek().has_value() && peek().value().type != TokenType::close_paren) {
      auto arg = parse_expr();
      if (!arg.has_value()) {
        add_error("Invalid Expression in function call", line);
      }
      args.push_back(arg.value());

      if (peek().has_value() && peek().value().type == TokenType::comma) {
        consume();
      } else if (peek().has_value() && peek().value().type != TokenType::close_paren) {
        add_error("Expected ',' or ')'", line);
      }
    }
    }

    if (has_args) {
      if (peek().has_value() && peek().value().type == TokenType::close_paren) consume();
      else {
        add_error("Expected ')'", line);
      }
    }

    std::vector<std::shared_ptr<NodeExpr>> wrapped_args;
    for (auto &arg : args) {
      wrapped_args.push_back(std::make_shared<NodeExpr>(std::move(arg)));
    }
    result = NodeStmt(NodeStmtCall{name, wrapped_args, line});

    // return NodeStmt(.var = NodeStmtCall{name, args}};
  } else {
    int line = peek()->line;
    auto target_expr_opt = parse_expr();
    if (!target_expr_opt.has_value())
      return std::nullopt;
    NodeExpr target_expr = *target_expr_opt;

    // '=' token
    if (!peek().has_value() || !is_assignment_op(peek().value())) {
      add_error("Expected op token", line);
      return std::nullopt;
    }
    Token op_tok = consume();

    // parse value
    auto value_expr = parse_expr();
    if (!value_expr.has_value())
      return std::nullopt;

    // crear NodeStmtAssign
    NodeStmtAssign assign_stmt{.target = target_expr,
      .op_tok = op_tok,
      .value = value_expr.value(),
      .line = line};
    result = NodeStmt{.var = assign_stmt};
  }

  if (need_semi && !in_for) {
    if (!peek().has_value() || peek().value().type != TokenType::semi)
      add_error("Expected ';' at the end of the instruction", line);
    else
      consume();
  }
  need_semi = true;
  return result;
}

std::optional<NodeProg> Parser::parse_prog(bool parse_only_one_expr) {
  NodeProg prog;
  if (parse_only_one_expr) {
    prog.expr = parse_expr().value();
    return prog;
  }
  while (peek().has_value()) {
    if (auto stmt = parse_stmt()) {
      prog.stmts.push_back(stmt.value());
    }
  }
  return prog;
}
