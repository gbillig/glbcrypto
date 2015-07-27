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



#endif /* INC_SHA_H_ */
