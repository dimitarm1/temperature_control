 /******************************************************************************
  * @file    ili9320.c
  * @author  www.armjishu.com 
  * @Update  www.armjishu.com 
  * @version V1.0
  * @date    03/16/2010
  * @brief   TFT Driver program
  ******************************************************************************/
  
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "ili9320.h"
#include "ili9320_font.h"
#include <stdio.h>

u16 DeviceIdCode;

  /*****************************
  **    Ӳ������˵��          **
  ** STM32         ili9320    **
  ** PE0~15 <----> DB0~15     **
  ** PD15   <----> nRD        **
  ** PD13   <----> RS         **
  ** PB14   <----> nWR        **
  ** PC8    <----> nCS        **
  ** Reset  <----> nReset     **
  ** VCC    <----> BK_LED     **
  ******************************/
  
/* Private typedef -----------------------------------------------------------*/

/*******************************************************************************
* Function Name  : LCD_DB_AS_InPut
* Description    : config MCU LCD_DB pins AS InPut
* Input          : None
* Output         : None
* Return         : None.
*******************************************************************************/
void LCD_DB_AS_InPut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // DB15--0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : LCD_DB_AS_OutPut
* Description    : config MCU LCD_DB pins AS OutPut
* Input          : None
* Output         : None
* Return         : None.
*******************************************************************************/
void LCD_DB_AS_OutPut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // DB15--0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : LCD_Pins_Config
* Description    : Configures LCD Pins
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Pins_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(LCD_CLK_RS | RCC_APB2Periph_GPIOE |
                         LCD_CLK_WR | LCD_CLK_RD |
                         LCD_CLK_CS, ENABLE);

  // DB15--0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  //LCD_Pin_WR
  GPIO_InitStructure.GPIO_Pin = LCD_Pin_WR;
  GPIO_Init(LCD_PORT_WR, &GPIO_InitStructure);

  //LCD_Pin_CS
  GPIO_InitStructure.GPIO_Pin = LCD_Pin_CS;
  GPIO_Init(LCD_PORT_CS, &GPIO_InitStructure);

  //LCD_Pin_RS
  GPIO_InitStructure.GPIO_Pin = LCD_Pin_RS;
  GPIO_Init(LCD_PORT_RS, &GPIO_InitStructure);

  //LCD_Pin_RD
  GPIO_InitStructure.GPIO_Pin = LCD_Pin_RD;
  GPIO_Init(LCD_PORT_RD, &GPIO_InitStructure);
  
  SetCs
}

void LCD_Init(void)
{
 /* Configure the LCD Control pins --------------------------------------------*/
  LCD_Pins_Config();
}


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* author         : www.armjishu.com 
*******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  ClrCs
  ClrRs
  ClrWr
  LCD_Write(LCD_Reg);
  SetWr
  /* Write 16-bit Reg */
  SetRs
  ClrWr
  LCD_Write(LCD_RegValue);
  SetWr
  SetCs
}

/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
*******************************************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{
  u16 data;
  
  /* Write 16-bit Index (then Read Reg) */
  ClrCs
  ClrRs
  ClrWr
  LCD_Write(LCD_Reg);
  SetWr

  /* Read 16-bit Reg */
  SetRs
  ClrRd
  SetRd
  data = LCD_Read(); 
 SetCs
    
 return    data;
}

u16 LCD_ReadSta(void)
{
  u16 data;

  /* Write 16-bit Index, then Write Reg */
  SetRs
  ClrRd
  SetRd
  data = LCD_Read(); 
  SetCs
    
  return    data;
}

void LCD_WriteCommand(u16 LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  ClrCs
  ClrRs
  ClrWr
  LCD_Write(LCD_RegValue);
  SetWr
  SetCs
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
  /* Write 16-bit Index, then Write Reg */
  ClrCs
  ClrRs
  ClrWr
  LCD_Write(R34);
  SetWr
  SetCs
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)					 
{
  /* Write 16-bit Index, then Write Reg */
  ClrCs
  SetRs
  ClrWr
  LCD_Write(RGB_Code);
  SetWr
  SetCs
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
*******************************************************************************/
u16 LCD_ReadRAM(void)
{
  u16 dummy;
  u16 data;
  LCD_WriteRAM_Prepare();
  LCD_DB_AS_InPut();
  dummy = LCD_ReadSta();
  dummy++;
  data = LCD_ReadSta();
  LCD_DB_AS_OutPut();
  
  return    data;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
  LCD_WriteReg(0x06,Ypos>>8);
  LCD_WriteReg(0x07,Ypos);
  
  LCD_WriteReg(0x02,Xpos>>8);
  LCD_WriteReg(0x03,Xpos);  
}			 

void Delay_nms(int n)
{
  
  u32 f=n,k;
  for (; f!=0; f--)
  {
    for(k=0xFFF; k!=0; k--);
  }
  
}

void Delay(__IO u32 nCount)
{
    __IO  u32 TimingDelay; 
    while(nCount--)
    {
        for(TimingDelay=0;TimingDelay<1000;TimingDelay++);
    }
}

/****************************************************************************
* ��    �ƣ�void ili9320_Initializtion()
* ��    �ܣ���ʼ�� ILI9320 ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Initializtion();
* ��    �ߣ� www.armjishu.com TEAM
****************************************************************************/
void ili9320_Initializtion(void)
{
  u16 i;
  u8 str[] = "www.armjishu.com";
  u8 len = sizeof(str)-1;
  u16 StartX;
    
  LCD_Init();
  Delay(5); /* delay 50 ms */
  LCD_WriteReg(0x0000,0x0001);  
  Delay(5); /* delay 50 ms */			//start internal osc
  LCD_DB_AS_InPut();
  Delay(1); /* delay 50 ms */			//start internal osc
  DeviceIdCode = LCD_ReadReg(0x0000);
  printf("\n\r ###### www.armjishu.com DeviceIdCode = 0x%x ###### ", DeviceIdCode);
  //printf("\n\r ###### DeviceIdCode = LCD_ReadReg(0x0000) = 0x%x ###### ", LCD_ReadReg(0x0000));
  //printf("\n\r ###### DeviceIdCode = LCD_ReadSta(0x0000) = 0x%x ###### ", LCD_ReadSta());

  LCD_DB_AS_OutPut();
  Delay(1); /* delay 50 ms */			//start internal osc

  if(DeviceIdCode == 0x8989)
    {
       printf("\n\r This LCD is SSD1289 ");
    }
   
   //DeviceIdCode=0x8989;
 
  if(DeviceIdCode == 0x9325)
    {
       printf("\n\r This LCD is ili9325 ");
    }
  
	if(DeviceIdCode==0x9325||DeviceIdCode==0x9328)
	{
    	LCD_WriteReg(0x00e3,0x3008);
    	LCD_WriteReg(0x00e7,0x0012);
    	LCD_WriteReg(0x00ef,0x1231);//Set the internal vcore voltage
 // 		LCD_WriteReg(0x00e7,0x0010);      
        LCD_WriteReg(0x0000,0x0001);  			//start internal osc
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700); 				//power on sequence                     
        LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4) ); 	//65K 
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0207);	           
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000); 				//display setting         
        LCD_WriteReg(0x000c,0x0001);				//display setting          
        LCD_WriteReg(0x000d,0x0000); 				//0f3c          
        LCD_WriteReg(0x000f,0x0000);
        //Power On sequence //
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0010,0x1590);   
        LCD_WriteReg(0x0011,0x0227);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0012,0x009c);                  
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0013,0x1900);   
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);           
        ///////////////////////////////////////////////////////      
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0030,0x0007); 
        LCD_WriteReg(0x0031,0x0707);   
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04); 
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);        
        LCD_WriteReg(0x0039,0x0706);     
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0050,0x0000);        
        LCD_WriteReg(0x0051,0x00ef);   
        LCD_WriteReg(0x0052,0x0000);     
        LCD_WriteReg(0x0053,0x013f);
        LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
      
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0600);  
    	if(DeviceIdCode==0x9328)
        {   
               LCD_WriteReg(0x0093,0x0003);
               LCD_WriteReg(0x0095,0x0110);
               LCD_WriteReg(0x0097,0x0000);        
               LCD_WriteReg(0x0098,0x0000);  
         }
         //display on sequence     
        LCD_WriteReg(0x0007,0x0133);
    
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);
	}
	else if(DeviceIdCode==0x9320||DeviceIdCode==0x9300)
	{
        printf("\n\r This LCD is ili%x.", DeviceIdCode);
        LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	//Driver Output Contral.
		LCD_WriteReg(0x02,0x0700);	//LCD Driver Waveform Contral.
//		LCD_WriteReg(0x03,0x1030);	//Entry Mode Set.
		//LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.
		LCD_WriteReg(0x03,0x1028);	//Entry Mode Set.
	
		LCD_WriteReg(0x04,0x0000);	//Scalling Contral.
		LCD_WriteReg(0x08,0x0202);	//Display Contral 2.(0x0207)
		LCD_WriteReg(0x09,0x0000);	//Display Contral 3.(0x0000)
		LCD_WriteReg(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		LCD_WriteReg(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
		LCD_WriteReg(0x0d,0x0000);	//Frame Maker Position.
		LCD_WriteReg(0x0f,0x0000);	//Extern Display Interface Contral 2.
	
		for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x07,0x0101);	//Display Contral.
		for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
	
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		LCD_WriteReg(0x11,0x0007);								//Power Control 2.(0x0001)
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));					//Power Control 3.(0x0138)
		LCD_WriteReg(0x13,0x0b00);								//Power Control 4.
		LCD_WriteReg(0x29,0x0000);								//Power Control 7.
	
		LCD_WriteReg(0x2b,(1<<14)|(1<<4));
		
		LCD_WriteReg(0x50,0);		//Set X Start.
		LCD_WriteReg(0x51,239);	//Set X End.
		LCD_WriteReg(0x52,0);		//Set Y Start.
		LCD_WriteReg(0x53,319);	//Set Y End.
	
		LCD_WriteReg(0x60,0x2700);	//Driver Output Control.
		LCD_WriteReg(0x61,0x0001);	//Driver Output Control.
		LCD_WriteReg(0x6a,0x0000);	//Vertical Srcoll Control.
	
		LCD_WriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
		LCD_WriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		LCD_WriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
		LCD_WriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
		LCD_WriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		LCD_WriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.
	
		LCD_WriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
		LCD_WriteReg(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
		LCD_WriteReg(0x93,0x0001);	//Panel Interface Contral 3.
		LCD_WriteReg(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
		LCD_WriteReg(0x97,(0<<8));	//
		LCD_WriteReg(0x98,0x0000);	//Frame Cycle Contral.

		LCD_WriteReg(0x07,0x0173);	//(0x0173)
	}
	else if(DeviceIdCode==0x9331)
	{
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100); // set SS and SM bit   0x0100
		LCD_WriteReg(0x0002, 0x0200); // set 1 line inversion
		LCD_WriteReg(0x0003, 0x1030); // set GRAM write direction and BGR=1.     0x1030
		LCD_WriteReg(0x0008, 0x0202); // set the back porch and front porch
		LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_WriteReg(0x000A, 0x0000); // FMARK function
		LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
		LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
		LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
		//*************Power On sequence ****************//
		LCD_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
		LCD_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
		ili9320_Delay(200); // Dis-charge capacitor power voltage
		LCD_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0012, 0x000C); // Internal reference voltage= Vci;
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
		LCD_WriteReg(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
		LCD_WriteReg(0x002B, 0x000B); // Set Frame Rate
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0020, 0x0000); // GRAM horizontal Address
		LCD_WriteReg(0x0021, 0x0000); // GRAM Vertical Address
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x0030, 0x0000);
		LCD_WriteReg(0x0031, 0x0106);
		LCD_WriteReg(0x0032, 0x0000);
		LCD_WriteReg(0x0035, 0x0204);
		LCD_WriteReg(0x0036, 0x160A);
		LCD_WriteReg(0x0037, 0x0707);
		LCD_WriteReg(0x0038, 0x0106);
		LCD_WriteReg(0x0039, 0x0707);
		LCD_WriteReg(0x003C, 0x0402);
		LCD_WriteReg(0x003D, 0x0C0F);
		//------------------ Set GRAM area ---------------//
		LCD_WriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
		LCD_WriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
		LCD_WriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
		LCD_WriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
		LCD_WriteReg(0x0060, 0x2700); // Gate Scan Line
		LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV
		LCD_WriteReg(0x006A, 0x0000); // set scrolling line
		//-------------- Partial Display Control ---------//
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		//-------------- Panel Control -------------------//
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007,0x0021);		
		ili9320_Delay(50);
		LCD_WriteReg(0x0007,0x0061);
		ili9320_Delay(50);
		LCD_WriteReg(0x0007,0x0133);  // 262K color and display ON
		ili9320_Delay(50);
	}
	else if(DeviceIdCode==0x9919)
	{
		//*********POWER ON &RESET DISPLAY OFF
		 LCD_WriteReg(0x28,0x0006);
		 LCD_WriteReg(0x00,0x0001);
		 LCD_WriteReg(0x10,0x0000);
		 LCD_WriteReg(0x01,0x72ef);
		 LCD_WriteReg(0x02,0x0600);
		 LCD_WriteReg(0x03,0x6a38);
		 LCD_WriteReg(0x11,0x6874);//70
		 
		     //  RAM WRITE DATA MASK
		 LCD_WriteReg(0x0f,0x0000); 
		    //  RAM WRITE DATA MASK
		 LCD_WriteReg(0x0b,0x5308); 
		 LCD_WriteReg(0x0c,0x0003);
		 LCD_WriteReg(0x0d,0x000a);
		 LCD_WriteReg(0x0e,0x2e00);  //0030
		 LCD_WriteReg(0x1e,0x00be);
		 LCD_WriteReg(0x25,0x8000);
		 LCD_WriteReg(0x26,0x7800);
		 LCD_WriteReg(0x27,0x0078);
		 LCD_WriteReg(0x4e,0x0000);
		 LCD_WriteReg(0x4f,0x0000);
		 LCD_WriteReg(0x12,0x08d9);
		 // -----------------Adjust the Gamma Curve----//
		 LCD_WriteReg(0x30,0x0000);	 //0007
		 LCD_WriteReg(0x31,0x0104);	   //0203
		 LCD_WriteReg(0x32,0x0100);		//0001
		 LCD_WriteReg(0x33,0x0305);	  //0007
		 LCD_WriteReg(0x34,0x0505);	  //0007
		 LCD_WriteReg(0x35,0x0305);		 //0407
		 LCD_WriteReg(0x36,0x0707);		 //0407
		 LCD_WriteReg(0x37,0x0300);		  //0607
		 LCD_WriteReg(0x3a,0x1200);		 //0106
		 LCD_WriteReg(0x3b,0x0800);		 
		 LCD_WriteReg(0x07,0x0033);
	}
	else if(DeviceIdCode==0x1505)
	{
        // second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        LCD_WriteReg(0x0007,0x0000);
        ili9320_Delay(5);
        LCD_WriteReg(0x0012,0x011C);//0x011A   why need to set several times?
        LCD_WriteReg(0x00A4,0x0001);//NVM
        LCD_WriteReg(0x0008,0x000F);
        LCD_WriteReg(0x000A,0x0008);
        LCD_WriteReg(0x000D,0x0008);
       
        //GAMMA CONTROL/
        LCD_WriteReg(0x0030,0x0707);
        LCD_WriteReg(0x0031,0x0007); //0x0707
        LCD_WriteReg(0x0032,0x0603); 
        LCD_WriteReg(0x0033,0x0700); 
        LCD_WriteReg(0x0034,0x0202); 
        LCD_WriteReg(0x0035,0x0002); //?0x0606
        LCD_WriteReg(0x0036,0x1F0F);
        LCD_WriteReg(0x0037,0x0707); //0x0f0f  0x0105
        LCD_WriteReg(0x0038,0x0000); 
        LCD_WriteReg(0x0039,0x0000); 
        LCD_WriteReg(0x003A,0x0707); 
        LCD_WriteReg(0x003B,0x0000); //0x0303
        LCD_WriteReg(0x003C,0x0007); //?0x0707
        LCD_WriteReg(0x003D,0x0000); //0x1313//0x1f08
        ili9320_Delay(5);
        LCD_WriteReg(0x0007,0x0001);
        LCD_WriteReg(0x0017,0x0001);   //Power supply startup enable
        ili9320_Delay(5);
  
        //power control//
        LCD_WriteReg(0x0010,0x17A0); 
        LCD_WriteReg(0x0011,0x0217); //reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        LCD_WriteReg(0x0012,0x011E);//0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        LCD_WriteReg(0x0013,0x0F00); //VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        LCD_WriteReg(0x002A,0x0000);  
        LCD_WriteReg(0x0029,0x000A); //0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        LCD_WriteReg(0x0012,0x013E); // 0x013C  power supply on
           //Coordinates Control//
        LCD_WriteReg(0x0050,0x0000);//0x0e00
        LCD_WriteReg(0x0051,0x00EF); 
        LCD_WriteReg(0x0052,0x0000); 
        LCD_WriteReg(0x0053,0x013F); 
        //Pannel Image Control//
        LCD_WriteReg(0x0060,0x2700); 
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006A,0x0000); 
        LCD_WriteReg(0x0080,0x0000); 
        //Partial Image Control//
        LCD_WriteReg(0x0081,0x0000); 
        LCD_WriteReg(0x0082,0x0000); 
        LCD_WriteReg(0x0083,0x0000); 
        LCD_WriteReg(0x0084,0x0000); 
        LCD_WriteReg(0x0085,0x0000); 
        //Panel Interface Control//
        LCD_WriteReg(0x0090,0x0013); //0x0010 frenqucy
        LCD_WriteReg(0x0092,0x0300); 
        LCD_WriteReg(0x0093,0x0005); 
        LCD_WriteReg(0x0095,0x0000); 
        LCD_WriteReg(0x0097,0x0000); 
        LCD_WriteReg(0x0098,0x0000); 
  
        LCD_WriteReg(0x0001,0x0100); 
        LCD_WriteReg(0x0002,0x0700); 
        LCD_WriteReg(0x0003,0x1030); 
        LCD_WriteReg(0x0004,0x0000); 
        LCD_WriteReg(0x000C,0x0000); 
        LCD_WriteReg(0x000F,0x0000); 
        LCD_WriteReg(0x0020,0x0000); 
        LCD_WriteReg(0x0021,0x0000); 
        LCD_WriteReg(0x0007,0x0021); 
        ili9320_Delay(20);
        LCD_WriteReg(0x0007,0x0061); 
        ili9320_Delay(20);
        LCD_WriteReg(0x0007,0x0173); 
        ili9320_Delay(20);
	}							 
	else if(DeviceIdCode==0x8989)
	{
	    LCD_WriteReg(0x0000,0x0001);		//�򿪾���
	    LCD_WriteReg(0x0003,0xA8A4);		//0xA8A4
	    LCD_WriteReg(0x000C,0x0000);        
	    LCD_WriteReg(0x000D,0x080C);        
	    LCD_WriteReg(0x000E,0x2B00);        
	    LCD_WriteReg(0x001E,0x00B0);        
	    LCD_WriteReg(0x0001,0x2B3F);		//�����������320*240  0x693F
	    LCD_WriteReg(0x0002,0x0600);     	//LCD Driving Waveform control
	    LCD_WriteReg(0x0010,0x0000);     
	    LCD_WriteReg(0x0011,0x6070);		//0x4030	//�������ݸ�ʽ  16λɫ	���� 0x6058
	    LCD_WriteReg(0x0005,0x0000);     
	    LCD_WriteReg(0x0006,0x0000);     
	    LCD_WriteReg(0x0016,0xEF1C);     
	    LCD_WriteReg(0x0017,0x0003);     
	    LCD_WriteReg(0x0007,0x0233);		//0x0233       
	    LCD_WriteReg(0x000B,0x0000);     
	    LCD_WriteReg(0x000F,0x0000);		//ɨ�迪ʼ��ַ
	    LCD_WriteReg(0x0041,0x0000);     
	    LCD_WriteReg(0x0042,0x0000);     
	    LCD_WriteReg(0x0048,0x0000);     
	    LCD_WriteReg(0x0049,0x013F);     
	    LCD_WriteReg(0x004A,0x0000);     
	    LCD_WriteReg(0x004B,0x0000);     
	    LCD_WriteReg(0x0044,0xEF00);     
	    LCD_WriteReg(0x0045,0x0000);     
	    LCD_WriteReg(0x0046,0x013F);     
	    LCD_WriteReg(0x0030,0x0707);     
	    LCD_WriteReg(0x0031,0x0204);     
	    LCD_WriteReg(0x0032,0x0204);     
	    LCD_WriteReg(0x0033,0x0502);     
	    LCD_WriteReg(0x0034,0x0507);     
	    LCD_WriteReg(0x0035,0x0204);     
	    LCD_WriteReg(0x0036,0x0204);     
	    LCD_WriteReg(0x0037,0x0502);     
	    LCD_WriteReg(0x003A,0x0302);     
	    LCD_WriteReg(0x003B,0x0302);     
	    LCD_WriteReg(0x0023,0x0000);     
	    LCD_WriteReg(0x0024,0x0000);     
	    LCD_WriteReg(0x0025,0x8000);     
	    LCD_WriteReg(0x004e,0);        //��(X)��ַ0
	    LCD_WriteReg(0x004f,0);        //��(Y)��ַ0
	}
    else
    {
       printf("\n\r ###### Err: Unknow DeviceIdCode 0x%x ###### ", DeviceIdCode);
    }

    ili9320_Delay(10);

    ili9320_Clear(Green);
    StartX = (320 - 16*len)/2;
    for (i=0;i<len;i++)
    {
        ili9320_PutChar_16x24((StartX+16*i),100,str[i],Yellow, Red);
    }

    ili9320_Delay(2000);
}

/****************************************************************************
* ��    �ƣ�void ili9320_SetCursor(u16 x,u16 y)
* ��    �ܣ�������Ļ����
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_SetCursor(10,10);
****************************************************************************/
inline void ili9320_SetCursor(u16 x,u16 y)
{
	if(DeviceIdCode==0x8989)
	{
	 	LCD_WriteReg(0x004e,y);        //��
    	LCD_WriteReg(0x004f,0x13f-x);  //��
	}
	else if((DeviceIdCode==0x9919))
	{
		LCD_WriteReg(0x004e,x); // ��
  		LCD_WriteReg(0x004f,y); // ��	
	}
    /*
	else if((DeviceIdCode==0x9325))
	{
		LCD_WriteReg(0x0020,x); // ��
  		LCD_WriteReg(0x0021,y); // ��	
	}
	*/
	else
	{
  		LCD_WriteReg(0x0020,y); // ��
  		LCD_WriteReg(0x0021,0x13f-x); // ��
	}
}

/****************************************************************************
* ��    �ƣ�void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* ��    �ܣ����ô�������
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_SetWindows(0,0,100,100)��
****************************************************************************/
inline void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  ili9320_SetCursor(StartX,StartY);
  LCD_WriteReg(0x0050, StartX);
  LCD_WriteReg(0x0052, StartY);
  LCD_WriteReg(0x0051, EndX);
  LCD_WriteReg(0x0053, EndY);
}

/****************************************************************************
* ��    �ƣ�void ili9320_Clear(u16 dat)
* ��    �ܣ�����Ļ����ָ������ɫ��������������� 0xffff
* ��ڲ�����dat      ���ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Clear(0xffff);
****************************************************************************/
void ili9320_Clear(u16 Color)
{
  u32 index=0;
  ili9320_SetCursor(0,0); 
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index=0;index<76800;index++)
   {
     LCD_WriteRAM(Color);
   }
}

/****************************************************************************
* ��    �ƣ�u16 ili9320_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡָ���������ɫֵ
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������ǰ������ɫֵ
* ˵    ����
* ���÷�����i=ili9320_GetPoint(10,10);
****************************************************************************/
u16 ili9320_GetPoint(u16 x,u16 y)
{
  ili9320_SetCursor(x,y);
  return (ili9320_BGR2RGB(LCD_ReadRAM()));
}
/****************************************************************************
* ��    �ƣ�void ili9320_SetPoint(u16 x,u16 y,u16 point)
* ��    �ܣ���ָ�����껭��
* ��ڲ�����x      ������
*           y      ������
*           point  �����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void ili9320_SetPoint(u16 x,u16 y,u16 point)
{
  if ( (x>320)||(y>240) ) return;
  ili9320_SetCursor(x,y);

  LCD_WriteRAM_Prepare();
  LCD_WriteRAM(point);
}

/****************************************************************************
* ��    �ƣ�void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
            pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ
* ���÷�����ili9320_DrawPicture(0,0,100,100,(u16*)demo);
****************************************************************************/
void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
{
  u32  i, total;
  u16 data1,data2,data3;
  u16 *picturepointer = pic;
  //ili9320_SetWindows(StartX,StartY,EndX,EndY);

  LCD_WriteReg(0x0003,(1<<12)|(0<<5)|(1<<4) ); 

  ili9320_SetCursor(StartX,StartY);
  
  LCD_WriteRAM_Prepare();
  total = (EndX + 1)*(EndY + 1 ) / 2;
  for (i=0;i<total;i++)
  {
      data1 = *picturepointer++;
      data2 = *picturepointer++;
      data3 = ((data1 >>3)& 0x001f) |((data1>>5) & 0x07E0) | ((data2<<8) & 0xF800);
      LCD_WriteRAM(data3);
      data1 = data2;
      data2 = *picturepointer++;
      data3 = ((data1 >>11)& 0x001f) |((data2<<3) & 0x07E0) | ((data2) & 0xF800);
      LCD_WriteRAM(data3);
  }

  LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4) ); 
}
/*
void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
{
  u32  i, total;
  ili9320_SetWindows(StartX,StartY,EndX,EndY);
  ili9320_SetCursor(StartX,StartY);
  
  LCD_WriteRAM_Prepare();
  total = EndX*EndY;
  for (i=0;i<total;i++)
  {
      LCD_WriteRAM(*pic++);
  }
}
*/
/****************************************************************************
* ��    �ƣ�void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)  // Lihao
{
  u16 i=0;
  u16 j=0;
  
  u8 tmp_char=0;
  
  if(HyalineBackColor == bkColor)
  {
    for (i=0;i<16;i++)
    {
      tmp_char=ascii_8x16[((c-0x20)*16)+i];
      for (j=0;j<8;j++)
      {
        if ( (tmp_char >> 7-j) & 0x01 == 0x01)
          {
            ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
          }
          else
          {
            // do nothing // ͸������
          }
      }
    }
  }
  else
  {
        for (i=0;i<16;i++)
    {
      tmp_char=ascii_8x16[((c-0x20)*16)+i];
      for (j=0;j<8;j++)
      {
        if ( (tmp_char >> 7-j) & 0x01 == 0x01)
          {
            ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
          }
          else
          {
            ili9320_SetPoint(x+j,y+i,bkColor); // ������ɫ
          }
      }
    }
  }
}

/****************************************************************************
* ��    �ƣ�void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void ili9320_PutChar_16x24(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{

  u16 i=0;
  u16 j=0;
  
  u16 tmp_char=0;

  if(HyalineBackColor == bkColor)
  {
    for (i=0;i<24;i++)
    {
      tmp_char=ASCII_Table_16x24[((c-0x20)*24)+i];
      for (j=0;j<16;j++)
      {
        if ( (tmp_char >> j) & 0x01 == 0x01)
          {
            ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
          }
          else
          {
              // do nothing // ͸������
          }
      }
    }
  }
  else
  {
    for (i=0;i<24;i++)
    {
      tmp_char=ASCII_Table_16x24[((c-0x20)*24)+i];
      for (j=0;j<16;j++)
      {
        if ( (tmp_char >> j) & 0x01 == 0x01)
          {
            ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
          }
          else
          {
            ili9320_SetPoint(x+j,y+i,bkColor); // ������ɫ
          }
      }
    }
  }
}
/****************************************************************************
* ��    �ƣ�u16 ili9320_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
u16 ili9320_BGR2RGB(u16 c)
{
  u16  r, g, b, rgb;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  rgb =  (b<<11) + (g<<5) + (r<<0);

  return( rgb );
}

/****************************************************************************
* ��    �ƣ�void ili9320_BackLight(u8 status)
* ��    �ܣ�������Һ������
* ��ڲ�����status     1:���⿪  0:�����
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_BackLight(1);
****************************************************************************/
void ili9320_BackLight(u8 status)
{
  if ( status >= 1 )
  {
    Lcd_Light_ON;
  }
  else
  {
    Lcd_Light_OFF;
  }
}

/****************************************************************************
* ��    �ƣ�void ili9320_Delay(vu32 nCount)
* ��    �ܣ���ʱ
* ��ڲ�����nCount   ��ʱֵ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Delay(10000);
****************************************************************************/
void ili9320_Delay(vu32 nCount)
{
   Delay(nCount);
  //for(; nCount != 0; nCount--);
}
