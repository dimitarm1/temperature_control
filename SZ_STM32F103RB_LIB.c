/********************   (C) COPYRIGHT 2013 www.armjishu.com   ********************
 * 文件名  ：SZ_STM32F103RB_LIB.c
 * 描述    ：提供STM32F103RB神舟I号开发板的库函数
 * 实验平台：STM32神舟开发板
 * 作者    ：www.armjishu.com 
**********************************************************************************/
  
/* Includes ------------------------------------------------------------------*/
#include "SZ_STM32F103RB_LIB.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"

__IO uint32_t TimingDelay;

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT};
const uint16_t GPIO_PIN_NUM[LEDn] = {LED1_PIN_NUM, LED2_PIN_NUM, LED3_PIN_NUM};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY1_BUTTON_GPIO_PORT, KEY2_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN_NUM[BUTTONn] = {KEY1_BUTTON_PIN_NUM, KEY2_BUTTON_PIN_NUM}; 
const uint16_t BUTTON_PIN[BUTTONn] = {KEY1_BUTTON_PIN, KEY2_BUTTON_PIN}; 
const uint32_t BUTTON_CLK[BUTTONn] = {KEY1_BUTTON_GPIO_CLK, KEY2_BUTTON_GPIO_CLK};
const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {KEY1_BUTTON_EXTI_LINE, KEY2_BUTTON_EXTI_LINE};
const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PORT_SOURCE, KEY2_BUTTON_EXTI_PORT_SOURCE};
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PIN_SOURCE, KEY2_BUTTON_EXTI_PIN_SOURCE}; 
const uint16_t BUTTON_IRQn[BUTTONn] = {KEY1_BUTTON_EXTI_IRQn, KEY2_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {SZ_STM32_COM1}; 
GPIO_TypeDef* COM_TX_PORT[COMn] = {SZ_STM32_COM1_TX_GPIO_PORT};
GPIO_TypeDef* COM_RX_PORT[COMn] = {SZ_STM32_COM1_RX_GPIO_PORT};
const uint32_t COM_USART_CLK[COMn] = {SZ_STM32_COM1_CLK};
const uint32_t COM_TX_PORT_CLK[COMn] = {SZ_STM32_COM1_TX_GPIO_CLK};
const uint32_t COM_RX_PORT_CLK[COMn] = {SZ_STM32_COM1_RX_GPIO_CLK};
const uint16_t COM_TX_PIN[COMn] = {SZ_STM32_COM1_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {SZ_STM32_COM1_RX_PIN};

const uint8_t STM32F10x_STR[] = {"\r\n"\
           "  _____ _______ __  __ ____ ___  ______ __  ___\r\n"\
           " / ____|__   __|  \\/  |___ \\__ \\|  ____/_ |/ _ \\\r\n"\
           "| (___    | |  | \\  / | __) | ) | |__   | | | | |_  __\r\n"\
           " \\___ \\   | |  | |\\/| ||__ < / /|  __|  | | | | \\ \\/ /\r\n"\
           " ____) |  | |  | |  | |___) / /_| |     | | |_| |>  <\r\n"\
           "|_____/   |_|  |_|  |_|____/____|_|     |_|\\___//_/\\_\\神舟系列开发板\r\n"\
           "\r\n"};

					 
uint32_t STM32DeviceSerialID[3]; /* 全局变量IntDeviceSerial存放读到的设备ID */

/**-------------------------------------------------------
  * @函数名 delay
  * @功能   简单的delay延时函数.
  * @参数   延迟周期数 0--0xFFFFFFFF
  * @返回值 无
***------------------------------------------------------*/
void delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}

/**-------------------------------------------------------
  * @函数名 NVIC_GroupConfig
  * @功能   配置NVIC中断优先级分组函数.
  *         默认配置为1比特表示主优先级, 3比特表示次优先级
  *         用户可以根据需要修改
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void NVIC_GroupConfig(void)
{
    /* 配置NVIC中断优先级分组:
     - 1比特表示主优先级  主优先级合法取值为 0 或 1 
     - 3比特表示次优先级  次优先级合法取值为 0..7
     - 数值越低优先级越高，取值超过合法范围时取低bit位 
    */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /*==================================================================================
      NVIC_PriorityGroup   |  主优先级范围  |  次优先级范围  |   描述
      ==================================================================================
     NVIC_PriorityGroup_0  |      0         |      0-15      |   0 比特表示主优先级
                           |                |                |   4 比特表示次优先级 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_1  |      0-1       |      0-7       |   1 比特表示主优先级
                           |                |                |   3 比特表示次优先级 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_2  |      0-3       |      0-3       |   2 比特表示主优先级
                           |                |                |   2 比特表示次优先级 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_3  |      0-7       |      0-1       |   3 比特表示主优先级
                           |                |                |   1 比特表示次优先级 
     ----------------------------------------------------------------------------------
     NVIC_PriorityGroup_4  |      0-15      |      0         |   4 比特表示主优先级
                           |                |                |   0 比特表示次优先级   
    ==================================================================================*/
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_SysTickInit
  * @功能   初始化系统定时器SysTick
  *         用户可以根据需要修改
  * @参数   每秒中断次数
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_SysTickInit(uint32_t HzPreSecond)
{
    /* HzPreSecond = 1000 to Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.
    
    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
       call. The NVIC_SetPriority() is defined inside the core_cm3.h file.

    4. To adjust the SysTick time base, use the following formula:
                            
         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
    */
    if (SysTick_Config(SystemCoreClock / HzPreSecond))
    { 
        /* Capture error */ 
        while (1);
    }
}

/**-------------------------------------------------------
  * @函数名 SysTickDelay
  * @功能   系统定时器实现的延时函数，可以在用户程序中调用
  *         用户可以根据需要修改
  * @参数   延迟周期数，延迟时间为nTime/HzPreSecond
  *         误差为 -1/HzPreSecond
  * @返回值 无
***------------------------------------------------------*/
void SysTickDelay(__IO uint32_t nTime)
{ 
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/**-------------------------------------------------------
  * @函数名 TimingDelay_Decrement
  * @功能   系统节拍定时器服务函数调用的子函数
  *         将全局变量TimingDelay减一，用于实现延时
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    { 
        TimingDelay--;
    }
}

extern void SysTick_Handler_User(void);
/**-------------------------------------------------------
  * @函数名 SysTick_Handler
  * @功能   系统节拍定时器服务请求处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();

    SysTick_Handler_User();
}

void GetDeviceSerialID(void)
{
    STM32DeviceSerialID[0] = *(__IO uint32_t*)(0x1FFFF7E8);
    STM32DeviceSerialID[1] = *(__IO uint32_t*)(0x1FFFF7EC);
    STM32DeviceSerialID[2] = *(__IO uint32_t*)(0x1FFFF7F0);
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_LEDInit
  * @功能   初始化LED的GPIO管脚，配置为推挽输出
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LEDInit(Led_TypeDef Led)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    /* 使能LED对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

    /* Configure the GPIO_LED pin */
    /* 初始化LED的GPIO管脚，配置为推挽输出 */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_LEDOn
  * @功能   点亮对应的LED指示灯
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LEDOn(Led_TypeDef Led)
{
    /* 指定管脚输出低电平，点亮对应的LED指示灯 */
    GPIO_PORT[Led]->BRR = GPIO_PIN[Led];  
    //若要提高效率，建议直接调用 LEDnOBB = 0;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_LEDOn
  * @功能   熄灭对应的LED指示灯
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LEDOff(Led_TypeDef Led)
{
    /* 指定管脚输出高电平，熄灭对应的LED指示灯 */
    GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];   
    //若要提高效率，建议直接调用 LEDnOBB = 1;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_LEDToggle
  * @功能   将对应的LED指示灯状态取反
  * @参数   LED1  对应开发板上第一个指示灯
  *         LED2  对应开发板上第二个指示灯
  *         LED3  对应开发板上第三个指示灯
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_LEDToggle(Led_TypeDef Led)
{
    /* 指定管脚输出异或 1，实现对应的LED指示灯状态取反目的 */
    GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
	//若要提高效率，建议直接调用 LEDnOBB = !LEDnOBB;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_KEYInit
  * @功能   初始化KEY按键的GPIO管脚，配置为带上拉的输入
  *         按键按下时为低电平0
  * @参数1  KEY1  对应开发板上第一个KEY按键
  *         KEY2  对应开发板上第二个KEY按键
  * @参数2  BUTTON_MODE_GPIO  配置为普通输入管脚使用
  *         BUTTON_MODE_EXTI  配置为外部EXTI中断使用
  *                           需要完善中断处理函数
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_KEYInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the BUTTON Clock */
    /* 使能KEY按键对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(BUTTON_CLK[Button] | RCC_APB2Periph_AFIO, ENABLE);

    /* Configure Button pin as input floating */
    /* 初始化KEY按键的GPIO管脚，配置为带上拉的输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
    GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

    /* 初始化KEY按键为中断模式 */
    if (Button_Mode == BUTTON_MODE_EXTI)
    {
        /* Connect Button EXTI Line to Button GPIO Pin */
        /* 将KEY按键对应的管脚连接到内部中断线 */    
        GPIO_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

        /* Configure Button EXTI line */
        /* 将KEY按键配置为中断模式，下降沿触发中断 */    
        EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable and set Button EXTI Interrupt to the lowest priority */
        /* 将KEY按键的中断优先级配置为最低 */  
        NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure); 
    }
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_KEYGetState
  * @功能   获取KEY按键的输入电平状态，按键按下时为低电平0
  * @参数   KEY1  对应开发板上第一个KEY按键
  *         KEY2  对应开发板上第二个KEY按键
  * @返回值 无
***------------------------------------------------------*/
uint32_t SZ_STM32_KEYGetState(Button_TypeDef Button)
{
    return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
	//若要提高效率，建议直接调用 KeyState = KEYnIBB;
}

/**-------------------------------------------------------
  * @函数名 SZ_STM32_KEYScan
  * @功能   获取KEY按键的输入电平状态，按键按下时为低电平0
  * @参数   无
  * @返回值 1  对应开发板上第一个KEY按键处于按下状态
  *         2  对应开发板上第二个KEY按键处于按下状态
  *         0  开发板上没有KEY按键处于按下状态
***------------------------------------------------------*/
uint32_t SZ_STM32_KEYScan(void)
{
    /* 获取KEY按键的输入电平状态，按键按下时为低电平0 */
    if(0 == KEY1IBB)
    {
         /* 延迟去抖 */
        delay(150000);
        if(0 == KEY1IBB)
        {
            return 1;
        }
    }

    /* 获取KEY按键的输入电平状态，按键按下时为低电平0 */
    if(0 == KEY2IBB)
    {
         /* 延迟去抖 */
        delay(150000);
        if(0 == KEY2IBB)
        {
            return 2;
        }
    }

    return 0;
}


/**-------------------------------------------------------
  * @函数名 __SZ_STM32_COMInit
  * @功能   对STM32的USART初始化底层函数
  * @参数1  COM1  对应STM32的USART1 对应开发板上串口1
  * @参数2  指向一个成员已赋值USART_InitTypeDef结构体的指针
  * @返回值 无
***------------------------------------------------------*/
void __SZ_STM32_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    /* 使能STM32的USART对应GPIO的Clock时钟 */
    RCC_APB2PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2Periph_AFIO, ENABLE);


    /* 使能STM32的USART1的Clock时钟 */
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 

    /* Configure USART Tx as alternate function push-pull */
    /* 初始化STM32的USART的TX管脚，配置为复用功能推挽输出 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    /* 初始化STM32的USART的RX管脚，配置为复用功能输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    /* 根据传入的参数初始化STM32的USART配置 */
    USART_Init(COM_USART[COM], USART_InitStruct);

    /* Enable USART */
    /* 使能STM32的USART功能模块 */
    USART_Cmd(COM_USART[COM], ENABLE);
}

/**-------------------------------------------------------
  * @函数名 __SZ_STM32_COMInit
  * @功能   面向用户的STM32的USART初始化函数
  * @参数1  COM1  对应STM32的USART1 对应开发板上串口1
  * @参数2  BaudRate 串口的波特率，例如"115200"
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_COMInit(COM_TypeDef COM, uint32_t BaudRate)
{
  
    USART_InitTypeDef USART_InitStructure;
  
    /* USARTx 默认配置:
          - BaudRate = 115200 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Hardware flow control disabled (RTS and CTS signals)
          - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate = BaudRate;              //串口的波特率，例如115200 最高达4.5Mbits/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据字长度(8位或9位)
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //可配置的停止位-支持1或2个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //双工模式，使能发送和接收
  
    __SZ_STM32_COMInit(COM, &USART_InitStructure);  // 调用STM32的USART初始化底层函数
  
    printf("\r\n\n\n\r WWW.ARMJISHU.COM  %s printf configured....", SZ_STM32_COM1_STR);
    printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");
  
    printf("%s", STM32F10x_STR);
  
    printf(" WWW.ARMJISHU.COM use __STM32F10X_STDPERIPH_VERSION %d.%d.%d",
  			__STM32F10X_STDPERIPH_VERSION_MAIN,
  			__STM32F10X_STDPERIPH_VERSION_SUB1,
  			__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("\n\r 产品内部Flash大小为：%dK字节！ \t www.armjishu.com",
              *(__IO uint16_t*)(0x1FFFF7E0));
    SystemCoreClockUpdate();
    printf("\n\r 系统内核时钟频率(SystemCoreClock)为：%dHz.\n\r",
            SystemCoreClock);
}


/*
    加入以下代码,支持printf函数,不需要选择use MicroLIB	  
*/
#ifndef MicroLIB
//#pragma import(__use_no_semihosting)             //没有实现fgetc时需要声明该参数   
/* 标准库需要的支持函数 使用printf()调试打印不需要实现该函数 */               
struct __FILE 
{ 
	int handle; 
    /* Whatever you require here. If the only file you are using is */    
    /* standard output using printf() for debugging, no file handling */    
    /* is required. */
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
/* 重定义fputc函数 如果使用MicroLIB只需要重定义fputc函数即可 */  
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(SZ_STM32_COM1, USART_FLAG_TC) == RESET)
    {}

    /* e.g. write a character to the USART */
    USART_SendData(SZ_STM32_COM1, (uint8_t) ch);

    return ch;
}
/*
可以直接使用putchar
不需要再定义 int putchar(int ch)，因为stdio.h中有如下定义
 #define putchar(c) putc(c, stdout)
*/

int ferror(FILE *f) {  
    /* Your implementation of ferror */  
    return EOF;  
} 
#endif 

FILE __stdin;

int fgetc(FILE *fp)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(SZ_STM32_COM1, USART_FLAG_RXNE) == RESET)
    {
    }

    ch = (int)SZ_STM32_COM1->DR & 0xFF;
	
    putchar(ch); //回显
	
	return ch;
}

#ifdef  USE_FULL_ASSERT
// 需要在工程设置Option(快捷键ALT+F7)C++属性页的define栏输入"USE_FULL_ASSERT"
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**-------------------------------------------------------
  * @函数名 ADC_GPIO_Configuration
  * @功能   ADC模数转换的GPIO配置初始化函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void ADC_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOC clock */
    /* 使能GPIOC时钟      */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	  //

    /* Configure PA.01 (ADC Channel12) as analog input -------------------------*/
    //PB1 作为模拟通道输入引脚                          
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       //管脚1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //输入模式
    GPIO_Init(GPIOC, &GPIO_InitStructure);          //GPIO组
}

/* STM32芯片ADC转换结果DR寄存器基地址 */
#define DR_ADDRESS                  ((uint32_t)0x4001244C) 
/* 存放ADC为12位模数转换器结果的变量，只有ADCConvertedValue的低12位有效 */
__IO uint16_t ADCConvertedValue;     // 

/**-------------------------------------------------------
  * @函数名 SZ_STM32_ADC_Configuration
  * @功能   ADC模数转换的参数配置函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SZ_STM32_ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;        //ADC初始化结构体声明
    DMA_InitTypeDef DMA_InitStructure;        //DMA初始化结构体声明     

    ADC_GPIO_Configuration();

    /* Enable DMA1 clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //使能DMA时钟

    /* DMA1 channel1 configuration ----------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);		  //开启DMA1的第一通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = DR_ADDRESS;		  //DMA对应的外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;   //内存存储基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA的转换模式为SRC模式，由外设搬移到内存
    DMA_InitStructure.DMA_BufferSize = 1;		   //DMA缓存大小，1个
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//接收一次数据后，设备地址禁止后移
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;	//关闭接收一次数据后，目标内存地址后移
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //定义外设数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA搬移数据尺寸，HalfWord就是为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //转换模式，循环缓存模式。
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA优先级高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2M模式禁用
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
    /* Enable DMA1 channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC时钟

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//独立的转换模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //开启扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   //开启连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC外部开关，关闭状态
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //对齐方式,ADC为12位中，右对齐方式
    ADC_InitStructure.ADC_NbrOfChannel = 1;	 //开启通道数，1个
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel10 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_55Cycles5);
                          //ADC通道组， 第9个通道 采样顺序1，转换时间 
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	  //ADC命令，使能
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);  //开启ADC1

    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);	  //重新校准
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待重新校准完成
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//开始校准
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));	   //等待校准完成
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//连续转换开始，ADC通过DMA方式不断的更新RAM区。
}

/**-------------------------------------------------------
  * @函数名 GetADCConvertedValue
  * @功能   获取ADC模数转换的结果
  * @参数   无
  * @返回值 ADC转换结果的变量，只有的低12位有效
***------------------------------------------------------*/
uint16_t GetADCConvertedValue(void)
{
    return ADCConvertedValue;
}

/******************* (C) COPYRIGHT 2013 www.armjishu.com *****END OF FILE****/
