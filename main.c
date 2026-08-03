#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Template C to x86 call

extern void asmhello();

int main(int argc, char* argv[]) {
	asmhello();
	return 0;
}