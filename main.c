// LBYARCH S25H - G05 (Ho, Pangan)

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>

// Template C to x86 call
extern void imgCvtGrayIntToDouble( 
	const uint8_t* input, // unsigned 8-bit integer
	double* output,       // double precision float
	int w,
	int h
);

int main(int argc, char* argv[]) {

	uint8_t input = 64;
	double output = 0.0;
	int w = 1, h = 1;

	imgCvtGrayIntToDouble(&input, &output, w, h);
	
	printf("Input: %d, Output: %f\n", input, output);
	
	return 0;
}