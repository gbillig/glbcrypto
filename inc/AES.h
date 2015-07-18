/*
 ============================================================================
 Name        : AES.h
 Author      : glb
 License     : MIT
 ============================================================================
 */

#ifndef AES_H_
#define AES_H_

int * rotWord(int input[], int shiftIndex);
int * subWord(int input[], int arrLength, int cryptoMode);
int * rcon(int input);
int * ek(int expandedKey[], int offset);
int * k(int key[], int offset);
int * shiftRow(int input[], int cryptoMode);
int * mixColumn(int input[], int mMatrix[]);

int functionTests(void);

int * expandKey(int keySize, int key[]);
int * encrypt(int state[], int expandedKey[], int keySize);
int * decrypt(int state[], int expandedKey[], int keySize);

#endif /* AES_H_ */
