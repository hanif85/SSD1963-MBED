#include "stm32f7xx.h"
#include "SSD1963_Driver.h"
#include "stdbool.h"

/*****************************************************************************
 *  Module for Microchip Graphics Library
 *  Solomon Systech. SSD1963 LCD controller driver
 *****************************************************************************
 * FileName:        SSD1963.c
 * Dependencies:    Graphics.h
 * Processor:       PIC24, PIC32
 * Compiler:       	MPLAB C30, MPLAB C32
 * Linker:          MPLAB LINK30, MPLAB LINK32
 * Company:			TechToys Company
 * Remarks:			The origin of this file was the ssd1926.c driver released
 *					by Microchip Technology Incorporated. 
 *
 * Software License Agreement as below:
 *
 * Company:         Microchip Technology Incorporatedpage

 *
 * Software License Agreement
 *
 * Copyright ?2008 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED ?S IS?WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Anton Alkhimenok     08/27/08	(Original)
 * 	
 *****************************************************************************/

// _Cs, _Rs, _nWr, _nRd, _Rst;


// Color
u16  _color;
// Clipping region control
u16 _clipRgn;
// Clipping region borders
u16 _clipLeft;
u16 _clipTop;
u16 _clipRight;
u16 _clipBottom;

// Active Page
u8  _activePage;
// Visual Page
u8  _visualPage;

// ssd1963 specific
u8 _gpioStatus = 0;
dataMap mapLCD;
// void SetArea(u16 start_x, u16 start_y, u16 end_x, u16 end_y);
// u16 SSD1963_Driver::rgbColor=0;

void SSD1963_Driver::setCs()
{
	_Cs=1;
}
void SSD1963_Driver::clrCs()
{
	_Cs=0;
}
void SSD1963_Driver::setRs()
{
	_Rs=1;
}
void SSD1963_Driver::clrRs()
{
	_Rs=0;
}
void SSD1963_Driver::setnWr()
{
	_nWr=1;
}
void SSD1963_Driver::clrnWr()
{
	_nWr=0;
}

void SSD1963_Driver::setnRd()
{
	_nRd=1;
}
void SSD1963_Driver::clrnRd()
{
	_nRd=0;
}
void SSD1963_Driver::setRst()
{
	_Rst=1;
}
void SSD1963_Driver::clrRst()
{
	_Rst=1;
}

/*********************************************************************
* Macros:  PMPWaitBusy()
*
* Overview: waits for PMP cycle end.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Note: 
********************************************************************/
#define PMPWaitBusy()  Nop();




SSD1963_Driver::SSD1963_Driver(PinName Cs, PinName Rs, PinName nWr, PinName nRd, PinName Rst, PinName BL, BusOut *data):_Cs(Cs),_Rs(Rs),_nWr(nWr),_nRd(nRd), _Rst(Rst), _BL(BL)
{
	_data = data;
	InitDevice();
	setBL(1);

}
SSD1963_Driver::~SSD1963_Driver()
{

}

void SSD1963_Driver::setBL(bool b)
{
	_BL = b;
}
/*********************************************************************
* Function:  SetArea(start_x,start_y,end_x,end_y)
*
* PreCondition: SetActivePage(page)
*
* Input: start_x, end_x	- start column and end column
*		 start_y,end_y 	- start row and end row position (i.e. page address)
*
* Output: none
*
* Side Effects: none
*
* Overview: defines start/end columns and start/end rows for memory access
*			from host to SSD1963
* Note: none
********************************************************************/

void SSD1963_Driver::SetArea(u16 start_x, u16 start_y, u16 end_x, u16 end_y)
{
	long offset;

	offset = (u16)_activePage*(GetMaxY()+1);

	start_y = offset + start_y;
	end_y   = offset + end_y;



	WriteCommand(CMD_SET_COLUMN);
	Clr_Cs;
	WriteData(start_x>>8);
	WriteData(start_x);
	WriteData(end_x>>8);
	WriteData(end_x);
	Set_Cs;
	WriteCommand(CMD_SET_PAGE);
	Clr_Cs;
	WriteData(start_y>>8);
	WriteData(start_y);
	WriteData(end_y>>8);
	WriteData(end_y);
	Set_Cs;
}

/*********************************************************************
* Function:  SetScrollArea(SHORT top, SHORT scroll, SHORT bottom)
*
* PreCondition: none
*
* Input: top - Top Fixed Area in number of lines from the top
*				of the frame buffer
*		 scroll - Vertical scrolling area in number of lines
*		 bottom - Bottom Fixed Area in number of lines
*
* Output: none
*
* Side Effects: none
*
* Overview:
*
* Note: Reference: section 9.22 Set Scroll Area, SSD1963 datasheet Rev0.20
********************************************************************/
void SSD1963_Driver::SetScrollArea(u16 top, u16 scroll, u16 bottom)
{
	WriteCommand(CMD_SET_SCROLL_AREA);
	Clr_Cs;
	WriteData(top>>8);
	WriteData(top);
	WriteData(scroll>>8);
	WriteData(scroll);
	WriteData(bottom>>8);
	WriteData(bottom);
	Set_Cs;	
}

/*********************************************************************
* Function:  void  SetScrollStart(SHORT line)
*
* Overview: First, we need to define the scrolling area by SetScrollArea()
*			before using this function. 
*
* PreCondition: SetScrollArea(SHORT top, SHORT scroll, SHORT bottom)
*
* Input: line - Vertical scrolling pointer (in number of lines) as 
*		 the first display line from the Top Fixed Area defined in SetScrollArea()
*
* Output: none
*
* Note: Example -
*
*		SHORT line=0;
*		SetScrollArea(0,272,0);
*		for(line=0;line<272;line++) {SetScrollStart(line);Delay(100);}
*		
*		Code above scrolls the whole page upwards in 100ms interval 
*		with page 2 replacing the first page in scrolling
********************************************************************/
void SSD1963_Driver::SetScrollStart(u16 line)
{
	WriteCommand(CMD_SET_SCROLL_START);
	Clr_Cs;
	WriteData(line>>8);
	WriteData(line);
	Set_Cs;
}

/*********************************************************************
* Function:  void EnterSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 enters sleep mode
* Note: Host must wait 5mS after sending before sending next command
********************************************************************/
void SSD1963_Driver::EnterSleepMode (void)
{
	WriteCommand(CMD_ENT_SLEEP);
}

/*********************************************************************
* Function:  void ExitSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 enters sleep mode
* Note:   none
********************************************************************/
void SSD1963_Driver::ExitSleepMode (void)
{
	WriteCommand(CMD_EXIT_SLEEP);
}

/*********************************************************************
* Function		: void DisplayOff(void)
* PreCondition	: none
* Input			: none
* Output		: none
* Side Effects	: none
* Overview		: SSD1963 changes the display state to OFF state
* Note			: none
********************************************************************/
void SSD1963_Driver::DisplayOff(void)
{
    WriteCommand(CMD_BLANK_DISPLAY);
}
/*********************************************************************
* Function		: void DisplayOn(void)
* PreCondition	: none
* Input			: none
* Output		: none
* Side Effects	: none
* Overview		: SSD1963 changes the display state to ON state
* Note			: none
********************************************************************/
void SSD1963_Driver::DisplayOn(void)
{
    WriteCommand(CMD_ON_DISPLAY);
}
/*********************************************************************
* Function		: void EnterDeepSleep(void)
* PreCondition	: none
* Input			: none
* Output		: none
* Side Effects	: none
* Overview		: SSD1963 enters deep sleep state with PLL stopped
* Note			: none
********************************************************************/
void SSD1963_Driver::EnterDeepSleep(void)
{
    WriteCommand(CMD_ENT_DEEP_SLEEP);
}

/*********************************************************************
* Function:  void  SetBacklight(BYTE intensity)
*
* Overview: This function makes use of PWM feature of ssd1963 to adjust
*			the backlight intensity. 
*
* PreCondition: Backlight circuit with shutdown pin connected to PWM output of ssd1963.
*
* Input: 	(BYTE) intensity from 
*			0x00 (total backlight shutdown, PWM pin pull-down to VSS)
*			0xff (99% pull-up, 255/256 pull-up to VDD)
*
* Output: none
*
* Note: The base frequency of PWM set to around 300Hz with PLL set to 120MHz.
*		This parameter is hardware dependent
********************************************************************/
void SSD1963_Driver::SetBacklight(u8 intensity)
{
	WriteCommand(0xBE);			// Set PWM configuration for backlight control
	Clr_Cs;
	WriteData(0x0E);			// PWMF[7:0] = 2, PWM base freq = PLL/(256*(1+5))/256 =
								// 300Hz for a PLL freq = 120MHz
	WriteData(intensity);		// Set duty cycle, from 0x00 (total pull-down) to 0xFF
								// (99% pull-up , 255/256)
	WriteData(0x01);			// PWM enabled and controlled by host (mcu)
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);

	Set_Cs;
}

/*********************************************************************
* Function:  void  SetTearingCfg(BOOL state, BOOL mode)
*
* Overview: This function enable/disable tearing effect
*
* PreCondition: none
*
* Input: 	BOOL state -	1 to enable
*							0 to disable
*			BOOL mode -		0:  the tearing effect output line consists
*								of V-blanking information only
*							1:	the tearing effect output line consists
*								of both V-blanking and H-blanking info.
* Output: none
*
* Note:
********************************************************************/
void SSD1963_Driver::SetTearingCfg(bool state, bool mode)
{


	if(state == 1)
	{
		WriteCommand(0x35);
		Clr_Cs;
		WriteData(mode&0x01);
		Set_Cs;
	}
	else
	{
		WriteCommand(0x34);
	}


}

void SSD1963_Driver::WriteCommand(u16 cmd)
{
	Clr_Rs;
	mapLCD.data = cmd;
	_data->write(mapLCD.data);
	// mapLCD.data=cmd;
	// wait_ns(100);
	wait_us(100);
	
	Clr_Cs; Clr_nWr;  Set_nWr; Set_Cs;
}

void SSD1963_Driver::WriteData(u16 dat)
{
	Set_Rs;
	mapLCD.data=dat;
	_data->write(mapLCD.data);
	wait_ns(10);
	
	Clr_nWr; 
	
	Set_nWr;
}

/*********************************************************************
* Function:  void ResetDevice()
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: Resets LCD, initializes PMP
*			Initialize low level IO port for mcu,
*			initialize SSD1963 for PCLK,
*			HSYNC, VSYNC etc
*
* Note: Need to set the backlight intensity by SetBacklight(BYTE intensity)
*		in main()
*
********************************************************************/
void SSD1963_Driver::InitDevice(void)
{	
    Set_Rst;
	
	Set_Cs;        	// SSD1963 is not selected by default
	Set_nRd;
	Set_nWr;
	wait_ms(1);	
	Clr_Rst;
	wait_ms(1);
	Set_Rst;       // release from reset state to sleep state

	//Set MN(multipliers) of PLL, VCO = crystal freq * (N+1)
	//PLL freq = VCO/M with 250MHz < VCO < 800MHz
	//The max PLL freq is around 120MHz. To obtain 120MHz as the PLL freq



	WriteCommand(0xE2);				// Set PLL with OSC = 10MHz (hardware)
	wait_ms(1);								// Multiplier N = 35, VCO (>250MHz)= OSC*(N+1), VCO = 360MHz
	Clr_Cs;
	WriteData(0x23);
	WriteData(0x02);				// Divider M = 2, PLL = 360/(M+1) = 120MHz
	WriteData(0x54);			// Validate M and N values
	Set_Cs;

	WriteCommand(0xE0);	
	wait_ms(1);				// Start PLL command
	Clr_Cs;
	WriteData(0x01);				// enable PLL
	Set_Cs;

	wait_ms(1);						// wait stablize
	WriteCommand(0xE0);	
	wait_ms(1);				// Start PLL command again
	Clr_Cs;
	WriteData(0x03);				// now, use PLL output as system clock
	Set_Cs;

	WriteCommand(0x01);				// Soft reset
	wait_ms(1);

	//Set LSHIFT freq, i.e. the DCLK with PLL freq 120MHz set previously
	//Typical DCLK for TY700TFT800480 is 33.3MHz(datasheet), experiment shows 30MHz gives a stable result
	//30MHz = 120MHz*(LCDC_FPR+1)/2^20
	//LCDC_FPR = 262143 (0x3FFFF)
	//Time per line = (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)/30 us = 1056/30 = 35.2us
	WriteCommand(0xE6);
	wait_ms(1);	
	Clr_Cs;
	WriteData(0x03);//WriteData(0x04);
	WriteData(0xff);
	WriteData(0xff);

	Set_Cs;

	//Set panel mode, varies from individual manufacturer
	WriteCommand(0xB0);
	wait_ms(1);	
	Clr_Cs;
	WriteData(0x10);				// set 18-bit for 7" panel TY700TFT800480
	WriteData(0x80);				// set TTL mode
	WriteData((DISP_HOR_RESOLUTION-1)>>8); //Set panel size

	WriteData(DISP_HOR_RESOLUTION-1);
	WriteData((DISP_VER_RESOLUTION-1)>>8);
	WriteData(DISP_VER_RESOLUTION-1);
	WriteData(0x00);				//RGB sequence
	Set_Cs;


	//Set horizontal period
	WriteCommand(0xB4);
	wait_ms(1);	
	#define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
	Clr_Cs;
	WriteData((HT-1)>>8);
	WriteData(HT-1);
	#define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
	WriteData((HPS-1)>>8);
	WriteData(HPS-1);
	WriteData(DISP_HOR_PULSE_WIDTH-1);
	WriteData((LPS>>8)&0X00FF);  //Set HPS
	WriteData(LPS&0X00FF);
	WriteData(0x0000);

	WriteData(0x0000);

	Set_Cs;
	//Set vertical period
	WriteCommand(0xB6);
	wait_ms(1);	
	#define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
	Clr_Cs;
	WriteData((VT-1)>>8);
	WriteData(VT-1);
	#define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)
	WriteData((VSP-1)>>8);
	WriteData(VSP-1);
	WriteData(DISP_VER_PULSE_WIDTH-1);
	WriteData((FPS>>8)&0X00FF);  //Set FPS
	WriteData(FPS&0X00FF);

	WriteData(0x0000);
	WriteData(0x0000);

	Set_Cs;
	wait_ms(1);

	WriteCommand(0xBA);
	wait_ms(1);	
	Clr_Cs;
	WriteData(0x0E);    //GPIO[3:0] out 1
	Set_Cs;
	wait_ms(1);

	WriteCommand(0xB8);
	wait_ms(1);	
	Clr_Cs;
	WriteData(0x07);    //GPIO3=input, GPIO[2:0]=output
	WriteData(0x01);    //GPIO0 normal
	Set_Cs;
	//rotation
	WriteCommand(0x36);
	wait_ms(1);	
	Clr_Cs;
	WriteData(0);//rotation setting be carefull make string mirror
    Set_Cs;

	WriteCommand(0x0B);//horizontal direction
	wait_ms(1);	
	Clr_Cs;
	WriteData(0x00FF);
    Set_Cs;

	//Set pixel format, i.e. the bpp
	WriteCommand(0x3A);//pixel data interface
	wait_ms(1);
	Clr_Cs;
	WriteData(0x50);
	Set_Cs;

	//Set pixel data interface
	WriteCommand(0xF0);
	wait_ms(1);	
	Clr_Cs;
	WriteData(0x03);				//16-bit(565 format) data for 16bpp PIC32MX only
	/*WriteData(0x00);				//8-bit data for 16bpp, PIC24 series
	Set_Cs;*/
	WriteCommand(0x26); //pixel data interface
	wait_ms(1);	
	Clr_Cs;
	WriteData(0x08);
	Set_Cs;
	Set_Cs;
	WriteCommand(0x29);
	wait_ms(1);	


	WriteCommand(0xd0);
	wait_ms(1);	
	Clr_Cs;
	WriteData(0x0d);// Turn on display; show the image on display
	Set_Cs;
	WriteCommand(0x2C);
	wait_ms(1);	
}



/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* PreCondition: none
*
* Input: x,y - pixel coordinates
*
* Output: none
*
* Side Effects: none
*
* Overview: puts pixel
*
* Note: 
********************************************************************/
void SSD1963_Driver::PutPixel(u16 x, u16 y, u16 color)
{
	if(_clipRgn){
		if(x<_clipLeft)
			return;
		if(x>_clipRight)
			return;
		if(y<_clipTop)
			return;
		if(y>_clipBottom)
			return;
	}

	SetArea(x,y,GetMaxX(),GetMaxY());
	WriteCommand(CMD_WR_MEMSTART);
	Clr_Cs;
	WriteData(color);
	Set_Cs;
}

/*********************************************************************
* Function: void ClearDevice(void)
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
* Overview: clears screen with current color 
*
* Note: none
*
********************************************************************/
void SSD1963_Driver::Clear(u16 color)
{
	u32     counter;
	u32 xcounter, ycounter;

	SetArea(0,0,GetMaxX(),GetMaxY());

	WriteCommand(CMD_WR_MEMSTART);

	Clr_Cs;
	for(ycounter=0;ycounter<GetMaxY()+1;ycounter++)
	{
		for(xcounter=0;xcounter<GetMaxX()+1;xcounter++)
		{
			WriteData(color);
		}
	}

	Set_Cs;
}
/*********************************************************************
* RGB565CONVERT(red, green, blue)
*
* Overview: Converts true color into 5:6:5 RGB format.
*
* PreCondition: none
*
* Input: Red, Green, Blue components.
*
* Output: 5 bits red, 6 bits green, 5 bits blue color.
*
* Side Effects: none
*
********************************************************************/
// u16 SSD1963_Driver::RGB565CONVERT(u8 r, u8 g, u8 b) 
// {
	
// 	return  rgbColor=(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
	
// }

void SSD1963_Driver:: Delay(u32 nCount)
{
//	HAL_Delay(nCount);
  nCount = nCount * 1000;
  for(; nCount != 0; nCount--);
}

void SSD1963_Driver::DelayUs(u32 nCount)
{
  for(; nCount != 0; nCount--);
}
