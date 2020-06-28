#ifndef __SSD1963_API_H
#define __SSD1963_API_H

#include "stm32f7xx.h"
#include "SSD1963_Driver.h"
#include "SSD1963_font.h"

#include <stdbool.h>


u16 Lcd_Color565(u32 RGB);  // RGB��ɫתΪ16λ(565)
typedef union
{
  u16 U16;
  u8 U8[2];
}ColorTypeDef;
typedef struct
{
	u16 X;
	u16 Y;
	u8 row;
	u8 column ;
	u32 offset;
	u8 RowOffset;
	u8 CharacterWidth;
	u8 CharacterHeight;
	u8 ReturnWidth;
	u8 BytesPrColumn;
	bool SetPosition;
}lcdVarTypedef;
typedef struct
{
	u16 strSize;
	u16 Width;
	u16 Heigth;
	u16 X;
	u16 Y;
}xyTypeDef;

class SSD1963_api:public SSD1963_Driver
{
private:
    /* data */
public:
    SSD1963_api(PinName Cs, PinName Rs, PinName nWr, PinName nRd, PinName Rst, PinName BL, BusOut *data);
    ~SSD1963_api();

    lcdVarTypedef PutChar(u16 x, u16 y, u8 Character,  unsigned char *Font, u16 ForeColor, u16 BackColor,bool onlyVar);
    lcdVarTypedef PutTransparentChar(u16 x, u16 y, u8 Character,  unsigned char *Font, u16 ForeColor);

    xyTypeDef Text(u16 x, u16 y, u8 *str,  unsigned char *Font, u16 ForeColor, u16 BackColor);
    xyTypeDef Text_Center(u16 x0, u16 y0, u16 x1, u16 y1, u8 *str,  unsigned char *Font, u16 ForeColor, u16 BackColor);
    void TransparentText(u16 x, u16 y, u8 *str,  unsigned char *Font, u16 ForeColor);
    void TransparentText_Center(u16 x0, u16 y0, u16 x1, u16 y1, u8 *str,  unsigned char *Font, u16 ForeColor);
    void TransparentLargeText(u16 x, u16 y, u8 *str,  unsigned int *Font, u16 ForeColor);
    void TransparentLargeText_Center(u16 x0, u16 y0, u16 x1, u16 y1, u8 *str,  unsigned int *Font, u16 ForeColor);
    void Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);
    void Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);
    void Rectangle(u16 left, u16 top, u16 right, u16 bottom, u16 color, bool fill);
    void Square(u16 x0, u16 y0, u16 width, u16 color,u8 fill);
    void ClearCharBox(u16 x,u16 y,u16 color);
    void fillScreen(uint16_t color);
    void DisplayRAW(u16 x0, u16 y0, const unsigned char *str);
    void DisplayRAW_Transparent(u16 x0, u16 y0, const unsigned char *str);
    void BackGround(u16 color);
    void DisplayBMP_FAT(uint16_t Xpos, uint16_t Ypos, const char* BmpName);
    void DisplayBMP_FAT_Transparent(uint16_t Xpos, uint16_t Ypos, const char* BmpName);
    u16 TextWidth(u8 *str, const unsigned char *Font);
    int power (int m, int n);
};


#endif /* __SSD1963_API_H */
