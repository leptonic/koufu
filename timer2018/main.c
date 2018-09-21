//ver 0.1.01
#include "LedControl.h"

#include <EEPROM.h>


LedControl lc=LedControl(12,11,10,2);
//LedControl lc2=LedControl(12,11,9,1);
/* we always wait a bit between updates of the display */
unsigned long delaytime=3;
int pinInterrupt = 2; //接中断信号的脚  
int Reset_PIN = 8;

int addr = 0;
int addr_mode=8;

int gtimer=54;
int gmode=0;
void(* resetFunc) (void) = 0;
byte snd_1[6] = {0xAA, 0x07, 0x02, 0x00, 0x01, 0xB4};//1 280
byte snd_2[6] = {0xAA, 0x07, 0x02, 0x00, 0x02, 0xB5};//2 200
void SendData (byte addr)
{
  Serial.write(addr);
}

void Sendcmd(byte  ps[], int sizef)
{
  for (int i = 0; i < sizef; i++)
  {
    SendData(ps[i]);
    //delay(1);
  }

}

void II_PlayWave(byte wname[], int delayt)
{


  Sendcmd(wname, 6);

	delay(delayt);
}

void III_Play_welcome()
{
	II_PlayWave(snd_1, 2300);

}
void III_Play_end()
{
	II_PlayWave(snd_2, 3000);

}




void set_volume()// aa 13 01 1e dc max
{


  SendData(0xaa);
  SendData(0x13);
  SendData(0x01);
  SendData(0x1c);
  SendData(0xda);


}


void setup() {
   digitalWrite(Reset_PIN, HIGH);
  delay(200); 
  pinMode(Reset_PIN, OUTPUT);     
  
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
 
  // EEPROM.write(addr, 99);
   delay(25);
	
	gtimer = EEPROM.read(addr);
	   delay(25);
	
	gmode=EEPROM.read(addr_mode);
	delay(25);
	 

	Serial.begin(9600); //打开串口	
	

	
	pinMode( pinInterrupt, INPUT);//设置管脚为输入  

//	attachInterrupt( pinInterrupt, onChange, FALLING);  

	//Enable中断管脚, 中断服务程序为onChange(), 监视引脚变化  
//	if(gmode!=99)
//		attachInterrupt( pinInterrupt, onChange, FALLING);  
//	else
//		detachInterrupt(pinInterrupt);
//  
}
void onChange()  
{  
   if ( digitalRead(pinInterrupt) == LOW )  
   	{
   	delay(18);
		if ( digitalRead(pinInterrupt) == LOW )  
		{  
		delay(49);
		 if ( digitalRead(pinInterrupt) == LOW )  
		   {
				delay(99);
		   if ( digitalRead(pinInterrupt) == LOW )	
			 {
				 
				EEPROM.write(addr_mode, 99);
			 delay(250);
				   digitalWrite(Reset_PIN, LOW);
   
		
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
	byte td[2][8]{{0x0A,0x1C,0x38,0x70,0x70,0x38,0x1C,0x0A},{0x50,0xE0,0xC0,0x01,0x01,0xC0,0xC0,0x50}};
    byte ud[2][8]{{0x28,0x1C,0x0E,0x07,0x07,0x0E,0x1C,0x28},{0x0A,0x07,0x03,0x81,0x81,0x03,0x07,0x0A}};
//  //byte c5[8]={};

	for(int j=0;j<2;j++)
	{
		for(int i=0;i<8;i++)
		{
		lc.setRow(0,i,ud[j][i]);
		delay(5);
		lc.setRow(1,i,td[j][i]);
		delay(50);
		}

	}
for(int j=0;j<2;j++)
{
	for(int i=0;i<8;i++)
	{
	lc.setRow(0,i,ud[j][i]);
	delay(5);
	lc.setRow(1,i,td[j][i]);
	delay(50);
	}

}

//  /* here is the data for the characters */
//  byte td[4][8]{{0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0xFF},{0x00,0xFF,0x80,0x80,0x80,0x80,0xFF,0x00},{0x00,0x00,0xFF,0x80,0x80,0xFF,0x00,0x00},{0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00}};
//  byte ud[4][8]{{0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0xFF},{0x00,0xFF,0x01,0x01,0x01,0x01,0xFF,0x00},{0x00,0x00,0xFF,0x01,0x01,0xFF,0x00,0x00},{0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00}};
//  //byte c5[8]={};

//for(int j=0;j<4;j++)
//{
//	for(int i=0;i<8;i++)
//	{
//	lc.setRow(0,i,ud[j][i]);
//	delay(5);
//	lc.setRow(1,i,td[j][i]);
//	delay(50);
//	}

//}
//	for(int j=3;j>-1;j--)
//	{
//		for(int i=0;i<8;i++)
//		{
//		lc.setRow(0,i,ud[j][i]);
//		delay(5);
//		lc.setRow(1,i,td[j][i]);
//		delay(50);
//		}
//	
//	}


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
void setting_welcome()
{
	
	/* here is the data for the characters */
	 
	  byte colon[8]={0x00,0x00,0x6E,0x88,0x6E,0x28,0xCE,0x00};
	  	  byte colon2[8]={0x00,0x00,0xE0,0x44,0x40,0x44,0x40,0x00};
	  //byte c5[8]={};
	 
		for(int i=0;i<8;i++)
		{
		lc.setRow(0,i,colon2[i]);
		}
	
		for(int i=0;i<8;i++)
		{
		lc.setRow(1,i,colon[i]);
		}


}
void run_timer(int start_timer)
{
	for(int i=start_timer;i>0;i--)
	{
			
		showNumber(i);
		onChange();
		delay(980);
	
	}

}
void loop()
{
	while(gmode==99)
	{
		int timeout;
		setting_welcome();
		delay(2000);
		showNumber(gtimer);	
		delay(250);
		showClear();
		delay(400);
		showNumber(gtimer);	
		for(timeout=50;timeout>0;timeout--)
		{
	
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
	 digitalWrite(Reset_PIN, LOW);
	resetFunc();
   
	}
	//welcome();
III_Play_welcome();

run_timer(gtimer);

III_Play_end();
showEnd();

while(true)
{
showClear();

;
}

//  single2();
}

