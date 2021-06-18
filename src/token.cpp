#include "include/token.hpp"

Token_f::Token_f(Token_type token_t, char* token_v) : m_type(token_t), m_value(token_v){}

Token_type Token_f::get_type() const{
    return m_type;
}
char* Token_f::get_value() const{
    return m_value;
}