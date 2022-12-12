/*
 * Config.h
 *
 *	Common configurations for ATMega32A
 *	
 * Created: 12/12/2022 4:36:23 AM
 *  Author: MHamiid
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

/**
 * Microcontroller's supplied clock frequency. Used for calculations in various drivers.
 * Defining incorrect F_CPU other than the actual supplied clock to the microcontroller will result in incorrect behavior in some drivers that relay on the F_CPU value for calculations.
 */
#define F_CPU 1000000UL



#endif /* CONFIG_H_ */