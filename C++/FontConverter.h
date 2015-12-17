#ifndef FONTCONVERTER_H
#define FONTCONVERTER_H

#include<fstream>
#include<iostream>
#include<memory>
#include<sstream>

class FontConverter {
public:
    FontConverter(const std::string& strInput)
        :_strbuf(strInput, std::ios::in)
        ,_in(&_strbuf)
        ,_out(&_outstr)
    {}
    std::string convert();
    void checkLRReplacements(char);
private:
    int indexOnArray = 1;
    std::stringbuf _strbuf;
    std::stringbuf _outstr;
    std::istream   _in;
    std::ostream   _out;
    void close();
};

#endif
