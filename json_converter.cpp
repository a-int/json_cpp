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
        void* string = new std::string(source.substr(begin+1, length));
        return make_pair(string, JSON::STRING);
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
            return make_pair(integral, JSON::INT);
        }
        //DOUBLE
        else{
            void* floating = new double(strtod(str_num.c_str(), NULL));
            return make_pair(floating, JSON::DOUBLE);
        }
    }
    //BOOL
    else if(source[begin] == 'f'){
            return make_pair(new bool(false), JSON::BOOL);
    }
    else if(source[begin] == 't'){
            return make_pair(new bool(true), JSON::BOOL);
    }
    //VECTOR
    else if(source[begin] == '['){
        source = source.substr(begin+1);
        std::vector<value_type>* new_vector = parse_vector(source);
        return make_pair(new_vector, JSON::VECTOR);
    }
    // //MAP
    else if(source[begin] == '{'){
        json_type* new_map = parse_map(source);
        return make_pair(new_map, JSON::MAP);
    }
    //raise error if value not found
}

//get pairs from the source and return it
JSON::json_type* JSON::parse_map(std::string& source){
    json_type* map_ptr = new json_type();

    while (!source.empty()){
        int isTherePair = source.find_first_of(":}");
        
        //find a key and a value if there is the pair and remove it form the source
        if(source[isTherePair] == ':') {
            std::string key = find_key(source);
            pop_key(source);
            value_type value = find_value(source);
            pop_value(source);
            map_ptr->operator[](key) = value;
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
        if(source[isTherePair] != ']') {
            value_type new_value = find_value(source);
            pop_value(source);
            vector_ptr->push_back(new_value);
        }
        else{
            source = source.substr(isTherePair+1);
            break;
        };
    }
    return vector_ptr;
}

void JSON::parse(std::string& source){
    data = parse_map(source);
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

JSON::pair JSON::make_pair(void* data, variable_type type){
    return pair{data, type};
}

JSON::pair& JSON::operator[](const std::string& key){
    return data->operator[](key);
}

JSON::pair& JSON::pair::operator=(const pair& new_pair){
    if (this->first != new_pair.first)
    {
        //free mem
        this->first = new_pair.first;
        this->second = new_pair.second;
    }
    return *this;
}

void JSON::pair::operator=(int new_value){
    int* data = reinterpret_cast<int*>(this->first);
    if (new_value != *data){
        delete data;
        this->first = new int(new_value);
    }
}
void JSON::pair::operator=(double new_value){
    double* data = reinterpret_cast<double*>(this->first);
    if (new_value != *data){
        delete data;
        this->first = new double(new_value);
    }
}
void JSON::pair::operator=(bool new_value){
    bool* data = reinterpret_cast<bool*>(this->first);
    if (new_value != *data){
        delete data;
        this->first = new bool(new_value);
    }
}
void JSON::pair::operator=(const std::string& new_value){
    std::string* data = reinterpret_cast<std::string*>(this->first);
    if (new_value != *data){
        delete data;
        this->first = new std::string(new_value);
    }
}

// void pair::operator=(const std::map& new_value){
    
// }
// void pair::operator=(const std::vector& new_value){
    
// }