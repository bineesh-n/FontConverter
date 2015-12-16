#include<fstream>
#include<iostream>
#include<memory>
#include<sstream>

#include "FontMap.h"

class FontConverter {
public:
    FontConverter(const std::string& strInput)
        :_strbuf(strInput, std::ios::in)
        ,_in(&_strbuf)
        ,_out(&_outstr)
    {}
    void convert();
    void checkLRReplacements(char);
private:
    int indexOnArray = 1;
    std::stringbuf _strbuf;
    std::stringbuf _outstr;
    std::istream   _in;
    std::ostream   _out;
    void close();
};
