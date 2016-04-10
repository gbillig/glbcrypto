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
#include "../inc/sha.h"
#include "../inc/misc.h"

int main(void) {
	printf("karabiner - a simple password manager\n\n\n");


	setvbuf(stdout, NULL, _IONBF, 0);

	int i;

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


	uint8_t *p;


	//key expansion
	uint8_t key0[16] = {0x74, 0x64, 0xe1, 0x5d, 0x0f, 0xfe, 0x22, 0x41, 0x7c, 0xaa, 0xac, 0x81, 0x9e, 0x2f, 0xec, 0x01};
	uint8_t key1[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
	uint8_t key[16] = {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
	uint8_t expanded_key[240];

	int key_size = 16;
	int expanded_key_size = (key_size + 28) * 4;
	p = aes_expand_key(key_size, key);

	for (i=0; i<expanded_key_size; i++) {
		expanded_key[i] = *(p+i);
	}


	uint8_t state[16] = {0x8c, 0x37, 0x4b, 0xbd, 0x02, 0xd8, 0x3c, 0xc7, 0x4e, 0x89, 0x2e, 0xf7, 0x3e, 0xfe, 0x86, 0xea};

	printf("\nOriginal plaintext is:\n");
	print_value_8(state, 16);
	printf("\n");

	p = aes_encrypt(state, expanded_key, key_size);
	for (i=0; i<16; i++) {
		state[i] = *(p+i);
	}

	printf("Ciphertext is:\n");
	print_value_8(state, 16);
	printf("\n");


	int cipher[16] = {0x24, 0x09, 0x80, 0xea, 0x06, 0x4c, 0x51, 0xd3, 0x77, 0x11, 0xd5, 0x81, 0xa2, 0x3e, 0xbc, 0x83};

	p = aes_decrypt(state, expanded_key, key_size);
	for (i=0; i<16; i++) {
		state[i] = *(p+i);
	}

	printf("Decrypted plaintext is:\n");
	print_value_8(state, 16);
	printf("\n");

	sha_testcase();

	return 0;
}
