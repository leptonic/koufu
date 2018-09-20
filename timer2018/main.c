//ver 0.0.01
#include "LedControl.h"

#include <EEPROM.h>


LedControl lc=LedControl(12,11,10,2);
//LedControl lc2=LedControl(12,11,9,1);
/* we always wait a bit between updates of the display */
unsigned long delaytime=10;
int pinInterrupt = 2; //接中断信号的脚  

int addr = 0;
int addr_mode=8;

int gtimer=54;
int gmode=0;
void(* resetFunc) (void) = 0;

void setup() {
 
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,6);
  /* and clear the display */
  lc.clearDisplay(0);

    lc.shutdown(1,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(1,6);
  /* and clear the display */
  lc.clearDisplay(1);
 
   EEPROM.write(addr, 99);
   delay(25);
	
	gtimer = EEPROM.read(addr);
	   delay(25);
	
	gmode=EEPROM.read(addr_mode);
	delay(25);
	 

	Serial.begin(9600); //打开串口	
	
	pinMode( pinInterrupt, INPUT);//设置管脚为输入  

	//Enable中断管脚, 中断服务程序为onChange(), 监视引脚变化  
	if(gmode!=99)
		attachInterrupt( pinInterrupt, onChange, FALLING);  
	else
		detachInterrupt(pinInterrupt);
  
}
void onChange()  
{  
   if ( digitalRead(pinInterrupt) == LOW )  
   	{
   	delay(300);
		if ( digitalRead(pinInterrupt) == LOW )  
		{  
		delay(400);
		 if ( digitalRead(pinInterrupt) == LOW )  
		   {
				delay(299);
		   if ( digitalRead(pinInterrupt) == LOW )	
			 {
				 
				EEPROM.write(addr_mode, 99);
			 delay(250);
				 resetFunc();
   
		
			 }
		   }
   
		}  
   }
}  


void showClear() 
{
  /* here is the data for the characters */
 
  byte colon[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  
  //byte c5[8]={};
 
	for(int i=0;i<8;i++)
	{
	lc.setRow(0,i,colon[i]);
	}

	for(int i=0;i<8;i++)
	{
	lc.setRow(1,i,colon[i]);
	}
}

void showNumber(int timers) 
{
  /* here is the data for the characters */
  byte n[11][8]{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},{0x00,0x08,0x18,0x08,0x08,0x08,0x08,0x1C},{0x00,0x18,0x24,0x04,0x08,0x10,0x20,0x3C},{0x00,0x18,0x24,0x04,0x18,0x04,0x24,0x18},
  {0x00,0x04,0x0C,0x14,0x24,0x3E,0x04,0x04},{0x00,0x3C,0x20,0x20,0x38,0x04,0x04,0x38},{0x00,0x18,0x20,0x20,0x38,0x24,0x24,0x18},
  {0x00,0x3E,0x22,0x04,0x08,0x08,0x08,0x00},{0x00,0x1C,0x22,0x22,0x1C,0x22,0x22,0x1C},{0x00,0x1C,0x22,0x22,0x1E,0x02,0x04,0x08}
  ,{0x00,0x1C,0x22,0x22,0x22,0x22,0x22,0x1C}};
  byte colon[8]={0x00,0x00,0x80,0x00,0x00,0x80,0x00,0x00};
  
  //byte c5[8]={};
  int ud=(timers%10)>0?(timers%10):10;
  int td=(timers/10)>0?(timers/10):0;
	for(int i=0;i<8;i++)
	{
	lc.setRow(0,i,n[ud][i]);
	}

	for(int i=0;i<8;i++)
	{
	lc.setRow(1,i,n[td][i]);
	}
}
void showEnd() 
{
  /* here is the data for the characters */
  byte td[4][8]{{0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0xFF},{0x00,0xFF,0x80,0x80,0x80,0x80,0xFF,0x00},{0x00,0x00,0xFF,0x80,0x80,0xFF,0x00,0x00},{0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00}};
  byte ud[4][8]{{0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0xFF},{0x00,0xFF,0x01,0x01,0x01,0x01,0xFF,0x00},{0x00,0x00,0xFF,0x01,0x01,0xFF,0x00,0x00},{0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00}};
  //byte c5[8]={};

for(int j=0;j<4;j++)
{
	for(int i=0;i<8;i++)
	{
	lc.setRow(0,i,ud[j][i]);
	delay(5);
	lc.setRow(1,i,td[j][i]);
	delay(50);
	}

}
	for(int j=3;j>-1;j--)
	{
		for(int i=0;i<8;i++)
		{
		lc.setRow(0,i,ud[j][i]);
		delay(5);
		lc.setRow(1,i,td[j][i]);
		delay(50);
		}
	
	}


}

/*
  This function lights up a some Leds in a row.
 The pattern will be repeated on every row.
 The pattern will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void rows() {
  for(int row=0;row<8;row++) {
    delay(delaytime);
    lc.setRow(0,row,B10100000);
    delay(delaytime);
    lc.setRow(0,row,(byte)0);
    for(int i=0;i<row;i++) {
      delay(delaytime);
      lc.setRow(0,row,B10100000);
      delay(delaytime);
      lc.setRow(0,row,(byte)0);
    }
  }
}

/*
  This function lights up a some Leds in a column.
 The pattern will be repeated on every column.
 The pattern will blink along with the column-number.
 column number 4 (index==3) will blink 4 times etc.
 */
void columns() {
  for(int col=0;col<8;col++) {
    delay(delaytime);
    lc.setColumn(0,col,B10100000);
    delay(delaytime);
    lc.setColumn(0,col,(byte)0);
    for(int i=0;i<col;i++) {
      delay(delaytime);
      lc.setColumn(0,col,B10100000);
      delay(delaytime);
      lc.setColumn(0,col,(byte)0);
    }
  }
}

/* 
 This function will light up every Led on the matrix.
 The led will blink along with the row-number.
 row number 4 (index==3) will blink 4 times etc.
 */
void show_time() {
  
}
void welcome()
{
	for(int row=0;row<8;row++) 
		{
			
	    for(int col=0;col<16;col++)
			{
	          delay(delaytime);
			  if(col<8)
	          lc.setLed(1,row,col,true);
			  else
			  lc.setLed(0,row,col-8,true);
			  
	          delay(delaytime);
		      for(int i=0;i<col;i++)
			  	{
		          if(col<8)
		          lc.setLed(1,row,col,false);
				  else
				  lc.setLed(0,row,col-8,false);
				
		        delay(delaytime);
		      	 if(col<8)
		          lc.setLed(1,row,col,true);
				  else
				  lc.setLed(0,row,col-8,true);
			
		        delay(delaytime);
		      }
    }
  }
//	

}
void run_timer(int start_timer)
{
	for(int i=start_timer;i>0;i--)
	{
			
		showNumber(i);
		delay(1000);
	
	}

}
void loop()
{
	while(gmode==99)
	{
		int timeout;
		
		for(timeout=50;timeout>0;timeout--)
		{
		showNumber(gtimer);	
		delay(500);
		showClear();
		delay(500);
		showNumber(gtimer);	
			if ( digitalRead(pinInterrupt) == LOW )  
				{  
				delay(47);
				 if ( digitalRead(pinInterrupt) == LOW )  
				   {
				 
						delay(67);
				   if ( digitalRead(pinInterrupt) == LOW )	
					 {
						 
				   timeout=30;
						  if(gtimer>0)
							   gtimer--;
						   else
							  gtimer=99;
						 showNumber(gtimer);		   
					   delay(500);
				
				
					 }
				   }
				
				}
			
			delay(200);

		}
		showNumber(gtimer);		 
		delay(1000);
		showClear();
		delay(1000);
		showNumber(gtimer);		 
		delay(1000);
		showClear();
		delay(1000);	
		showNumber(gtimer);		 
		delay(1000);
		showClear();
		delay(1000);
	EEPROM.write(addr, gtimer);	
	delay(250);
   EEPROM.write(addr_mode, 0);
   delay(250);
  	gmode=0;
	resetFunc();
   
	}
	//welcome();

run_timer(gtimer);

showEnd();



//  single2();
}

