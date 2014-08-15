#include<iostream.h>
#include "fdef.cpp"

using namespace std;

class FontConverter {
	int indexOnArray;
	ifstream in; ofstream out;
	void close();
public:
	FontConverter(int index, char *fin, char *fout) {
		indexOnArray = index;
		in.open(fin, ios::in | ios::binary);
		out.open(fout, ios::out | ios::binary);
	}
	void convert();
	void checkLRReplacements(char);
};

void FontConverter::convert() {
	in.seekg(2, ios::beg);
	//check if size not zero
	char c1[2], c2[2], c3[2], c4[2];
	char zd = 0x0d, fd = 0x4d, tw = 0x20;
	int z = 0;
	while (in.tellg() > 0) {
		if (in.read(c1, 2) == 0) { break; }
		int char_ok = 0;
		//checking for 12 bytes match -> 1byte replace
		//dont require this search if 4d0d is not next character
		//the symbols only conversion is not designed to support starting space or other characters..
		int i = 0;
		if (c1[1] != zd && c1[1] != tw) {
			out << c1[0] << c1[1];
			continue;
		}
		//Matching xx0d
		if (in.read(c3, 2) == 0) i = 42;  // if eof reached
		else {
			if (c3[0] == fd && c3[1] == zd) {
				//Matching xx0d4d0d
				if (in.read(c4, 2) == 0)  i = 42;
				else if (c4[0] == zd && c4[1] == tw) {
					//matched xx0d4d0d0d20 form
					i = 43;
				}
				else if (c4[1] != zd) {
					i = 42;
					in.seekg(-4, ios::cur);
				}
				else i = -1;
			}
			else {
				in.seekg(-2, ios::cur);
				i = 42;
			}
		}
		//This code is an update: to support nta(8B) and sta(10B) already 6B is OK in form.
		//checking sta:
		if (i == -1) {
			//i = 0;//to skip if c4 is not read
			if (c1[0] == 0x38 && c4[0] == 0x31) { //otherwise, skip
				char upd1[2], upd2[2], to = 0x31;
				if (in.read(upd1, 2) != 0)  {//skip the check if no more bytes.
					if (upd1[0] == fd && upd1[1] == zd) {//matching again 4d0d
						if (in.read(upd2, 2) != 0) {//skip check if no more bytes..
							if (upd2[0] == to && upd2[1] == zd) {//matching ending 310d
								checkLRReplacements((char)0xcc);
								char_ok = 1;
							}
							else in.seekg(-2, ios::cur);
						}
					}
					else in.seekg(-2, ios::cur);
				}
			}

		}
		//checked & matched xx0d4d0dxx0d or xx0d4d0d0d20. need to check 2 or 1 bytes.
		else if (i == 43) {
			//checking 280d4d0d0d20310d
			if (c1[0] == 0x28) {
				char upd[2];
				if (in.read(upd, 2) != 0) {
					if (upd[0] == 0x31 && upd[1] == 0x0d){
						checkLRReplacements((char)0xe2);
						char_ok = 1;
					}
					else {
						in.seekg(-2, ios::cur);
					}
				}

			}

			if (char_ok == 1) continue;
			//checking xx0d4d0d0d20
			for (i = 0; i < 5; i++) {
				char h1 = fc20_array[i][0];
				if (h1 == c1[0]) {
					out << (char)fc20_array[i][1];
					char_ok = 1;
					break;
				}
			}
			if (char_ok == 0) { in.seekg(-4, ios::cur); i = 42; }
		}

		if (char_ok == 1) continue;
		//checking xx0d4d0dxx0d
		if (i == -1) {
			for (i = 0; i < 61; i++) {
				char h1 = fc4_array[i][0] >> 8, h2 = fc4_array[i][0];
				if (h1 == c1[0] && h2 == c4[0]) {
					checkLRReplacements((char)fc4_array[i][1]);
					char_ok = 1;
					break;
				}
				else if (i == 60) in.seekg(-4, ios::cur);
			}
		}
		//end of 12B check
		if (char_ok == 1) continue;
		//checking 4B match -> 1B replace
		i = 0;
		if (in.read(c2, 2) == 0) { i = 3; }
		for (; i < 2; i++) {
			char h1 = fcr4_array[i][0] >> 8, h2 = fcr4_array[i][0];
			if (c1[0] == h1) {
				if (c2[1] == zd && c2[0] == h2) {
					out << (char)fcr4_array[i][1];
					char_ok = 1;
					break;
				}
			}
			if (i == 1 && char_ok == 0) in.seekg(-2, ios::cur);
		}
		//end of 4B check
		if (char_ok == 1) continue;
		//checking for 2 bytes match -> 1 byte replace
		for (i = 0; i < 56; i ++) {
			c2[0] = fc_array[i][0];
			if (c1[0] == c2[0]) {
				if (fc_array[i][0] > 0x14 && fc_array[i][0] < 0x3a) {
					checkLRReplacements((char)fc_array[i][1]);
				}
				else {
					out << (char)fc_array[i][1];
				}
				char_ok = 1;
				break;
			}
		}
		
		//end of 2B check
		if (char_ok == 1) continue;
		
		//checking for 2 bytes match->4 byte replace
		for (i = 0; i < 6; i++ ) {
			c2[0] = fc2_array[i][0];
			if (c1[0] == c2[0]) {
				//if matching hex 15-31, need to check left-right, left replace conditions 
				char h1 = fc2_array[i][1] >> 8, h2 = fc2_array[i][1];
				out << h1 << h2;
				char_ok = 1;
				break;
			}
		}
		//end of 2B check 2

		if (char_ok == 0)
			out << c1[0] << c1[1];
	}
	close();
}

void FontConverter::checkLRReplacements(char h1) {
	char c[2]; 
	in.read(c, 2);
	
	//checking left replacements:
	for (int i = 0; i < 2; i++)
		if (c[0] == fcl_array[i][0]) {
			out << (char)fcl_array[i][1] << h1;
			return;
		}
	//checking left-right replacements:
	for (int i = 0; i < 3; i++)
		if (c[0] == fcm_array[i][0]) {
			char h = fcm_array[i][1] >> 8;
			out << h << h1 << (char)fcm_array[i][1];
			return;
		}
	//check left 2B replacements:
	if (c[0] == fcl2_array[0]) {
		char h = fcl2_array[1] >> 8;
		out << h << (char)fcl2_array[1] << h1;
		return;
	}
	//check for 4B match:
	char d[2], h = fcl4_array[0] >> 8, h2 = fcl4_array[0];
	if (h == c[0]) {
		if (in.read(d, 2) != 0) {
			if (d[1] == 0x0d && d[0] == h2) {
				out << (char)fcl4_array[1] << h1;
				return;
			}
			else {
				in.seekg(-2, ios::cur);
			}
		}
	}
	out << h1;
	in.seekg(-2, ios::cur);
	
}

void FontConverter::close() {
	
	out.close();
	in.close();
}

/*
char c1, c2;
int i = 0;
while (i < 20) {
int hex = fc_array[i++][0];
c1 = hex >> 8;
c2 = hex;
out << c1 << c2;
}
*/
