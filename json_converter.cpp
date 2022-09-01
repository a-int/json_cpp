#include "json_converter.hpp"

//return the name of the first key from the source
std::string JSON::find_key(const std::string& source){
    std::size_t begin, length;
    begin = source.find("\"");
    if (begin == std::string::npos) return "";

    length = source.find("\"", begin+1) - begin-1;
    return source.substr(begin+1, length);
}

JSON::value_type JSON::find_value(std::string& source){
    std::size_t begin = source.find_first_not_of(" \t\n");
    std::size_t length=0;
    
    //STRING
    if (source[begin] == '\"'){
        length = source.find("\"",begin+1) - begin - 1;
        std::string* string = new std::string(source.substr(begin+1, length));
        return build_pair(string, JSON::STRING);
    }
    //INT or DOUBLE
    else if(('0'<= source[begin]) && (source[begin] <= '9')){
        //find a number in the source and take it
        int end = source.find_first_of(" \t\n}],",begin+1);
        length = (source.substr(begin,end - begin)).find_last_of("1234567890");
        std::string str_num = source.substr(begin,length+1);

        //INT
        if(str_num.find(".") == std::string::npos) {
            void* integral = new int(strtol(str_num.c_str(), NULL, 10));
            return build_pair(integral, JSON::INT);
        }
        //DOUBLE
        else{
            void* floating = new double(strtod(str_num.c_str(), NULL));
            return build_pair(floating, JSON::DOUBLE);
        }
    }
    //BOOL
    else if(source[begin] == 'f'){
            return build_pair(new bool(false), JSON::BOOL);
    }
    else if(source[begin] == 't'){
            return build_pair(new bool(true), JSON::BOOL);
    }
    //VECTOR
    else if(source[begin] == '['){
        source = source.substr(begin+1);
        json_vector* new_vector = parse_vector(source);
        return build_pair(new_vector, JSON::VECTOR);
    }
    // //MAP
    else if(source[begin] == '{'){
        json_map* new_map = parse_map(source);
        return build_pair(new_map, JSON::MAP);
    }
    //raise error if value not found
}

//get pairs from the source and return it
JSON::json_map* JSON::parse_map(std::string& source){
    json_map* map_ptr = new json_map();

    while (!source.empty()){
        int isTherepair = source.find_first_of(":}");
        
        //find a key and a value if there is the pair and remove it form the source
        if(source[isTherepair] == ':') {
            std::string key = find_key(source);
            pop_key(source);
            value_type value = find_value(source);
            pop_value(source);
            map_ptr->operator[](key) = value;
        }
        else{
            source = source.substr(isTherepair+1);
            break;
        }
    }
    return map_ptr;
}

//get values from the source and return it
JSON::json_vector* JSON::parse_vector(std::string& source){
    json_vector* vector_ptr = new json_vector();
    while (!source.empty()){
        //check is there any element
        int isTherepair = source.find_first_of("\"1234567890ft]");
        //find a value and remove it from the source
        if(source[isTherepair] != ']') {
            value_type new_value = find_value(source);
            pop_value(source);
            vector_ptr->push_back(new_value);
        }
        else{
            source = source.substr(isTherepair+1);
            break;
        };
    }
    return vector_ptr;
}

void JSON::parse(std::string& source){
    data = parse_map(source);
}

//Find the first pair and remove it from the source
void JSON::pop_value(std::string& source){
    int cut_pos = source.find_first_of("}],");
    if(cut_pos == std::string::npos) {/*ERORR*/}
    else if(source[cut_pos] == ',') source = source.substr(cut_pos+1);
    else source = source.substr(cut_pos);
}

void JSON::pop_key(std::string& source){
    std::size_t begin, new_begin;
    new_begin = source.find(":");
    source = source.substr(new_begin+1);
}

//copy a tenew_valuet from a source file
std::string JSON::get_content(const char* fileName){
    std::fstream file(fileName, std::ios::in);
    if (!file.is_open()) return "";
    
    std::string file_tenew_valuet, tmp;
    while(std::getline(file,tmp)){
        file_tenew_valuet += tmp;
    }
    file.close();
    return file_tenew_valuet;
}

JSON::variable_type JSON::get_type(const std::string& key){
    if (data->count(key) != 0)
        return data->operator[](key).second;
}

pair build_pair(void* data, JSON::variable_type type){
    return pair(data, type);
}

 void pair::operator=(const pair& new_pair){
    if (this->first != new_pair.first)
    {
        this->free();
        this->first = new_pair.first;
        this->second = new_pair.second;
        this->usr_count = new_pair.usr_count;
        *this->usr_count += 1;
    }
 }

void pair::operator=(void* new_data){
    if (new_data != this->first)
    {
        this->free();
        this->first = new_data;
    }
    
}

pair& JSON::operator[](const std::string& key){
    if (data->count(key) != 0)    
        return data->operator[](key);
    //else erorr;
}

void pair::free(){
    *this->usr_count -= 1;
    if(*usr_count == 0)
    {
        if(this->second == JSON::INT)
        {
           delete get_ptr<int>(*this);
        }
        else if (this->second == JSON::DOUBLE)
        {
            delete get_ptr<double>(*this);
        }
        else if (this->second == JSON::BOOL)
        {
            delete get_ptr<bool>(*this);
        }
        else if (this->second == JSON::STRING)
        {
            delete get_ptr<std::string>(*this);
        }
        else if (this->second == JSON::VECTOR)
        {
            delete get_ptr<JSON::json_vector>(*this);
        }
        else if (this->second == JSON::MAP)
        {
            delete get_ptr<JSON::json_map>(*this);
        }
        delete usr_count;
    }
}