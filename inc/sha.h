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
int SHA256(uint8_t* message, uint64_t length);
uint32_t* SHA256_padding(uint8_t* message, uint64_t length);

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);
uint32_t bit_rotation(uint32_t x, int shift);
uint32_t bit_shift(uint32_t x, int shift);



#endif /* INC_SHA_H_ */
