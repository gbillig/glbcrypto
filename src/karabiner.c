/*
 ============================================================================
 Name        : karabiner.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../inc/aes_cipher.h"
#include "../inc/aes_family.h"
#include "../inc/sha.h"
#include "../inc/misc.h"

int main(void) {
	printf("karabiner - a simple password manager\n\n");


	setvbuf(stdout, NULL, _IONBF, 0);

	int i, rval;

	//
	//functionTests();


	//-------------------
	//INPUT KEY FROM FILE
	//-------------------

	/*
	int key_size;
	FILE * keyFile;

	keyFile = fopen("key.txt", "r");

	printf("Please enter a key size: \n");
	scanf("%d", &key_size);
	int key[key_size];

	for (i=0; i<key_size; i++) {
		fscanf(keyFile, "%x", &key[i]);
		printf("%0x\n", key[i]);
	}
	fclose(keyFile);
	for (i=0; i<key_size; i++) {
		printf("%0x", key[i]);
	}
	printf("\n");
	*/


	uint8_t* p;

	uint8_t* key = malloc(sizeof(uint8_t) * 16);
	key = (uint8_t[16]) {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};

	uint8_t* plaintext = malloc(sizeof(uint8_t) * 16);
	plaintext = (uint8_t[16]) {0x8c, 0x37, 0x4b, 0xbd, 0x02, 0xd8, 0x3c, 0xc7, 0x4e, 0x89, 0x2e, 0xf7, 0x3e, 0xfe, 0x86, 0xea};

	uint8_t* ciphertext = malloc(sizeof(uint8_t) * 16);

	int key_size = 16;
	int expanded_key_size = (key_size + 28) * 4;

	printf("\nOriginal plaintext is:\n");
	print_value_8(plaintext, 16);
	printf("\n");

	rval = aes_128(ciphertext, plaintext, key, key_size, 0);
	if (rval == 1) {
		return 1;
	}

	printf("Ciphertext is:\n");
	print_value_8(ciphertext, 16);
	printf("\n");


	int cipher[16] = {0x24, 0x09, 0x80, 0xea, 0x06, 0x4c, 0x51, 0xd3, 0x77, 0x11, 0xd5, 0x81, 0xa2, 0x3e, 0xbc, 0x83};

	rval = aes_128(plaintext, ciphertext, key, key_size, 1);
	if (rval == 1) {
		return 1;
	}

	printf("Decrypted plaintext is:\n");
	print_value_8(plaintext, 16);
	printf("\n");

	sha_testcase();

	return 0;
}
