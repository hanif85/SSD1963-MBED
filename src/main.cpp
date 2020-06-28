 #include <mbed.h>
#include "SSD1963_api.h"
// #include "SSD1963.h"
#include "MyTouch.h"
#include "SSD1963_Driver.h"
#include "USBSerial.h"
// #include "USBMSD.h"

void mytouch_irq_handler(void);
  // SSD1963_api LCdisplay;
MYTOUCH Touch( PA_7, PA_6, PA_5, PB_6, PB_8, USE_12BITS, &mytouch_irq_handler); // mosi, miso, sck, cs, irq pin, mode, pointer to touch panel irq handler;
BusOut sendData(PJ_7,PJ_8,PJ_9,PJ_10,PJ_11,PK_0,PK_1,PK_2,PE_4,PJ_13,PJ_14,PJ_15,PG_12,PK_4,PK_5,PK_6);
SSD1963_api  sLCD(PI_9,PJ_12,PK_7,PI_12,PI_14,PK_3,&sendData);
USBSerial dbg;
uint16_t CoorX,CoorY;
char Buff[100];

// **********************************************************************

int main() {

  // LCdisplay.InitLCD();
// ResetDevice();
  wait_ms(500);
// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3,(GPIO_PinState) true);
  // SetBacklight(200);
  // put your setup code here, to run once:
  
  sLCD.InitDevice();
  sLCD.SetBacklight(75);
  sLCD.Clear(BLACK);

    sLCD.Clear(0xF856);
    wait_ms(10);
// sLCD.setBL(1);
// sLCD.BackGround(BLUE);
// sLCD.fillScreen(GREEN);
// bool swtch=0;
  while(1) {
    // CoorX = Touch.TP_GetX();
    // CoorY = Touch.TP_GetY();
    // Touch.TP_GetX();
        sLCD.Text(150,150,(u8*)"Test ",Arial28R,RED,YELLOW);
    // sprintf(Buff,"Coordinate: %u, %u",&CoorX,&CoorY);

    sLCD.Text(50,50,(u8*)Buff,Arial28R,RED,YELLOW);
    
    // swtch = !swtch;
    // sLCD.setBL(swtch);
    // wait_ms(200);
    // sLCD.fillScreen(RED);
    // wait_ms(100);
    
    // sLCD.fillScreen(BLUE);
    // wait_ms(100);
    // sLCD.fillScreen(RED);
    // wait_ms(100);
    // sLCD.fillScreen(YELLOW);
    // wait_ms(100);
    // sLCD.fillScreen(GREEN);
    // wait_ms(100);
    
    // put your main code here, to run repeatedly:
  }
}

void mytouch_irq_handler(void)
    {
        float xf, yf ;
        CoorX = Touch.TP_GetX();
        CoorY = Touch.TP_GetY();
        xf = CoorX ;
        yf = CoorY ;
        xf = (xf/1024)*800 ;
        yf = (yf/1024)*480 ;
        CoorX = (int)xf ;
        CoorY = (int)yf ;
        // lcd.DrawCircle(x,y,2, COLOR_GREEN) ;
        wait_ms(200);
    }