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

#include "../inc/aes.h"
#include "../inc/aes_cipher.h"
#include "../inc/sha.h"
#include "../inc/misc.h"

int main(void) {
	printf("karabiner - a simple password manager\n\n");


	setvbuf(stdout, NULL, _IONBF, 0);

	int rval;

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

	uint8_t* key = malloc(sizeof(uint8_t) * 16);
	key = (uint8_t[16]) {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
	key = (uint8_t[16]) {0x56, 0xe4, 0x7a, 0x38, 0xc5, 0x59, 0x89, 0x74, 0xbc, 0x46, 0x90, 0x3d, 0xba, 0x29, 0x03, 0x49};

	uint8_t* iv = malloc(sizeof(uint8_t) * 16);
	iv = (uint8_t[16]) {0x8c, 0xe8, 0x2e, 0xef, 0xbe, 0xa0, 0xda, 0x3c, 0x44, 0x69, 0x9e, 0xd7, 0xdb, 0x51, 0xb7, 0xd9};

	uint8_t* plaintext = malloc(sizeof(uint8_t) * 16);
	plaintext = (uint8_t[16]) {0x8c, 0x37, 0x4b, 0xbd, 0x02, 0xd8, 0x3c, 0xc7, 0x4e, 0x89, 0x2e, 0xf7, 0x3e, 0xfe, 0x86, 0xea};
	plaintext = (uint8_t[16]) {0xcf, 0xfb, 0x32, 0xb9, 0x5e, 0xf4, 0x6f, 0x01, 0x4c, 0x12, 0xea, 0xb5, 0x7c, 0x22, 0x40, 0x81};

	uint8_t* ciphertext = malloc(sizeof(uint8_t) * 16);

	int key_size = 16;
	int msg_size = 16;

	printf("\nOriginal plaintext is:\n");
	print_value_8(plaintext, msg_size);
	printf("\n");

	rval = aes_128(ciphertext, plaintext, key, key_size, 0);
	if (rval == 1) {
		return 1;
	}

	printf("Ciphertext is:\n");
	print_value_8(ciphertext, msg_size);
	printf("\n");

	rval = aes_128(plaintext, ciphertext, key, key_size, 1);
	if (rval == 1) {
		return 1;
	}

	printf("Decrypted plaintext is:\n");
	print_value_8(plaintext, msg_size);
	printf("\n");

	free(ciphertext);

	//---------------

	key = (uint8_t[32]) {
		0x51, 0x48, 0xe5, 0xfd, 0x8e, 0x2f, 0xc5, 0x9b, 0x13, 0xdc, 0xe6, 0xda, 0xa7, 0x74, 0xf5, 0xa3,
		0x93, 0x2a, 0x30, 0x15, 0xce, 0xb3, 0xba, 0x70, 0x41, 0x43, 0x76, 0x24, 0xa7, 0xf2, 0x59, 0x9c
	};

	key_size = 32;

	plaintext = (uint8_t[64]) {
		0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
		0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
		0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
		0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf
	};

	ciphertext = malloc(sizeof(uint8_t) * 64);

	msg_size = 64;

	printf("\nTest 256\n");
	rval = aes_256(ciphertext, plaintext, key, key_size, 0);
	print_value_8(ciphertext, 8);


	printf("\nOriginal plaintext is:\n");
	print_value_8(plaintext, msg_size);
	printf("\n");

	aes_256_cbc(ciphertext, plaintext, msg_size, iv, key, key_size, 0);

	printf("Ciphertext is:\n");
	print_value_8(ciphertext, msg_size);
	printf("\n");

	aes_256_cbc(plaintext, ciphertext, msg_size, iv, key, key_size, 1);

	printf("Decrypted plaintext is:\n");
	print_value_8(plaintext, msg_size);
	printf("\n");

	free(ciphertext);

	//---------------

	sha_testcase();

	return 0;
}
