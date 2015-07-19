/*
 ============================================================================
 Name        : AES.c
 Author      : glb
 License     : MIT
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#include "../inc/AES.h"
#include "../inc/misc.h"


//shiftIndex: positive is to the left
int * rotWord(int input[], int shift_index) {

	static int new_array[4];
	int i, j;
	for (i=0; i<4; i++) {
		j = (i + shift_index + 4) % 4;
		new_array[i] = input[j];
	}
	return new_array;
};

int * subWord(int input[], int arr_length, int crypto_mode) {
	unsigned char S[256] =
	{
	   0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	   0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	   0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	   0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	   0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	   0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	   0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	   0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	   0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	   0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	   0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	   0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	   0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	   0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	   0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	   0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
	};

	unsigned char inv_S[256] =
	{
	   0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
	   0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
	   0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
	   0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
	   0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
	   0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
	   0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
	   0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
	   0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
	   0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
	   0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
	   0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
	   0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
	   0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
	   0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
	   0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
	};

	static int new_array[16];
	int i;

	for (i=0; i<arr_length; i++) {
		if (crypto_mode) {
			new_array[i] = S[input[i]];
		} else {
			new_array[i] = inv_S[input[i]];
		}
	}

	return new_array;
}

int * rcon(int input) {
	static int new_array[4] = {0x00, 0x00, 0x00, 0x00};

	switch(input) {
	case 0 :
		new_array[0] = 0x01;
		break;
	case 1 :
		new_array[0] = 0x02;
		break;
	case 2 :
		new_array[0] = 0x04;
		break;
	case 3 :
		new_array[0] = 0x08;
		break;
	case 4 :
		new_array[0] = 0x10;
		break;
	case 5 :
		new_array[0] = 0x20;
		break;
	case 6 :
		new_array[0] = 0x40;
		break;
	case 7 :
		new_array[0] = 0x80;
		break;
	case 8 :
		new_array[0] = 0x1B;
		break;
	case 9 :
		new_array[0] = 0x36;
		break;
	case 10 :
		new_array[0] = 0x6C;
		break;
	case 11 :
		new_array[0] = 0xD8;
		break;
	case 12 :
		new_array[0] = 0xAB;
		break;
	case 13 :
		new_array[0] = 0x4D;
		break;
	case 14 :
		new_array[0] = 0x9A;
		break;
	}

	return new_array;
};

int * ek(int expanded_key[], int offset) {
	static int new_array[4];
	int i;

	for (i=0; i<4; i++){
		new_array[i] = expanded_key[offset + i];
	}

	return new_array;
};

int * k(int key[], int offset) {
	static int new_array[4];
	int i;

	for (i=0; i<4; i++) {
		new_array[i] = key[offset + i];
	}

	return new_array;
}

int * shiftRow(int input[], int crypto_mode) {
	static int new_array[16];
	static int row0[4];
	static int row1[4];
	static int row2[4];
	static int row3[4];
	int i;
	int *t;

	int shift_direction;

	if (crypto_mode) {
		shift_direction = 1;
	} else {
		shift_direction = -1;
	}


	for (i=0; i<4; i++) {
		row0[i] = input[(i*4)+0];
		row1[i] = input[(i*4)+1];
		row2[i] = input[(i*4)+2];
		row3[i] = input[(i*4)+3];
	}

	t = rotWord(row1, 1*shift_direction);
	for (i=0; i<4; i++) {
		row1[i] = *(t+i);
	}
	t = rotWord(row2, 2*shift_direction);
	for (i=0; i<4; i++) {
		row2[i] = *(t+i);
	}
	t = rotWord(row3, 3*shift_direction);
	for (i=0; i<4; i++) {
		row3[i] = *(t+i);
	}

	for (i=0; i<4; i++) {
		new_array[(i*4)+0] = row0[i];
		new_array[(i*4)+1] = row1[i];
		new_array[(i*4)+2] = row2[i];
		new_array[(i*4)+3] = row3[i];
	}

	return new_array;
}

int * mixColumn(int input[], int m_matrix[]) {
	unsigned char E[256] =
	{
			0x01, 0x03, 0x05, 0x0F, 0x11, 0x33, 0x55, 0xFF, 0x1A, 0x2E, 0x72, 0x96, 0xA1, 0xF8, 0x13, 0x35,
			0x5F, 0xE1, 0x38, 0x48, 0xD8, 0x73, 0x95, 0xA4, 0xF7, 0x02, 0x06, 0x0A, 0x1E, 0x22, 0x66, 0xAA,
			0xE5, 0x34, 0x5C, 0xE4, 0x37, 0x59, 0xEB, 0x26, 0x6A, 0xBE, 0xD9, 0x70, 0x90, 0xAB, 0xE6, 0x31,
			0x53, 0xF5, 0x04, 0x0C, 0x14, 0x3C, 0x44, 0xCC, 0x4F, 0xD1, 0x68, 0xB8, 0xD3, 0x6E, 0xB2, 0xCD,
			0x4C, 0xD4, 0x67, 0xA9, 0xE0, 0x3B, 0x4D, 0xD7, 0x62, 0xA6, 0xF1, 0x08, 0x18, 0x28, 0x78, 0x88,
			0x83, 0x9E, 0xB9, 0xD0, 0x6B, 0xBD, 0xDC, 0x7F, 0x81, 0x98, 0xB3, 0xCE, 0x49, 0xDB, 0x76, 0x9A,
			0xB5, 0xC4, 0x57, 0xF9, 0x10, 0x30, 0x50, 0xF0, 0x0B, 0x1D, 0x27, 0x69, 0xBB, 0xD6, 0x61, 0xA3,
			0xFE, 0x19, 0x2B, 0x7D, 0x87, 0x92, 0xAD, 0xEC, 0x2F, 0x71, 0x93, 0xAE, 0xE9, 0x20, 0x60, 0xA0,
			0xFB, 0x16, 0x3A, 0x4E, 0xD2, 0x6D, 0xB7, 0xC2, 0x5D, 0xE7, 0x32, 0x56, 0xFA, 0x15, 0x3F, 0x41,
			0xC3, 0x5E, 0xE2, 0x3D, 0x47, 0xC9, 0x40, 0xC0, 0x5B, 0xED, 0x2C, 0x74, 0x9C, 0xBF, 0xDA, 0x75,
			0x9F, 0xBA, 0xD5, 0x64, 0xAC, 0xEF, 0x2A, 0x7E, 0x82, 0x9D, 0xBC, 0xDF, 0x7A, 0x8E, 0x89, 0x80,
			0x9B, 0xB6, 0xC1, 0x58, 0xE8, 0x23, 0x65, 0xAF, 0xEA, 0x25, 0x6F, 0xB1, 0xC8, 0x43, 0xC5, 0x54,
			0xFC, 0x1F, 0x21, 0x63, 0xA5, 0xF4, 0x07, 0x09, 0x1B, 0x2D, 0x77, 0x99, 0xB0, 0xCB, 0x46, 0xCA,
			0x45, 0xCF, 0x4A, 0xDE, 0x79, 0x8B, 0x86, 0x91, 0xA8, 0xE3, 0x3E, 0x42, 0xC6, 0x51, 0xF3, 0x0E,
			0x12, 0x36, 0x5A, 0xEE, 0x29, 0x7B, 0x8D, 0x8C, 0x8F, 0x8A, 0x85, 0x94, 0xA7, 0xF2, 0x0D, 0x17,
			0x39, 0x4B, 0xDD, 0x7C, 0x84, 0x97, 0xA2, 0xFD, 0x1C, 0x24, 0x6C, 0xB4, 0xC7, 0x52, 0xF6, 0x01
	};

	unsigned char L[256] =
	{
			0x00, 0x00, 0x19, 0x01, 0x32, 0x02, 0x1A, 0xC6, 0x4B, 0xC7, 0x1B, 0x68, 0x33, 0xEE, 0xDF, 0x03,
			0x64, 0x04, 0xE0, 0x0E, 0x34, 0x8D, 0x81, 0xEF, 0x4C, 0x71, 0x08, 0xC8, 0xF8, 0x69, 0x1C, 0xC1,
			0x7D, 0xC2, 0x1D, 0xB5, 0xF9, 0xB9, 0x27, 0x6A, 0x4D, 0xE4, 0xA6, 0x72, 0x9A, 0xC9, 0x09, 0x78,
			0x65, 0x2F, 0x8A, 0x05, 0x21, 0x0F, 0xE1, 0x24, 0x12, 0xF0, 0x82, 0x45, 0x35, 0x93, 0xDA, 0x8E,
			0x96, 0x8F, 0xDB, 0xBD, 0x36, 0xD0, 0xCE, 0x94, 0x13, 0x5C, 0xD2, 0xF1, 0x40, 0x46, 0x83, 0x38,
			0x66, 0xDD, 0xFD, 0x30, 0xBF, 0x06, 0x8B, 0x62, 0xB3, 0x25, 0xE2, 0x98, 0x22, 0x88, 0x91, 0x10,
			0x7E, 0x6E, 0x48, 0xC3, 0xA3, 0xB6, 0x1E, 0x42, 0x3A, 0x6B, 0x28, 0x54, 0xFA, 0x85, 0x3D, 0xBA,
			0x2B, 0x79, 0x0A, 0x15, 0x9B, 0x9F, 0x5E, 0xCA, 0x4E, 0xD4, 0xAC, 0xE5, 0xF3, 0x73, 0xA7, 0x57,
			0xAF, 0x58, 0xA8, 0x50, 0xF4, 0xEA, 0xD6, 0x74, 0x4F, 0xAE, 0xE9, 0xD5, 0xE7, 0xE6, 0xAD, 0xE8,
			0x2C, 0xD7, 0x75, 0x7A, 0xEB, 0x16, 0x0B, 0xF5, 0x59, 0xCB, 0x5F, 0xB0, 0x9C, 0xA9, 0x51, 0xA0,
			0x7F, 0x0C, 0xF6, 0x6F, 0x17, 0xC4, 0x49, 0xEC, 0xD8, 0x43, 0x1F, 0x2D, 0xA4, 0x76, 0x7B, 0xB7,
			0xCC, 0xBB, 0x3E, 0x5A, 0xFB, 0x60, 0xB1, 0x86, 0x3B, 0x52, 0xA1, 0x6C, 0xAA, 0x55, 0x29, 0x9D,
			0x97, 0xB2, 0x87, 0x90, 0x61, 0xBE, 0xDC, 0xFC, 0xBC, 0x95, 0xCF, 0xCD, 0x37, 0x3F, 0x5B, 0xD1,
			0x53, 0x39, 0x84, 0x3C, 0x41, 0xA2, 0x6D, 0x47, 0x14, 0x2A, 0x9E, 0x5D, 0x56, 0xF2, 0xD3, 0xAB,
			0x44, 0x11, 0x92, 0xD9, 0x23, 0x20, 0x2E, 0x89, 0xB4, 0x7C, 0xB8, 0x26, 0x77, 0x99, 0xE3, 0xA5,
			0x67, 0x4A, 0xED, 0xDE, 0xC5, 0x31, 0xFE, 0x18, 0x0D, 0x63, 0x8C, 0x80, 0xC0, 0xF7, 0x70, 0x07
	};

	static int new_array[16];
	int i;
	int column_num, row_num;

	for (i=0;i<16;i++) {
		column_num = i/4;
		row_num = i%4;
		new_array[i] =
				E[ (L[ input[ column_num * 4 + 0]] + L[ m_matrix[ row_num * 4 + 0]]) % 255] ^
				E[ (L[ input[ column_num * 4 + 1]] + L[ m_matrix[ row_num * 4 + 1]]) % 255] ^
				E[ (L[ input[ column_num * 4 + 2]] + L[ m_matrix[ row_num * 4 + 2]]) % 255] ^
				E[ (L[ input[ column_num * 4 + 3]] + L[ m_matrix[ row_num * 4 + 3]]) % 255];

		//printf("b%d = b%d * %d XOR b%d * %d XOR b%d * %d XOR b%d * %d\n", i, columnNum * 4 + 0, rowNum * 4 + 0, columnNum * 4 + 1, rowNum * 4 + 1, columnNum * 4 + 2, rowNum * 4 + 2, columnNum * 4 + 3, rowNum * 4 + 3);
		//printf("E(L(%0x) + L(%0x)) ^ E(L(%0x) + L(%0x)) ^ E(L(%0x) + L(%0x)) ^ E(L(%0x) + L(%0x))\n", input[ columnNum * 4 + 0], mMatrix[ rowNum * 4 + 0], input[ columnNum * 4 + 1], mMatrix[ rowNum * 4 + 1], input[ columnNum * 4 + 2], mMatrix[ rowNum * 4 + 2], input[ columnNum * 4 + 3], mMatrix[ rowNum * 4 + 3]);
		//printf("E(%0x + %0x) ^ E(%0x + %0x) ^ E(%0x + %0x) ^ E(%0x + %0x)\n", L[ input[ columnNum * 4 + 0]], L[ mMatrix[ rowNum * 4 + 0]], L[ input[ columnNum * 4 + 1]], L[ mMatrix[ rowNum * 4 + 1]], L[ input[ columnNum * 4 + 2]], L[ mMatrix[ rowNum * 4 + 2]], L[ input[ columnNum * 4 + 3]], L[ mMatrix[ rowNum * 4 + 3]]);
		//printf("E(%0x) ^ E(%0x) ^ E(%0x) ^ E(%0x)\n", (L[ input[ columnNum * 4 + 0]] + L[ mMatrix[ rowNum * 4 + 0]]) % 255, (L[ input[ columnNum * 4 + 1]] + L[ mMatrix[ rowNum * 4 + 1]]) % 255, (L[ input[ columnNum * 4 + 2]] + L[ mMatrix[ rowNum * 4 + 2]]) % 255, (L[ input[ columnNum * 4 + 3]] + L[ mMatrix[ rowNum * 4 + 3]]) % 255);
		//printf("%0x ^ %0x ^ %0x ^ %0x\n", E[ (L[ input[ columnNum * 4 + 0]] + L[ mMatrix[ rowNum * 4 + 0]]) % 255],	E[ (L[ input[ columnNum * 4 + 1]] + L[ mMatrix[ rowNum * 4 + 1]]) % 255], E[ (L[ input[ columnNum * 4 + 2]] + L[ mMatrix[ rowNum * 4 + 2]]) % 255], E[ (L[ input[ columnNum * 4 + 3]] + L[ mMatrix[ rowNum * 4 + 3]]) % 255]);
		//printf("%0x\n", new_array[i]);
	}

	return new_array;
}

//-----------------------
//TEST FOR BASE FUNCTIONS
//-----------------------

int functionTests(void) {

	int i,j;
	int testArray[4] = {0xAA, 0x45, 0xB9, 0xF0};
	int testState1[16] = {0xdb, 0x13, 0x53, 0x45, 0x0f, 0xfe, 0x22, 0x41, 0x7c, 0xaa, 0xac, 0x81, 0x9e, 0x2f, 0xec, 0x01};
	int testState[16] = {0x1d, 0x0, 0xe8, 0xb2, 0x7b, 0x90, 0x9a, 0xc7, 0xc7, 0xa4, 0x26, 0xee, 0x2, 0x99, 0x9e, 0xa2};
	int resultArray[4];
	int resultState[16];
	int * p;
	int encryptionMultiplicationMatrix[16] = {2, 3, 1, 1, 1, 2, 3, 1, 1, 1, 2, 3, 3, 1, 1, 2};
	int decryptionMultiplicationMatrix[16] = {0x0E, 0x0B, 0x0D, 0x09, 0x09, 0x0E, 0x0B, 0x0D, 0x0D, 0x09, 0x0E, 0x0B, 0x0B, 0x0D, 0x09, 0x0E};
	int oldKey[16] = {0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98};
	int key[16] = {0xdc, 0x90, 0x37, 0xb0, 0x9b, 0x49, 0xdf, 0xe9, 0x97, 0xfe, 0x72, 0x3f, 0x38, 0x81, 0x15, 0xa7};

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

int * expandKey(int key_size, int key[]) {

	int i,j;
	int expansion_rounds = key_size + 28;
	int expanded_key_size = expansion_rounds * 4;
	int small_coef = key_size / 4;
	static int expanded_key[240];
	int *t;
	int t1Array[4];
	int t2Array[4];
	int t3Array[4];

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

int * encrypt(int state[], int expanded_key[], int key_size) {
	int i,j;
	int expanded_key_size = (key_size + 28) * 4;
	int small_coef = key_size / 4;
	int encryption_rounds = small_coef + 6;
	int expanded_key_round = 0;
	int multiplication_matrix[16] = {2, 3, 1, 1, 1, 2, 3, 1, 1, 1, 2, 3, 3, 1, 1, 2};


	int *p;


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

int * decrypt(int state[], int expanded_key[], int key_size) {

	int i,j,k;
	int expanded_key_size = (key_size + 28) * 4;
	int small_coef = key_size / 4;
	int encryption_rounds = small_coef + 6;
	int expanded_key_round = 0;
	int multiplicationMatrix[16] = {0x0E, 0x0B, 0x0D, 0x09, 0x09, 0x0E, 0x0B, 0x0D, 0x0D, 0x09, 0x0E, 0x0B, 0x0B, 0x0D, 0x09, 0x0E};


	int *p;

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
