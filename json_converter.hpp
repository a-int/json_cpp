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
        pair(void* ptr, std::size_t size): first(ptr), second(size) {}
        pair& operator=(const pair&);
        
        void operator=(const char*);
        void operator=(int);
        void operator=(double);
        void operator=(bool);
        // void operator=(const std::map& new_value);
        // void operator=(const std::vector& new_value);

        void* first;
        std::size_t second;
    };

    typedef std::pair<JSON::pair, variable_type>            value_type;
    typedef std::map<const std::string, value_type>         json_map;
    typedef std::vector<value_type>                         json_vector;

    //return the name of the first key from the source
    std::string find_key(const std::string& source);
    //return the first value from provided source text
    value_type find_value(std::string& source);
    
    //get pairs from the source and return it
    json_map* parse_map(std::string& source);
    //get values from the source and return it
    json_vector* parse_vector(std::string& source);
    //parse source and build json_object
    void parse(std::string& source);

    //Find the first pair and remove it from the source
    void pop_key(std::string& source);
    //Find the first pair and remove it from the source
    void pop_value(std::string& source);
    
    //copy a text from a source file
    std::string get_content(const char* fileName);
    variable_type get_type(const std::string& key);

    pair& operator[](const std::string&);

    json_map::iterator         begin()         {return data->begin();}
    json_map::const_iterator   begin() const   {return data->cbegin();}
    json_map::iterator         end()           {return data->end();}
    json_map::const_iterator   end()   const   {return data->cend();}

private:
    pair make_pair(void*, std::size_t);
private:
    json_map* data;
};

template<typename T>
T& get(JSON::pair& value){
    return *(reinterpret_cast<T*>(value.first));
}

template<typename T>
std::vector<T> convert_vector(JSON::json_vector& vec_val){
    std::vector<T> new_vec;
    for (auto& i: vec_val)
    {
        new_vec.push_back(get<T>(i));
    }
    return new_vec;
}
