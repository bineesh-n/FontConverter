#include "FontConverter.h"
#include "FontMap.h"

std::string FontConverter::convert() {
    _in.seekg(2, std::ios::beg);
    //check if size not zero
    char c1[2], c2[2], c3[2], c4[2];
    char zd = 0x0d, fd = 0x4d, tw = 0x20;
    int z = 0;
    while (_in.tellg() > 0) {
        std::cout << _in.tellg() << std::endl;
        if (!_in.read(c1, 2)) { break; }
        int char_ok = 0;
        //checking for 12 bytes match -> 1byte replace
        //dont require this search if 4d0d is not next character
        //the symbols only conversion is not designed to support starting space or other characters..
        int i = 0;
        if (c1[1] != zd && c1[1] != tw) {
            _out << c1[0] << c1[1];
            continue;
        }
        //Matching xx0d
        if (!_in.read(c3, 2)) i = 42;  // if eof reached
        else {
            if (c3[0] == fd && c3[1] == zd) {
                //Matching xx0d4d0d
                if (!_in.read(c4, 2))  i = 42;
                else if (c4[0] == zd && c4[1] == tw) {
                    //matched xx0d4d0d0d20 form
                    i = 43;
                }
                else if (c4[1] != zd) {
                    i = 42;
                    _in.seekg(-4, std::ios::cur);
                }
                else i = -1;
            }
            else {
                _in.seekg(-2, std::ios::cur);
                i = 42;
            }
        }
        //This code is an update: to support nta(8B) and sta(10B) already 6B is OK in form.
        //checking sta:
        if (i == -1) {
            //i = 0;//to skip if c4 is not read
            if (c1[0] == 0x38 && c4[0] == 0x31) { //otherwise, skip
                char upd1[2], upd2[2], to = 0x31;
                if (_in.read(upd1, 2))  {//skip the check if no more bytes.
                    if (upd1[0] == fd && upd1[1] == zd) {//matching again 4d0d
                        if (_in.read(upd2, 2)) {//skip check if no more bytes..
                            if (upd2[0] == to && upd2[1] == zd) {//matching ending 310d
                                checkLRReplacements((char)0xcc);
                                char_ok = 1;
                            }
                            else _in.seekg(-2, std::ios::cur);
                        }
                    }
                    else _in.seekg(-2, std::ios::cur);
                }
            }
            
        }
        //checked & matched xx0d4d0dxx0d or xx0d4d0d0d20. need to check 2 or 1 bytes.
        else if (i == 43) {
            //checking 280d4d0d0d20310d
            if (c1[0] == 0x28) {
                char upd[2];
                if (!_in.read(upd, 2)) {
                    if (upd[0] == 0x31 && upd[1] == 0x0d){
                        checkLRReplacements((char)0xe2);
                        char_ok = 1;
                    }
                    else {
                        _in.seekg(-2, std::ios::cur);
                    }
                }
                
            }
            
            if (char_ok == 1) continue;
            //checking xx0d4d0d0d20
/*            for (i = 0; i < 5; i++) {
                char h1 = fc20_map[i];
                if (h1 == c1[0]) {
                    _out << (char)fc20_array[i][1];
                    char_ok = 1;
                    break;
                }
                }*/
            char h1;
            auto it = fc20_map.find(c1[0]);
            if(it != fc20_map.end()) {
                _out << (char)it->second;
                char_ok = 1;
                break;
            }
            
            if (char_ok == 0) { _in.seekg(-4, std::ios::cur); i = 42; }
        }
        
        if (char_ok == 1) continue;
        //checking xx0d4d0dxx0d
        if (i == -1) {
            /*for (i = 0; i < 61; i++) {
                char h1 = fc4_array[i][0] >> 8, h2 = fc4_array[i][0];
                if (h1 == c1[0] && h2 == c4[0]) {
                    checkLRReplacements((char)fc4_array[i][1]);
                    char_ok = 1;
                    break;
                }
                else if (i == 60) _in.seekg(-4, std::ios::cur);
                }*/
            int c1i = c1[0] << 8;
            auto it = fc4_map.find(c1i + c4[0]);
            if(it != fc4_map.end())
            {
                checkLRReplacements((char)it->second);
                char_ok = 1;
                break;
            }
            else
            {
                _in.seekg(-4, std::ios::cur);
            }
            
        }
        //end of 12B check
        if (char_ok == 1) continue;
        //checking 4B match -> 1B replace
        /*for (; i < 2; i++) {
            char h1 = fcr4_array[i][0] >> 8, h2 = fcr4_array[i][0];
            if (c1[0] == h1) {
                if (c2[1] == zd && c2[0] == h2) {
                    _out << (char)fcr4_array[i][1];
                    char_ok = 1;
                    break;
                }
            }
            if (i == 1 && char_ok == 0) _in.seekg(-2, std::ios::cur);
        }*/
        if (_in.read(c2, 2))
        {
            int c1i = c1[0] << 8;
            auto it = fcr4_map.find(c1i + c2[0]);
            if(it != fcr4_map.end() && c2[1] == zd)
            {
                checkLRReplacements((char)it->second);
                char_ok = 1;
                break;
            }
            else
            {
                _in.seekg(-4, std::ios::cur);
            }
        }
        //end of 4B check
        if (char_ok == 1) continue;
        //checking for 2 bytes match -> 1 byte replace
        /*for (i = 0; i < 56; i ++) {
            c2[0] = fc_array[i][0];
            if (c1[0] == c2[0]) {
                if (fc_array[i][0] > 0x14 && fc_array[i][0] < 0x3a) {
                    checkLRReplacements((char)fc_array[i][1]);
                }
                else {
                    _out << (char)fc_array[i][1];
                }
                char_ok = 1;
                break;
            }
            }*/
        
        auto it = fc_map.find(c1[0]);
        if(it != fc_map.end())
        {
            if (it->second > 0x14 && it->second < 0x3a) {
                checkLRReplacements((char)it->second);
            }
            else {
                _out << (char) it->second;
            }
            char_ok = 1;
            break;
        }
        //end of 2B check
        if (char_ok == 1) continue;
	
        //checking for 2 bytes match->4 byte replace
        /*for (i = 0; i < 6; i++ ) {
            c2[0] = fc2_array[i][0];
            if (c1[0] == c2[0]) {
                //if matching hex 15-31, need to check left-right, left replace conditions 
                char h1 = fc2_array[i][1] >> 8, h2 = fc2_array[i][1];
                _out << h1 << h2;
                char_ok = 1;
                break;
            }
            }*/
        //end of 2B check 2
        it = fc2_map.find(c1[0]);
        if(it != fc2_map.end())
        {
            _out << (char) (it->second >> 8) << (char) it->second;
            char_ok = 1;
            break;
        }
        
        if (char_ok == 0)
            _out << c1[0] << c1[1];
    }

    return _outstr.str();
    //close();
}

void FontConverter::checkLRReplacements(char h1) {
    char c[2]; 
    _in.read(c, 2);
    
    //checking left replacements:
    /*for (int i = 0; i < 2; i++)
        if (c[0] == fcl_array[i][0]) {
            _out << (char)fcl_array[i][1] << h1;
            return;
        }*/
    auto it = fcl_map.find(c[0]);
    if (it != fcl_map.end())
    {
        _out << (char) it->second;
        return;
    }
    //checking left-right replacements:
    /*for (int i = 0; i < 3; i++)
        if (c[0] == fcm_array[i][0]) {
            char h = fcm_array[i][1] >> 8;
            _out << h << h1 << (char)fcm_array[i][1];
            return;
            }*/
    it = fcm_map.find(c[0]);
    if (it != fcm_map.end())
    {
        _out << (char)(it->second >> 8) << h1 << (char) it->second;
        return;
    }
    //check left 2B replacements:
    /*if (c[0] == fcl2_array[0]) {
        char h = fcl2_array[1] >> 8;
        _out << h << (char)fcl2_array[1] << h1;
        return;
        }*/
    it = fcl2_map.find(c[0]);
    if (it != fcl2_map.end())
    {
        _out << (char)(it->second >> 8) << (char) it->second << h1;
        return;
    }
    //check for 4B match:
    /* char d[2], h = fcl4_array[0] >> 8, h2 = fcl4_array[0];
    if (h == c[0]) {
        if (_in.read(d, 2)) {
            if (d[1] == 0x0d && d[0] == h2) {
                _out << (char)fcl4_array[1] << h1;
                return;
            }
            else {
                _in.seekg(-2, std::ios::cur);
            }
        }
        }*/
    char d[2];
    if(_in.read(d, 2))
    {
        int c0i = c[0] << 8;
        it = fcl4_map.find(c0i + d[0]);
        if (it != fcl4_map.end() && d[1] == 0x0d)
        {
            _out << (char) it->second << h1;
        }
    }
    
    _out << h1;
    _in.seekg(-2, std::ios::cur);
	
}

void FontConverter::close() {
	
//	_out.close();
//	_in.close();
}

/*
char c1, c2;
int i = 0;
while (i < 20) {
int hex = fc_array[i++][0];
c1 = hex >> 8;
c2 = hex;
_out << c1 << c2;
}
*/
