#include "json_converter.hpp"

//return the name of the first key from the source
std::string JSON::find_key(const std::string& source){
    std::size_t begin, length;
    begin = source.find("\"");
    if (begin == std::string::npos) return "";

    length = source.find_first_of("\"", begin+1) - begin-1;
    return source.substr(begin+1, length);
}

JSON::value_type JSON::find_value(std::string& source){
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
    //VECTOR
    else if(source[begin] == '['){
        std::vector<value_type>* new_vector = parse_vector(source);
        return std::make_pair(new_vector, JSON::VECTOR);
    }
    // //MAP
    else if(source[begin] == '{'){
        json_type* new_map = parse_map(source);
        return std::make_pair(new_map, JSON::MAP);
    }
    //raise error if value not found
}

//get pairs from the source and return it
JSON::json_type* JSON::parse_map(std::string& source){
    json_type* map_ptr = new json_type();

    while (!source.empty()){
        int isTherePair = source.find_first_of(":}");
        std::string key;
        value_type value;
        
        //find a key and a value if there is the pair and remove it form the source
        if(source[isTherePair] == ':') {
            key = find_key(source);
            value = find_value(source);
            map_ptr->operator[](key) = value;
            pop_pair(source);
        }
        else{
            source = source.substr(isTherePair+1);
            break;
        }
    }
    return map_ptr;
}

//get values from the source and return it
std::vector<JSON::value_type>* JSON::parse_vector(std::string& source){
    std::vector<value_type>* vector_ptr = new std::vector<value_type>();
    while (!source.empty()){
        //check is there any element
        int isTherePair = source.find_first_of("\"1234567890ft]");
        //find a value and remove it from the source
        if(isTherePair != ']') {
            value_type new_value = find_value(source);
            vector_ptr->push_back(new_value);
            pop_pair(source);
        }
        else{
            source = source.substr(isTherePair+1);
            break;
        };
    }
    return vector_ptr;
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
void JSON::pop_pair(std::string& source){
    int cut_pos = source.find_first_of("}],");
    if(cut_pos == std::string::npos) {/*ERORR*/}
    else if(source[cut_pos] == ',') source = source.substr(cut_pos+1);
    else source = source.substr(cut_pos);
}
