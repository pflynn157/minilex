#include <iostream>

#include "lex.hpp"

void Token::print() {
    switch (this->type) {
        case EmptyToken: std::cout << "???" << std::endl; break;
        case Eof: std::cout << "EOF" << std::endl; break;
        
        case Func: std::cout << "func" << std::endl; break;
        case Is: std::cout << "is" << std::endl; break;
        case End: std::cout << "end" << std::endl; break;
        case Var: std::cout << "var" << std::endl; break;
        case Return: std::cout << "return" << std::endl; break;
        case SemiColon: std::cout << ";" << std::endl; break;
        case Assign: std::cout << "=" << std::endl; break;
        case Colon: std::cout << ":" << std::endl; break;
        case Assign2: std::cout << ":=" << std::endl; break;
        case NEQ: std::cout << "!=" << std::endl; break;
        
        case Id: std::cout << "ID(" << id_val << ")" << std::endl; break;
        case String: std::cout << "STR(" << id_val << ")" << std::endl; break;
        case CharL: std::cout << "CHAR(" << i8_val << ")" << std::endl; break;
        case Int32: std::cout << "INT(" << i32_val << ")" << std::endl; break;
        case FloatL: std::cout << "FL(" << float_val << ")" << std::endl; break;
    }
}

