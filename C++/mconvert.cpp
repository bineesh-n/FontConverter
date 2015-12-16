#include<string.h>
#include<fstream.h>
#include "FontConverter.cpp"
#include "fexport.h"

void display_codes();

int main() {
    //opt may useful in future
    //switch (opt) {
    std::shared_ptr<FontConverter> fontConverter(new FontConverter(1, "input_pgmFC.txt", "output_pgmFC.txt"));
    fc->convert();
    //}
    
    return 0;
}



void display_codes() {
	cout << "\t1 - ML-TTIndulekha";
} 
