#include "helpers.h"

string Helpers::__CovertStreamToString__(ifstream& file){
    stringstream strStream;
    strStream << file.rdbuf();
    string str = strStream.str();
    return str;
};
