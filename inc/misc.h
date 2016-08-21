/*
 ============================================================================
 Name        : misc.h
 Author      : glb
 License     : MIT
 ============================================================================
 */

#ifndef INC_MISC_H_
#define INC_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

void print_value_8(uint8_t* value, int length);
void print_value_32(uint32_t* value, int length);
void print_value_64(uint64_t* value, int length);
void copy_array_8(uint8_t* src, uint8_t* dst, int length);
void copy_array_32(uint32_t* src, uint32_t* dst, int length);

void get_random(uint8_t* output, int size);

#ifdef __cplusplus
}
#endif

#endif /* INC_MISC_H_ */
