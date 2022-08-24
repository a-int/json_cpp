#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>

class JSON{
public:
    //return the name of the first key from the source
    std::string find_key(const std::string& source);
    //return the first value from provided source text
    std::string find_value(const std::string& source);
    
    void parse(const char* fileName);
    
    //copy a text from a source file
    std::string get_content(const char* fileName);

    //Find the first pair and remove it from the source
    void pop_pair(std::string& string, std::string& key, std::string& value);
private:
    enum variable_type{
        STRING = 0, INT, DOUBLE, BOOL, VECTOR, MAP
    };
    std::map<std::string, std::pair<void*, variable_type>> data;
};