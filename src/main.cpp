#include <iostream>

#include "include/io.hpp"
#include "include/lexer.hpp"

int main(int args, char** argv){
    if(args < 2)
        io_info();
    
    char* content = io_read_file(argv[1]);
    Lexer_f* lexer = new Lexer_f(content);

    // Token_f* token = lexer->get_token();
    // while(token->get_type() != Token_type::EOF_TOKEN){
    //     std::cout << token->get_value() << " - " << token->get_type() << std::endl;
    //     token = lexer->get_token();
    // }

    
}