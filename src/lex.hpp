#pragma once

#include <fstream>
#include <string>
#include <stack>

// Represents a token
enum TokenType {
    EmptyToken,
    Eof,
    
    Func,
    Is,
    End,
    Var,
    Return,
    SemiColon,
    Assign,
    Colon,
    Assign2,
    NEQ,
    
    // Literals
    Id,
    String,
    CharL,
    Int32,
    FloatL
};

struct Token {
    TokenType type;
    std::string id_val;
    char i8_val;
    int i32_val;
    double float_val = 0;
    
    Token();
    void print();
};

// The main lexical analysis class
class Scanner {
public:
    explicit Scanner(std::string input);
    ~Scanner();
    
    void rewind(Token token);
    Token getNext();
    
    std::string getRawBuffer();
    int getLine() { return currentLine; }
    
    bool isEof() { return reader.eof(); }
    bool isError() { return error; }
private:
    std::ifstream reader;
    bool error = false;
    std::stack<Token> token_stack;
    
    // Control variables for the scanner
    std::string rawBuffer = "";
    std::string buffer = "";
    bool inQuote = false;
    int currentLine = 1;
    bool skipNextLineCount = false;
    
    // Functions
    bool isSymbol(char c);
    TokenType getKeyword();
    TokenType getSymbol(char c);
    bool isInt();
    bool isHex();
    bool isFloat();
};

