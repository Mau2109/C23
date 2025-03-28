#include <stdio.h>
#include <stdlib.h>
int main() {
	int *array1 = (int *)malloc(1024 * sizeof(int)); // Allocate 4 KB
	int *array2 = (int *)malloc(2048 * sizeof(int)); // Allocate 8 KB
	if (array1 == NULL || array2 == NULL) {
		perror("Memory allocation failed");
		return 1;
	}
	printf("Array 1: %p\n", (void *)array1);
	printf("Array 2: %p\n", (void *)array2);
	free(array1);
	free(array2);
	return 0;
}