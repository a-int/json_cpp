#include "json_converter.hpp"

//return the name of the first key from the source
std::string JSON::find_key(const std::string& source){
    std::size_t begin, length;
    begin = source.find("\"");
    if (begin == std::string::npos) return "";

    length = source.find_first_of("\"", begin+1) - begin-1;
    return source.substr(begin+1, length);
}

std::string JSON::find_value(const std::string& source){
     std::size_t colon = source.find(':');
    if(colon == std::string::npos) return "";

    //begind is the index of the char that describe the value type
    std::size_t begin = source.find_first_not_of(" \t", colon+1);
    std::size_t length=0;
    
    //STRING
    if (source[begin] == '\"'){
        length = source.find("\"",begin+1) - begin - 1;
        return source.substr(begin+1, length);
    }
    //INT or DOUBLE
    else if(('0'<= source[begin]) && (source[begin] <= '9')){
        //convert value to int64_t
        int comma = source.find(",",begin+1);
        length = (source.substr(begin,comma - begin)).find_last_of("1234567890");
        std::string tmp = source.substr(begin,length+1);

        //INT
        if(tmp.find(".") != std::string::npos) return tmp;
        //DOUBLE
        else return tmp;
    }
    //BOOL
    else if(source[begin] == 'f'){
            return source.substr(begin,5);
    }
    else if(source[begin] == 't'){
            return source.substr(begin,4);
    }
    // //VECTOR
    // else if(source[begin] == '['){
    //     colon = source.find(']');
    // }
    // //MAP
    // else if(source[begin] == '{'){
    //     colon = source.find('}');
    // }
    return "";
}

//build object from source JSON file
void JSON::parse(const char* fileName){
    std::string source = get_content(fileName);

    std::string key, value;
    while (!source.empty())
    {
        pop_pair(source, key, value);
        //build the JSON the JSON object;
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
void JSON::pop_pair(std::string& string, std::string& key, std::string& value){
    key = find_key(string);
    value = find_value(string);

    int colon = string.find(",");
    string = string.substr(colon+1);
}