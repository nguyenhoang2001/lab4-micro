/*
 * function_confic.c
 *
 *  Created on: Nov 5, 2021
 *      Author: MSI-
 */
#include "main.h"
#include "function_config.h"

static unsigned char Error_code_G = 0;
static unsigned int smallest_time_index = 0;
typedef unsigned char tByte;
#define ERROR_SCH_CANNOT_DELETE_TASK	1
#define ERROR_SCH_TOO_MANY_TASKS		2
#define RETURN_ERROR					1
#define RETURN_NORMAL					0


void SCH_Report_Status ( void ) {
#ifdef SCH_REPORT_ERRORS
	// ONLY APPLIES IF WE ARE REPORTING ERRORS
	// Check for a new error code
	if ( Error_code_G != Last_error_code_G ) {
		// Negative logic on LEDs assumed
		Error_port = 255 − Error_code_G ;
		Last_error_code_G = Error_code_G ;
		if( Error_code_G != 0 ) {
			Error_tick_count_G = 60000;
		} else {
			Error_tick_count_G = 0;
		}
	} else{
		if ( Error_tick_count_G != 0 ) {
			if (--Error_tick_count_G == 0 ) {
			Error_code_G = 0; // Reset error code
			}
		}
	}
#endif
}

typedef struct {
	 // Pointer to the task (must be a ’ void ( void ) ’ function )
	 void ( * pTask ) ( void ) ;
	 // Delay ( ticks ) until the function will ( next ) be run
	 uint32_t Delay ;
	 uint32_t saveDelay;
	 // Interval (ticks) between subsequent runs .
	 uint32_t Period ;
	 // Incremented (by scheduler) when task i s due to execute
	 uint8_t RunMe;
	 //This i s a hint to solve the question below .
	 uint32_t TaskID ;
}sTask;

// MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCH_MAX_TASKS 	100
#define NO_TASK_ID 		0
sTask SCH_tasks_G[SCH_MAX_TASKS];

unsigned char SCH_Delete_Task ( const tByte TASK_INDEX) {
	unsigned char Return_code;
	if( SCH_tasks_G [TASK_INDEX].pTask == 0 ) {
		// No task at this location...
		//
		// Set the global error variable
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;

		// . . . also return an error code
		Return_code = RETURN_ERROR;
	} else {
		Return_code = RETURN_NORMAL;
	}
	SCH_tasks_G [TASK_INDEX].pTask = 0x0000 ;
	SCH_tasks_G [TASK_INDEX].Delay = 0;
	SCH_tasks_G [TASK_INDEX].saveDelay = 0;
	SCH_tasks_G [TASK_INDEX].Period = 0;
	SCH_tasks_G [TASK_INDEX].RunMe = 0;
	return Return_code ; // return status
}

//void Timer_init() {
//	HAL_TIM_Base_Start_IT(&htim2);
//}

void SCH_Init ( void ) {
	unsigned char i ;
	for( i = 0; i < SCH_MAX_TASKS; i ++) {
		SCH_Delete_Task(i) ;
	}
	// Reset the global error variable
	// − SCH_Delete_Task () will generate an error code ,
	// (because the task array is empty)
	Error_code_G = 0;
	//Timer_init() ;
	//Watchdog_init ( ) ;
}

void Update_Delay_whenCome_zero(int Index) {
	SCH_tasks_G[Index].RunMe = 1;
	if(SCH_tasks_G[Index].Period) {
		SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
		SCH_tasks_G[Index].saveDelay = SCH_tasks_G[Index].Period;
	}
}

void SCH_Update ( void ) {
	unsigned int Index = smallest_time_index;
	if(SCH_tasks_G[Index].pTask) {
		if(SCH_tasks_G[Index].Delay == 0) {
			if(SCH_tasks_G[Index].RunMe == 0) {
				SCH_tasks_G[Index].RunMe = 1;
				if(SCH_tasks_G[Index].Period) {
					SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
					SCH_tasks_G[Index].saveDelay = SCH_tasks_G[Index].Period;
				}
			}
		}
		else {
			SCH_tasks_G[Index].Delay -= 1;
		}
	}

}

void Update_when_smallest_Run(void) {
	unsigned int Index = smallest_time_index;
	if(SCH_tasks_G[Index].pTask) {
		if(SCH_tasks_G[Index].RunMe == 1) {
			int count = 0;
			int smallest_index = 0;
			int smallest_delay = 0;
			if(SCH_tasks_G[Index].Delay > 0) {
				smallest_delay = SCH_tasks_G[Index].Delay;
				smallest_index = Index;
				count += 1;
			}
			for(int i = 0; i < SCH_MAX_TASKS; i++) {
				if(i == Index)
					continue;
				if(SCH_tasks_G[i].pTask) {
					SCH_tasks_G[i].Delay -= SCH_tasks_G[Index].saveDelay;

					if(SCH_tasks_G[i].Delay == 0) {
						Update_Delay_whenCome_zero(i);
					}

					if(count == 0) {
						smallest_delay = SCH_tasks_G[i].Delay;
						smallest_index = i;
						count += 1;
					}
					else {
						if(smallest_delay > SCH_tasks_G[i].Delay) {
							smallest_delay = SCH_tasks_G[i].Delay;
							smallest_index = i;
						}
					}
				}
			}
			smallest_time_index = (unsigned int)smallest_index;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim ) {
	SCH_Update();
	Update_when_smallest_Run();
}

unsigned char SCH_Add_Task( void(*pFunction)() , unsigned int DELAY,unsigned int PERIOD)
{
	unsigned char Index = 0;
	// First find a gap in the array ( if there is one)
	while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
	{
		Index++;
	}
	// Have we reached the end of the list?
	if (Index == SCH_MAX_TASKS)
	{
		// Task list is full
		// Set the global error variable
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
		// Also return an error code
		return SCH_MAX_TASKS;
	}
	// If we’re here , there is a space in the task array
	SCH_tasks_G[Index].pTask = pFunction ;
	SCH_tasks_G[Index].Delay = DELAY;
	SCH_tasks_G[Index].saveDelay = DELAY;
	SCH_tasks_G[Index].Period = PERIOD;
	SCH_tasks_G[Index].RunMe = 0;

	// Find the smallest delay
	int smallest_delay = DELAY;
	int smallest_index = Index;

	for(int i = 0; i < SCH_MAX_TASKS; i++) {
		if(SCH_tasks_G[i].pTask) {
			if(smallest_delay > SCH_tasks_G[i].Delay) {
				smallest_delay = SCH_tasks_G[i].Delay;
				smallest_index = i;
			}
		}
	}
	smallest_time_index = smallest_index;
	//return position of task ( to allow later deletion )
	return Index;
}

void SCH_Dispatch_Tasks ( void )
{
	unsigned char Index ;
	// Dispatches (runs ) the next task ( i f one i s ready )
	for ( Index = 0; Index < SCH_MAX_TASKS; Index++) {
		if ( SCH_tasks_G[Index].RunMe > 0 ) {
			(*SCH_tasks_G[Index].pTask )(); // Run the task
			SCH_tasks_G[Index].RunMe = 0; // Reset / reduce RunMe flag
			// Periodic tasks will automatically run again
			// − if this is a ’one shot ’ task , remove it from the array
			if(SCH_tasks_G[Index].Period == 0 )
			{
				SCH_Delete_Task(Index) ;
			}
		}
	}
	// Report system status
	//SCH_Report_Status();
	// The scheduler enters idle mode at this point
	//SCH_Go_To_Sleep() ;
}

