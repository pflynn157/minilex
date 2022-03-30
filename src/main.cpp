#include <iostream>
#include <string>

#include "lex.hpp"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cerr << "Error: No input file" << std::endl;
        return 1;
    }
    
    std::string input = argv[1];
    
    Scanner *scanner = new Scanner(input);
    Token token = scanner->getNext();
    while (token.type != Eof) {
        token.print();
        token = scanner->getNext();
    }
    token = scanner->getNext();
    
    delete scanner;
    
    return 0;
}

