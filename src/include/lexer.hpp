#if !defined(LEXER_H)
#define LEXER_H

#include "token.hpp"
#include <cstdio>

class Lexer_f{
    char* m_content;
    char m_current;
    
    size_t m_index;
    size_t m_content_size;

public:
    explicit Lexer_f(char* content);
    ~Lexer_f();

    Token_f* get_token();
    
private:
    Token_f* get_id();
    Token_f* get_string();
    Token_f* get_number();
    Token_f* get_next_token(Token_f* token);

    void skip_current();
    void skip_spaces();
    char* get_current();
};

#endif // LEXER_H
