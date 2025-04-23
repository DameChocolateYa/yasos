use std::process;

#[derive(Debug)]
pub enum TokenType {
    IntLit,
    StrLit,
    FloatLit,
    Semi,
    Comma,
    OpenParen,
    CloseParen,
    Ident,
    Let,
    Eq,
    StrType,
    IntType,
    FloatType,
    Dp,
    CommentBegin,
    CommentEnd,
    _Import,
    _Use,
    LArrow,
    RArrow,
    LKey,
    RKey,
    Plus,
    Minus,
    Star,
    Slash,
    PlusEq,
    MinusEq,
    StarEq,
    SlashEq,
    PlusPlus,
    MinusMinus,
    _If,
    _Else,
    _Elif,
    _And,
    _Or,
    Lt,
    Lte,
    Gt,
    Gte,
    EqEq,
    BangEq
}

#[derive(Debug)]
pub struct Token {
    token_type: TokenType,
    value: Option<String>
}

pub struct Lexer {
    m_src: String,
    m_index: usize
}

impl Lexer {
    pub fn new(src: String) -> Self {
        Lexer {
            m_src: src,
            m_index: 0
        }
    }

    fn peek(&self, offset: usize) -> Option<char> {
        let index: usize = self.m_index + offset;
        return self.m_src.chars().nth(index);
    }

    fn consume(&mut self) -> char {
        let c = self.m_src.chars().nth(self.m_index).unwrap();
        self.m_index += 1;
        c
    }

    pub fn tokenize(&mut self) -> Vec<Token> {
        let mut tokens: Vec<Token> = Vec::new();
        let mut buf: String = String::new();

        while self.peek(0).is_some() {
            if matches!(self.peek(0), Some(c) if c.is_ascii_alphabetic()) {
                buf.push(self.consume());
                while self.peek(0).is_some() {
                    buf.push_str(self.consume().to_string().as_str());
                }
                if buf == "let" {
                    tokens.push(Token{token_type: TokenType::Let, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "str" {
                    tokens.push(Token{token_type: TokenType::StrType, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "int" {
                    tokens.push(Token{token_type: TokenType::IntType, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "float" {
                    tokens.push(Token{token_type: TokenType::FloatType, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "if" {
                    tokens.push(Token{token_type: TokenType::_If, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "else" {
                    tokens.push(Token{token_type: TokenType::_Else, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "elif" {
                    tokens.push(Token{token_type: TokenType::_Elif, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "or" {
                    tokens.push(Token{token_type: TokenType::_Or, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "and" {
                    tokens.push(Token{token_type: TokenType::_And, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "eq" {
                    tokens.push(Token{token_type: TokenType::Eq, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "neq" {
                    tokens.push(Token{token_type: TokenType::BangEq, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "gt" {
                    tokens.push(Token{token_type: TokenType::Gt, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "gte" {
                    tokens.push(Token{token_type: TokenType::Gte, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "lt" {
                    tokens.push(Token{token_type: TokenType::Lt, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "lte" {
                    tokens.push(Token{token_type: TokenType::Lte, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "import" {
                    tokens.push(Token{token_type: TokenType::_Import, value: None});
                    buf.clear();
                    continue;
                }
                else if buf == "use" {
                    tokens.push(Token{token_type: TokenType::_Use, value: None});
                    buf.clear();
                    continue;
                }
                else {
                    tokens.push(Token{token_type: TokenType::Ident, value: Some(buf.clone())});
                    buf.clear();
                    continue;
                }
            }
            else if matches!(self.peek(0), Some(c) if c.is_digit(10)) {
                buf.push(self.consume());
                let mut is_float: bool = false;

                while self.peek(0).is_some() && matches!(self.peek(0), Some(c) if c.is_digit(10)) {
                    buf.push(self.consume());
                }

                if self.peek(0).is_some() && matches!(self.peek(0), Some(c) if c == '.') {
                    is_float = true;
                    buf.push(self.consume());

                    if self.peek(0).is_some() && matches!(self.peek(0), Some(c) if c.is_digit(10)) {
                        while self.peek(0).is_some() && matches!(self.peek(0), Some(c) if c.is_digit(10)) {
                            buf.push(self.consume());
                        }
                    }
                    else {
                        println!("Error: decimal dot without numbers after\n");
                        std::process::exit(1);
                    }
                }

                tokens.push(Token{
                    token_type: if is_float {TokenType::FloatLit} else {TokenType::IntLit},
                    value: Some(buf.clone())
                });
                buf.clear();
            }
            else if matches!(self.peek(0), Some(c) if c == '"') {
                self.consume(); // "
                while self.peek(0).is_some() && matches!(self.peek(0), Some(c) if c != '"') {
                    buf.push(self.consume());
                }
                if !self.peek(0).is_some() || matches!(self.peek(0), Some(c) if c != '"') {
                    println!("Undeterminated string literal");
                    std::process::exit(1);
                }
                self.consume(); // "

                tokens.push(Token{token_type: TokenType::StrLit, value: Some(buf.clone())});
                buf.clear();
            }
            else if matches!(self.peek(0), Some(c) if c == '(') {
                self.consume();
                tokens.push(Token{token_type: TokenType::OpenParen, value: None});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == ')') {
                self.consume();
                tokens.push(Token{token_type: TokenType::CloseParen, value: None});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == ';') {
                self.consume();
                tokens.push(Token{token_type: TokenType::Semi, value: None});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == ':') {
                self.consume();
                tokens.push(Token{token_type: TokenType::Dp, value: None});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '+') && self.peek(1).is_some() && matches!(self.peek(1), Some(c) if c == '+') {
                self.consume(); self.consume();
                tokens.push(Token{token_type: TokenType::PlusPlus, value: Some(String::from("++"))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '-') && self.peek(1).is_some() && matches!(self.peek(1), Some(c) if c == '-') {
                self.consume(); self.consume();
                tokens.push(Token{token_type: TokenType::MinusMinus, value: Some(String::from("--"))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '+') && self.peek(1).is_some() && matches!(self.peek(1), Some(c) if c == '=') {
                self.consume(); self.consume();
                tokens.push(Token{token_type: TokenType::PlusEq, value: Some(String::from("+="))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '-') && self.peek(1).is_some() && matches!(self.peek(1), Some(c) if c == '=') {
                self.consume(); self.consume();
                tokens.push(Token{token_type: TokenType::MinusEq, value: Some(String::from("-="))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '*') && self.peek(1).is_some() && matches!(self.peek(1), Some(c) if c == '=') {
                self.consume(); self.consume();
                tokens.push(Token{token_type: TokenType::StarEq, value: Some(String::from("*="))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '/') && self.peek(1).is_some() && matches!(self.peek(1), Some(c) if c == '=') {
                self.consume(); self.consume();
                tokens.push(Token{token_type: TokenType::SlashEq, value: Some(String::from("/="))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '+') {
                self.consume();
                tokens.push(Token{token_type: TokenType::Plus, value: Some(String::from("+"))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '-') {
                self.consume();
                tokens.push(Token{token_type: TokenType::Minus, value: Some(String::from("-"))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '*') {
                self.consume();
                tokens.push(Token{token_type: TokenType::Star, value: Some(String::from("*"))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c == '/') {
                self.consume();
                tokens.push(Token{token_type: TokenType::Slash, value: Some(String::from("/"))});
                continue;
            }
            else if matches!(self.peek(0), Some(c) if c.is_whitespace()) {
                self.consume();
                continue;
            }
            //TODO: < > <= >? comments...
        }

        self.m_index = 0;
        return tokens;
    }
}