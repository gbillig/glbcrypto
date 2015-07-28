/*
 ============================================================================
 Name        : sha.h
 Author      : glb
 License     : MIT
 ============================================================================
 */

#ifndef INC_SHA_H_
#define INC_SHA_H_

void SHA_test();
uint32_t* SHA256(uint8_t* message, uint64_t length);
uint32_t* SHA256_padding(uint8_t* message, uint64_t length, int k);

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);
uint32_t bit_rotation(uint32_t x, int shift);
uint32_t bit_shift(uint32_t x, int shift);

uint32_t big_sigma0(uint32_t x);
uint32_t big_sigma1(uint32_t x);
uint32_t small_sigma0(uint32_t x);
uint32_t small_sigma1(uint32_t x);

uint32_t* expanded_message(uint32_t* M);

#endif /* INC_SHA_H_ */
