#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
struct pair;
class JSON{
public:
    enum variable_type{
        NONE = 0, STRING, INT, DOUBLE, BOOL, VECTOR, MAP
    };

    typedef pair                                    value_type;
    typedef std::map<const std::string, value_type> json_map;
    typedef std::vector<value_type>                 json_vector;

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
    json_map* data;
};

struct pair{
        pair(): first(nullptr), second(JSON::NONE), usr_count(new int(1)){};
        pair(void* data, JSON::variable_type type): first(data), second(type), usr_count(new int(1)){}
        pair(const pair& orig): first(orig.first), second(orig.second), usr_count(orig.usr_count) {*usr_count +=1;}
        ~pair(){free();}
        
        void operator=(const pair&);
        void operator=(void*);
        void free();
        
        void* first;
        JSON::variable_type second;
        int *usr_count;
};

pair build_pair(void*, JSON::variable_type);

template<typename T>
T& get(const JSON::value_type& value){
    return *(get_ptr<T>(value));
}

template<typename T>
T* get_ptr(const JSON::value_type& value){
    return reinterpret_cast<T*>(value.first);
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
