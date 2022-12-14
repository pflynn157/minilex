#include <iostream>
#include <cctype>

#include "lex.hpp"

// The token debug function
Token::Token() {
    type = EmptyToken;
    id_val = "";
    i32_val = 0;
}

// The scanner functions
Scanner::Scanner(std::string input) {
    reader = std::ifstream(input.c_str());
    if (!reader.is_open()) {
        std::cout << "Unknown input file." << std::endl;
        error = true;
    }
}

Scanner::~Scanner() {
    reader.close();
}

void Scanner::rewind(Token token) {
    token_stack.push(token);
}

// The main scanning function
Token Scanner::getNext() {
    if (token_stack.size() > 0) {
        Token top = token_stack.top();
        token_stack.pop();
        return top;
    }

    Token token;
    if (reader.eof()) {
        token.type = Eof;
        return token;
    }
    
    for (;;) {
        char next = reader.get();
        if (reader.eof()) {
            token.type = Eof;
            break;
        }
        
        rawBuffer += next;
        
        if (next == '#') {
            while (next != '\n' && !reader.eof()) {
                next = reader.get();
                rawBuffer += next;
            }
            continue;
        }
        std::string __next = "";
        __next += next;
        __next += reader.get();
        if (reader.eof()) {
            token.type = Eof;
            break;
        }
        if (__next == "//") {
            while (next != '\n' && !reader.eof()) {
                next = reader.get();
                rawBuffer += next;
            }
            continue;
        }
        reader.unget();
        
        if (next == '*') {
          if (reader.peek() == '/') {
            reader.get();
    while (!reader.eof()) {
        char __c = reader.get();
        if (__c == '*' && reader.get() == '/' ) break;
    }
    continue;
          }
        }
        
        
        // TODO: This needs some kind of error handleing
        if (next == '\'') {
            char c = reader.get();
            rawBuffer += c;
            if (c == '\\') {
                c = reader.get();
                if (c == 'n') {
                    c = '\n';
                    rawBuffer += c;
                }
            }
        
            Token charL;
            charL.i8_val = c;
            charL.type = CharL;
            
            next = reader.get();
            rawBuffer += next;
            return charL;
        }
        
        if (next == '\"') {
            if (inQuote) {
                Token str;
                str.type = String;
                str.id_val = buffer;
                
                buffer = "";
                inQuote = false;
                return str;
            } else {
                inQuote = true;
                continue;
            }
        }
        
        if (inQuote) {
            if (next == '\\') {
                next = reader.get();
                rawBuffer += next;
                switch (next) {
                    case 'n': buffer += '\n'; break;
                    case 't': buffer += '\t'; break;
                    default: buffer += '\\' + next;
                }
            } else {
                buffer += next;
            }
            continue;
        }
        
        if (next == ' ' || next == '\n' || isSymbol(next)) {
            if (next == '\n') {
                if (skipNextLineCount) skipNextLineCount = false;
                else ++currentLine;
            }
        
            if (buffer.length() == 0) {
                if (isSymbol(next)) {
                    Token sym;
                    sym.type = getSymbol(next);
                    return sym;
                }
                continue;
            }
            
            // Check if we have a symbol
            // Here, we also check to see if we have a floating point
            if (next == '.') {
                if (isInt()) {
                    buffer += ".";
                    continue;
                } else {
                    Token sym;
                    sym.type = getSymbol(next);
                    token_stack.push(sym);
                }
            } else if (isSymbol(next)) {
                Token sym;
                sym.type = getSymbol(next);
                token_stack.push(sym);
            }
            
            // Now check the buffer
            token.type = getKeyword();
            if (token.type != EmptyToken) {
                buffer = "";
                break;
            }
            
            if (isInt()) {
                token.type = Int32;
                token.i32_val = std::stoi(buffer);
            } else if (isHex()) {
                token.type = Int32;
                token.i32_val = std::stoi(buffer, 0, 16);
            } else if (isFloat()) {
                token.type = FloatL;
                token.float_val = std::stod(buffer);
            } else {
                token.type = Id;
                token.id_val = buffer;
            }
            
            // Reset everything
            buffer = "";
            return token;
        } else {
            buffer += next;
        }
    }
    
    return token;
}

std::string Scanner::getRawBuffer() {
    std::string ret = rawBuffer;
    rawBuffer = "";
    return ret;
}

bool Scanner::isSymbol(char c) {
    switch (c) {
        //case ';':
        case ';': return true;
        case '=': return true;
        case ':': return true;
        case '!': return true;
        
        default: return false;
    }
    return false;
}

TokenType Scanner::getKeyword() {
    //if (buffer == "extern") return Extern;
    if (buffer == "func") return Func;
    else if (buffer == "is") return Is;
    else if (buffer == "end") return End;
    else if (buffer == "var") return Var;
    else if (buffer == "return") return Return;
    return EmptyToken;
}

TokenType Scanner::getSymbol(char c) {
    switch (c) {
        case ';': return SemiColon;
        case '=': return Assign;
        case ':': {
            char c2 = reader.get();
            if (c2 == '=') {
                rawBuffer += c2;
                return Assign2;
            } else {
                reader.unget();
                return Colon;
            }
        } break;
        case '!': {
            char c2 = reader.get();
            if (c2 == '=') {
                rawBuffer += c2;
                return NEQ;
            } else {
                reader.unget();
            }
        } break;
        default: return EmptyToken;
    }
    return EmptyToken;
}

bool Scanner::isInt() {
    for (char c : buffer) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool Scanner::isHex() {
    if (buffer.length() < 3) return false;
    if (buffer[0] != '0' || buffer[1] != 'x') return false;
    
    for (int i = 2; i<buffer.length(); i++) {
        if (!isxdigit(buffer[i])) return false;
    }
    return true;
}

bool Scanner::isFloat() {
    bool foundDot = false;
    for (char c : buffer) {
        if (c == '.') {
            if (foundDot) return false;
            foundDot = true;
        } else if (!isdigit(c)) {
            return false;
        }
    }
    if (!foundDot) return false;
    return true;
}

