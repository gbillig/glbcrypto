/*
 * mode_of_operation.h
 *
 *  Created on: Apr 11, 2016
 *      Author: glb
 */

#ifndef INC_MODE_OF_OPERATION_H_
#define INC_MODE_OF_OPERATION_H_

#ifdef __cplusplus
extern "C" {
#endif

int cbc(uint8_t* output_msg, uint8_t* input_msg, int msg_size, uint8_t* iv, uint8_t* key, int key_size, int mode,
		int (*cipher)(uint8_t*, uint8_t*, uint8_t*, int, int));

#ifdef __cplusplus
}
#endif

#endif /* INC_MODE_OF_OPERATION_H_ */
