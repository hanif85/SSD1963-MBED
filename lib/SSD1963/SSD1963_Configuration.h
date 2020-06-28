
#ifndef __HARDWARE_PROFILE_H
#define __HARDWARE_PROFILE_H

#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************
* START OF GRAPHICS RELATED MACROS
********************************************************************/

#define USE_16BIT_PMP
//Add support for PIC32MX Starter Kit
//#define PIC32_STARTER_KIT
#define STM32_STAMP // STM32F103RET6
#define DISPLAY_CONTROLLER SSD1963
//Define TFT panel here
#define DISPLAY_PANEL TY700TFT800480
#define COLOR_DEPTH 16

/*********************************************************************
* DISPLAY SETTINGS 
********************************************************************/

// Error Checking
#ifndef DISPLAY_CONTROLLER
	#error "Error: DISPLAY_CONTROLLER not defined"
#endif

#if (DISPLAY_PANEL == TY700TFT800480)
	/*********************************************************************
	* Overview: Image orientation (can be 0, 90, 180, 270 degrees).
	*********************************************************************/	
  	#define DISP_ORIENTATION    180
	/*********************************************************************
	* Overview: Panel Data Width (R,G,B) in (6,6,6)
	*********************************************************************/
	#define DISP_DATA_WIDTH                 18
	/*********************************************************************
	* Overview: Horizontal and vertical display resolution
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_HOR_RESOLUTION 800
	#define DISP_VER_RESOLUTION 480
	/*********************************************************************
	* Overview: Horizontal synchronization timing in pixels
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_HOR_PULSE_WIDTH		1
	#define DISP_HOR_BACK_PORCH			210
	#define DISP_HOR_FRONT_PORCH		45 
	/*********************************************************************
	* Overview: Vertical synchronization timing in lines
	*                  (from the glass datasheet).
	*********************************************************************/
	#define DISP_VER_PULSE_WIDTH		1
	#define DISP_VER_BACK_PORCH			34	
	#define DISP_VER_FRONT_PORCH		10
	/*********************************************************************
	* Definition for SPI interface for SSD1963 
	* Hardware dependent!
	*********************************************************************/
	#define GPIO3 3
	#define GPIO2 2
	#define GPIO1 1
	#define GPIO0 0
	#define LCD_RESET 	0
	#define LCD_SPENA	0
	#define LCD_SPCLK	0
	#define LCD_SPDAT	0
	/* End of definition for DISPLAY_PANEL == TY700TFT800480 */

#else
	#error "Graphics controller is not defined"
#endif // DISPLAY_CONTROLLER == ABC

/*********************************************************************
* IOS FOR THE DISPLAY CONTROLLER
*********************************************************************/
#ifdef __cplusplus
}
#endif

#endif // __HARDWARE_PROFILE_H

