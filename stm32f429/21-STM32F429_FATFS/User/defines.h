/**
 *  Defines for your entire project at one place
 * 
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

/* Put your global defines for all libraries here used in your project */

/* Use SPI communication with SDCard */
#define    FATFS_USE_SDIO                0

/* Select your SPI settings */
#define FATFS_SPI				SPI1
#define FATFS_SPI_PINSPACK		TM_SPI_PinsPack_1
/* The Default SPI setting is "SPI1 with Pins_pack 1". */

/* Custom CS pin for SPI communication */	
#define FATFS_CS_PORT		GPIOB
#define FATFS_CS_PIN		GPIO_Pin_5
/* SPI has a pin which is used to Select the Slave. We called it CS (Chip Select) or SS (Slave Select).
	At STM32, we connect CS (SS) to GPIOB, Pin_5 (PB5) to control the High voltage or Low voltage. */

// Use custom get_fattime() function
#define FATFS_CUSTOM_FATTIME				1
/* To enable the FAT Time with RTC */
/* If you enable this option, you should implement the get_fattime() function */


#endif