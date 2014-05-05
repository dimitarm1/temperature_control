/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : main.c
* Author             : WWW.ARMJISHU.COM之STM32核心团队  JESSE 
* Version            : 
* Date               : 
* Description        : STM32神舟I号开发板    广州
*******************************************************************************/
#include "stm32f10x.h"
#include "ili9320.h"
#include "SZ_STM32F103RB_LIB.h"
#include <stdio.h>
#include <string.h>



USART_InitTypeDef USART_InitStructure;
void SZ_STM32_ADC_Configuration(void);
uint16_t GetADCConvertedValue(void);

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

static void Delay_ARMJISHU(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

void SysTick_Handler_User(void){
}

void STM32_Shenzhou_COMInit(USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  USART_Init(USART1, USART_InitStruct);	/* USART configuration */
  USART_Cmd(USART1, ENABLE);  /* Enable USART */
}

typedef struct {
	int time; // time in seconds
	int temperature;
}profile_data_t;

const profile_data_t profile_data1[] = {
	{120, 180},
	{90, 275}
};
int measured_data[100];
int measured_data_index = 0;

void draw_diagram(){
	int i,j, data, data_prev, index_prev;
	index_prev = (measured_data_index + 99)%100;
//	GUI_Square(100,1,198,Black,1);
//	GUI_Square(99,0,202,White,0);
	data_prev = 0;
	for (i = 1; i< 100; i++){
		data = (measured_data[(index_prev + i)%100]*200)/4096;
		GUI_Line(100 + i*2-1,201 - data_prev,100 + i*2,201 - data, Black);
		data_prev = data;
	}
	data_prev = 0;
	for (i = 1; i< 100; i++){
		data = (measured_data[(measured_data_index + i)%100]*200)/4096;
		GUI_Line(100 + i*2-1,201 - data_prev,100 + i*2,201 - data, White);
		data_prev = data;
	}
}

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
  volatile uint16_t ADCConvertedValueLocal, Precent = 0, Voltage = 0;
  int8_t i=0,j=0;
  char text[]= "DETELINA";
	
	memset(measured_data,0,sizeof(measured_data));
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM32_Shenzhou_COMInit(&USART_InitStructure);

  /* Output a message on Hyperterminal using printf function */
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
 // printf("\r\n\n\n WWW.ARMJISHU.COM  %s configured....", EVAL_COM1_STR);
  printf("\r\n\n\n WWW.ARMJISHU.COM configured....");
  printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");

  printf("\r\n");
  printf("  _____ _______ __  __ ____ ___  ______ __  ___\r\n");
  printf(" / ____|__   __|  \\/  |___ \\__ \\|  ____/_ |/ _ \\\r\n");
  printf("| (___    | |  | \\  / | __) | ) | |__   | | | | |_  __\r\n");
  printf(" \\___ \\   | |  | |\\/| ||__ < / /|  __|  | | | | \\ \\/ /\r\n");
  printf(" ____) |  | |  | |  | |___) / /_| |     | | |_| |>  <\r\n");
  printf("|_____/   |_|  |_|  |_|____/____|_|     |_|\\___//_/\\_\\\r\n");
  printf("\r\n");
  
  printf("\n\r WWW.ARMJISHU.COM use __STM32F10X_STDPERIPH_VERSION %d.%d.%d\n\r",
			__STM32F10X_STDPERIPH_VERSION_MAIN,
			__STM32F10X_STDPERIPH_VERSION_SUB1,
			__STM32F10X_STDPERIPH_VERSION_SUB2);

  SZ_STM32_ADC_Configuration();

  ili9320_Initializtion();
	ili9320_Clear(0);
  
	//GUI_Rectangle(0,0,200,100,Green,1);
	GUI_Square(99,0,202,White,0);

	while (1){
		 GetADCConvertedValue();
		 delay(16000000); 
		 SZ_STM32_LEDToggle(LED1);

     ADCConvertedValueLocal = GetADCConvertedValue();
		 sprintf(text,"T=%d",ADCConvertedValueLocal);
		 for (j = strlen(text) - 1; j>-1; j--){
		   ili9320_PutChar_16x24(j*16,24,text[j],White,Black);
	   }
		 measured_data_index++; 
		 if (measured_data_index > 99) measured_data_index = 0;
		 measured_data[measured_data_index] = ADCConvertedValueLocal;
		 draw_diagram();
	}
	
	/*TFT-LCD彩屏显示数字1*/
	//for(j=0;j<2;j++)
	//{
	//	for(i=1;i<13;i++)
	//	{
	//		LCD_DrawChar(j*48, i*24, &ASCII_Table[(i+j*13)*144]);
	//	}
	//} 
}

//PUTCHAR_PROTOTYPE
//{
//  USART_SendData(USART1, (uint8_t) ch);

//  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//  {}

//  return ch;
//}
