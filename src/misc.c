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

void print_value_64(uint64_t* value, int length) {
	int i;

	for (i=0; i<length; i++) {
		printf("%016llx ", value[i]);
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

//TODO: Make this cross-platform compatible.
void get_random(uint8_t* output, int size) {
	FILE* random_data = fopen("/dev/urandom", "rb");

	fread(output, sizeof(uint8_t), size, random_data);
	fclose(random_data);
}

// How does a user know how much memory to allocate for the output of the padding?
void pad_message(uint8_t* dst, uint8_t* src, int length, int block_size) {
	int i, k;
	k = block_size - (length % block_size);

	for (i = 0; i < length + k; i++) {
		if (i < length) {
			dst[i] = src[i];
		} else {
			dst[i] = k;
		}
	}
}

void strip_message_pad(uint8_t* dst, uint8_t* src, int length, int block_size) {
	int i, k;
	k = src[length - 1];

	for (i = 0; i < length - k; i++) {
		dst[i] = src[i];
	}
}
