#include<string>
#include<fstream>
#include "FontConverter.h"

extern "C" {
    __declspec(dllexport) char* __cdecl convert(const char *);
}

char * convert(const char *str_) {
    std::string inStr(str_);
    FontConverter fontConverter(inStr);
    return fontConverter.convert().c_str();
}

