// LBYARCH S25H - G05 (Ho, Pangan)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include <math.h>

// Function to check if the output matches the expected values
int outputCorrect(const uint8_t* input, const double* output, int totalPixels) {
	for (int i = 0; i < totalPixels; i++) {
		double e = input[i] / 255.0; // expected value
		double diff = fabs(e - output[i]);

		if (diff > 1e-9) {
			printf("Mismatch at index %d: expected %f, got %f\n", i, e, output[i]);
			return 0; // mismatch found
		}
	}
	return 1; // all values match
}


void benchmark(int w, int h) {
	// TODO: Implement benchmarking logic here
}


// Template C to x86 call
extern void imgCvtGrayIntToDouble( 
	const uint8_t* input, // 1d uint8 array
	double* output,       // 1d float64 array
	int w,				  // width of the image
	int h				  // height of the image
);


int main(int argc, char* argv[]) {

	uint8_t* input;
	double* output;
	int totalPixels, h, w;

	// get width and height
	printf("Input height and width: ");
	scanf("%d %d", &h, &w);

	// must be positive integers
	if (h <= 0 || w <= 0) {
		printf("Height and width must be positive integers.\n");
		return 1;
	}

	// get total pixels, allocate memory for input and output arrays
	totalPixels = w * h;
	input = malloc(totalPixels * sizeof(uint8_t));
	output = malloc(totalPixels * sizeof(double));

	// read input pixels and validate
	printf("Enter %d pixels: ", totalPixels);
	for (int i = 0; i < totalPixels; i++) {
		scanf("%hhu", &input[i]);

		// validate value is in range [0, 255]
		// hhu and uint8_t are already [0, 255], but checking for safety
		if (input[i] < 0 || input[i] > 255) {
			printf("Pixel values must be in the range [0, 255].\n");
			free(input);
			free(output);
			return -1;
		}
	}

	// call asm func to convert int to double
	imgCvtGrayIntToDouble(input, output, w, h);

	// OPTIONAL double check output against expected values
	if (outputCorrect(input, output, totalPixels)) {
		printf("CHECK: Output matches expected values.\n");
	}
	else {
		printf("CHECK: Output does NOT match expected values.\n");
	}
	
	// print output pixels in matrix form
	printf("\nOutput: \n");
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			printf("%.2f ", output[i * w + j]);
		}
		printf("\n");
	}

	// free memory
	free(input);
	free(output);
	
	return 0;
}