#include "include/io.hpp"

#include <fstream>
#include <iostream>

char* io_read_file(char* file_path){
    std::ifstream input_file(file_path);
    if(input_file.is_open()){
        input_file.seekg(0, std::ios::end);
        size_t file_length = input_file.tellg();
        input_file.seekg(0, std::ios::beg);

        char* content = new char[file_length + 1];
        input_file.read(content, file_length);

        return content;
    }

    std::cout << "File error: couldn't find the file - " << file_path << std::endl;
    exit(1);
}

void io_info(){
    std::cout << R"(To interpret code:
    > friday <path/file_name>)" << std::endl;

    exit(1);
}