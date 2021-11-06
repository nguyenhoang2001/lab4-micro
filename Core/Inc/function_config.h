/*
 * function_config.h
 *
 *  Created on: Nov 5, 2021
 *      Author: MSI-
 */

#ifndef INC_FUNCTION_CONFIG_H_
#define INC_FUNCTION_CONFIG_H_

void SCH_Init(void);
unsigned char SCH_Add_Task( void(*pFunction)() , unsigned int DELAY,unsigned int PERIOD);
void SCH_Dispatch_Tasks ( void );

#endif /* INC_FUNCTION_CONFIG_H_ */
