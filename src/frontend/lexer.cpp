// src/frontend/lexer.cpp
#include "lexer.h"

#include <cctype>
#include <unordered_map>
#include <iostream>

namespace falloy::frontend {

const std::unordered_map<std::string, TokenKind> KEYWORDS = {
    {"package", TokenKind::Package},
    {"import", TokenKind::Import},
    {"fn", TokenKind::Fn},
    {"let", TokenKind::Let},
    {"const", TokenKind::Const},
    {"if", TokenKind::If},
    {"else", TokenKind::Else},
    {"return", TokenKind::Return},
    {"true", TokenKind::True},
    {"false", TokenKind::False},
    {"void", TokenKind::Void},
    {"int", TokenKind::Int},
    {"long", TokenKind::Long},
    {"float", TokenKind::Float},
    {"double", TokenKind::Double},
    {"string", TokenKind::String},
    {"char", TokenKind::Char},
    {"bool", TokenKind::Bool},
    {"bytes", TokenKind::Bytes},
    {"weak", TokenKind::Weak},
    {"unique", TokenKind::Unique},
};

// ============== Lexer 实现 ==============

Lexer::Lexer(const std::string& source, const std::string& filename)
    : source_(source)
    , filename_(filename)
    , pos_(0)
    , line_(1)
    , column_(1)
{
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token tok;
    do {
        tok = nextToken();
        tokens.push_back(tok);
    } while (tok.kind != TokenKind::EndOfFile && tok.kind != TokenKind::Error);
    return tokens;
}

Token Lexer::nextToken() {
    skipWhitespaceAndComments();
    
    if (isAtEnd()) {
        return makeToken(TokenKind::EndOfFile);
    }
    
    char ch = current();
    
    if (isAlpha(ch) || ch == '_') {
        return readIdentifier();
    }
    
    if (isDigit(ch)) {
        return readNumber();
    }
    
    if (ch == '"') {
        return readString();
    }
    
    if (ch == '\'') {
        return readChar();
    }
    
    return readSymbol();
}

// ============== 辅助方法 ==============

char Lexer::current() const {
    if (isAtEnd()) return '\0';
    return source_[pos_];
}

char Lexer::peek() const {
    if (pos_ + 1 >= source_.size()) return '\0';
    return source_[pos_ + 1];
}

void Lexer::advance() {
    if (!isAtEnd()) {
        if (source_[pos_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        pos_++;
    }
}

bool Lexer::isAtEnd() const {
    return pos_ >= source_.size();
}

bool Lexer::isAlpha(char ch) const {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool Lexer::isDigit(char ch) const {
    return ch >= '0' && ch <= '9';
}

bool Lexer::isAlphaNumeric(char ch) const {
    return isAlpha(ch) || isDigit(ch) || ch == '_';
}

void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char ch = current();
        
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
            advance();
            continue;
        }
        
        if (ch == '/' && peek() == '/') {
            while (!isAtEnd() && current() != '\n') {
                advance();
            }
            continue;
        }
        
        if (ch == '/' && peek() == '*') {
            advance(); advance();
            while (!isAtEnd()) {
                if (current() == '*' && peek() == '/') {
                    advance(); advance();
                    break;
                }
                advance();
            }
            continue;
        }
        
        break;
    }
}

Token Lexer::makeToken(TokenKind kind) const {
    return Token{kind, "", line_, column_, filename_};
}

Token Lexer::makeToken(TokenKind kind, const std::string& text) const {
    return Token{kind, text, line_, column_, filename_};
}

Token Lexer::errorToken(const std::string& message) {
    errors_.push_back(message);
    return Token{TokenKind::Error, message, line_, column_, filename_};
}

// ============== 读取具体 Token ==============

Token Lexer::readIdentifier() {
    size_t start_pos = pos_;
    int start_col = column_;
    int start_line = line_;
    
    while (!isAtEnd() && isAlphaNumeric(current())) {
        advance();
    }
    
    std::string text = source_.substr(start_pos, pos_ - start_pos);
    
    auto it = KEYWORDS.find(text);
    if (it != KEYWORDS.end()) {
        return Token{it->second, text, start_line, start_col, filename_};
    }
    
    return Token{TokenKind::Identifier, text, start_line, start_col, filename_};
}

Token Lexer::readNumber() {
    size_t start_pos = pos_;
    int start_col = column_;
    int start_line = line_;
    bool is_float = false;
    
    while (!isAtEnd() && isDigit(current())) {
        advance();
    }
    
    if (!isAtEnd() && current() == '.' && isDigit(peek())) {
        is_float = true;
        advance();
        while (!isAtEnd() && isDigit(current())) {
            advance();
        }
    }
    
    if (!isAtEnd() && (current() == 'e' || current() == 'E')) {
        is_float = true;
        advance();
        if (!isAtEnd() && (current() == '+' || current() == '-')) {
            advance();
        }
        if (!isAtEnd() && isDigit(current())) {
            while (!isAtEnd() && isDigit(current())) {
                advance();
            }
        } else {
            return errorToken("Invalid exponent in number literal");
        }
    }
    
    if (!isAtEnd()) {
        char ch = current();
        if (ch == 'f' || ch == 'F') {
            is_float = true;
            advance();
        } else if (ch == 'l' || ch == 'L') {
            advance();
        }
    }
    
    std::string text = source_.substr(start_pos, pos_ - start_pos);
    TokenKind kind = is_float ? TokenKind::FloatLiteral : TokenKind::IntegerLiteral;
    
    return Token{kind, text, start_line, start_col, filename_};
}

Token Lexer::readString() {
    size_t start_pos = pos_;
    int start_col = column_;
    int start_line = line_;
    std::string str_value;
    
    advance(); // 跳过开始的 "
    
    while (!isAtEnd()) {
        char ch = current();
        
        if (ch == '"') {
            advance();
            break;
        }
        
        if (ch == '\\') {
            advance();
            if (isAtEnd()) {
                return errorToken("Unterminated escape sequence");
            }
            char escaped = current();
            switch (escaped) {
                case 'n': str_value += '\n'; break;
                case 't': str_value += '\t'; break;
                case 'r': str_value += '\r'; break;
                case '\\': str_value += '\\'; break;
                case '"': str_value += '"'; break;
                case '\'': str_value += '\''; break;
                case '0': str_value += '\0'; break;
                default:
                    return errorToken("Invalid escape sequence: \\" + std::string(1, escaped));
            }
            advance();
            continue;
        }
        
        if (ch == '\n') {
            return errorToken("Unterminated string literal (newline)");
        }
        
        str_value += ch;
        advance();
    }
    
    if (isAtEnd() && source_.back() != '"') {
        return errorToken("Unterminated string literal");
    }
    
    std::string text = source_.substr(start_pos, pos_ - start_pos);
    return Token{TokenKind::StringLiteral, text, start_line, start_col, filename_};
}

Token Lexer::readChar() {
    size_t start_pos = pos_;
    int start_col = column_;
    int start_line = line_;
    
    advance(); // 跳过开始的 '
    
    if (isAtEnd()) {
        return errorToken("Unterminated character literal");
    }
    
    char ch = current();
    
    if (ch == '\\') {
        advance();
        if (isAtEnd()) {
            return errorToken("Unterminated escape sequence in char literal");
        }
        ch = current();
    }
    
    advance();
    
    if (isAtEnd() || current() != '\'') {
        return errorToken("Unterminated character literal");
    }
    advance();
    
    std::string text = source_.substr(start_pos, pos_ - start_pos);
    return Token{TokenKind::CharLiteral, text, start_line, start_col, filename_};
}

Token Lexer::readSymbol() {
    char ch = current();
    int start_line = line_;
    int start_col = column_;
    
    if (ch == '=' && peek() == '=') {
        advance(); advance();
        return Token{TokenKind::EqualEqual, "==", start_line, start_col, filename_};
    }
    
    if (ch == '!' && peek() == '=') {
        advance(); advance();
        return Token{TokenKind::BangEqual, "!=", start_line, start_col, filename_};
    }
    
    if (ch == '<' && peek() == '=') {
        advance(); advance();
        return Token{TokenKind::LessEqual, "<=", start_line, start_col, filename_};
    }
    
    if (ch == '>' && peek() == '=') {
        advance(); advance();
        return Token{TokenKind::GreaterEqual, ">=", start_line, start_col, filename_};
    }
    
    if (ch == '-' && peek() == '>') {
        advance(); advance();
        return Token{TokenKind::Arrow, "->", start_line, start_col, filename_};
    }
    
    if (ch == '&' && peek() == '&') {
        advance(); advance();
        return Token{TokenKind::AndAnd, "&&", start_line, start_col, filename_};
    }
    
    if (ch == '|' && peek() == '|') {
        advance(); advance();
        return Token{TokenKind::OrOr, "||", start_line, start_col, filename_};
    }
    
    TokenKind kind;
    switch (ch) {
        case '(': kind = TokenKind::LParen; break;
        case ')': kind = TokenKind::RParen; break;
        case '{': kind = TokenKind::LBrace; break;
        case '}': kind = TokenKind::RBrace; break;
        case '[': kind = TokenKind::LBracket; break;
        case ']': kind = TokenKind::RBracket; break;
        case ';': kind = TokenKind::Semicolon; break;
        case ':': kind = TokenKind::Colon; break;
        case ',': kind = TokenKind::Comma; break;
        case '+': kind = TokenKind::Plus; break;
        case '-': kind = TokenKind::Minus; break;
        case '*': kind = TokenKind::Star; break;
        case '/': kind = TokenKind::Slash; break;
        case '=': kind = TokenKind::Equal; break;
        case '<': kind = TokenKind::Less; break;
        case '>': kind = TokenKind::Greater; break;
        case '!': kind = TokenKind::Bang; break;
        default:
            advance();
            return errorToken("Unexpected character: '" + std::string(1, ch) + "'");
    }
    
    advance();
    return Token{kind, std::string(1, ch), start_line, start_col, filename_};
}

// ============== Token 辅助 ==============

std::string toString(TokenKind kind) {
    static const std::unordered_map<TokenKind, std::string> names = {
        {TokenKind::Package, "Package"},
        {TokenKind::Import, "Import"},
        {TokenKind::Fn, "Fn"},
        {TokenKind::Let, "Let"},
        {TokenKind::Const, "Const"},
        {TokenKind::If, "If"},
        {TokenKind::Else, "Else"},
        {TokenKind::Return, "Return"},
        {TokenKind::True, "True"},
        {TokenKind::False, "False"},
        {TokenKind::Void, "Void"},
        {TokenKind::Int, "Int"},
        {TokenKind::Long, "Long"},
        {TokenKind::Float, "Float"},
        {TokenKind::Double, "Double"},
        {TokenKind::String, "String"},
        {TokenKind::Char, "Char"},
        {TokenKind::Bool, "Bool"},
        {TokenKind::Bytes, "Bytes"},
        {TokenKind::LParen, "LParen"},
        {TokenKind::RParen, "RParen"},
        {TokenKind::LBrace, "LBrace"},
        {TokenKind::RBrace, "RBrace"},
        {TokenKind::LBracket, "LBracket"},
        {TokenKind::RBracket, "RBracket"},
        {TokenKind::Semicolon, "Semicolon"},
        {TokenKind::Colon, "Colon"},
        {TokenKind::Arrow, "Arrow"},
        {TokenKind::Comma, "Comma"},
        {TokenKind::Plus, "Plus"},
        {TokenKind::Minus, "Minus"},
        {TokenKind::Star, "Star"},
        {TokenKind::Slash, "Slash"},
        {TokenKind::Equal, "Equal"},
        {TokenKind::EqualEqual, "EqualEqual"},
        {TokenKind::Less, "Less"},
        {TokenKind::Greater, "Greater"},
        {TokenKind::LessEqual, "LessEqual"},
        {TokenKind::GreaterEqual, "GreaterEqual"},
        {TokenKind::Bang, "Bang"},
        {TokenKind::BangEqual, "BangEqual"},
        {TokenKind::IntegerLiteral, "IntegerLiteral"},
        {TokenKind::FloatLiteral, "FloatLiteral"},
        {TokenKind::StringLiteral, "StringLiteral"},
        {TokenKind::CharLiteral, "CharLiteral"},
        {TokenKind::Identifier, "Identifier"},
        {TokenKind::EndOfFile, "EndOfFile"},
        {TokenKind::Error, "Error"},
    };
    
    auto it = names.find(kind);
    if (it != names.end()) {
        return it->second;
    }
    return "Unknown";
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << "Token(" << toString(token.kind)
       << ", '" << token.text << "'"
       << ", " << token.filename << ":" << token.line << ":" << token.column << ")";
    return os;
}

} // namespace falloy::frontend