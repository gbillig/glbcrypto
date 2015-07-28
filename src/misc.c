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

void print_value_8(uint8_t* value, int length) {
	int i;

	for (i=0; i<length; i++) {
		printf("%02x ", value[i]);
	}
	printf("\n");
}

void print_value_32(uint32_t* value, int length) {
	int i;

	for (i=0; i<length; i++) {
		printf("%08x ", value[i]);
	}
	printf("\n");
}

void copy_array_8(uint8_t* src, uint8_t* dst, int length) {
	int i;
	for (i=0; i<length; i++) {
		dst[i] = src[i];
	}
}

void copy_array_32(uint32_t* src, uint32_t* dst, int length) {
	int i;
	for (i=0; i<length; i++) {
		dst[i] = src[i];
	}
}
