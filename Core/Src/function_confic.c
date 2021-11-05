/*
 * function_confic.c
 *
 *  Created on: Nov 5, 2021
 *      Author: MSI-
 */

static unsigned char Error_code_G = 0;

void SCH_Report_Status ( void ) {
#ifdef SCH_REPORT_ERRORS
	// ONLY APPLIES IF WE ARE REPORTING ERRORS
	// Check for a new error code
	if ( Error_code_G != Last_error_code_G ) {
		// Negative logic on LEDs assumed
		Error_por t = 255 − Error_code_G ;
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
	 // Interval (ticks) between subsequent runs .
	 uint32_t Period ;
	 // Incremented (by scheduler) when task i s due to execute
	 uint8_t RunMe;
	 //This i s a hint to solve the question below .
	 uint32_t TaskID ;
} sTask ;

// MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCH_MAX_TASKS 	40
#define NO_TASK_ID 		0
sTask SCH_tasks_G[SCH_MAX_TASKS ] ;

unsigned char SCH_Delete_Task ( const tByte TASK_INDEX) {
	unsigned char Return_code ;
	if( SCH_tasks_G [TASK_INDEX ].pTask == 0 ) {
		// No task at this location...
		//
		// Set the global error variable
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK

		// . . . also return an error code
		Return_code = RETURN_ERROR;
	} else {
		Return_code = RETURN_NORMAL;
	}
	SCH_tasks_G [TASK_INDEX ] . pTask = 0x0000 ;
	SCH_tasks_G [TASK_INDEX ] . Delay = 0;
	SCH_tasks_G [TASK_INDEX ] . Period = 0;
	SCH_tasks_G [TASK_INDEX ] .RunMe = 0;
	return Return_code ; // return s t a tus
}

void SCH_Init ( void ) {
	unsigned char i ;
	for( i = 0; i < SCH_MAX_TASKS; i ++) {
	 //SCH_Delete_Task ( i ) ;
	}
	// Reset the global e r ro r v a ri abl e
	// − SCH_Delete_Task ( ) will generate an e r ro r code ,
	// ( because the task array i s empty)
	Error_code_G = 0;
	Timer_ini t ( ) ;
	Watchdog_init ( ) ;
}


void SCH_Update ( void ) {
	unsigned char Index ;
	// NOTE: calculations are in *TICKS*( not milliseconds )
	for ( Index = 0; Index < SCH_MAX_TASKS; Index++) {
	// Check i f there i s a task a t t hi s loca tion
		if ( SCH_tasks_G [ Index].pTask ) {
			if ( SCH_tasks_G [ Index ].Delay == 0 ) {
				// The task i s due to run
				// Inc . the ’RunMe’ fl a g
				SCH_tasks_G [ Index ].RunMe += 1;
				if ( SCH_tasks_G [Index].Period ) {
				// Schedule periodic tasks to run again
					SCH_tasks_G [ Index ].Delay = SCH_tasks_G [ Index ].Period ;
				}
			} else {
				// Not ye t ready to run : j u s t decrement the delay
				SCH_tasks_G[ Index].Delay -= 1;
			}
		}
	}
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim ) {
	SCH_Update();
}

unsigned char SCH_Add_Task( void(*pFunction)() , unsigned int DELAY,unsigned int PERIOD)
{
	unsigned char Index = 0;
	// First find a gap in the array ( if there is one)
	while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS))
	{
		Index ++;
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
	SCH_tasks_G[Index].Period = PERIOD;
	SCH_tasks_G[Index].RunMe = 0;
	// return position of task ( to allow l a t e r deletion )
	return Index ;
}

void SCH_Dispatch_Tasks ( void )
{
	unsigned char Index ;
	// Dispatches (runs ) the next task ( i f one i s ready )
	for ( Index = 0; Index < SCH_MAX_TASKS; Index++) {
		if ( SCH_tasks_G[Index].RunMe > 0 ) {
			(*SCH_tasks_G[Index].pTask )(); // Run the task
			SCH_tasks_G[Index].RunMe -= 1; // Reset / reduce RunMe flag
			// Periodic tasks will automatically run again
			// − if this is a ’one shot ’ task , remove it from the array
			if(SCH_tasks_G[Index].Period == 0 )
			{
				SCH_Delete_Task(Index) ;
			}
		}
	}
	// Report system status
	SCH_Report_Status();
	// The scheduler enters idle mode at this point
	//SCH_Go_To_Sleep() ;
}

