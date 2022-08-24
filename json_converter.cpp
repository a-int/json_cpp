#include "json_converter.hpp"

//return the name of the first key from the source
std::string JSON::find_key(const std::string& source){
    std::size_t begin, length;
    begin = source.find("\"");
    if (begin == std::string::npos) return "";

    length = source.find_first_of("\"", begin+1) - begin-1;
    return source.substr(begin+1, length);
}

JSON::value_type JSON::find_value(const std::string& source){
     std::size_t colon = source.find(':');
    //if(colon == std::string::npos) return "";

    //begind is the index of the char that describe the value type
    std::size_t begin = source.find_first_not_of(" \t\n", colon+1);
    std::size_t length=0;
    
    //STRING
    if (source[begin] == '\"'){
        length = source.find("\"",begin+1) - begin - 1;
        void* string = new std::string(source.substr(begin+1, length));
        return std::make_pair(string, JSON::STRING);
    }
    //INT or DOUBLE
    else if(('0'<= source[begin]) && (source[begin] <= '9')){
        //find a number in the source and take it
        int end = source.find_first_of(" \t\n}],",begin+1);
        length = (source.substr(begin,end - begin)).find_last_of("1234567890");
        std::string str_num = source.substr(begin,length+1);

        //INT
        if(str_num.find(".") != std::string::npos) {
            void* integral = new int(strtol(str_num.c_str(), NULL, 10));
            return std::make_pair(integral, JSON::INT);
        }
        //DOUBLE
        else{
            void* floating = new double(strtod(str_num.c_str(), NULL));
            return std::make_pair(floating, JSON::DOUBLE);
        }
    }
    //BOOL
    else if(source[begin] == 'f'){
            return std::make_pair(new bool(false), JSON::BOOL);
    }
    else if(source[begin] == 't'){
            return std::make_pair(new bool(true), JSON::BOOL);
    }
    // //VECTOR
    // else if(source[begin] == '['){
    //     colon = source.find(']');
    // }
    // //MAP
    // else if(source[begin] == '{'){
        // map *new_map = new map();
        // std::string key("NOT_FOUND"), value;

        // string copy = source.substring();
        // while(!brakets_array_ptr->empty()){
        //     pop_pair(copy, key, value, brakets_array_ptr);
        //     new_map[key] = value;
        // }
        // return std::make_pair(new_map, JSON::MAP);
    // }
    //raise error if value not found
}

//build object from source JSON file
void JSON::parse(const char* fileName){
    std::string source = get_content(fileName);

    std::string key;
    value_type value;
    while (!source.empty())
    {
        pop_pair(source, key, value);
        data[key] = value;
    }
}

//copy a text from a source file
std::string JSON::get_content(const char* fileName){
    std::fstream file(fileName, std::ios::in);
    if (!file.is_open()) return "";
    
    std::string file_text, tmp;
    while(std::getline(file,tmp)){
        file_text += tmp;
    }
    file.close();
    return file_text;
}

//Find the first pair and remove it from the source
void JSON::pop_pair(std::string& string, std::string& key, value_type& value){
    //if(string.find_first_of(":}],"))
    //there is at least one pair in scope
    //if(string[found] == ':'){
        key = find_key(string);
        value = find_value(string);
    //}
    //cut processed element from the source text
    int cut_pos = string.find_first_of("}],");
    if(cut_pos == std::string::npos) //end of the scope
    string = string.substr(cut_pos+1);
}