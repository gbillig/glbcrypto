/*
 ============================================================================
 Name        : misc.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include "../inc/misc.h"

void print_value(uint8_t* value, int length) {
	int i;

	for (i=0; i<length; i++) {
		printf("%02x ", value[i]);
	}
	printf("\n");
}
