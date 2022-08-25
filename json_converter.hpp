#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>


class JSON{
public:
    enum variable_type{
        NONE = 0, STRING, INT, DOUBLE, BOOL, VECTOR, MAP
    };

    struct pair{
        pair():first(nullptr), second(NONE) {}
        pair(void* ptr, variable_type type): first(ptr), second(type) {}
        pair& operator=(const pair&);
        
        void operator=(int);
        void operator=(double);
        void operator=(bool);
        void operator=(const std::string&);
        // void operator=(const std::map& new_value);
        // void operator=(const std::vector& new_value);

        void* first;
        variable_type second;
    };

    typedef pair value_type;
    typedef std::map<const std::string, value_type> json_type;

    //return the name of the first key from the source
    std::string find_key(const std::string& source);
    //return the first value from provided source text
    value_type find_value(std::string& source);
    
    //get pairs from the source and return it
    json_type* parse_map(std::string& source);

    //get values from the source and return it
    std::vector<value_type>* parse_vector(std::string& source);

    void parse(std::string& source);
    
    //copy a text from a source file
    std::string get_content(const char* fileName);

    //Find the first pair and remove it from the source
    void pop_key(std::string& source);
    void pop_value(std::string& source);

    value_type& operator[](const std::string&);

private:
    pair make_pair(void*, variable_type);
private:
    json_type* data;
};