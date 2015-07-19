/*
 ============================================================================
 Name        : AES.h
 Author      : glb
 License     : MIT
 ============================================================================
 */

#ifndef AES_H_
#define AES_H_

int * rotWord(int input[], int shift_index);
int * subWord(int input[], int arr_length, int crypto_mode);
int * rcon(int input);
int * ek(int expanded_key[], int offset);
int * k(int key[], int offset);
int * shiftRow(int input[], int crypto_mode);
int * mixColumn(int input[], int m_matrix[]);

int functionTests(void);

int * expandKey(int key_size, int key[]);
int * encrypt(int state[], int expanded_key[], int key_size);
int * decrypt(int state[], int expanded_key[], int key_size);

#endif /* AES_H_ */
