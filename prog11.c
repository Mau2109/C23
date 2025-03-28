#include <stdio.h>
#include <stdlib.h>
int main() {
	int *array = (int *)malloc(10 * sizeof(int));
	if (array == NULL) {
		perror("Memory allocation failed");
		return 1;
	}
	// Accessing memory beyond the allocated range (undefined behavior)
	array[10] = 42; // This may cause a segmentation fault
	free(array);
	return 0;
}