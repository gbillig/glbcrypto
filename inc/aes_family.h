/*
 * aes_family.h
 *
 *  Created on: Apr 10, 2016
 *      Author: glb
 */

#ifndef INC_AES_FAMILY_H_
#define INC_AES_FAMILY_H_

uint8_t aes(uint8_t* output_msg, uint8_t* input_msg, uint8_t key[], int key_size, int mode);

uint8_t aes_128(uint8_t* output_msg, uint8_t* input_msg, uint8_t key[], int key_size, int mode);
uint8_t aes_192(uint8_t* output_msg, uint8_t* input_msg, uint8_t key[], int key_size, int mode);
uint8_t aes_256(uint8_t* output_msg, uint8_t* input_msg, uint8_t key[], int key_size, int mode);

#endif /* INC_AES_FAMILY_H_ */
