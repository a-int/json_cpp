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

    template<typename T>
    T get();
    template<typename T>
    T get_vec();
    template<typename T>
    T get_map();
};

pair build_pair(void*, JSON::variable_type);

template<typename T>
T* get_ptr(const JSON::value_type& value){
    return reinterpret_cast<T*>(value.first);
}

template<typename T>
T get_val(const JSON::value_type& value){
    return *get_ptr<T>(value);
}

template<typename T>
std::vector<T> convert_vector(JSON::json_vector& vec_val){
    std::vector<T> new_vec;
    for (auto& i: vec_val)
    {
        new_vec.push_back(get_val<T>(i));
    }
    return new_vec;
}

template<typename key_type, typename mapped_type>
std::map<key_type, mapped_type> convert_map(JSON::json_map& map_val){
    std::map<key_type, mapped_type> new_map;
    for (auto& i: map_val)
    {
        new_map[i.first] = get_auto<mapped_type>(i.second);
    }
    return new_map;
}

template<typename T>
T pair::get(){
    return *reinterpret_cast<T*>(this->first);
}

template<typename T>
T pair::get_vec(){
    auto vec = get_val<JSON::json_vector>(*this);
    using type = typename T::value_type;
    return convert_vector<type>(vec);
}
template<typename T>
T pair::get_map(){
    auto map = get_val<JSON::json_map>(*this);
    using key_type = typename T::key_type;
    using mapped_type = typename T::mapped_type;
    return convert_map<key_type, mapped_type>(map);
}