/*
 * aes.h
 *
 *  Created on: Apr 10, 2016
 *      Author: glb
 */

#ifndef INC_AES_H_
#define INC_AES_H_

#ifdef __cplusplus
extern "C" {
#endif

int aes(uint8_t* output_msg, uint8_t* input_msg, uint8_t* key, int key_size, int mode);

int aes_128(uint8_t* output_msg, uint8_t* input_msg, uint8_t* key, int key_size, int mode);
int aes_192(uint8_t* output_msg, uint8_t* input_msg, uint8_t* key, int key_size, int mode);
int aes_256(uint8_t* output_msg, uint8_t* input_msg, uint8_t* key, int key_size, int mode);

int aes_128_cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode);
int aes_192_cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode);
int aes_256_cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode);

int aes_128_ecb(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* key, int key_size, int mode);
int aes_192_ecb(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* key, int key_size, int mode);
int aes_256_ecb(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* key, int key_size, int mode);

#ifdef __cplusplus
}
#endif

#endif /* INC_AES_H_ */
