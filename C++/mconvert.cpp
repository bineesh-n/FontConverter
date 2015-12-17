#include<string>
#include<fstream>
#include "FontConverter.h"
//#include "fexport.h"

void display_codes();

int main(int argc, char *argv[]) {
    //opt may useful in future
    //switch (opt) {
    FontConverter fontConverter(argv[1]);
    std::ofstream out("a.txt", std::ios::out);
    out << fontConverter.convert() << std::endl;
    //}
    
    return 0;
}



void display_codes() {
    std::cout << "\t1 - ML-TTIndulekha";
} 
