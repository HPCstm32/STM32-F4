/**
 *	Keil project for FatFS for SD cards
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_fatfs.h"
#include <stdio.h>
#include <string.h>

/* Fatfs object */
FATFS FatFs;
/* File object */
FIL fil;

/*
 *	Device Control Interface
 *	注意，在使用 API 的同時，你依舊需要去實作底層
 *	包含這幾個 Function:
 *		disk_status 	- Get device status
 *		disk_initialize - Initialize device
 *		disk_read 		- Read sector(s)
 *		disk_write 		- Write sector(s)
 *		disk_ioctl 		- Control device dependent features
 *		get_fattime 	- Get current time
 */
int main(void) {
	/* Free and total space */
	uint32_t total, free;
	
	/* Initialize system */
		// 在 STM32 範例程式中，已從 startup_stm32f4xx.s 呼叫 SystemInit() 了
		// 所以在那邊，就不重複呼叫。但在這邊，算是初始化，會呼叫一次。
	SystemInit();
	
	/* Initialize delays */
		// 初始化 Timer
	TM_DELAY_Init();
	
	/* Initialize LEDs */
		// 初始化 LED
	TM_DISCO_LedInit();

	/* Mount drive */
	// http://elm-chan.org/fsw/ff/en/mount.html
	// 特別是第二個參數 http://elm-chan.org/fsw/ff/en/filename.html
	// 		"#define _FS_RPATH 2"，所代表的是 drive 0 目前的資料夾
	// 第三個參數代表，1: Force mounted the volume to check if the FAT volume is available.
	if (f_mount(&FatFs, "0:", 1) == FR_OK) {
		/* Mounted OK, turn on RED LED */
		// 如果 mount 成功，亮紅燈
		TM_DISCO_LedOn(LED_RED);
		
		/* Try to open file */
		// 嘗試打開 1stfile.txt
		// 第三個參數則是需要參考 http://elm-chan.org/fsw/ff/en/open.html
		//		產生新檔，並可以讀取與寫入
		if (f_open(&fil, "0:1stfile.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {
			/* File opened, turn off RED and turn on GREEN led */
			// 如果 Open 成功，紅燈熄，綠燈亮
			TM_DISCO_LedOn(LED_GREEN);
			TM_DISCO_LedOff(LED_RED);
			
			/* If we put more than 0 characters (everything OK) */
			// http://elm-chan.org/fsw/ff/en/puts.html
			// 利用 f_puts 寫入檔案，回傳成功的 byte 數 (大於 0 代表 OK)
			if (f_puts("First string in my file\n", &fil) > 0) {
				// 呼叫 TM_FATFS_DriveSize 之後，會將 total 所有空間與 free 剩餘空間，回傳回來
				if (TM_FATFS_DriveSize(&total, &free) == FR_OK) {
					/* Data for drive size are valid */
				}
				
				/* Turn on both leds */
				// 上述步驟都完成，綠 紅燈皆亮
				TM_DISCO_LedOn(LED_GREEN | LED_RED);
			}
			
			/* Close file, don't forget this! */
			f_close(&fil);
		}
		
		/* Unmount drive, don't forget this! */
		// To unregister the work area, specify a NULL to the fs, and then the work area can be discarded.
		// 要 unmount 時，在 FileDescriptor (第一個參數) 填上 NULL 或 0
		f_mount(0, "0:", 1);
	}
	
	while (1) {

	}
}

