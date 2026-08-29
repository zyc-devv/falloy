// src/frontend/lexer.h
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <ostream>

namespace falloy::frontend {

enum class TokenKind {
    // 关键字
    Package, Import, Fn, Let, Const, If, Else, Return,
    True, False, Void,
    // 类型
    Int, Long, Float, Double, String, Char, Bool, Bytes,
    // 内存管理
    Weak, Unique,
    // 标点符号
    LParen, RParen, LBrace, RBrace, LBracket, RBracket,
    Semicolon, Colon, Arrow, Comma,
    // 运算符
    Plus, Minus, Star, Slash, Equal, EqualEqual,
    Less, Greater, LessEqual, GreaterEqual, Bang, BangEqual,
    AndAnd, OrOr,
    // 字面量
    IntegerLiteral, FloatLiteral, StringLiteral, CharLiteral,
    Identifier,
    // 特殊
    EndOfFile, Error
};

struct Token {
    TokenKind kind;
    std::string text;
    int line;
    int column;
    std::string filename;
};

class Lexer {
public:
    Lexer(const std::string& source, const std::string& filename = "<input>");
    
    std::vector<Token> tokenize();
    Token nextToken();
    
    bool hasErrors() const { return !errors_.empty(); }
    const std::vector<std::string>& getErrors() const { return errors_; }
    
private:
    std::string source_;
    std::string filename_;
    size_t pos_;
    int line_;
    int column_;
    std::vector<std::string> errors_;  // 简单错误收集，不依赖 DiagnosticCollector
    
    // 辅助方法
    char current() const;
    char peek() const;
    void advance();
    bool isAtEnd() const;
    bool isAlpha(char ch) const;
    bool isDigit(char ch) const;
    bool isAlphaNumeric(char ch) const;
    void skipWhitespaceAndComments();
    
    Token makeToken(TokenKind kind) const;
    Token makeToken(TokenKind kind, const std::string& text) const;
    Token errorToken(const std::string& message);
    
    // Token 读取
    Token readIdentifier();
    Token readNumber();
    Token readString();
    Token readChar();
    Token readSymbol();
};

// 辅助函数
std::string toString(TokenKind kind);
std::ostream& operator<<(std::ostream& os, const Token& token);

} // namespace falloy::frontend