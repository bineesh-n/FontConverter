/* combiner.h */
#include <stdio.h>
#define exp __declspec(dllexport)
extern "C"{
	exp int __stdcall main(int );
}
