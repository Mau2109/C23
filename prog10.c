#include <stdio.h>
#include <stdlib.h>
int main() {
	int *array = (int *)malloc(10 * sizeof(int)); // Allocate memory for 10 integers
	if (array == NULL) {
		perror("Memory allocation failed");
		return 1;
	}
	for (int i = 0; i < 10; i++) {
		array[i] = i * 10; // Initialize the array
	}
	for (int i = 0; i < 10; i++) {
		printf("%d ", array[i]); // Print the array
	}
	printf("\n");
	free(array); // Deallocate memory
	return 0;
}