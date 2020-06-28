

#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "SSD1963_Configuration.h"
#include "stm32f7xx_hal_gpio.h"
#include "mbed.h"


#ifndef _SSD1963_H
#define _SSD1963_H

//include the command table for SSD1963
#include "SSD1963_CMD.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef long long             s64;
typedef unsigned long long    u64;


#define HDP 799

// #define HT 928
// #define HPS 46
#define LPS 15
#define HPW 48

#define VDP 479

// #define VT 525
#define VPS 16
#define FPS 8
#define VPW 16



/*********************************************************************
* PARAMETERS VALIDATION
*********************************************************************/
#if COLOR_DEPTH != 16
#error This driver supports 16 BPP only.
#endif

#if (DISP_HOR_RESOLUTION % 8) != 0
#error Horizontal resolution must be divisible by 8.
#endif

#if (DISP_ORIENTATION != 0) && (DISP_ORIENTATION != 180) && (DISP_ORIENTATION != 90) && (DISP_ORIENTATION != 270)
#error The display orientation selected is not supported. It can be only 0,90,180 or 270.
#endif

/*********************************************************************
* Overview: Clipping region control codes to be used with SetClip(...)
*           function. 
*********************************************************************/
#define CLIP_DISABLE       0 	// Disables clipping.
#define CLIP_ENABLE        1	// Enables clipping.


/*********************************************************************
* Overview: Clipping region control and border settings.
*********************************************************************/
// Clipping region enable control
extern u16 _clipRgn;

// Left clipping region border
extern u16 _clipLeft;
// Top clipping region border
extern u16 _clipTop;
// Right clipping region border
extern u16 _clipRight;
// Bottom clipping region border
extern u16 _clipBottom;


// Active and Visual Pages 
extern u8 _activePage;
extern u8	_visualPage;
//u16 dataLCD;
typedef struct{
    bool bit0:1;
    bool bit1:1;
    bool bit2:1;
    bool bit3:1;
    bool bit4:1;
    bool bit5:1;
    bool bit6:1;
    bool bit7:1;
    bool bit8:1;
    bool bit9:1;
    bool bit10:1;
    bool bit11:1;
    bool bit12:1;
    bool bit13:1;
    bool bit14:1;
    bool bit15:1;
}bitmaps;
typedef  union {
    uint16_t data;
    bitmaps bits;
}dataMap;
typedef struct {
	u16 dataLcd;
}lcd;

// lcd buff,send;
extern dataMap mapLCD;


/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/
// #define BLACK               {0,    0,      0}
    #define RGB565CONVERT(red, green, blue) ((u16)(((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3)))
    

/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/
#define BLACK               RGB565CONVERT(0,    0,      0)
#define BRIGHTBLUE          RGB565CONVERT(0,    0,      255)
#define BRIGHTGREEN         RGB565CONVERT(0,    255,    0)
#define BRIGHTCYAN          RGB565CONVERT(0,    255,    255)
#define BRIGHTRED           RGB565CONVERT(255,  0,      0)
#define BRIGHTMAGENTA       RGB565CONVERT(255,  0,      255)
#define BRIGHTYELLOW        RGB565CONVERT(255,  255,    0)
#define BLUE                RGB565CONVERT(0,    0,      128)
#define GREEN               RGB565CONVERT(0,    128,    0)
#define CYAN                RGB565CONVERT(0,    128,    128)
#define RED                 RGB565CONVERT(128,  0,      0)
#define MAGENTA             RGB565CONVERT(128,  0,      128)
#define BROWN               RGB565CONVERT(255,  128,    0)
#define LIGHTGRAY           RGB565CONVERT(128,  128,    128)
#define DARKGRAY            RGB565CONVERT(64,   64,     64)
#define LIGHTBLUE           RGB565CONVERT(128,  128,    255)
#define LIGHTGREEN          RGB565CONVERT(128,  255,    128)
#define LIGHTCYAN           RGB565CONVERT(128,  255,    255)
#define LIGHTRED            RGB565CONVERT(255,  128,    128)
#define LIGHTMAGENTA        RGB565CONVERT(255,  128,    255)
#define YELLOW              RGB565CONVERT(255,  255,    128)
#define WHITE               RGB565CONVERT(255,  255,    255)
                            
#define GRAY0       	    RGB565CONVERT(224,  224,    224)
#define GRAY1         	    RGB565CONVERT(192,  192,    192)   
#define GRAY2               RGB565CONVERT(160,  160,    160)   
#define GRAY3               RGB565CONVERT(128,  128,    128)
#define GRAY4               RGB565CONVERT(96,   96,     96)
#define GRAY5               RGB565CONVERT(64,   64,     64)
#define GRAY6	            RGB565CONVERT(32,   32,     32)


/*********************************************************************
* Macros:  GetMaxX()
*
* Overview: Returns maximum horizontal coordinate for visual display
*
* PreCondition: none
*
* Input: none
*
* Output: Maximum horizontal coordinates.
*
* Side Effects: none
*
* Note: 
********************************************************************/
#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

#define	GetMaxX() 			(DISP_VER_RESOLUTION-1)
	

#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

#define	GetMaxX() 			(DISP_HOR_RESOLUTION-1)

#endif

/*********************************************************************
* Macros:  GetMaxY()
*
* Overview: Returns maximum vertical coordinate for visual display
*			
* PreCondition: none
*
* Input: none
*
* Output: Maximum vertical coordinates.
*
* Side Effects: none
*
* Note: 
********************************************************************/
#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

#define	GetMaxY() 			(DISP_HOR_RESOLUTION-1)
#define ORIENTATION_REGISTER	0x0060  // Vertical rotation	 - A[5]=1; A[6]=0; A[7]=1;

#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

#define	GetMaxY() 			(DISP_VER_RESOLUTION-1)
#define ORIENTATION_REGISTER	0x0000  // Horizontal rotation   - A[5]=0; A[6]=0; A[7]=0;

#endif

// Definition for memory page size with virtual display taken into account
// The maximum memory space of SSD1928 is 262kbyte
#ifndef PAGE_MEM_SIZE
// Memory allocation for a single page 
#define PAGE_MEM_SIZE 	(GetMaxX()+1)*(GetMaxY()+1)*COLOR_DEPTH/8

#if PAGE_MEM_SIZE > 1215000ul	//this is the memory boundary of ssd1963
#error MEMORY ALLOCATION FAILED. CHECK SSD1963.h
#endif

#endif

/*********************************************************************
* Macros:  SetActivePage(page)
*
* Overview: Sets active graphic page.
*
* PreCondition: none
*
* Input: page - Graphic page number.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetActivePage(page)	_activePage = page

/*********************************************************************
* Macros: SetVisualPage(page)
*
* Overview: Sets graphic page to display.
*
* PreCondition: none
*
* Input: page - Graphic page number
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
//debug
#define SetVisualPage(page) {_visualPage = page; \
SetScrollArea(0,GetMaxY()+1,0); \
SetScrollStart((SHORT)_visualPage*(GetMaxY()+1));}
//debug
/*********************************************************************
* Macros: SetClipRgn(left, top, right, bottom)
*
* Overview: Sets clipping region.
*
* PreCondition: none
*
* Input: left - Defines the left clipping region border.
*		 top - Defines the top clipping region border.
*		 right - Defines the right clipping region border.
*	     bottom - Defines the bottom clipping region border.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetClipRgn(left,top,right,bottom) _clipLeft=left; _clipTop=top; _clipRight=right; _clipBottom=bottom;

/*********************************************************************
* Macros: GetClipLeft()
*
* Overview: Returns left clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Left clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipLeft() _clipLeft

/*********************************************************************
* Macros: GetClipRight()
*
* Overview: Returns right clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Right clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipRight() _clipRight

/*********************************************************************
* Macros: GetClipTop()
*
* Overview: Returns top clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Top clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipTop() _clipTop

/*********************************************************************
* Macros: GetClipBottom()
*
* Overview: Returns bottom clipping border.
*
* PreCondition: none
*
* Input: none
*
* Output: Bottom clipping border.
*
* Side Effects: none
*
********************************************************************/
#define GetClipBottom() _clipBottom

/*********************************************************************
* Macros: SetClip(control)
*
* Overview: Enables/disables clipping.
*
* PreCondition: none
*
* Input: control - Enables or disables the clipping.
*			- 0: Disable clipping
*			- 1: Enable clipping
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetClip(control) _clipRgn=control;

/*********************************************************************
* Macros: IsDeviceBusy()
*
* Overview: Returns non-zero if LCD controller is busy 
*           (previous drawing operation is not completed).
*
* PreCondition: none
*
* Input: none
*
* Output: Busy status.
*
* Side Effects: none
*
********************************************************************/
#define IsDeviceBusy()  0

/*********************************************************************
* Macros: SetPalette(colorNum, color)
*
* Overview:  Sets palette register.
*
* PreCondition: none
*
* Input: colorNum - Register number.
*        color - Color.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetPalette(colorNum, color)

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
*
********************************************************************/
/************************************************************************
* Macro: Lo                                                             *
* Preconditions: None                                                   *
* Overview: This macro extracts a low byte from a 2 byte word.          *
* Input: None.                                                          *
* Output: None.                                                         *
************************************************************************/
#define Lo(X)   (u8)(X&0x00ff)

/************************************************************************
* Macro: Hi                                                             *
* Preconditions: None                                                   *
* Overview: This macro extracts a high byte from a 2 byte word.         *
* Input: None.                                                          *
* Output: None.                                                         *
************************************************************************/
#define Hi(X)   (u8)((X>>8)&0x00ff)



/*********************************************************************
* Macros:  WriteCommand(cmd)
*
* PreCondition: 
*
* Input: cmd - controller command
*
* Output: none
*
* Side Effects: none
*
* Overview: writes command
*
* Note: none
********************************************************************/
class SSD1963_Driver
{
private:
    /* data */
public:
    SSD1963_Driver(PinName Cs, PinName Rs, PinName nWr, PinName nRd, PinName Rst, PinName BL, BusOut *data);
    ~SSD1963_Driver();
    void Clear(u16 color);
    void EnterDeepSleep(void);
    void setBL(bool b);



/*********************************************************************
* Function:  void  WriteData(WORD data)
*
* PreCondition: 
*
* Input:  value - value to be written in WORD format
*
* Output: none
*
* Side Effects: none
*
* Overview: 
********************************************************************/

    void WriteCommand(u16 cmd);
    void WriteData(u16 dat);

/*********************************************************************
* Function:  void InitDevice()
*
* Overview: Initializes LCD module.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    void InitDevice(void);

/*********************************************************************
* Function: void PutPixel(SHORT x, SHORT y)
*
* Overview: Puts pixel with the given x,y coordinate position.
*
* PreCondition: none
*
* Input: x - x position of the pixel.
*		 y - y position of the pixel.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    void PutPixel(u16 x, u16 y, u16 color);



    void SetScrollArea(u16 top, u16 scroll, u16 bottom);

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
*		SetScrollArea(0,272,272);
*		for(line=0;line<272;line++) {SetScrollStart(line);Ms(100);}
*		
*		Code above scrolls the whole page upwards in 100ms interval 
*		with page 2 replacing the first page in scrolling
********************************************************************/
    void SetScrollStart(u16 line);

/*********************************************************************
* Function:  void EnterSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 enters sleep mode
* Note: Host must wait 5mS after sending before sending next command
********************************************************************/
    void EnterSleepMode (void);

/*********************************************************************
* Function:  void ExitSleepMode (void)
* PreCondition: none
* Input:  none
* Output: none
* Side Effects: none
* Overview: SSD1963 exits sleep mode
* Note:   cannot be called sooner than 15
********************************************************************/
    void ExitSleepMode (void);

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
    void SetBacklight(u8 intensity);

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
    void SetTearingCfg(bool state, bool mode);

    void SetArea(u16 start_x, u16 start_y, u16 end_x, u16 end_y);
    void DisplayOff(void);
    void DisplayOn(void);
    void Delay(u32 nCount);
    void DelayUs(u32 nCount);
    void setCs();
    void clrCs();
    void setRs();
    void clrRs();
    void setnWr();
    void clrnWr();
    void setnRd();
    void clrnRd();
    void setRst();
    void clrRst();
    // void fillScreen(uint16_t color);
    // u16 RGB565CONVERT(u8 r, u8 g, u8 b);

public:
    DigitalOut _Cs, _Rs, _nWr, _nRd, _Rst, _BL;
    BusOut* _data;
    u16 rgbColor;
    

};


#define Set_Cs  SSD1963_Driver::setCs()
#define Clr_Cs  SSD1963_Driver::clrCs()

#define Set_Rs  SSD1963_Driver::setRs()
#define Clr_Rs  SSD1963_Driver::clrRs()


#define Set_nWr SSD1963_Driver::setnWr()
#define Clr_nWr SSD1963_Driver::clrnWr()

#define Set_nRd SSD1963_Driver::setnRd()
#define Clr_nRd SSD1963_Driver::clrnRd()

#define Set_Rst SSD1963_Driver::setRst()
#define Clr_Rst SSD1963_Driver::clrRst()




#endif // _SSD1963_H
