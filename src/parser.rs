use crate::lexer::*;

pub mod parser {
    use crate::lexer;


enum VarType {
    Str, Int, Float
}

pub enum NodeExpr {
    IntLit(NodeExprIntLit),
    Ident(NodeExprIdent),
    StrLit(NodeExprStrLit),
    FloatLit(NodeExprFloatLit),
    Call(NodeExprCall),
    Binary(NodeExprBinary),
    BinaryAssign(NodeExprBinaryAssign),
    UnaryIncDev(NodeExprUnaryIncDec)
}

struct NodeExprIntLit {
    pub int_lit: lexer::Token
}

struct NodeExprIdent {
    pub ident: lexer::Token
}

struct NodeExprStrLit {
    pub str_lit: lexer::Token
}

struct NodeExprFloatLit {
    pub float_lit: lexer::Token
}

struct NodeExprBinary {
    pub lhs: Box<NodeExpr>,
    pub op_token: lexer::Token,
    pub rhs: Box<NodeExpr>
}

struct NodeExprBinaryAssign {
    pub lhs: lexer::Token,
    pub op_token: lexer::Token,
    pub rhs: Box<NodeExpr>
}

struct NodeExprUnaryIncDec {
    pub ident: lexer::Token,
    pub op_token: lexer::Token
}

struct NodeExprCall {
    pub name: lexer::Token,
    pub args: Vec<Box<NodeExpr>>
}

pub enum NodeStmt {
    Let(NodeStmtLet),
    Call(NodeStmtCall),
    Import(NodeStmtImport),
    Use(NodeStmtUse),
    If(NodeStmtIf)
}

struct NodeStmtLet {
    pub ident: lexer::Token,
    pub token_type: lexer::Token,
    pub expr: Box<NodeExpr>
}

struct NodeStmtCall {
    pub name: lexer::Token,
    pub args: Vec<Box<NodeExpr>>
}

struct NodeStmtUse {
    pub use_vec: Vec<lexer::Token>
}

struct NodeStmtImport {
    pub include: lexer::Token
}

struct NodeStmtIf {
    pub condition: Box<NodeExpr>,
    pub then_branch: Vec<Box<NodeStmt>>,
    pub else_branch: Option<Box<NodeStmtIf, Vec<NodeStmt>>>
}

pub struct NodeProg {
    stmts: Vec<NodeStmt>
}

pub struct Parser {
    m_index: usize,
    m_tokens: Vec<lexer::Token>
}

impl Parser {
    fn peek(&mut self, offset: usize) -> Option<lexer::Token> {
        let index: usize = self.m_index + offset;
        return self.m_tokens.get(index).map();
    }

    pub fn new(tokens: Vec<lexer::Token>) {
        Parser {
            m_index: 0,
            m_tokens: tokens
        }
    }
}
}