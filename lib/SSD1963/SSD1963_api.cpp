#include "stm32f7xx.h"
#include "SSD1963_api.h"

lcdVarTypedef lcdVar;
xyTypeDef var;
SSD1963_api::SSD1963_api(PinName Cs, PinName Rs, PinName nWr, PinName nRd, PinName Rst, PinName BL, BusOut *data):SSD1963_Driver( Cs,  Rs,  nWr,  nRd,  Rst,  BL,  data)
{
	
}

SSD1963_api::~SSD1963_api()
{
}
void SSD1963_api::Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
{
 	u16 x,y;
 	u16 dx;// = abs(x1 - x0);
 	u16 dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			PutPixel(x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}
 
 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			PutPixel(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			PutPixel(x,y,color);
  		}
 	}
 	else
 	{
 		PutPixel(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				PutPixel(x, y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				PutPixel(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				PutPixel(x, y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				PutPixel(x, y,color);
    			}
   			}
  		}
 	}
}


void SSD1963_api::Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
	if (cx > GetMaxX() || cy > GetMaxY() || (cx+r) > GetMaxX() || (cy+r) > GetMaxY()) return;
	u16 x,y;
	s16 delta,tmp;
	x=0;
	y=r;
	delta=3-(r<<1);

	while(y>x)
	{
		if(fill)
		{
			Line(cx+x,cy+y,cx-x,cy+y,color);
			Line(cx+x,cy-y,cx-x,cy-y,color);
			Line(cx+y,cy+x,cx-y,cy+x,color);
			Line(cx+y,cy-x,cx-y,cy-x,color);
		}
		else
		{
			PutPixel(cx+x,cy+y,color);
			PutPixel(cx-x,cy+y,color);
			PutPixel(cx+x,cy-y,color);
			PutPixel(cx-x,cy-y,color);
			PutPixel(cx+y,cy+x,color);
			PutPixel(cx-y,cy+x,color);
			PutPixel(cx+y,cy-x,color);
			PutPixel(cx-y,cy-x,color);
		}
		x++;
		if(delta>=0)
		{
			y--;
			tmp=(x<<2);
			tmp-=(y<<2);
			delta+=(tmp+10);
		}
		else
		{
			delta+=((x<<2)+6);
		}
	}
}
void SSD1963_api::BackGround(u16 color)
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
	// wait_ms(1);

	Set_Cs;
}
/*********************************************************************
* Function: WORD Rectangle(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*
* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/

void SSD1963_api::Rectangle(u16 left, u16 top, u16 right, u16 bottom, u16 color, bool fill)
{
	register u16  x,y;


  if (!fill) {
    Line(left,top,left,bottom,color);
    Line(left,bottom,right,bottom,color);
    Line(right,bottom,right,top,color);
    Line(right,top,left,top,color);
  
  } else { // Filled Rectangle
		SetArea(left,top,right,bottom);

		WriteCommand(CMD_WR_MEMSTART);
		Clr_Cs;
		for(y=top; y<bottom+1; y++){
			for(x=left; x<right+1; x++){
				WriteData(color);
			}
		}
		Set_Cs;
  }
}

void SSD1963_api::Square(u16 x0, u16 y0, u16 width, u16 color,u8 fill)
{
	Rectangle(x0, y0, x0+width, y0+width, color,fill);
}

void SSD1963_api::ClearCharBox(u16 x,u16 y,u16 color)
{
	Rectangle(x*8,y*16,x*8+8,y*16+16,color,true);
}



int SSD1963_api::power (int base, int n) {
    int     i,
            p;
    p = 1;
    for (i = 1; i <= n; ++i)
	p *= base;
    return p;
}
u16 SSD1963_api::TextWidth(u8 *str, const unsigned char *Font)
{
    u32 CharacterOffset;
    u16 Width = 0;
    
    while (*str != 0)
    {
        CharacterOffset = ((*str - Font[4]) * (((uint16_t)Font[0] << 8) + Font[1])) + 5;
        Width += Font[CharacterOffset];
        str++;        
    }

    return Width;
}
lcdVarTypedef SSD1963_api::PutChar(u16 x, u16 y, u8 Character,  unsigned char *Font, u16 ForeColor, u16 BackColor,bool onlyVar)
{


	lcdVar.row = 0;
	lcdVar.column = 0;
	lcdVar.offset = ((Character - Font[4]) * (((uint16_t)Font[0] << 8) + Font[1])) + 5;
	lcdVar.RowOffset = 1;

	lcdVar.CharacterWidth = Font[2];
	lcdVar.CharacterHeight = Font[3];
	lcdVar.ReturnWidth = Font[lcdVar.offset];

    lcdVar.BytesPrColumn = lcdVar.CharacterHeight / 8;
    if (!onlyVar) {
		if ((lcdVar.CharacterWidth % 8) > 0)
			lcdVar.BytesPrColumn += 1;

		if (Character == ' ') // This is just a space - return the width of the selected character set
		{
			return lcdVar;
		}

		SetArea(x, y, x+lcdVar.CharacterWidth-1, y+lcdVar.CharacterHeight-1);
		WriteCommand(CMD_WR_MEMSTART);
		Clr_Cs;
		for (lcdVar.row = 0; lcdVar.row < lcdVar.CharacterHeight; lcdVar.row++)
		{
			for (lcdVar.column = 0; lcdVar.column < lcdVar.CharacterWidth; lcdVar.column++)
			{
				if (lcdVar.row == 0)
				{
					if ((Font[lcdVar.BytesPrColumn * lcdVar.column + lcdVar.RowOffset + lcdVar.offset] & 1) > 0)
	//                if ((Font[BytesPrColumn * column - RowOffset - offset] & 1) > 0)
					{
						WriteData(ForeColor);
					} else {
						WriteData(BackColor);
					}
				}
				else if ((lcdVar.row % 8) == 0)
				{
					if (lcdVar.column == 0) lcdVar.RowOffset++;
					if ((Font[lcdVar.BytesPrColumn * lcdVar.column + lcdVar.RowOffset + lcdVar.offset] & 1) > 0)
	//                if ((Font[BytesPrColumn * column - RowOffset - offset] & 1) > 0)
					{
						WriteData(ForeColor);
					} else {
						WriteData(BackColor);
					}
				}
				else if ((lcdVar.row % 8) > 0)
				{
					if ((Font[lcdVar.BytesPrColumn * lcdVar.column + lcdVar.RowOffset + lcdVar.offset] & (power(2, (lcdVar.row % 8)))) > 0)  // Math.Pow = ^ (power)
	//                if ((Font[BytesPrColumn * column - RowOffset - offset] & (power(2, (row % 8)))) > 0)  // Math.Pow = ^ (power)

					{
						WriteData(ForeColor);
					} else {
						WriteData(BackColor);
					}
				}
			}
		}
		Set_Cs;
    }
    return lcdVar;
//    return ReturnWidth;
}


xyTypeDef SSD1963_api::Text(u16 x, u16 y, u8 *str,  unsigned char *Font, u16 ForeColor, u16 BackColor)
{

  while (*str != 0)
  {
	  x += PutChar(x,y,*str,Font,ForeColor,BackColor, false).ReturnWidth;
//     xytypedef.Width;
	  str++;
  }
  var.strSize = sizeof(str);
  var.Width = PutChar(x,y,*str,Font,ForeColor,BackColor,true).ReturnWidth * var.strSize;
  var.X = x ;
  var.Y = y + PutChar(x,y,*str,Font,ForeColor,BackColor,true).CharacterHeight-1;
  return var;
}
xyTypeDef SSD1963_api::Text_Center(u16 x0, u16 y0, u16 x1, u16 y1, u8 *str,  unsigned char *Font, u16 ForeColor, u16 BackColor)
{

  u8 i = 0;
  u16 x, y;

  u8 CharacterCount = 0;
  u8 *tmpPtr = str;
  u8 length = sizeof(str);
  while (*tmpPtr != 0)
  {
    tmpPtr++;
    CharacterCount++;
  }
  
  if (x0 != x1)
//    x = (((x1-x0) - TextWidth(str, Font)) / 2) + x0;
  	x = (((x1-x0) - TextWidth(str, Font)) / 2) + x0;
  else
    x = x0;
   
  if (y0 != y1)
    y = (((y1-y0) - Font[3]) / 2) + y0;
  else
    y = y0;


  for (i = 0; i < CharacterCount; i++)
  {
	  x +=   PutChar(x,y,*str,Font,ForeColor,BackColor, false).ReturnWidth;
	  str++;
 }
  var.strSize = sizeof(str);
  var.Width = PutChar(x,y,*str,Font,ForeColor,BackColor,true).ReturnWidth * var.strSize;
  var.X = x ;
  var.Y = y + PutChar(x,y,*str,Font,ForeColor,BackColor,true).CharacterHeight-1;
  return var;
}

lcdVarTypedef SSD1963_api::PutTransparentChar(u16 x, u16 y, u8 Character,  unsigned char *Font, u16 ForeColor)
{

	lcdVar.row = 0;
	lcdVar.column = 0;
	lcdVar.offset = ((Character - Font[4]) * (((uint16_t)Font[0] << 8) + Font[1])) + 5;
	lcdVar.RowOffset = 1;
	lcdVar.ReturnWidth = Font[lcdVar.offset];
    lcdVar.SetPosition = false;
       
    lcdVar.CharacterWidth = Font[2];
    lcdVar.CharacterHeight = Font[3];
        
    lcdVar.BytesPrColumn = lcdVar.CharacterHeight / 8;
    if ((lcdVar.CharacterWidth % 8) > 0)
    	lcdVar.BytesPrColumn += 1;
        
    if (Character == ' ') // This is just a space - return the width of the selected character set
    {
    	return lcdVar;
    }
    
    SetArea(x, y, x+lcdVar.CharacterWidth-1, y+lcdVar.CharacterHeight-1);
    WriteCommand(CMD_WR_MEMSTART);
    Clr_Cs;
    for (lcdVar.row = 0; lcdVar.row < lcdVar.CharacterHeight; lcdVar.row++)
    {
        for (lcdVar.column = 0; lcdVar.column < lcdVar.CharacterWidth; lcdVar.column++)
        {
            if (lcdVar.SetPosition == true) {
                SetArea(x+lcdVar.column, y+lcdVar.row, x+Font[2]-1, y+Font[3]-1);
                Set_Cs;
                WriteCommand(CMD_WR_MEMSTART);
                Clr_Cs;
                lcdVar.SetPosition = false;
            }
                
            if (lcdVar.row == 0)
            {
                if ((Font[lcdVar.BytesPrColumn * lcdVar.column + lcdVar.RowOffset + lcdVar.offset] & 1) > 0)
                {
                    WriteData(ForeColor);
                } else {
                	lcdVar.SetPosition = true;
                }
            }
            else if ((lcdVar.row % 8) == 0)
            {
                if (lcdVar.column == 0) lcdVar.RowOffset++;
                if ((Font[lcdVar.BytesPrColumn * lcdVar.column + lcdVar.RowOffset + lcdVar.offset] & 1) > 0)
                {
                    WriteData(ForeColor);
                } else {
                	lcdVar.SetPosition = true;
                }
            }
            else if ((lcdVar.row % 8) > 0)
            {
                if ((Font[lcdVar.BytesPrColumn * lcdVar.column + lcdVar.RowOffset + lcdVar.offset] & (power(2, (lcdVar.row % 8)))) > 0)  // Math.Pow = ^ (power)
                {
                    WriteData(ForeColor);
                } else {
                	lcdVar.SetPosition = true;
                }
            }
//            Set_Cs;
        }
    }
    
    Set_Cs;
    return lcdVar;
//    return ReturnWidth;
}    

void SSD1963_api::TransparentText(u16 x, u16 y, u8 *str,  unsigned char *Font, u16 ForeColor)
{
  while (*str != 0)
  {
	  x +=PutTransparentChar(x,y,*str,Font,ForeColor).ReturnWidth;
	  str++;
  }
}
void SSD1963_api::TransparentText_Center(u16 x0, u16 y0, u16 x1, u16 y1, u8 *str,  unsigned char *Font, u16 ForeColor)
{
//	xyTypeDef xytypedef;
  u8 i = 0;
  u16 x, y;
  u8 CharacterCount = 0;

  u8 *tmpPtr = str;
  while (*tmpPtr != 0)
  {
    tmpPtr++;
    CharacterCount++;
  }
  
  if (x0 != x1)
    x = (((x1-x0) - TextWidth(str, Font)) / 2) + x0;
  else
    x = x0;
   
  if (y0 != y1)
    y = (((y1-y0) - Font[3]) / 2) + y0;
  else
    y = y0;


  for (i = 0; i < CharacterCount; i++)
  {
	  x += PutTransparentChar(x,y,*str,Font,ForeColor).ReturnWidth;
	  str++;
  }
}


void SSD1963_api::DisplayRAW(u16 x0, u16 y0, const unsigned char *str)
{

	u32 temp, i, difference;    
    u16 x1, y1;    
    u16 imageWidth, imageHeight;        
    uint16_t data;
    
	data = str[1];
	data |= (str[0] << 8);
    imageWidth = data;
    x1 = imageWidth-1 + x0;

	data = str[3];
	data |= (str[2] << 8);
    imageHeight = data;
    y1 = imageHeight-1 + y0;
    
	SetArea(x0,y0,x1,y1);
	WriteCommand(CMD_WR_MEMSTART);

	for (temp = 2; temp < (imageWidth*imageHeight)+2; temp++)
	{  
		data = str[(2 * temp) + 1];
		data |= (str[(2 * temp)] << 8);
	
		WriteData(data);
	}

//==============================  
}

void SSD1963_api::DisplayRAW_Transparent(u16 x0, u16 y0, const unsigned char *str)
{

	u32 temp, i, difference;    
    u16 x1, y1;  
    u16 x, y;
    bool Reposition = false;
    u16 imageWidth, imageHeight;        
    ColorTypeDef color;
    
    color.U8[1] =*(unsigned short *)(&str[ 0]);
	color.U8[0]=*(unsigned short *)(&str[ 1]);
    imageWidth = color.U16;
    x1 = imageWidth-1 + x0;

    color.U8[1] =*(unsigned short *)(&str[ 2]);
	color.U8[0]=*(unsigned short *)(&str[ 3]);
    imageHeight = color.U16;
    y1 = imageHeight-1 + y0;
    
    x = x0;
    y = y0;

	SetArea(x0,y0,x1,y1);
	WriteCommand(CMD_WR_MEMSTART);

	for (temp = 2; temp < (imageWidth*imageHeight)+2; temp++)
	{  
		color.U8[1] =*(unsigned short *)(&str[ 2 * temp]);
		color.U8[0]=*(unsigned short *)(&str[ 2 * temp+1]);
		
        if (color.U16 != BRIGHTMAGENTA) {
            if (Reposition == true) {
                SetArea(x,y,x1,y1); // Set the pixel position to the next pixel, so we skip this "transparent" one
                WriteCommand(CMD_WR_MEMSTART);     // Then we also have to re-enable the direct LCD writes
                Reposition = false;
            }
            WriteData(color.U16);
        } else {        
            Reposition = true;
        }    
        x++; // Next Pixel Pos X
        if (x > x1) {
          y++;  // Next Pixel Pos Y
          x = x0;
        }     
	}

//==============================  
}
void SSD1963_api::fillScreen(uint16_t color){

    uint16_t start_x = 0;
    uint16_t end_x   = 799;
    uint16_t start_y = 0;
    uint16_t end_y   = 479;

    WriteCommand(0x2A);
//    nCS_LOW;
    Clr_Cs;
    WriteData(start_x>>8);
    WriteData(start_x);
    WriteData(end_x>>8);
    WriteData(end_x);
	wait_ms(1);
    //nCS_HIGH;
    Set_Cs;
    WriteCommand(0x2B);
	wait_ms(1);
    Clr_Cs;
    WriteData(start_y>>8);
    WriteData(start_y);
    WriteData(end_y>>8);
    WriteData(end_y);
    Set_Cs;

    WriteCommand(0x2C);
	wait_ms(1);
    Clr_Cs;
    uint16_t i,j;
    for(i=0;i<800;i++){
        for (j=0;j<480;j++){
        	WriteData(color);
			// wait_us(1);
        }
    }
	wait_ms(10);
    Set_Cs;
	// WriteCommand(CMD_NOP);
}

// #include "ff.h"
// #include "diskio.h"
//#include "sdio_sd.h"
static unsigned char BMP_buffer[1024];
/*******************************************************************************
* Function Name  : WriteBMP_FAT_Transparent
* Description    : Displays a bitmap picture loaded from FAT system from SD
                 : Xpoz - X position
                 : Ypoz - Y position
                 : BmpName - filename of BMP file
* Output         : None
* Return         : None
*******************************************************************************/
//void DisplayBMP_FAT(uint16_t Xpos, uint16_t Ypos, const char* BmpName)
//{
//  uint32_t index = 0, size = 0, width=0, height=0, i1 = 0;
//  uint32_t BmpAddress;
//  uint16_t data;
//  FATFS fs;
//  FIL F;
//  unsigned int bytesRead;
//
//  SD_Lock();
//  if (f_mount(0, &fs) != FR_OK) return;
//  if (f_open (&F, BmpName, FA_READ) != FR_OK) return;
//  if (f_read (&F, BMP_buffer, 26, &bytesRead) != FR_OK) return;
//
//  BmpAddress = (uint32_t)BMP_buffer;
//
//  /* Read bitmap size */
//  size = *(uint16_t *) (BmpAddress + 2);
//  size |= (*(uint16_t *) (BmpAddress + 4)) << 16;
//
//  /* Get bitmap data address offset */
//  index = *(uint16_t *) (BmpAddress + 10);
//  index |= (*(uint16_t *) (BmpAddress + 12)) << 16;
//
//  /* Read bitmap width*/
//  width = *(uint16_t *) (BmpAddress + 18);
//  width |= (*(uint16_t *) (BmpAddress + 20)) << 16;
//  /*read bitmap height*/
//  height = *(uint16_t *) (BmpAddress + 22);
//  height |= (*(uint16_t *) (BmpAddress + 24)) << 16;
//
//  size = (size - index)/2;
//  size = size - 1; // Adobe Photoshop Anomally
//
//  // Synchronize f_read right in front of the image data
//  f_read (&F, BMP_buffer, index-26, &bytesRead);
//
//  /* Set GRAM write direction to "Bottom to Top" */
//  SetAddressOrder(DIR_BOTTOM_TO_TOP);
//  /*Setup the window*/
//  SetArea(Xpos, Ypos, Xpos+width-1, Ypos+height-1);
//  /*Prepare for direct LCD writes*/
//  WriteCommand(CMD_WR_MEMSTART);
//
//  do  {
//    if (size*2 < 512) {
//        i1 = size*2;
//    } else {
//        i1 = 512;
//    }
//    size -= i1/2;
//    f_read (&F, BMP_buffer, i1, &bytesRead);
//
//    for(index = 0; index < i1/2; index++)
//    {
//        WriteData(*(uint16_t *)BmpAddress);
//        BmpAddress += 2;	 //jump on next pixel
//    }
//    BmpAddress = (uint32_t)BMP_buffer;
//  } while (size > 0);
//
//  f_close (&F);
//
//  SD_UnLock();
//
//  /* Set GRAM write direction to "Top to Bottom" */
//  SetAddressOrder(DIR_TOP_TO_BOTTOM);
//}

/*******************************************************************************
* Function Name  : WriteBMP_FAT
* Description    : Displays a transparent (MAGENTA = 255,0,255) bitmap picture
                 : loaded from FAT system from SD
                 : Xpoz - X position
                 : Ypoz - Y position
                 : BmpName - filename of BMP file
* Output         : None
* Return         : None
*******************************************************************************/
//void DisplayBMP_FAT_Transparent(uint16_t Xpos, uint16_t Ypos, const char* BmpName)
//{
//  uint32_t index = 0, size = 0, width=0, height=0, i1 = 0;
//  uint16_t x, y;
//  bool Reposition = false;
//  uint32_t BmpAddress;
//  FATFS fs;
//  FIL F;
//  unsigned int bytesWritten;
//
//  SD_Lock();
//  f_mount(0, &fs);
//  f_open (&F, BmpName, FA_READ);
//  f_read (&F, BMP_buffer, 26, &bytesWritten);
//
//  BmpAddress = (uint32_t)BMP_buffer;
//
//  /* Read bitmap size */
//  size = *(uint16_t *) (BmpAddress + 2);
//  size |= (*(uint16_t *) (BmpAddress + 4)) << 16;
//
//  /* Get bitmap data address offset */
//  index = *(uint16_t *) (BmpAddress + 10);
//  index |= (*(uint16_t *) (BmpAddress + 12)) << 16;
//
//  /* Read bitmap width*/
//  width = *(uint16_t *) (BmpAddress + 18);
//  width |= (*(uint16_t *) (BmpAddress + 20)) << 16;
//  /*read bitmap height*/
//  height = *(uint16_t *) (BmpAddress + 22);
//  height |= (*(uint16_t *) (BmpAddress + 24)) << 16;
//
//  size = (size - index)/2;
//  size = size - 1; // Adobe Photoshop Anomally
//
//  // Synchronize f_read right in front of the image data
//  f_read (&F, BMP_buffer, index-26, &bytesWritten);
//
//  /* Set GRAM write direction to "Bottom to Top" */
//  SetAddressOrder(DIR_BOTTOM_TO_TOP);
//  /*Setup the window*/
//  SetArea(Xpos, Ypos, Xpos+width-1, Ypos+height-1);
//  /*Prepare for direct LCD writes*/
//  WriteCommand(CMD_WR_MEMSTART);
//
//  x = Xpos;
//  y = Ypos+height-1;
//  do  {
//    if (size*2 < 512) {
//        i1 = size*2;
//    } else {
//        i1 = 512;
//    }
//    size -= i1/2;
//    f_read (&F, BMP_buffer, i1, &bytesWritten);
//
//    for(index = 0; index < i1/2; index++)
//    {
//        if ((*(uint16_t *)BmpAddress) != BRIGHTMAGENTA) {
//            if (Reposition == true) {
//                SetArea(x, Ypos, Xpos+width-1, y); // Set the pixel position to the next pixel, so we skip this "transparent" one
//                WriteCommand(CMD_WR_MEMSTART);     // Then we also have to re-enable the direct LCD writes
//                Reposition = false;
//            }
//            WriteData(*(uint16_t *)BmpAddress);
//        } else {
//            Reposition = true;
//        }
//        x++; // Next Pixel Pos X
//        if (x == Xpos+width) {
//          y--;  // Next Pixel Pos Y
//          x = Xpos;
//        }
//        BmpAddress += 2;	 //jump on next pixel
//    }
//    BmpAddress = (uint32_t)BMP_buffer;
//  } while (size > 0);
//
//  f_close (&F);
//  f_mount (0, 0);
//  SD_UnLock();
//
//  /* Set GRAM write direction to "Top to Bottom" */
//  SetAddressOrder(DIR_TOP_TO_BOTTOM);
//}
