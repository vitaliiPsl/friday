#include <iostream>

#include "include/io.hpp"
#include "include/lexer.hpp"
#include "include/parser.hpp"
#include "include/visitor.hpp"

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

    Scope_f* g_scope = new Scope_f();
    Parser_f* parser = new Parser_f(lexer, g_scope);
    Ast_f* root = parser->parse();
    // std::cout << "Size: " << root->compound_size << std::endl;
    // std::cout << "Type: " << root->compound_value[root->compound_size - 2]->get_type() << std::endl;
    // for(int i = 0; i < root->compound_size; i++){
    //     std::cout << i << ". Type: " << root->compound_value[i]->get_type() << std::endl;
    // }

    Visitor_f* visitor = new Visitor_f(root->scope);
    visitor->visit(root);
}   