/*
 ============================================================================
 Name        : sha.h
 Author      : glb
 License     : MIT
 ============================================================================
 */

#ifndef INC_SHA_H_
#define INC_SHA_H_

void sha_testcase();
int sha_256(uint8_t* output_msg, uint8_t* input_msg, uint64_t length);
int sha_512(uint8_t* output_msg, uint8_t* input_msg, uint64_t length);

int sha_256_pad_message(uint8_t* padded_msg, uint8_t* input_msg, uint64_t length, int k);
int sha_512_pad_message(uint8_t* padded_msg, uint8_t* input_msg, uint64_t length, int k);

#endif /* INC_SHA_H_ */
