#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>

class JSON{
public:
    enum variable_type{
        STRING = 0, INT, DOUBLE, BOOL, VECTOR, MAP
    };
    typedef std::pair<void*, variable_type> value_type;
    typedef std::map<std::string, value_type> json_type;

    //return the name of the first key from the source
    std::string find_key(const std::string& source);
    //return the first value from provided source text
    value_type find_value(std::string& source);
    
    //get pairs from the source and return it
    json_type* parse_map(std::string& source);

    //get values from the source and return it
    std::vector<value_type>* parse_vector(std::string& source);
    
    //copy a text from a source file
    std::string get_content(const char* fileName);

    //Find the first pair and remove it from the source
    void pop_pair(std::string& string);
private:
    json_type data;
};