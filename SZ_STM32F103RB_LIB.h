/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：SZ_STM32F103RB_LIB.h
 * 描述    ：提供STM32F103RB神舟I号开发板的库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
**********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>

#ifdef __cplusplus
 extern "C" {
#endif

/* 别名区宏定义 bitband macro ------------------------------------------------*/
/* 使用bitband可以提高设计bit操作的程序的效率，外设GPIO管脚的控制效果尤为明显 */
/* 外设别名区 对32MB外设别名区的访问映射为对1MB 外设bit-band区的访问(实际大小根据芯片有关) */
#define Periph_BASE         0x40000000  // 外设基地址 Peripheral 
#define Periph_BB_BASE      0x42000000  // 外设别名区基地址 Peripheral bitband

/* 注意：传入常量参数时，在编译时编译器会计算出别名区地址，这样能达到提升效率的目的(推荐)
         如果传入变量参数，只能在运行时由STM32自己计算别名区地址，效率会大打折扣(不推荐) */
#define Periph_BB(PeriphAddr, BitNumber)    \
          *(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2))
	 
#define Periph_ResetBit_BB(PeriphAddr, BitNumber)    \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)) = 0)
   
#define Periph_SetBit_BB(PeriphAddr, BitNumber)       \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)) = 1)

#define Periph_GetBit_BB(PeriphAddr, BitNumber)       \
          (*(__IO uint32_t *) (Periph_BB_BASE | ((PeriphAddr - Periph_BASE) << 5) | ((BitNumber) << 2)))

/* 外设GPIO输入输出别名区定义，n为bit位置范围为0到15    */
/* 我们对GPIOA.15操作，首先要初始化GPIO，然后使用方法： */
/* 对GPIOA.15输出低电平：   PAOutBit(15) = 0;           */
/* 对GPIOA.15输出低电平：   PAOutBit(15) = 1;           */
/* 读取GPIOA.15输入的电平： data = PAInBit(15);         */
#define PAOutBit(n)     Periph_BB((uint32_t)&GPIOA->IDR,n)  //输出 
#define PASetBit(n)     (PAOutBit(n) = 1)                   //输出 高
#define PAResetBit(n)   (PAOutBit(n) = 0)                   //输出 低
#define PAInBit(n)      Periph_BB((uint32_t)&GPIOA->IDR,n)  //输入 

#define PBOutBit(n)     Periph_BB((uint32_t)&GPIOB->ODR,n)  //输出 
#define PBSetBit(n)     (PBOutBit(n) = 1)                   //输出 高
#define PBResetBit(n)   (PBOutBit(n) = 0)                   //输出 低
#define PBInBit(n)      Periph_BB((uint32_t)&GPIOB->IDR,n)  //输入 

#define PCOutBit(n)     Periph_BB((uint32_t)&GPIOC->ODR,n)  //输出 
#define PCSetBit(n)     (PCOutBit(n) = 1)                   //输出 高
#define PCResetBit(n)   (PCOutBit(n) = 0)                   //输出 低
#define PCInBit(n)      Periph_BB((uint32_t)&GPIOC->IDR,n)  //输入 

#define PDOutBit(n)     Periph_BB((uint32_t)&GPIOD->ODR,n)  //输出 
#define PDSetBit(n)     (PDOutBit(n) = 1)                   //输出 高
#define PDResetBit(n)   (PDOutBit(n) = 0)                   //输出 低
#define PDInBit(n)      Periph_BB((uint32_t)&GPIOD->IDR,n)  //输入 

#define PEOutBit(n)     Periph_BB((uint32_t)&GPIOE->ODR,n)  //输出 
#define PESetBit(n)     (PEOutBit(n) = 1)                   //输出 高
#define PEResetBit(n)   (PEOutBit(n) = 0)                   //输出 低
#define PEInBit(n)      Periph_BB((uint32_t)&GPIOE->IDR,n)  //输入

#define PFOutBit(n)     Periph_BB((uint32_t)&GPIOF->ODR,n)  //输出 
#define PFSetBit(n)     (PFOutBit(n) = 1)                   //输出 高
#define PFResetBit(n)   (PFOutBit(n) = 0)                   //输出 低
#define PFInBit(n)      Periph_BB((uint32_t)&GPIOF->IDR,n)  //输入

#define PGOutBit(n)     Periph_BB((uint32_t)&GPIOG->ODR,n)  //输出 
#define PGSetBit(n)     (PGOutBit(n) = 1)                   //输出 高
#define PGResetBit(n)   (PGOutBit(n) = 0)                   //输出 低
#define PGInBit(n)      Periph_BB((uint32_t)&GPIOG->IDR,n)  //输入


/* 内部SRAM别名区 对32MB SRAM别名区的访问映射为对1MB SRAMbit-band区的访问(实际大小根据芯片有关) */
#define RAM_BASE            0x20000000  // 内部SRAM基地址  
#define RAM_BB_BASE         0x22000000  // 内部SRAM别名区基地址

#define SRAM_ResetBit_BB(VarAddr, BitNumber)    \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 0)
   
#define SRAM_SetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)) = 1)

#define SRAM_GetBit_BB(VarAddr, BitNumber)       \
          (*(__IO uint32_t *) (RAM_BB_BASE | ((VarAddr - RAM_BASE) << 5) | ((BitNumber) << 2)))


/* 资源定义 ------------------------------------------------------------------*/

/** 定义为枚举类型，方便多个指示灯时扩展 **/
/** 指示灯定义 **/
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
} Led_TypeDef;

/** 按键定义 **/
typedef enum 
{  
  KEY1 = 0,
  KEY2 = 1,
} Button_TypeDef;

/** 按键模式定义，查询模式和中断模式 **/
typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum 
{
  COM1 = 0,   
} COM_TypeDef;   

/** 指示灯管脚资源个数定义  **/

#define LEDn                             3


/** LED指示灯管脚资源定义  输出低电平点亮指示灯 **/
#define LED1_PIN_NUM                     2 /* bitband别名区使用宏定义  */
#define LED1_PIN                         GPIO_Pin_2
#define LED1_GPIO_PORT                   GPIOA
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOA 
#define LED1OBB                          Periph_BB((uint32_t)&LED1_GPIO_PORT->ODR, LED1_PIN_NUM)//等价于Periph_BB((uint32_t)&GPIOD->ODR, 2)

#define LED2_PIN_NUM                     2 
#define LED2_PIN                         GPIO_Pin_2
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOB  
#define LED2OBB                          Periph_BB((uint32_t)&LED2_GPIO_PORT->ODR, LED2_PIN_NUM)

#define LED3_PIN_NUM                     3 
#define LED3_PIN                         GPIO_Pin_3  
#define LED3_GPIO_PORT                   GPIOA
#define LED3_GPIO_CLK                    RCC_APB2Periph_GPIOA  
#define LED3OBB                          Periph_BB((uint32_t)&LED3_GPIO_PORT->ODR, LED3_PIN_NUM)

/** 按键管脚资源个数定义  **/
#define BUTTONn                          2

/** KEY按键管脚资源定义    按键按下时输入低电平 按键释放时输入高电平 **/

/** KEY1按键管脚  **/
#define KEY1_BUTTON_PIN_NUM              0 
#define KEY1_BUTTON_PIN                  GPIO_Pin_0
#define KEY1_BUTTON_GPIO_PORT            GPIOA
#define KEY1_BUTTON_GPIO_CLK             RCC_APB2Periph_GPIOA
#define KEY1_BUTTON_EXTI_LINE            EXTI_Line0
#define KEY1_BUTTON_EXTI_PORT_SOURCE     GPIO_PortSourceGPIOA
#define KEY1_BUTTON_EXTI_PIN_SOURCE      GPIO_PinSource0
#define KEY1_BUTTON_EXTI_IRQn            EXTI0_IRQn  
#define KEY1IBB                          Periph_BB((uint32_t)&KEY1_BUTTON_GPIO_PORT->IDR, KEY1_BUTTON_PIN_NUM) //等价于Periph_BB((uint32_t)&GPIOC->IDR, 4)

/** KEY2按键管脚  **/
#define KEY2_BUTTON_PIN_NUM              1
#define KEY2_BUTTON_PIN                  GPIO_Pin_1
#define KEY2_BUTTON_GPIO_PORT            GPIOA
#define KEY2_BUTTON_GPIO_CLK             RCC_APB2Periph_GPIOA
#define KEY2_BUTTON_EXTI_LINE            EXTI_Line1
#define KEY2_BUTTON_EXTI_PORT_SOURCE     GPIO_PortSourceGPIOA
#define KEY2_BUTTON_EXTI_PIN_SOURCE      GPIO_PinSource1
#define KEY2_BUTTON_EXTI_IRQn            EXTI1_IRQn  
#define KEY2IBB                          Periph_BB((uint32_t)&KEY2_BUTTON_GPIO_PORT->IDR, KEY2_BUTTON_PIN_NUM)

/** 串口管脚资源个数定义  **/
#define COMn                             1

/** 串口1管脚资源定义  **/
#define SZ_STM32_COM1_STR                "USART1"
#define SZ_STM32_COM1                    USART1
#define SZ_STM32_COM1_CLK                RCC_APB2Periph_USART1
#define SZ_STM32_COM1_TX_PIN             GPIO_Pin_9
#define SZ_STM32_COM1_TX_GPIO_PORT       GPIOA
#define SZ_STM32_COM1_TX_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SZ_STM32_COM1_RX_PIN             GPIO_Pin_10
#define SZ_STM32_COM1_RX_GPIO_PORT       GPIOA
#define SZ_STM32_COM1_RX_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SZ_STM32_COM1_IRQn               USART1_IRQn

/** 通用函数声明  **/  
void delay(__IO uint32_t nCount);
void NVIC_GroupConfig(void);
void SZ_STM32_SysTickInit(uint32_t HzPreSecond);
void SysTickDelay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

void GetDeviceSerialID(void);

/** 接口函数声明  **/  
void SZ_STM32_LEDInit(Led_TypeDef Led);
void SZ_STM32_LEDOn(Led_TypeDef Led);
void SZ_STM32_LEDOff(Led_TypeDef Led);
void SZ_STM32_LEDToggle(Led_TypeDef Led);

void SZ_STM32_KEYInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t SZ_STM32_KEYGetState(Button_TypeDef Button);
uint32_t SZ_STM32_KEYScan(void);

void __SZ_STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void SZ_STM32_COMInit(COM_TypeDef COM, uint32_t BaudRate);

void SZ_STM32_ADC_Configuration(void);
uint16_t GetADCConvertedValue(void);
    
#ifdef __cplusplus
}
#endif
/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/

