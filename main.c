// LBYARCH S25H - G09 (Ho, Pangan)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdint.h>
#include <math.h>
#include <time.h>


// Template C to x86 call
extern void imgCvtGrayIntToDouble(
	const uint8_t* input, // 1d uint8 array
	double* output,       // 1d float64 array
	int w,				  // width of the image
	int h				  // height of the image
);


/*
	Function that computes the expected value per pixel in the w * h pixel array.
	Returns 1 if all values match expected values, else 0 if at least one pixel 
	has an error exceeding 1e-9.
*/
int checker(const uint8_t* input, const double* output, int totalPixels) {
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

/*
	Function that holds the main conversion program. Asks user to input width and height, 
	then w * h pixel values [0, 255]. Outputs the resulting pixel array. Returns 0 if no errors,
	else 1 for input errors.
*/
int convert() {
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
			return 1;
		}
	}

	// call asm func to convert int to double
	imgCvtGrayIntToDouble(input, output, w, h);

	// UNCOMMENT to print - start
	// double check output against expected values
	 if (checker(input, output, totalPixels)) {
	 	printf("CHECK: Output matches expected values.\n");
	 }
	 else {
	 	printf("CHECK: Output does NOT match expected values.\n");
	 }
	// end

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

/*
	Function that creates a w * h pixel array with a generated value [0, 255] and converts
	the array into double using imgCvtGrayIntToDouble(). 
	Returns execution time of the conversion function in processing the w * h pixel array.
*/
double timeConversion(int w, int h) {
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	double elapsed;
	
	int totalPixels = w * h;
	int r = rand() % 256;

	uint8_t* input = malloc(totalPixels * sizeof(uint8_t));
	double* output = malloc(totalPixels * sizeof(double));
	
	// fill entire array with the random value
	for (int i = 0; i < totalPixels; i++) {
		input[i] = (uint8_t)r;
	}

	// sanity check input
	//printf("Input: %hhu\n", input[totalPixels - 1]);

	// time the conversion
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	imgCvtGrayIntToDouble(input, output, w, h);
	QueryPerformanceCounter(&end);

	// calculate elapsed time in miliseconds (change to 1000000.0 for microseconds)
	elapsed = ((double)(end.QuadPart - start.QuadPart) / frequency.QuadPart) * 1000.0;

	// UNCOMMENT to print - start
	// double check output against expected values
	if (checker(input, output, totalPixels)) {
		printf("CHECK: Output matches expected values.\n");
	}
	else {
		printf("CHECK: Output does NOT match expected values.\n");
	}
	 // end

	// UNCOMMENT to print
	// sanity check output
	//printf("\nOutput: %.2f \n", output[totalPixels - 1]);

	// UNCOMMENT to print
	// print elapsed time
	//printf("Elapsed time: %f ms\n", elapsed);

	// free memory
	free(input);
	free(output);

	return elapsed;
}

/*
	Function that benchmarks the conversion function over a number of iterations and returns
	the average elapsed time.
*/
double benchmark(int w, int h, int iterations) {
	double avgTime, totalTime = 0.0;

	// run conversion and accumulate time
	for (int i = 0; i < iterations; i++) {
		totalTime += timeConversion(w, h);
	}
	avgTime = totalTime / iterations;
	
	// UNCOMMENT to print
	//printf("%dx%d: Average execution time over %d iterations: %.4f microseconds\n",w, h, iterations, avgTime);
	return avgTime;
}

/*
	Function that runs the benchmark suite for the three required cases:
	10x10, 100x100, and 1000x1000. Prints the average elapsed time for each case.
 */
void benchmarkSuite(int iterations) {
	double case10 = benchmark(10, 10, iterations);
	double case100 = benchmark(100, 100, iterations);
	double case1000 = benchmark(1000, 1000, iterations);

	// print summary
	printf("\nAvg. Time Summary (%d Iterations):\n", iterations);
    printf("10x10:     %.4f ms\n", case10);
    printf("100x100:   %.4f ms\n", case100);
    printf("1000x1000: %.4f ms\n", case1000);
}

int main(int argc, char* argv[]) {

	// init random seed w time ONCE
	srand(time(NULL));

	// time a single instance of conversion
	//timeConversion(1000, 1000);

	// time the avg of multiple conversions of same dimensions
	//benchmark(10, 10, 30);

	// time the avg for each of three cases
	//benchmarkSuite(30);

	// conversion via user input
	return convert();
	//return 0;
}