#include <stdio.h>
#include <stdlib.h>
int main() {
	void *ptr1 = malloc(100); // Allocate 100 bytes
	void *ptr2 = malloc(200); // Allocate 200 bytes
	void *ptr3 = malloc(100); // Allocate 100 bytes
	free(ptr2); // Free the middle block
	void *ptr4 = malloc(250); // Attempt to allocate 250 bytes (may fail due to fragmentation)
	if (ptr4 == NULL) {
		printf("Memory allocation failed due to fragmentation.\n");
	}
	free(ptr1);
	free(ptr3);
	return 0;
}