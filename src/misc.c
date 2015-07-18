/*
 ============================================================================
 Name        : misc.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>

void print_value(int* value, int length) {
	int i;

	for (i=0; i<length; i++) {
		printf("%02x ", value[i]);
	}
	printf("\n");
}
