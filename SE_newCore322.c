//<<<<<<<<SON BOARD FIRMWARE>>>>>
//1.7 frequency hopping version
//V186 START TO DEBUG WIRELESS V3
//v188 ONLY for first debug MB
//v200 Add Sleep mode
//v210 try to with TRM replace new libray to MIRF
//230 another version new libray to TRM
#define VERISON 230
//#define SLEEP_TEST 1
//#define CONFIGRATION 1
//#define _DEBUG_LOWPOWER 1  //1 means true; 2 means false ; should remove this define without debuging

//#define STEP2STEP_DEBUG 1
//#define DEBUGMODE 11

//#define _DEBUG_ONLINE 10
#define _DBG_STATIC_FREQNCY 12
//#define _DBG_RF11_INFO		  17
#define READING_MODE false
#define WRITING_MODE true

#define ZQ_T 'A'
#define ZQ_O '1'
#define ZT_T 'B'
#define ZT_O '2'
#define YT_T 'C'
#define YT_O '3'
#define QL_T 'D'
#define QL_O '4'
#define BH_T 'E'
#define BH_O '5'
#define XW_T 'F'
#define XW_O '6'

#define OFF 'X'

#define NAME1_PIN 6
#define NAME2_PIN 5
#define NAME3_PIN 4

#define CHANNEL_1_PIN A3
#define CHANNEL_2_PIN A2
#define CHANNEL_THRESHOLD_VAULE 50

#define BEAT_PIN A5
#define BEAT_THRESHOLD_VAULE 125
#define BATTERY_PIN A7
#define BATTERY_THRESHOLD_VAULE 640//2v
#define BAUD_RATE 115200

#define LED_PIN 9
#define LED2_PIN 10

#define NAME_FRONT_ZHUQUE 1
#define NAME_LEFT_FOOT 2
#define NAME_RIGHT_FOOT 3
#define NAME_LEFT_QINGLONG 4
#define NAME_RIGHT_BAIHU 5
#define NAME_BEHIND_XUANWU 6
#define NAME_ERROR 7



//#if CONFIGRATION == 1
//#define CHECK_BIT ZQ_T
//#define KEY_BIT ZQ_O
//#elif CONFIGRATION == 2
//#define CHECK_BIT ZT_T
//#define KEY_BIT ZT_O
//#elif CONFIGRATION == 3
//#define CHECK_BIT YT_T
//#define KEY_BIT YT_O
//#elif CONFIGRATION == 4
//#define CHECK_BIT QL_T
//#define KEY_BIT QL_O
//#elif CONFIGRATION == 5
//#define CHECK_BIT BH_T
//#define KEY_BIT BH_O
//#elif CONFIGRATION == 6
//#define CHECK_BIT XW_T
//#define KEY_BIT XW_O
//#endif




#define VOTLAGE_VALUE  292
#define BASE_FREQUNCY   90
#define SPEED_DATA_RATES 0x0e  // 0x06 7 1M 0x0f 2M 

#include <avr/wdt.h>
#define TIMEOUT WDTO_8S 

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <SPI.h>
//#include <Mirf.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"


//rf trm part

const int pinCE = 8; //This pin is used to set the nRF24 to standby (0) or active mode (1)
const int pinCSN = 7; //This pin is used to tell the nRF24 whether the SPI communication is a command or message to send out

volatile byte rBuffer[2]; 


volatile int rCount;
RF24 radio(pinCE, pinCSN); // Create your nRF24 object or wireless SPI connection
byte address[][5] = { 0xCC,0xCE,0xCC,0xCE,0xCC , 0xCE,0xCC,0xCE,0xCC,0xCE};

typedef enum { role_sender = 1, role_receiver } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Sender", "Receiver"};  // The debug-friendly names of those roles
role_e role;   

static uint32_t message_count = 0;

//rf o



//#include <MirfHardwareSpiDriver.h>
void(* resetFunc) (void) = 0;
/*SONboard Base*/
int value;
int timeout;
int myname;
int myChannel;

char CHECK_BIT;
char KEY_BIT;




void III_Control_LED(bool sw);


void Sleep_avr(){
  ACSR |=_BV(ACD);//OFF ACD
  ADCSRA=0;//OFF ADC

  set_sleep_mode(SLEEP_MODE_PWR_DOWN  ); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System sleeps here
}

void III_Get_myName()
{
	//myname=NAME_RIGHT_BAIHU;
	//return;

	int result;
		pinMode(NAME1_PIN,INPUT);
		pinMode(NAME2_PIN,INPUT);
		pinMode(NAME3_PIN,INPUT);
	if(digitalRead(NAME1_PIN))
		result=0x01;
	else
		result=0x00;
	if(digitalRead(NAME2_PIN))
			result|=0x02;
	if(digitalRead(NAME3_PIN))
				result|=0x04;
#ifdef DEBUGMODE
//Serial.print("pinstate:");
//Serial.println(result);
#endif
	switch(result)
	{
	case 0:
		myname=NAME_FRONT_ZHUQUE;
		break;
	case 1:
		myname=NAME_LEFT_FOOT;
		break;
	case 2:
		myname=NAME_RIGHT_FOOT;
		break;	
	case 3:
		myname=NAME_LEFT_QINGLONG;
		break;
	case 4:
		myname=NAME_RIGHT_BAIHU;
		break;
	default:
		myname=NAME_BEHIND_XUANWU;
		break;
	
	
	}

    
}

int III_Get_Channel()
{
#ifdef DEBUGMODE
	int result;
//	result=analogRead(CHANNEL_1_PIN );
//	Serial.print("dbChannel 1:");
//	Serial.println(result);
//	delay(1000);
//	
//	result=analogRead(CHANNEL_2_PIN  );
//	Serial.print("dbChannel 2:");
//	Serial.println(result);
//	delay(1000);
	result=0x00;
	if(analogRead(CHANNEL_1_PIN)>CHANNEL_THRESHOLD_VAULE )
		result=0x01;
	
	if(analogRead(CHANNEL_2_PIN )>CHANNEL_THRESHOLD_VAULE )
		result|=0x02;
	Serial.begin(BAUD_RATE); 
	Serial.print("CHANNEL NUMBER:");
	Serial.println(result);
	Serial.end();
	return result;

#else
	int result;
	
	result=0x00;
	if(analogRead(CHANNEL_1_PIN)>CHANNEL_THRESHOLD_VAULE )
		result=0x01;
	
	if(analogRead(CHANNEL_2_PIN )>CHANNEL_THRESHOLD_VAULE )
		result|=0x02;
	Serial.begin(BAUD_RATE);
	Serial.print("CHANNEL NUMBER:");
	Serial.println(result);
	Serial.end();
	return result;

#endif
}


void get_rf_frequncy()
{
#ifdef _DBG_STATIC_FREQNCY 
			myChannel=0;
#else
			 myChannel= III_Get_Channel();			
#ifdef ONLINE_DEBUG
			Serial.begin(BAUD_RATE);
			Serial.print("init_Channel:");
			Serial.println(Mirf.channel);			
			Serial.end();
		  
#endif
		  
#endif


}


void RF_24L01_Init()
{
	    role = role_receiver;
  Serial.begin(BAUD_RATE);
  printf_begin();
  Serial.print(F("\n\rRF24/examples/pingpair_irq\n\rROLE: "));
  Serial.println(role_friendly_name[role]);

  // Setup and configure rf radio
  radio.begin();  
  //radio.setPALevel(RF24_PA_LOW);
  radio.enableAckPayload();                         // We will be using the Ack Payload feature, so please enable it
  radio.enableDynamicPayloads();                    // Ack payloads are dynamic payloads
                                                    // Open pipes to other node for communication
  if ( role == role_sender )
  	{                      // This simple sketch opens a pipe on a single address for these two nodes to 
     radio.openWritingPipe(address[0]);             // communicate back and forth.  One listens on it, the other talks to it.
     radio.openReadingPipe(1,address[1]); 
  }else{
    radio.openWritingPipe(address[1]);
    radio.openReadingPipe(1,address[0]);
    radio.startListening();
    radio.writeAckPayload( 1, &message_count, sizeof(message_count) );  // Add an ack packet for the next time around.  This is a simple
    //++message_count;        
  }
  radio.printDetails();                             // Dump the configuration of the rf unit for debugging
  delay(50);
   attachInterrupt(0, D2onChange, LOW);             // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver

	
}
void RF_reset()
{
    radio.powerDown();	
	delay(50);
radio.begin();	
//radio.setPALevel(RF24_PA_LOW);
radio.enableAckPayload();						  // We will be using the Ack Payload feature, so please enable it
radio.enableDynamicPayloads();					  // Ack payloads are dynamic payloads
												  // Open pipes to other node for communication
if ( role == role_sender )
  { 					 // This simple sketch opens a pipe on a single address for these two nodes to 
   radio.openWritingPipe(address[0]);			  // communicate back and forth.  One listens on it, the other talks to it.
   radio.openReadingPipe(1,address[1]); 
      radio.stopListening();
}
else
	{
  radio.openWritingPipe(address[1]);
  radio.openReadingPipe(1,address[0]);
  radio.startListening();
  radio.writeAckPayload( 1, &message_count, sizeof(message_count) );  // Add an ack packet for the next time around.  This is a simple
  //++message_count;		
}

//	 wirelessSPI.powerUp();
//	wirelessSPI.begin();			 //Start the nRF24 module
//		  wirelessSPI.setChannel(BASE_FREQUNCY-myChannel*20);
//		  wirelessSPI.setAutoAck(1);					// Ensure autoACK is enabled so rec sends ack packet to let you know it got the transmit packet payload
//		  wirelessSPI.setRetries( 15, 5 ) ;
//		  
//		  wirelessSPI.enableAckPayload();				// Allow optional ack payloads
//		  wirelessSPI.setPALevel(RF24_PA_LOW);
//		  wirelessSPI.openWritingPipe(pWAddress);		// pipe address that we will communicate over, must be the same for each nRF24 module
//		 wirelessSPI.openReadingPipe(1,pRAddress);	
//	
//		 wirelessSPI.maskIRQ(1,1,0);		 

}
void SetRF_Mode(bool pWorkingMode)
{
	if(pWorkingMode)// sender
	   {
	   
		if(role != role_sender)
		   {
			 role=role_sender;
			 RF_reset();
		   }
	   }
	else // receiver
	   {
  
		if(role == role_sender)
		   {
			   role = role_receiver;
			   RF_reset();
		   }
	   }
	

}



void III_Rf_Init()
{
  RF_24L01_Init();

}
void D2onChange()  
{  

  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);                     // What happened?
  rCount++;
  if ( tx ) {                                         // Have we successfully transmitted?
      if ( role == role_sender ){ 
//	  	Serial.println(F("Send:OK")); 
	  		}
      if ( role == role_receiver )
	  	{
//	  	Serial.println(F("Ack Payload:Sent")); 
			

	  }
  }
//  
  if ( fail ) {                                       // Have we failed to transmit?
      if ( role == role_sender ){  
//	  	Serial.println(F("Send:Failed")); 
	  }
      if ( role == role_receiver ){ 
	  	
		
//	  	Serial.println(F("Ack Payload:Failed")); 
	  }
  }
  
  if ( rx || radio.available()){                      // Did we receive a message?
    
    if ( role == role_sender ) {                      // If we're the sender, we've received an ack payload
        radio.read(&message_count,sizeof(message_count));
//        Serial.print(F("Ack: "));
//        Serial.println(message_count);
    }

    
    if ( role == role_receiver ) {                    // If we're the receiver, we've received a time message
                     // Get this payload and dump it
      radio.read( &rBuffer, sizeof(rBuffer) );  
	 
     
      radio.writeAckPayload( 1, &message_count, sizeof(message_count) );  // Add an ack packet for the next time around.  This is a simple
      ++message_count;                                // packet counter
    }
  }
} 

void III_Set_name()
{
	III_Get_myName();
#if 1//def _DEBUG_ONLINE
Serial.begin(BAUD_RATE);
   Serial.print("Myname:");
   Serial.println(myname);
Serial.end();
#endif
	switch(myname)
	{
	case 1:
		CHECK_BIT =ZQ_T;
        KEY_BIT=ZQ_O;
	break;
	case 2:
		CHECK_BIT =ZT_T;
        KEY_BIT=ZT_O;
	break;
	case 3:
		CHECK_BIT =YT_T;
        KEY_BIT=YT_O;
	break;
	case 4:
		CHECK_BIT =QL_T;
        KEY_BIT=QL_O;
	break;
	case 5:
		CHECK_BIT =BH_T;
        KEY_BIT=BH_O;
	break;
	
	case 6:
		CHECK_BIT =XW_T;
        KEY_BIT=XW_O;
	break;

	}


}



void setup()
{
  // put your setup code here, to run once:
#ifdef DEBUGMODE 
Serial.begin(BAUD_RATE);

//  Serial.print("Sender "); 
//  Serial.print(CONFIGRATION); 
  Serial.print(" ver "); 
  Serial.println(VERISON); 
Serial.end();
#endif
 


  get_rf_frequncy();

  III_Rf_Init();
  
  III_Set_name();
  pinMode(LED_PIN,OUTPUT);
  pinMode(LED2_PIN,OUTPUT);

  III_Control_LED(1);
  delay(200);
  III_Control_LED(0);
//  Serial.begin(BAUD_RATE);
//	   Serial.println("Son weak up!");  
//	     Serial.print(" ver "); 
//  Serial.println(VERISON); 
//Serial.end();
 // wdt_enable(TIMEOUT);
  // attachInterrupt(0, Receive, FALLING);
  rCount=0;
  message_count=0;

}

void rf_Send(char *str)
{
 int lens;
if(role != role_sender)
  SetRF_Mode(WRITING_MODE);
  
  lens = strlen(str);

  char msg[lens];
  int i;
  for (i = 0; i < lens; i++)
  {
    msg[i] = int(str[i]);
  }
  radio.startWrite( &msg, lens ,0);
//  if (!wirelessSPI.write( &msg, lens ))
//  	{  //if the send fails let the user know over serial monitor
//#ifdef ONLINE_DEBUG
//Serial.begin(BAUD_RATE);
//	   Serial.println("packet delivery failed");  
//Serial.end();
//#endif
//   }
}
void rf_Send_key()
{
  int i;
  char sd[2];
  sd[0] = '$';
  sd[1] = KEY_BIT;

    rf_Send(sd);


}
bool III_Get_Battery_State() // false means low power
{
#ifdef _DEBUG_LOWPOWER
#if _DEBUG_LOWPOWER == 1
  return true;
#elif _DEBUG_LOWPOWER == 2
  return false;
#endif
#endif
  int valb;
  valb = analogRead(BATTERY_PIN );
 #ifdef DEBUGMODE
Serial.begin(BAUD_RATE);
  Serial.print("Batter ADC:");
  Serial.println(valb);
Serial.end();
 #endif
  delay(200);
  if (valb <= BATTERY_THRESHOLD_VAULE )
  {
    valb = analogRead(BATTERY_PIN );
    if (valb <= BATTERY_THRESHOLD_VAULE )
    {
      return false;
    }
    else
		return true;

  }
  else
  	return true;

}
void III_Control_LED(bool sw)
{
  if (sw)
  {
    //Trun on
    digitalWrite(LED_PIN , HIGH);
  }
  else
  {
    //Trun off
    digitalWrite(LED_PIN, LOW);
  }

}
void III_Control_LED2(bool sw)
{
  if (sw)
  {
    //Trun on
    digitalWrite(LED2_PIN , HIGH);
  }
  else
  {
    //Trun off
    digitalWrite(LED2_PIN, LOW);
  }

}
void II_Blink_LED()
{
int fi;
for(fi=0;fi<3;fi++)
{
    III_Control_LED(1);
	delay(500);
	III_Control_LED(0);
	delay(300);
	III_Control_LED(1);
	delay(500);
	III_Control_LED(0);
	delay(200);
	III_Control_LED(1);
	delay(500);
	III_Control_LED(0);
	delay(100);
}
	III_Get_myName();
//	if(myname==NAME_ERROR)
//	{
//	II_Blink_LED();
//	}

}
bool III_Get_KeyState()
{ int result;
  result=analogRead(BEAT_PIN);
#ifdef DEBUGMODE
Serial.begin(BAUD_RATE);
  Serial.print("get key state :");
  Serial.println(result);
Serial.end();
#endif

  if(result<BEAT_THRESHOLD_VAULE)
  {
    delay(10);
    result=analogRead(BEAT_PIN);
     if(result<BEAT_THRESHOLD_VAULE)
     return true;
        else
    return false;

    }
    else
    return false;
}
bool get_key()
{
  bool sre;
  if ( III_Get_KeyState())
  {
    delay(20);
    if ( III_Get_KeyState())
    {
      sre = true;
    }
    else
      sre = false;
  }
  else
    sre = false;
  return sre;
}
void run_training()
{
  int timeout;
  III_Control_LED(1);
  delay(3);
  timeout = 0;
  while (timeout < 199)
  {
    if (get_key())
    {
      III_Control_LED(0);
      rf_Send_key();
    break;
    }
    timeout++;
    delay(5);
  }
  III_Control_LED(0);

}
void _test()
{

	
	int _ii;

	
//	 Serial.println("==1st test item get name==");
//		for(_ii=0;_ii<6;_ii++)
//		{
//			III_Get_myName();

//			Serial.print(_ii);
//				Serial.print(" NAME:");
//				Serial.println(myname);		
//				delay(1000);
//		}	
//	 Serial.println("==Next test item get channel==");
//		for(_ii=0;_ii<5;_ii++)
//		{
//		myChannel= III_Get_Channel();
//		 delay(1000);
//		}
			
//	 Serial.println("==Next test item LED state==");
//		for(_ii=0;_ii<2;_ii++)
//		{
//		III_Control_LED(1);
//		III_Control_LED2(1);
//		delay(1000);
//		III_Control_LED(0);
//		III_Control_LED2(0);
//		delay(1000);
//		}
//	 Serial.println("==Next test item ADC state==");
//		for(_ii=0;_ii<50;_ii++)
//		{
//	 	if(III_Get_Battery_State())
//			Serial.println("ok");
//		else
//			Serial.println("low power");
//		delay(100);
//		}
//	 Serial.println("==Next test item key state==");
//	if(get_key())
//		Serial.println("get beat");
//	 	
//		delay(20);
	
//	III_Get_Battery_State();
//delay(1000);

}
void loop()
{
  
  //wdt_reset();
 // III_Rf_Init();
 #ifdef DEBUGMODE

_test();
 
 #else // working mode
 if(role == role_sender)
 SetRF_Mode(READING_MODE);



  if(rCount>0)
  {

//Serial.begin(BAUD_RATE);
//	Serial.print(F("Gotl "));
//		Serial.print(rBuffer[0]);
//		Serial.println(rBuffer[1]);
//Serial.end();

    if (rBuffer[0] == '%') //beat
    {

      if (rBuffer[1] == KEY_BIT)
      {
        run_training();
      }

    }
    else if (rBuffer[0] == '#') //Open led
    {

//      if (data[1] == KEY_BIT)
      {
        III_Control_LED(1);
      }

    }
    else if (rBuffer[0] == '!') //Close led
    {

//      if (data[1] == KEY_BIT)
      {
        III_Control_LED(0);
      }

    }

    else if (rBuffer[0] == '+')//+ //check online & power level +
    {

      if (rBuffer[1] == KEY_BIT)
      {

      #if 1
	  char sd[2];
	   sd[0] = '$';
	   sd[1] = KEY_BIT;
	  delay(100);
	   rf_Send(sd);
	   

	    III_Control_LED(1);
       delay(50);
       III_Control_LED(0);
	  delay(50);		  
	  III_Control_LED(1);
	  delay(200);
	  III_Control_LED(0);
	  delay(50);				
	  III_Control_LED(1);
	  delay(50);
	  III_Control_LED(0);
	  
		
	  #else
        char sd[2];
//        III_Control_LED(1);
        sd[0] = '$';
        if (!III_Get_Battery_State())
        {
          sd[1] = 'L';
        }
        else
        {
          sd[1] = KEY_BIT;
        }
    //timeout=0;
     // delay(CONFIGRATION*10);
    //delay(5);
      Serial.println("get");
        //while((!Mirf.dataReady())&&timeout<300)
       // for(timeout=0;timeout<50;timeout++)
        {
        III_Rf_Init();
          rf_Send(sd);
//      delay(19);
//       rf_Send(sd);
//       delay(39);
//        rf_Send(sd);
//       delay(79);
  
        }
       III_Control_LED(1);
       delay(50);
       III_Control_LED(0);
	  delay(150);		  
	  III_Control_LED(1);
	  delay(170);
	  III_Control_LED(0);
	  delay(100);				
	  III_Control_LED(1);
	  delay(50);
	  III_Control_LED(0);
	  #endif

      }
    }// +++

  rCount=0;
  rBuffer[0]=0;
  rBuffer[1]=0;  

    
  }

  else //for beat birds
  {
    if (get_key())
    {
#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
      Serial.println(KEY_BIT);
Serial.end();
#endif
      III_Control_LED(0);
  //  III_Rf_Init();
      rf_Send_key();
    // III_Rf_Init();
      delay(30);
    }


  }
 // delay(2);
#ifdef SLEEP_TEST
  ////////debug sleep mode 
   III_Control_LED(1);
  delay(1000);
   III_Control_LED(0);
   delay(1000);
      III_Control_LED(1);
  delay(1000);
   III_Control_LED(0);
   delay(1000);
      III_Control_LED(1);
  delay(1000);
   III_Control_LED(0);
   
  delay(3000);
  
  Sleep_avr();
#endif
//  III_Control_LED(0);// for idle mode
#endif
}


