/*
 * HMI.c
 *
 * Created: 9/10/2022 9:30:25 PM
 * Author : MHamiid
 */ 


#include "Application/Application.h"


int main(void)
{
	application_init();
	
	while (1)
	{
		application_loop();
	}
	
	return 0;
}