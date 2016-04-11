/*
 * mode_of_operation.h
 *
 *  Created on: Apr 11, 2016
 *      Author: glb
 */

#ifndef INC_MODE_OF_OPERATION_H_
#define INC_MODE_OF_OPERATION_H_

int cbc(uint8_t* output, uint8_t* input, uint8_t* iv, uint8_t* key, int key_size, int mode,
		int (*cipher)(uint8_t*, uint8_t*, uint8_t*, int, int));

#endif /* INC_MODE_OF_OPERATION_H_ */
