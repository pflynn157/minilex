#include <iostream>

#include "lex.hpp"

void Token::print() {
    switch (this->type) {
        case EmptyToken: std::cout << "???" << std::endl; break;
        case Eof: std::cout << "EOF" << std::endl; break;
        
        //##TOKEN PRINT
        
        case Id: std::cout << "ID(" << id_val << ")" << std::endl; break;
        case String: std::cout << "STR(" << id_val << ")" << std::endl; break;
        case CharL: std::cout << "CHAR(" << i8_val << ")" << std::endl; break;
        case Int32: std::cout << "INT(" << i32_val << ")" << std::endl; break;
        case FloatL: std::cout << "FL(" << float_val << ")" << std::endl; break;
    }
}

