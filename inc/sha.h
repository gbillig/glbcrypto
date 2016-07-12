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
uint32_t* sha_256_hash(uint8_t* message, uint64_t length);
uint64_t* sha_512_hash(uint8_t* message, uint64_t length);

uint32_t* sha_256_pad_message(uint8_t* message, uint64_t length, int k);
uint64_t* sha_512_pad_message(uint8_t* message, uint64_t length, int k);

uint32_t Ch_32(uint32_t x, uint32_t y, uint32_t z);
uint64_t Ch_64(uint64_t x, uint64_t y, uint64_t z);
uint32_t Maj_32(uint32_t x, uint32_t y, uint32_t z);
uint64_t Maj_64(uint64_t x, uint64_t y, uint64_t z);

uint32_t bit_rotation_32(uint32_t x, int shift);
uint64_t bit_rotation_64(uint64_t x, int shift);
uint32_t bit_shift_32(uint32_t x, int shift);
uint64_t bit_shift_64(uint64_t x, int shift);

uint32_t big_sigma0_32(uint32_t x);
uint64_t big_sigma0_64(uint64_t x);
uint32_t big_sigma1_32(uint32_t x);
uint64_t big_sigma1_64(uint64_t x);
uint32_t small_sigma0_32(uint32_t x);
uint64_t small_sigma0_64(uint64_t x);
uint32_t small_sigma1_32(uint32_t x);
uint64_t small_sigma1_64(uint64_t x);

uint32_t* expanded_message_32(uint32_t* M);
uint64_t* expanded_message_64(uint64_t* M);

#endif /* INC_SHA_H_ */
