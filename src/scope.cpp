#include "include/scope.hpp"
#include <cstdlib>
#include <cstring>

Scope_f::Scope_f(bool scope_t = 0) : m_type(scope_t){
    m_variable_definition_size = 0;
    m_variable_definitions = nullptr;

    m_function_definition_size = 0;
    m_function_definitions = nullptr;
}

void Scope_f::add_variable_definition(Ast_f* variable_definition){
    m_variable_definition_size++;
    if(!m_variable_definitions){
        m_variable_definitions = new Ast_f*[m_variable_definition_size];
    }
    else{
        m_variable_definitions = (Ast_f**)realloc(m_variable_definitions, m_variable_definition_size * sizeof(Ast_f*));
    }
    m_variable_definitions[m_variable_definition_size - 1]  = variable_definition;
}

Ast_f* Scope_f::get_variable_definition(char* variable_name){
    Ast_f* variable_definition;

    for(int i = 0; i < m_variable_definition_size; i++){
        variable_definition = m_variable_definitions[i];
        if(strcmp(variable_definition->variable_definition_name, variable_name) == 0)
            return variable_definition;
    }

    return nullptr;
}

void Scope_f::add_function_definition(Ast_f* function_definition){
    m_function_definition_size++;
    if(!m_function_definitions){
        m_function_definitions = new Ast_f*[m_function_definition_size];
    }
    else{
        m_function_definitions = (Ast_f**)realloc(m_function_definitions, m_function_definition_size * sizeof(Ast_f*));
    }
    m_function_definitions[m_function_definition_size - 1]  = function_definition;
}

Ast_f* Scope_f::get_function_definition(char* function_name){
    Ast_f* function_definition;

    for(int i = 0; i < m_function_definition_size; i++){
        function_definition = m_function_definitions[i];
        if(strcmp(function_definition->function_definition_name, function_name) == 0)
            return function_definition;
    }

    return nullptr;
}