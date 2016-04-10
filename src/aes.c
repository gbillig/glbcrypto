/*
 ============================================================================
 Name        : aes.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "../inc/aes.h"
#include "../inc/misc.h"


//-----------------------
//TEST FOR BASE FUNCTIONS
//-----------------------

int aes_testcase(void) {

	int i;
	uint8_t testArray[4] = {0xAA, 0x45, 0xB9, 0xF0};
	uint8_t testState1[16] = {0xdb, 0x13, 0x53, 0x45, 0x0f, 0xfe, 0x22, 0x41, 0x7c, 0xaa, 0xac, 0x81, 0x9e, 0x2f, 0xec, 0x01};
	uint8_t testState[16] = {0x1d, 0x0, 0xe8, 0xb2, 0x7b, 0x90, 0x9a, 0xc7, 0xc7, 0xa4, 0x26, 0xee, 0x2, 0x99, 0x9e, 0xa2};
	uint8_t resultArray[4];
	uint8_t resultState[16];
	uint8_t * p;
	uint8_t encryptionMultiplicationMatrix[16] = {2, 3, 1, 1, 1, 2, 3, 1, 1, 1, 2, 3, 3, 1, 1, 2};
	uint8_t decryptionMultiplicationMatrix[16] = {0x0E, 0x0B, 0x0D, 0x09, 0x09, 0x0E, 0x0B, 0x0D, 0x0D, 0x09, 0x0E, 0x0B, 0x0B, 0x0D, 0x09, 0x0E};
	uint8_t oldKey[16] = {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
	uint8_t key[16] = {0xdc, 0x90, 0x37, 0xb0, 0x9b, 0x49, 0xdf, 0xe9, 0x97, 0xfe, 0x72, 0x3f, 0x38, 0x81, 0x15, 0xa7};

	for (i=0; i < 16; i++) {
		resultState[i] = testState[i] ^ key[0 * 16 + i];
		printf("%02x ", resultState[i]);
	}
	printf("\n");

	p = mixColumn(testState, decryptionMultiplicationMatrix);
	for (i = 0; i<16; i++) {
		resultState[i] = *(p+i);
		printf("%02x ", resultState[i]);
	}
	printf("\n");

	p = subWord(testState, 16, 0);
	for (i = 0; i<16; i++) {
		resultState[i] = *(p+i);
		printf("%02x ", resultState[i]);
	}
	printf("\n");


	p = shiftRow(testState, 0);
	for (i = 0; i<16; i++) {
		resultState[i] = *(p+i);
		printf("%02x ", resultState[i]);
	}
	printf("\n");

	p = rotWord(testArray, -3);
	for (i = 0; i<4; i++) {
		resultArray[i] = *(p+i);
		printf("%02x ", resultArray[i]);
	}
	printf("\n");


	p = subWord(testArray, 4, 1);
	for (i = 0; i<4; i++) {
		resultArray[i] = *(p+i);
		printf("%02x ", resultArray[i]);
	}
	printf("\n");

	p = rcon(13);
	for (i = 0; i<4; i++) {
		resultArray[i] = *(p+i);
		printf("%02x ", resultArray[i]);
	}

	printf("\n\n\n");

	return 0;
}

//-----------------------
//KEY EXPANSION ALGORITHM
//-----------------------

uint8_t * aes_expand_key(int key_size, uint8_t key[]) {

	int i,j;
	int expansion_rounds = key_size + 28;
	int expanded_key_size = expansion_rounds * 4;
	int small_coef = key_size / 4;
	static uint8_t expanded_key[240];
	uint8_t *t;
	uint8_t t1Array[4];
	uint8_t t2Array[4];
	uint8_t t3Array[4];

	int byte_index;
	for(i=0; i<expansion_rounds; i++) {
		byte_index = i*4;
		if (i<4) {
			t = k(key, byte_index);
			for (j=0; j<4; j++) {
				expanded_key[byte_index + j] = *(t+j);
			}
			//printf("Round %d: K(%d)\n", i, byte_index);
		} else if (i % small_coef == 0) {
			t = ek(expanded_key, (i-1)*4);
			for (j=0; j<4; j++) {
				t1Array[j] = *(t+j);
			}
			t = rotWord(t1Array, 1);
			for (j=0; j<4; j++) {
				t1Array[j] = *(t+j);
			}
			t = subWord(t1Array, 4, 1);
			for (j=0; j<4; j++) {
				t1Array[j] = *(t+j);
			}

			t = rcon(i/small_coef - 1);
			for (j=0; j<4; j++) {
				t2Array[j] = *(t+j);
			}

			t = ek(expanded_key, (i-small_coef)*4);
			for (j=0; j<4; j++) {
				t3Array[j] = *(t+j);
			}

			for (j=0; j<4; j++) {
				expanded_key[byte_index + j] = t1Array[j] ^ t2Array[j] ^ t3Array[j];
			}
			//printf("Round %d: SubWord(RotWord(EK(%d-1)*4)) XOR Rcon((%d/%d)-1) XOR EK((%d-%d)*4)\n", i, i, i, small_coef, i, small_coef);

		} else if (key_size == 32 && (i-4) % 8 == 0) {
			t = ek(expanded_key, (i-1)*4);
			for (j=0; j<4; j++) {
				t1Array[j] = *(t+j);
			}
			t = subWord(t1Array, 4, 1);
			for (j=0; j<4; j++) {
				t1Array[j] = *(t+j);
			}

			t = ek(expanded_key, (i-8)*4);
			for (j=0; j<4; j++) {
				t2Array[j] = *(t+j);
			}

			for (j=0; j<4; j++) {
				expanded_key[byte_index + j] = t1Array[j] ^ t2Array[j];
			}
			//printf("Round %d: SubWord(EK((%d-1)*4)) XOR EK(%d-8)*4\n", i, i, i);
		} else {
			t = ek(expanded_key, (i-1)*4);
			for (j=0; j<4; j++) {
				t1Array[j] = *(t+j);
			}
			t = ek(expanded_key, (i-small_coef)*4);
			for (j=0; j<4; j++) {
				t2Array[j] = *(t+j);
			}

			for (j=0; j<4; j++) {
				expanded_key[byte_index + j] = t1Array[j] ^ t2Array[j];
			}
			//printf("Round %d: EK((%d-1)*4) XOR EK((%d-%d)*4)\n", i, i, i, small_coef);
		}
	}

	printf("Expanded Key:\n");
	for(i=0; i<expanded_key_size; i++) {
		printf("%02x ", expanded_key[i]);
	}
	printf("\n");

	return expanded_key;
}

//----------
//ENCRYPTION
//----------

uint8_t * aes_encrypt(uint8_t state[], uint8_t expanded_key[], int key_size) {
	int i,j;
	int expanded_key_size = (key_size + 28) * 4;
	int small_coef = key_size / 4;
	int encryption_rounds = small_coef + 6;
	int expanded_key_round = 0;
	uint8_t multiplication_matrix[16] = {2, 3, 1, 1, 1, 2, 3, 1, 1, 1, 2, 3, 3, 1, 1, 2};


	uint8_t *p;


	//initial Add Round Key
	for (i=0; i < 16; i++) {
		state[i] = state[i] ^ expanded_key[expanded_key_round * 16 + i];
	}
	expanded_key_round++;

	//print_value(state, 16);

	for (i=0; i < encryption_rounds; i++) {

		p = subWord(state, 16, 1);
		for (j=0; j < 16; j++) {
			state[j] = *(p+j);
		}

		//print_value(state, 16);

		p = shiftRow(state, 1);
		for (j=0; j < 16; j++) {
			state[j] = *(p+j);
		}

		//print_value(state, 16);

		if (i != encryption_rounds - 1) {
			p = mixColumn(state, multiplication_matrix);
			for (j=0; j < 16; j++) {
				state[j] = *(p+j);
			}

			//print_value(state, 16);
		}

		for (j=0; j < 16; j++) {
			state[j] = state[j] ^ expanded_key[expanded_key_round * 16 + j];
		}
		expanded_key_round++;

		//print_value(state, 16);
	}

	return state;
}

//----------
//DECRYPTION
//----------

uint8_t * aes_decrypt(uint8_t state[], uint8_t expanded_key[], int key_size) {

	int i,j;
	int expanded_key_size = (key_size + 28) * 4;
	int small_coef = key_size / 4;
	int encryption_rounds = small_coef + 6;
	int expanded_key_round = 0;
	uint8_t multiplicationMatrix[16] = {0x0E, 0x0B, 0x0D, 0x09, 0x09, 0x0E, 0x0B, 0x0D, 0x0D, 0x09, 0x0E, 0x0B, 0x0B, 0x0D, 0x09, 0x0E};


	uint8_t *p;

	//initial Add Round Key
	for (i=0; i < 16; i++) {
//		printf("%0x XOR %0x\n", state[i], expanded_key[expanded_key_size - (expanded_key_round + 1) * 16 + i]);
		state[i] = state[i] ^ expanded_key[expanded_key_size - (expanded_key_round + 1) * 16 + i];
	}
	expanded_key_round++;

	//print_value(state, 16);

	for (i=0; i < encryption_rounds; i++) {

		p = shiftRow(state, 0);
		for (j=0; j < 16; j++) {
			state[j] = *(p+j);
		}

		//print_value(state, 16);

		p = subWord(state, 16, 0);
		for (j=0; j < 16; j++) {
			state[j] = *(p+j);
		}

		//print_value(state, 16);

		for (j=0; j < 16; j++) {
			state[j] = state[j] ^ expanded_key[expanded_key_size - (expanded_key_round + 1) * 16 + j];
		}
		expanded_key_round++;

		//print_value(state, 16);

		if (i < encryption_rounds - 1) {
			p = mixColumn(state, multiplicationMatrix);
			for (j=0; j < 16; j++) {
				state[j] = *(p+j);
			}

			//print_value(state, 16);
		}
	}

	return state;
}
