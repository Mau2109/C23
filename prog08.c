#include <stdio.h>
#include <stdlib.h>
int main() {
	int *large_array = (int *)malloc(1024 * 1024 * sizeof(int)); //Allocate 4 MB of memory
	if (large_array == NULL) {
		perror("Memory allocation failed");
		return 1;
	}
	for (int i = 0; i < 1024 * 1024; i++) {
		large_array[i] = i; // Access memory
	}
	free(large_array);
	return 0;
}