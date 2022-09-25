/*
 * NodeOne.c
 *
 * Created: 9/21/2022 12:37:53 AM
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

