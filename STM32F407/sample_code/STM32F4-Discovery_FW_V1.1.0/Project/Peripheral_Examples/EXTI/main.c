/**
  ******************************************************************************
  * @file    EXTI/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup EXTI
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
EXTI_InitTypeDef   EXTI_InitStructure;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void EXTILine0_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */

  /* Initialize LEDs mounted on STM32F4-Discovery board */
  // 這邊和 前一個 Example 一樣，只不過用了 Function 包裝了起來
  // 因為我們需要 LED，所以在這邊先 Init LED GPIO
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);

  /* Configure EXTI Line0 (connected to PA0 pin) in interrupt mode */
  // 設定 EXTI 0 和 PA0(Button)
  EXTILine0_Config();

  /* Generate software interrupt: simulate a rising edge applied on EXTI0 line */
  // 自己產生一個 Software Interrupt
  EXTI_GenerateSWInterrupt(EXTI_Line0);

  while (1)
  {
  }
}

/**
  * @brief  Configures EXTI Line0 (connected to PA0 pin) in interrupt mode
  * @param  None
  * @retval None
  */
  /*
   *	GPIO A 0 -----------------
   *							 |
   *	GPIO B 0 -----------------
   *							 |
   *	GPIO C 0 ---------------------------------> EXTI 0
   *							 |
   *	GPIO D 0 -----------------
   *							 |
   *	GPIO E 0 -----------------
   *
   *
   *	GPIO A 1 -----------------
   *							 |
   *	GPIO B 1 -----------------
   *							 |
   *	GPIO C 1 ---------------------------------> EXTI 1
   *							 |
   *	GPIO D 1 -----------------
   *							 |
   *	GPIO E 1 -----------------
   *
   *
   *
   *	GPIO A 2 -----------------
   *							 |
   *	GPIO B 2 -----------------
   *							 |
   *	GPIO C 2 ---------------------------------> EXTI 2
   *							 |
   *	GPIO D 2 -----------------
   *							 |
   *	GPIO E 2 -----------------
   *	.
   *	.
   *	.
   *	.
   *	GPIO A 15-----------------
   * 							 |
   *    GPIO B 15-----------------
   * 						 	 |
   *    GPIO C 15---------------------------------> EXTI 15
   * 							 |
   *    GPIO D 15-----------------
   * 							 |
   *    GPIO E 15-----------------
   *
   *
   *
   *	EXTI 0 ----------
   *	                |
   *	EXTI 1 ----------
   *                    |
   *	EXTI 2 -------------------> NVIC
   *	.
   *	.
   *	.
   *
   */
void EXTILine0_Config(void)
{
  
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	// NVIC 要啟動 System Config，而且還是在 APB2 Bus
  															// AHB1, AHB2, AHB3
  															// APB1, APB2
  
  /* Configure PA0 pin as input floating */
  // Enable GPIO Group A
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;				// Button is input mode.
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			// FIXME: ???
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					// 第 0 根 Pin
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI Line0 */
  // 這邊設定 EXTI 的 Pin 腳
  	// 是否為 Event or Interrupt
  		// 正緣觸發 或是 負緣觸發
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;							// FIXME: ????
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  // 這邊設定 EXTI 的 Prority, Channel, IRQ
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				// FIXME: ???
  NVIC_Init(&NVIC_InitStructure);
	
	// 不要忘記，中斷被觸發時，會去 stm32f4xx_it.c 裡面的 EXTI0_IRQHandler
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
