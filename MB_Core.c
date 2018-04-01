﻿//<<<<MOTHER BOARD PROGRAM>>>>

//0.6 Debug mode ,base onKongFu 0.0.41
//0.7
///1029 Prerelease version
//1.0 Nov-04 new rule ,and fixed button fake press,and reindex all postionof keys
//1.5 Follow btverison v1.5
//1.6 remove link check and add silence mode
//1.7 only for new PCB v3
//1.8 ADD RESET CMD for PCB V3
//1.9 Wirless Version1219 1220121
//2.0 Wirless +frequency hopping
//216 test for debug mother board 
//219--117MB Merge single version"V198"
//223 Merge single version v199""
//228 add battery voltage check part TBD
//230 try to with TRM replace new  MIRF
//240 another version new libray to TRM
//242 implement TRM Plan in global
//250 try version
//260 modify the  sound play again
//270 ADD BEAT FEEDBACK 
//280 DEBUG
//290 ADD led blink feature

#include <SoftwareSerial.h>
#include <SPI.h>
//#include <Mirf.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

//#include <MirfHardwareSpiDriver.h>

#define VERION    290
//#define DEBUG 11
//#define ONLINE_DEBUG 12
//#define STEP2STEP_DEBUG 13
#define _DBG_STATIC_FREQNCY  15
#define _DBG_NOCHECK_BATTERY 16
//#define _DBG_RF11_INFO		  17



#define STATE_TRAINNING 1
#define STATE_IDEA      2
#define STATE_LOWPOWER  3
#define STATE_BLUETOOTH 4
#define STATE_IDEA_SPKOVER    5
#define STATE_UNPARING_S1  6
#define STATE_UNSELECTMODE_S24       7
#define STATE_PRE_TRAINNING_S5 8

#define GAME_MODE_SINGLE  10
#define GAME_MODE_ROUND   11
#define GAME_MODE_ERROR    13
#define BAUD_RATE 9600
#define GET_WHO_IS_ONLINE_TRYTIMES  250

#define BASE_FREQUNCY   90
#define SPEED_DATA_RATES 0x0e// 0x06 7 1M 0x0f 2M 

#define AMP 4 //A0
#define BTP 15 //A1
#define V3V 3 //A1


#define ANT_POPA 300
#define ANT_POPB 500
#define ANT_POPC 1000
#define DEBONDSTIME 20

#define BT 1
#define SS 2
#define CK 3
#define LON 4
#define LOFF 5
#define ACK 6

#define SW_A2  100
#define SW_A3  100
#define SW_A4  100

#define CHANNEL_1_PIN A3
#define CHANNEL_2_PIN A2
#define CHANNEL_THRESHOLD_VAULE 50


#define BATTERY_PIN A7
#define BATTERY_THRESHOLD_VAULE 640

#define READING_MODE false
#define WRITING_MODE true


#ifdef ONLINE_DEBUG
#define PAIRING_TIMEOUT 30000
#define CONNECT_CHECK_TIMEOUT 500
#define UP_RESULT_CONNECT_CHECK_TIMEOUT 10000//1500
#define FORCE_SHARE_INFORMATION_TIMEOUT 60000
#define TRY_AGAIN_TIMEOUT 5
#define TRY_AGAIN_TIMEO 36000
#define RUN_SCRIPT_TIME 20
#define ONEBIRD_START_TIMEOUT 5
#else
#define PAIRING_TIMEOUT 30000
#define CONNECT_CHECK_TIMEOUT 500
#define UP_RESULT_CONNECT_CHECK_TIMEOUT 10000//1500
#define FORCE_SHARE_INFORMATION_TIMEOUT 60000

#define TRY_AGAIN_TIMEOUT 15
#define TRY_AGAIN_TIMEO 36000
#define RUN_SCRIPT_TIME 90
#define ONEBIRD_START_TIMEOUT 10

#endif


SoftwareSerial ss(9,6);
//byte rowPins[3] = {3, 4, 5}; //connect to the row pinouts of the keypad
//byte colPins[3] = {11, 12, 7}; //connect to the column pinouts of the keypad
//char hexaKeys[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}}; // the key value  NEED MODFIY BY SETUP
//LedControl lc = LedControl(8, 9, 10, 1); // pin 8,9,10

int value;
unsigned long delaytimes = 2000;
//Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, 3, 3);
byte SectionSelect;
byte GameMode;

////resource part
byte snd_1[6] = {0xAA, 0x07, 0x02, 0x00, 0x01, 0xB4};//1 280
byte snd_2[6] = {0xAA, 0x07, 0x02, 0x00, 0x02, 0xB5};//2 200
byte snd_3[6] = {0xAA, 0x07, 0x02, 0x00, 0x03, 0xB6};//3 12000
byte snd_4[6] = {0xAA, 0x07, 0x02, 0x00, 0x04, 0xB7};// 4 630
byte snd_5[6] = {0xAA, 0x07, 0x02, 0x00, 0x05, 0xB8};// 5 7900
byte snd_6[6] = {0xAA, 0x07, 0x02, 0x00, 0x06, 0xB9};// 6 5900
byte snd_7[6] = {0xAA, 0x07, 0x02, 0x00, 0x07, 0xBA};// 7 2400
byte snd_8[6] = {0xAA, 0x07, 0x02, 0x00, 0x08, 0xBB};// 8 674
byte snd_9[6] = {0xAA, 0x07, 0x02, 0x00, 0x09, 0xBC};// 9 4860
byte snd_10[6] = {0xAA, 0x07, 0x02, 0x00, 0x0A, 0xBD};// 10 2297
byte snd_11[6] = {0xAA, 0x07, 0x02, 0x00, 0x0B, 0xBE};// 11 4000
byte snd_12[6] = {0xAA, 0x07, 0x02, 0x00, 0x0C, 0xBF};// 12 2000
byte snd_13[6] = {0xAA, 0x07, 0x02, 0x00, 0x0D, 0xC0};// 13 1962
byte snd_14[6] = {0xAA, 0x07, 0x02, 0x00, 0x0E, 0xC1};// 14 2529
byte snd_15[6] = {0xAA, 0x07, 0x02, 0x00, 0x0F, 0xC2};// 15 3281
byte snd_16[6] = {0xAA, 0x07, 0x02, 0x00, 0x10, 0xC3};// 16 650
byte snd_17[6] = {0xAA, 0x07, 0x02, 0x00, 0x11, 0xC4};// 17 2000
byte snd_18[6] = {0xAA, 0x07, 0x02, 0x00, 0x12, 0xC5};// 18 4000
byte snd_19[6] = {0xAA, 0x07, 0x02, 0x00, 0x13, 0xC6};// 19 2441
byte snd_20[6] = {0xAA, 0x07, 0x02, 0x00, 0x14, 0xC7};// 20 666
byte snd_21[6] = {0xAA, 0x07, 0x02, 0x00, 0x15, 0xC8};// 21 3411
byte snd_22[6] = {0xAA, 0x07, 0x02, 0x00, 0x16, 0xC9};// 22 5700
byte snd_23[6] = {0xAA, 0x07, 0x02, 0x00, 0x17, 0xCA};// 23 624

byte snd_24[6] = {0xAA, 0x07, 0x02, 0x00, 0x18, 0xCB};// 24 3138
byte snd_25[6] = {0xAA, 0x07, 0x02, 0x00, 0x19, 0xCC};// 25 600
byte snd_26[6] = {0xAA, 0x07, 0x02, 0x00, 0x1A, 0xCD};// 26 4385
byte snd_27[6] = {0xAA, 0x07, 0x02, 0x00, 0x1B, 0xCE};// 27 500

byte snd_28[6] = {0xAA, 0x07, 0x02, 0x00, 0x1C, 0xCF};// 28 3800
byte snd_29[6] = {0xAA, 0x07, 0x02, 0x00, 0x1D, 0xD0};// 29 3800
byte snd_30[6] = {0xAA, 0x07, 0x02, 0x00, 0x1E, 0xD1};// 30 5000

byte snd_31[6] = {0xAA, 0x07, 0x02, 0x00, 0x1F, 0xD2};// 31 3000
byte snd_32[6] = {0xAA, 0x07, 0x02, 0x00, 0x20, 0xD3};// 32 3100
byte snd_33[6] = {0xAA, 0x07, 0x02, 0x00, 0x21, 0xD4};// 33 6300



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

//static uint32_t message_count = 0;

//rf o


int timeout;
int total_record;
int total_take_time;
int work_state;
String comdata = "";
String ID = "";
bool gSilence_Mode;

int myChannel;

void(* resetFunc) (void) = 0;
void bt_Throwout_Error_withoutack();
int get_who_is_online();
void Sendcmd(byte  ps[], int sizef);
void bt_upload_state_withoutack();// 0 means no respond.
void bt_upload_select_withoutack();// 0 means no respond.
void bt_Throwout_Error_withoutack();
bool bt_upload_LowPower();

void system_reset()
{
  //tbd
  II_Play_s22_BT_disconnect();
  bt_Throwout_Error_withoutack();//v1.8
  III_BT(0);
  delay(5000);
  III_BT(1);
  resetFunc();
  III_BT(1);


}
void III_AMP(bool sw)
{

  digitalWrite(AMP, sw);

}
bool III_Get_Battery_State() // false means low power
{

  int valb;
  valb = analogRead(BATTERY_PIN );
 #ifdef DEBUG
Serial.begin(BAUD_RATE);
  Serial.print("Batter ADC:");
  Serial.println(valb);
Serial.end();
 #endif
  delay(200);
  if (valb <= BATTERY_THRESHOLD_VAULE )
  {
  delay(499);
    valb = analogRead(BATTERY_PIN );
    if (valb <= BATTERY_THRESHOLD_VAULE )
    {
      return false;
    }
    else return true;

  }
  else return true;

}


void III_BT(bool sw)
{

  digitalWrite(V3V, !sw);

}

void III_3v3(bool sw)// rf reset
{

  digitalWrite(V3V, !sw);
  
}
int III_Get_Channel()
{
#ifdef DEBUG
	int result;
	result=analogRead(CHANNEL_1_PIN );
Serial.begin(BAUD_RATE);
	Serial.print("dbChannel 1:");
	Serial.println(result);
Serial.end();
	delay(1000);
	
	result=analogRead(CHANNEL_2_PIN  );
Serial.begin(BAUD_RATE);
	Serial.print("dbChannel 2:");
	Serial.println(result);
Serial.end();
	delay(1000);
	

#else
	int result;
	if(analogRead(CHANNEL_1_PIN )>CHANNEL_THRESHOLD_VAULE )
		result=0x01;
	else
		result=0x00;
	
	if(analogRead(CHANNEL_2_PIN  )>CHANNEL_THRESHOLD_VAULE )
		result!=0x02;
	
	return result;
#endif
}

//byte snd_car_abnormal[6] = {0xAA, 0x07, 0x02, 0x00, 0x05, 0xB8};// Car abnormal
//byte snd_batterylow[6] = {0xAA, 0x07, 0x02, 0x00, 0x06, 0xB9};// battery low
void II_PlayWave(byte wname[], int delayt)
{

  //byte snd_temp[6];
  // memcpy(snd_temp, wname, 6);
  //set_volume();
  //  delay(200);
  III_AMP(1);
  delay(20);

  Sendcmd(wname, 6);
  delay(delayt);
  III_AMP(0);
  //delay(500);
  // set_playover();
  //delay(100);
  //set_volume_silence();
  //delay(200);

}

void II_Play_S1_start()//zhinengleiba kaiqi
{
	
#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	Serial.println("II_Play_S1_start");
Serial.end();
#endif

  //  II_PlayWave(snd_24,3138+500);
  II_PlayWave(snd_24, 3138 + ANT_POPC);

}
void II_Play_end()
{
  //II_PlayWave(, );
}
void II_Play_beat1()
{
  II_PlayWave(snd_1, 300);
}
void II_Play_beat2()
{
  II_PlayWave(snd_2, 300);
}
void II_Play_S3_singlemode()
{
  II_PlayWave(snd_7, 2400 + ANT_POPB);
  GameMode = GAME_MODE_SINGLE;
}
void II_Play_S4_roundmode()
{
  II_PlayWave(snd_19, 2441 + ANT_POPB);
  GameMode = GAME_MODE_ROUND;
}
void II_Play_forward()
{
  II_PlayWave(snd_25, 600 + ANT_POPA);
  //Sendcmd(snd_25, 6);
  //delay(800);
}
void II_Play_backward()
{
  II_PlayWave(snd_20, 666 + ANT_POPA);
  // Sendcmd(snd_20, 6);
  //delay(800);
}
void II_Play_leftward()
{
  II_PlayWave(snd_16, 650 + ANT_POPA);
  //    Sendcmd(snd_16, 6);
  // delay(800);
}
void II_Play_rightward()
{
  II_PlayWave(snd_4, 630 + ANT_POPA);
  //Sendcmd(snd_4, 6);
  // delay(800);
}
void II_Play_levelE()
{
  II_PlayWave(snd_9, 4860 + ANT_POPC);
}
void II_Play_levelD()
{
  II_PlayWave(snd_26, 4385 + ANT_POPC);
}
void II_Play_levelC()
{
  II_PlayWave(snd_18, 4000 + ANT_POPC);
}
void II_Play_levelB()
{
  II_PlayWave(snd_5, 7900 + ANT_POPC);
}
void II_Play_levelA()
{
  II_PlayWave(snd_6, 5900 + ANT_POPC);
}
void II_Play_S5_welcome()
{
	
#ifdef STEP2STEP_DEBUG	
Serial.begin(BAUD_RATE);
	Serial.println("II_Play_S5_welcome");
Serial.end();
#endif

  II_PlayWave(snd_3, 12000 + ANT_POPC);
}

void II_Play_S7_startword()
{
	bt_upload_state_withoutack();

#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	Serial.println("II_Play_S7_startword");
Serial.end();
#endif
  II_PlayWave(snd_11, 4000 + ANT_POPC);
}
void II_Play_lowpower()
{
#ifdef STEP2STEP_DEBUG
  Serial.begin(BAUD_RATE);
	  Serial.println("II_Play_lowpower");
  Serial.end();
#endif

  II_PlayWave(snd_8, 674 + ANT_POPC);
}
void II_Play_S15_BT_SendOver_dididi()//di~di~di~
{
  II_PlayWave(snd_14, 2529 + ANT_POPC);
}

void II_Play_btsend()
{
  //II_PlayWave(, );
}
void II_Play_S14_cycleEnd()
{
  II_PlayWave(snd_21, 3411 + ANT_POPC);
}
void II_Play_kongfu()
{
  //II_PlayWave(, );
}
void II_Play_foot_R()
{
  II_PlayWave(snd_23, 624 + ANT_POPA);
  //Sendcmd(snd_23, 6);
  //delay(1000);
}
void II_Play_foot_L()
{
  II_PlayWave(snd_27, 500 + ANT_POPA);
  //    Sendcmd(snd_27, 6);
  //  delay(1000);
}

void II_Play_S25_again()
{
#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	Serial.println("II_Play_S25_again");
Serial.end();
#endif
  II_PlayWave(snd_10, 2297 + ANT_POPC);
}
void II_Play_S2_BT_connectOK()
{
#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	Serial.println("II_Play_S2_BT_connectOK");
Serial.end();
#endif
  II_PlayWave(snd_12, 2000 + ANT_POPC);
}

void II_Play_s22_BT_disconnect()
{
#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	Serial.println("II_Play_s22_BT_disconnect");
Serial.end();
#endif
  II_PlayWave(snd_13, 1962 + ANT_POPC);
}
void II_Play_S23_Closing()
{
  II_PlayWave(snd_15, 3281 + ANT_POPC);
}
void II_Play_S24_Select_mode()
{
#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	Serial.println("II_Play_S24_Select_mode");
Serial.end();
#endif
 bt_upload_select_withoutack();
  II_PlayWave(snd_17, 2000 + ANT_POPC);
}
void II_Play_S6_start_QuanPuwords()
{
  int result;

  result = random(1, 5);// 9 targets version
#ifdef  STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
  Serial.print("QuanPu-");

  Serial.println(result);
Serial.end();
#endif
  switch (result)
  {
    case 1:
      II_Play_S6_G1();
      break;
    case 2:
      II_Play_S6_G2();
      break;
    case 3:
      II_Play_S6_G3();
      break;
    default:
      II_Play_S6_G4();
      break;

  }
}
void II_Play_S6_G1()
{
  II_PlayWave(snd_22, 5700 + ANT_POPC);
}

void II_Play_S6_G2()
{
  II_PlayWave(snd_28, 3800 + ANT_POPC);
}
void II_Play_S6_G3()
{
  II_PlayWave(snd_29, 3800 + ANT_POPC);
}

void II_Play_S6_G4()
{
  II_PlayWave(snd_30, 5000 + ANT_POPC);
}
void II_Play_SX_Error_TargetPower()
{
Serial.begin(BAUD_RATE);
  Serial.print("#TARGET_OFF#");
Serial.end();

  II_PlayWave(snd_32, 3100 + ANT_POPC);
}
void II_Play_S26_Share()//v197
{
   II_PlayWave(snd_33, 6300 + ANT_POPC);
}

void II_Play_SX_ReselectMode()// TBD
{
  II_PlayWave(snd_31, 3000 + ANT_POPC);
}

void III_Play_Who_LowPower(int who)
{
  for (int i = 0; i < 2; i++)
  {
    switch (who)
    {
      case 1:
        II_Play_forward();
        break;

      case 2:
        break;
        II_Play_foot_L();

      case 3:
        II_Play_foot_R();
        break;

      case 4:
        II_Play_leftward();
        break;

      case 5:
        II_Play_rightward();
        break;

      case 6:
        II_Play_backward();
        break;
      default:
        break;

    }

    delay(500);
  }
  #ifdef STEP2STEP_DEBUG
  Serial.begin(BAUD_RATE);
	  Serial.println("II_Play_direct");
  Serial.end();
#endif
  II_Play_lowpower();
}
char _itoa(int a)
{
  //return atoi(a);
  char result;
  //Serial.println(a);
  switch (a)
  {
    case 1:
      result = '1';
      break;
    case 2:
      result = '2';
      break;
    case 3:
      result = '3';
      break;
    case 4:
      result = '4';
      break;
    case 5:
      result = '5';
      break;
    case 6:
      result = '6';
      break;
    case 7:
      result = '7';
      break;
    case 8:
      result = '8';
      break;
    case 9:
      result = '9';
      break;
    default:
      result = '0';
      break;


  }
  return result;
}

int _atoi(char a)
{
  //return atoi(a);
  int result;
  //Serial.println(a);
  switch (a)
  {
    case '1':
      result = 1;
      break;
    case '2':
      result = 2;
      break;
    case '3':
      result = 3;
      break;
    case '4':
      result = 4;
      break;
    case '5':
      result = 5;
      break;
    case '6':
      result = 6;
      break;
    case '7':
      result = 7;
      break;
    case '8':
      result = 8;
      break;
    case '9':
      result = 9;
      break;
    default:
      result = 0;
      break;


  }
  return result;
}



void w_Send_oneSignal(int type, int num)
{
  char sd[2];
 
	SetRF_ModeM(WRITING_MODE);

  if (type == BT)
  {
    sd[0] = '%';
  }
  else if (type == SS)
  {
    sd[0] = '*';
  }
  else if (type == CK)
  {
    sd[0] = '+';
  }
  else if (type == LON)
  {
    sd[0] = '#';
  }
  else if (type == LOFF)
  {
    sd[0] = '!';
  }
    else if (type == ACK)
  {
    sd[0] = '~';
  }
  switch (num)
  {
    case 1:
      sd[1] = '1';
      break;
    case 2:
      sd[1] = '2';
      break;
    case 3:
      sd[1] = '3';
      break;
    case 4:
      sd[1] = '4';
      break;
    case 5:
      sd[1] = '5';
      break;
    case 6:
      sd[1] = '6';
      break;
	case 99:
	  sd[1] = 'A';
      break;
  }
//  III_Rf_Init(0);
 // w_Sends(sd);
 radio.startWrite( sd, 2 ,0);

  //  Serial.println(sd[1]);
}
void III_TrunON_All_LED()
{
  int i;
  for (i = 1; i < 2; i++)
  {
    w_Send_oneSignal(LON, i);
    delay(5);
//    w_Send_oneSignal(LON, i);
  }
}
void  III_TrunOFF_All_LED()
{
  int i;
  for (i = 1; i < 3; i++)
  {
    w_Send_oneSignal(LOFF, i);
    delay(5);
//    w_Send_oneSignal(LOFF, i);
  }
}
void  III_Switch_LED(bool sw, int i)
{

  if (sw)
    w_Send_oneSignal(LON, i);
  else
    w_Send_oneSignal(LOFF, i);
}



void test_all_target()
{
#if 1 // v242 revivify
  int  i;

  for (i = 1; i < 3; i++)
  {
    delay(500);
    timeout = 0;
    w_Send_oneSignal(BT, i);
	  SetRF_ModeM(READING_MODE);
    while ((timeout < 205)&&(rBuffer[1]==0))
    {
      timeout++;
	  delay(50);
    	}
     // byte data[Mirf.payload];
      if (rBuffer[1]!=0)
      {
        int ii;
        String Temp;
        for (ii = 0; ii < 2; ii++) //
        {
          Temp += char(rBuffer[ii]);
        }
		Serial.begin(BAUD_RATE);
        Serial.println(i);

        if (rBuffer[0] == '$')
        {
          //record ++ //TBD

          Serial.println("jj");
          if ((char)rBuffer[1] == _itoa(i))
          {
            Serial.println("ggg");
            break;
          }
        }
	  Serial.end();

      }
       
    

  }
#endif
}


////////############# Test Function Part
void LED_blink()
{
	w_Send_oneSignal(SS,3);
	delay(300);
	w_Send_oneSignal(SS,3);
	delay(200);
	w_Send_oneSignal(SS,3);
	delay(100);
	w_Send_oneSignal(SS,3);
	




}
void run_Script_test()
{
  work_state = STATE_TRAINNING;

  int i;
  int act_d;
  int delayt;
  int j;
 // set_volume(); //TBD
  for (j = 0; j < 10;j ++)
	  {
			  for (i = 1; i < 7; i++)
		  {

		    oneBeat(i);
		   delay(1000); //TAKE A CARE
		  }
	  }
  
  //delay(1000);
  // set_playover();
  //  delay(100);
  // set_volume_silence();

}


void first_sndtest_debug()
{
  set_volume();
  delay(50);

  Sendcmd(snd_1, 6);
  delay(3000);
  Sendcmd(snd_2, 6);
  delay(3000);
  Sendcmd(snd_3, 6);
  delay(3000);

  Sendcmd(snd_4, 6);
  delay(3000);

  Sendcmd(snd_5, 6);
  delay(3000);

  Sendcmd(snd_6, 6);
  delay(3000);
  Sendcmd(snd_7, 6);
  delay(3000);
  Sendcmd(snd_8, 6);
  delay(3000);
  Sendcmd(snd_9, 6);

  delay(3000);
  Sendcmd(snd_10, 6);
  delay(3000);
  Sendcmd(snd_11, 6);
  delay(3000);
  Sendcmd(snd_12, 6);
  delay(3000);
  Sendcmd(snd_13, 6);
  delay(3000);
  Sendcmd(snd_14, 6);
  delay(3000);
  Sendcmd(snd_15, 6);
  delay(3000);
  Sendcmd(snd_16, 6);
  delay(3000);
  Sendcmd(snd_17, 6);
  delay(3000);
  Sendcmd(snd_18, 6);
  delay(3000);
  Sendcmd(snd_19, 6);
  delay(3000);
  Sendcmd(snd_20, 6);
  delay(3000);
  Sendcmd(snd_21, 6);
  delay(3000);
  Sendcmd(snd_22, 6);
  delay(3000);
  Sendcmd(snd_23, 6);
  delay(3000);
  Sendcmd(snd_24, 6);
  delay(3000);
  Sendcmd(snd_25, 6);
  delay(3000);
  Sendcmd(snd_26, 6);
  delay(3000);
  Sendcmd(snd_27, 6);
  delay(3000);

  //   delay(5);
  //  set_volume_silence();
  // delay(5000);


}
void DEBUG_sndtest()
{

#if 1
  //set_volume();

  II_Play_S1_start();
  delay(1000);
  II_Play_beat1();
  delay(1000);
  II_Play_beat2();
  delay(1000);
  II_Play_S3_singlemode();
  delay(1000);
  II_Play_S4_roundmode();
  delay(1000);
  II_Play_forward();
  delay(1000);
  II_Play_backward();
  delay(1000);
  II_Play_leftward();
  delay(1000);
  II_Play_rightward();
  delay(1000);
  II_Play_levelE();
  delay(1000);
  II_Play_levelD();
  delay(1000);
  II_Play_levelC();
  delay(1000);
  II_Play_levelB();
  delay(1000);
  II_Play_levelA();
  delay(1000);
  II_Play_S5_welcome();
  delay(1000);
  II_Play_S7_startword();
  delay(1000);
  II_Play_lowpower();
  delay(1000);
  II_Play_S15_BT_SendOver_dididi();
  delay(1000);
  II_Play_S14_cycleEnd();
  delay(1000);
  II_Play_foot_R();
  delay(1000);
  II_Play_foot_L();
  delay(1000);
  II_Play_S25_again();
  delay(1000);
  II_Play_S2_BT_connectOK();
  delay(1000);
  II_Play_s22_BT_disconnect();
  delay(1000);
  II_Play_S23_Closing();
  delay(1000);
  II_Play_S24_Select_mode();
  delay(1000);
  II_Play_S6_G1();
  delay(1000);
  II_Play_S6_G2();

  delay(1000);
  II_Play_S6_G3();

  delay(1000);
  II_Play_S6_G4();

  II_Play_SX_ReselectMode();
  delay(1000);
  II_Play_SX_Error_TargetPower();
  delay(1000);
   II_Play_S26_Share();


#endif

}
/////////////#################### Test Function Part over
////////////$$$$$$$$$$$$$$$$$$$$$ Base Function Part

void set_volume_silence()// aa 13 01 1e dc max
{

  SendData(0xaa);
  SendData(0x13);
  SendData(0x01);
  SendData(0x00);
  SendData(0xbe);

}

void set_volume()// aa 13 01 1e dc max
{
#ifdef STEP2STEP_DEBUG
	Serial.begin(BAUD_RATE);
	Serial.println("_set_volume");
	Serial.end();
#endif

  SendData(0xaa);
  SendData(0x13);
  SendData(0x01);
  SendData(0x1c);
  SendData(0xda);


}
void set_playover()
{
  SendData(0xaa);
  SendData(0x10);
  SendData(0x00);
  SendData(0xba);

}
void Clean_InputDataM()
{
	rCount=0;
	rBuffer[0]=0;
	rBuffer[1]=0;  

}

void SendData (byte addr)
{
  ss.write(addr);
}
void Sendcmd(byte  ps[], int sizef)
{
  for (int i = 0; i < sizef; i++)
  {
    SendData(ps[i]);
    //delay(1);
  }

}
bool beat_bird_toSTART()
{
#if 1 //v242 revivify
  int random_i; 
  timeout = 0;
  //III_TrunOFF_All_LED();
//  II_Play_S6_start_QuanPuwords(); v260
	LED_blink();

  III_TrunON_All_LED();


//  w_Send_oneSignal(BT,99);

  SetRF_ModeM(READING_MODE);

  while ((timeout < 64000)&&(rBuffer[1]==0))
  {
    timeout++;
	if(timeout%600==0)
		{
	   bt_upload_ready_withoutack();//v192
//	    w_Send_oneSignal(BT,99);
	   }
	delay(5);
  	}
#if 0 // remove this region 
    random_i = timeout % 7 + 1;
    if (gCheck_Section_Select(random_i))
      III_Switch_LED(true, random_i);
    else
      III_TrunOFF_All_LED();
#else
	
   

#endif
   // byte data[Mirf.payload];
	if(rBuffer[1]!=0)
    {     
      int i;
      String Temp;
      for (i = 0; i < 2; i++)
      {
        Temp += char(rBuffer[i]);
      }
#ifdef ONLINE_DEBUG
	  Serial.begin(BAUD_RATE);
      Serial.println(Temp);
#endif

      if ((rBuffer[0] == '$') )
      {
#ifdef ONLINE_DEBUG
        Serial.println(">Beat Bird!");
		Serial.end();
#endif
        II_Play_beat1();
        III_TrunOFF_All_LED();
		Clean_InputDataM();
        return true;

      }


    }
	else
  
#endif
  return false;


}



////////////////////$$$$$$$$$$$$$$$ Base Function part over
/////////////////////^^^^^^^^^^^^ APP Function Parts

// INT section
//----------------------------
///////////////////////////////            1-6
//           4,5+6(foot)
//       7          8
//             9
//-------------------------------
//     M 7 8 9 x x x x
//     a a a a x x x x
//0b   1 0 0 0  //only M(1-6) mode
//0b
bool  gCheck_Section_Select(int random_in)
{
  //SectionSelect default =0x0f 0b00001111
  switch (random_in)
  {

    case 1:
      if (SectionSelect & 0x01)
        return true;
      else
        return false;
      break;
    case 2:
      if (SectionSelect & 0x02)
        return true;
      else
        return false;
      break;
    case 3:
      if (SectionSelect & 0x04)
        return true;
      else
        return false;
      break;
    case 4:
      if (SectionSelect & 0x08)
        return true;
      else
        return false;
    case 5:
      if (SectionSelect & 0x10)
        return true;
      else
        return false;
    case 6:
      if (SectionSelect & 0x20)
        return true;
      else
        return false;
    default:

      return false;
      break;

  }
}
void gSection_Select_outspaker()
{
#ifdef ONLINE_DEBUG
Serial.begin(BAUD_RATE);
  Serial.print("ss=");
  Serial.println(SectionSelect);
Serial.end();
#endif
  if (!(SectionSelect & 0x0f))
    GameMode = GAME_MODE_ERROR;
  else
  {
    if ((SectionSelect & 0x08) || (SectionSelect & 0xf0))
    {
#ifdef  ONLINE_DEBUG
Serial.begin(BAUD_RATE);
      Serial.println("Round Mode");
Serial.end();
#endif
      II_Play_S4_roundmode();
    }
    else
    {
#ifdef  ONLINE_DEBUG
Serial.begin(BAUD_RATE);
      Serial.println("Single Mode");
Serial.end();
#endif
      II_Play_S3_singlemode();
    }
  }

}


int get_random_number()
{
  int result;
  //result = random(1, 10);// 9 targets version
  result = random(1, 7);// 9 targets version
  while (!gCheck_Section_Select(result))
  {
    result = random(1, 7);

  }
  return result;


}
void play_ss_ForTarget(int iDirect)
{
  switch (iDirect)
  {
    case 1:
      II_Play_forward();
      break;

    case 2:
      II_Play_foot_L();
      break;

    case 3:
      II_Play_foot_R();
      break;

    case 4:
      II_Play_leftward();
      break;

    case 5:
      II_Play_rightward();
      break;

    case 6:
      II_Play_backward();
      break;
  }
}
bool SendOneKeyforBeat(int key_in)
{
	Clean_InputDataM();
	timeout = 0;

	w_Send_oneSignal(BT, key_in); //send data
	SetRF_ModeM(READING_MODE);
	
		timeout = 0;
		while ((timeout < 3)&&(rBuffer[0]!='&'))
		{
		  timeout++;
		  w_Send_oneSignal(BT, key_in); //send data
		  SetRF_ModeM(READING_MODE);
		  	
		  delay(20);
		}

};
bool  oneBeat(int key_in)
{
#if 1 //v242  revivify
  int direct;
  int i;
  int vkey;
  timeout = 0;
  vkey = 0;
  bool sndr;

  sndr = false;
  Clean_InputDataM();
 // III_Rf_Init();

  if (key_in < 7)
  {
	play_ss_ForTarget(key_in);

    //  delay(1000);
          timeout = 0;

//    w_Send_oneSignal(BT, key_in); //send data
	SendOneKeyforBeat(key_in);

	SetRF_ModeM(READING_MODE);

    timeout = 0;
  	while ((timeout < 1009)&&(rBuffer[1]==0))
    {
      timeout++;
	  delay(1);
  	}

	if(timeout>1000)
	{
	//TODO HAVN'T BEAT
	}
	else
	{
			if ((rBuffer[0] == '$') )
			{
				 
				
				 if ((char)rBuffer[1] == _itoa(key_in))
				  {
				   if (!sndr)
				  {
					sndr = true;
					if (timeout % 2 == 0)
					  II_Play_beat1();
					else
					  II_Play_beat2();
				  }
#ifdef  ONLINE_DEBUG
					Serial.begin(BAUD_RATE);
					Serial.print("g");
					Serial.println(key_in);
					Serial.end();
#endif
			        total_record += 1;
			        total_take_time += timeout;

					w_Send_oneSignal(ACK,key_in);
//					w_Send_oneSignal(ACK,key_in);
					Clean_InputDataM();   
			        return true;
	       		}
		       else // wrong target TBD
		       {
		        //== TBD  2017-Dec-28
#if 0 //def  ONLINE_DEBUG /// CANNEL SINCE v280
				Serial.begin(BAUD_RATE);
				Serial.print("f:");
				Serial.println(rBuffer[1]);
				Serial.end();
#endif				
		       }


		    }
		
   }
	


  }


  Clean_InputDataM();	

  total_take_time += timeout;
  #endif
  return false;
}


void bt_sendData(int data) //bt send part TBD
{
  work_state = STATE_BLUETOOTH;
  II_Play_btsend();

Serial.begin(BAUD_RATE);
  Serial.print(data);
Serial.end();
  work_state = STATE_IDEA;

}

void bt_receivedata()
{

Serial.begin(BAUD_RATE);
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
  }
  if (comdata.length() > 0)
  {

    II_Play_S15_BT_SendOver_dididi();
    SectionSelect = comdata[0];
    byte state = (byte)get_who_is_online();
    // if command over state than report error to app and set state as real state.
    if (!check_state_error(SectionSelect, state))
    {
      bt_sendError();
      SectionSelect = state;
    }
    else
    {
      delay(2000);
      gSection_Select_outspaker();
      // Serial.println(comdata);
      comdata = "";
    }
  }
Serial.end();

}
void check_targetOnline_withVoice()
{
  byte state = (byte)get_who_is_online();
  if (SectionSelect == state)
  	{
	gSection_Select_outspaker();
	return;
    }
    


  // if command over state than report error to app and set state as real state.
#ifdef STEP2STEP_DEBUG
	 
Serial.begin(BAUD_RATE);
  Serial.println(SectionSelect);
  Serial.println(state);
Serial.end();


#endif

  if (!check_state_error(SectionSelect, state))
  {
 #ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
    Serial.println("Error: Too many targets was selected!");
Serial.end();
 #endif
    bt_sendError();
    SectionSelect = state; // Pluged < Selected
  }
  else
  {
    SectionSelect = SectionSelect; // Pluged > Selected
    delay(100);
    gSection_Select_outspaker();
    // Serial.println(comdata);

  }

}
void bt_sendError()
{
  work_state = STATE_BLUETOOTH;
Serial.begin(BAUD_RATE);
  Serial.print(0xff);
Serial.end();
  byte state = (byte)get_who_is_online();

  // II_Play_btsend();

Serial.begin(BAUD_RATE);
  Serial.print(state);
Serial.end();
  work_state = STATE_IDEA;

}
void bt_sendState()
{
  work_state = STATE_BLUETOOTH;

  byte state = (byte)get_who_is_online();

  // II_Play_btsend();

Serial.begin(BAUD_RATE);
  Serial.print(state);
Serial.end();
  work_state = STATE_IDEA;


}

bool check_state_error(byte app, byte host) // true is right ,false is error
{
  bool result;
  
	if((app&host)==app)
	{
	    return true;
	}
	else
		return false;
//  if ((app & 0x01 == 1) && (host & 0x01 == 0))
//  {
//    return result;
//  }
//  if ((app & 0x02 == 0x02) && (host & 0x02 == 0))
//  {
//    return result;
//  }
//  if ((app & 0x08 == 0x08) && (host & 0x08 == 0))
//  {
//    return result;
//  }
//  if ((app & 0x10 == 0x10) && (host & 0x10 == 0))
//  {
//    return result;
//  }
//  if ((app & 0x20 == 0x20) && (host & 0x20 == 0))
//  {
//    return result;
//  }
//  if ((app & 0x04 == 0x04) && (host & 0x04 == 0))
//  {
//    return result;
//  }

//  result = true;

//  return result;


}
void do_Battery_check()
{

#ifndef _DBG_NOCHECK_BATTERY
if(III_Get_Battery_State())
{
	II_Play_lowpower();
	delay(3000);

	II_Play_lowpower();
	delay(3000);
	
	II_Play_lowpower();
	delay(3000);
	system_reset();

}
#endif
}
void D2onChange()  
{  
	
	  bool tx,fail,rx;
	  radio.whatHappened(tx,fail,rx);					  // What happened?
//	  rCount++;
	  if ( tx ) {										  // Have we successfully transmitted?
		  if ( role == role_sender ){ 
	//		Serial.println(F("Send:OK")); 
			
				}
		  if ( role == role_receiver )
			{
	//		Serial.println(F("Ack Payload:Sent")); 
				
	
		  }
	  }
	//	
	  if ( fail ) { 									  // Have we failed to transmit?
		  if ( role == role_sender ){  
	//		Serial.println(F("Send:Failed")); 
		  }
		  if ( role == role_receiver ){ 
			
			
	//		Serial.println(F("Ack Payload:Failed")); 
		  }
	  }
	  
	  if ( rx || radio.available()){					  // Did we receive a message?
		
		if ( role == role_sender ) {					  // If we're the sender, we've received an ack payload
//		    radio.read(&message_count,sizeof(message_count));
			
	//		  Serial.print(F("Ack: "));
	//		  Serial.println(message_count);
		}
	
		
		if ( role == role_receiver ) {					  // If we're the receiver, we've received a time message
						 // Get this payload and dump it
		  radio.read( &rBuffer, sizeof(rBuffer) );	
		 
		 rCount++;
//		  radio.writeAckPayload( 1, &message_count, sizeof(message_count) );  // Add an ack packet for the next time around.  This is a simple
//		  ++message_count;								  // packet counter
		}
	  }

} 

int get_who_is_online()//
{
  int value;
  int result ;

  String Temp;
  int i, iii;
  int j;
  int q;
  result = 0;
  // first check itself 
  do_Battery_check();
 // for(q=0;q<3;q++)
  	{
  for (i = 1; i < 7; i++)
  {
    int rd=	random(3,9);
	delay(rd*53);

//	if((result&(0x01 << (i - 1)))==1)
//		continue;
	for (j = 0; j <7; j++)
    {
    	int rd=	random(3,7);
		delay(rd*47);

//#ifdef  STEP2STEP_DEBUG
//			Serial.begin(BAUD_RATE);
//		 Serial.print("i=");
//		 Serial.println(i);
//		 
//			Serial.end();
//#endif

	  w_Send_oneSignal(CK, i);// Direct bit target
//#ifdef  STEP2STEP_DEBUG
//			Serial.begin(BAUD_RATE);
//		  Serial.print("o");		   
//			Serial.end();
//#endif
	
	
      delay(30);

		SetRF_ModeM(READING_MODE);
	
//      while ((timeout < 550)&&(rCount==0))
//	while(radio.available()){		
//		 radio.read(&rBuffer,2);
//		 rCount++;
//		}
	   timeout = 0;
	  while ((timeout < GET_WHO_IS_ONLINE_TRYTIMES)&&(rBuffer[1]==0))
		{
		timeout++;
		delay(1);
	  }

	  if(rBuffer[1]!=0)
	  	{
//Serial.begin(BAUD_RATE);
//	 Serial.print("timeout:"); 	
//	 Serial.println(rBuffer[0]);
//	 Serial.println(rBuffer[1]);
//	 
//Serial.end();
		//Serial.begin();
				if (rBuffer[0] == '$')
				{
				  if ((char)rBuffer[1] == _itoa(i))
				  {
					result |= 0x01 << (i - 1);
#ifdef  STEP2STEP_DEBUG
//		Serial.begin(BAUD_RATE);
//				   Serial.print("get :");
//				  Serial.println(i);
//			  Serial.println(result);
//		Serial.end();
#endif
					
	  //		  w_Send_oneSignal(VT, i);
				j=8;//	goto NextTarget;
				  }
				  else if (rBuffer[1] == 'L')
				  {
	  
	  
#ifdef  STEP2STEP_DEBUG
				Serial.begin(BAUD_RATE);
					Serial.print("LowPower : ");
					Serial.println(i);
				Serial.end();
#endif
	  
					III_Play_Who_LowPower(i);
					
	  //			w_Send_oneSignal(VT, i);
					j=8;//goto NextTarget;  // v190 Repeat 1 times for low power
				  }
				}
		Clean_InputDataM();		

	  }
	  else
	  	{
	  	Clean_InputDataM();		
	     
#ifdef  STEP2STEP_DEBUG
				Serial.begin(BAUD_RATE);
		   Serial.print(i); 

		   Serial.print(".");			
				Serial.end();
#endif		

	    }

      
    }
//	NextTarget:
//	continue;
  }

}
  delay(10);
  return result;

}



////////////////////////=======
void run_Script(int times)
{
  work_state = STATE_TRAINNING;

  int i;
  int act_d;
  int delayt;
  total_record = 0;
  total_take_time = 0;


  for (i = 0; i < times; i++)
  {
    if (i % 5 == 0)
      bt_upload_state_withoutack();

    act_d = get_random_number();

    oneBeat(act_d);
	delay(800);

  }


}
void calc_result()
{
#if 0// 
  if (total_record != 0)
  {
    delay(500);
    //II_Play_end();
    delay(1000);
    if (total_record < 600)
    {
      II_Play_levelE();
    }
    else if ((total_record >= 600) && (total_record < 900))
    {
      II_Play_levelD();
    }
    else if ((total_record >= 900) && (total_record < 1350))
    {
      II_Play_levelC();
    }
    else if ((total_record >= 1350) && (total_record < 1800))
    {
      II_Play_levelB();
    }
    else
    {
      II_Play_levelA();
    }
  }//total_record!=0
  work_state = STATE_IDEA;
#else//debug
  // if (total_record != 0)
  {
    // delay(500);
    //II_Play_end();
    delay(1000);
    if (total_record < 18)
    {
      II_Play_levelE();
    }
    else if ((total_record >= 18) && (total_record < 36))
    {
      II_Play_levelD();
    }
    else if ((total_record >= 36) && (total_record < 54))
    {
      II_Play_levelC();
    }
    else if ((total_record >= 54) && (total_record < 72))
    {
      II_Play_levelB();
    }
    else
    {
      II_Play_levelA();
    }
  }//total_record!=0
  #ifdef STEP2STEP_DEBUG	
Serial.begin(BAUD_RATE);
	Serial.println("PLAY_Voice_Result");
Serial.end();
#endif
  work_state = STATE_IDEA;
#endif
}
/////////^^^^^^^^^^^^APP Function Part Over
int get_key()
{
  int result = 0;


  return result;

}


void gpio_init()
{

	pinMode(V3V,OUTPUT);
	delay(100);
	pinMode(AMP,OUTPUT);
	delay(100);


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
			Serial.println(myChannel);
		Serial.end();
		  
#endif
		  
#endif


}
void RF_24L01_InitM()
{
	role = role_sender;	
#ifdef _DBG_RF11_INFO
	Serial.begin(BAUD_RATE);
	printf_begin();
	Serial.print(F("\n\rRF24/examples/pingpair_irq\n\rROLE: "));
	Serial.println(role_friendly_name[role]);
	
#endif
	// Setup and configure rf radio
	radio.begin();	
	//radio.setPALevel(RF24_PA_LOW);
//	radio.enableAckPayload();						  // We will be using the Ack Payload feature, so please enable it
//	radio.enableDynamicPayloads();					  // Ack payloads are dynamic payloads
													  // Open pipes to other node for communication
	if ( role == role_sender )
	  { 					 // This simple sketch opens a pipe on a single address for these two nodes to 
	   radio.openWritingPipe(address[0]);			  // communicate back and forth.  One listens on it, the other talks to it.
	   radio.openReadingPipe(1,address[1]); 
	     radio.stopListening();
	}else{
	  radio.openWritingPipe(address[1]);
	  radio.openReadingPipe(1,address[0]);
	  radio.startListening();
//	  radio.writeAckPayload( 1, &message_count, sizeof(message_count) );  // Add an ack packet for the next time around.  This is a simple
	  //++message_count;		
	}
#ifdef _DBG_RF11_INFO
	radio.printDetails();							  // Dump the configuration of the rf unit for debugging
#endif
	delay(50);
	 attachInterrupt(0, D2onChange, LOW);			  // Attach interrupt handler to interrupt #0 (using pin 2) on BOTH the sender and receiver

}
void RF_resetM()
{
//    radio.powerDown();	
//	delay(50);

radio.begin();	
//radio.setPALevel(RF24_PA_LOW);
//radio.enableAckPayload();						  // We will be using the Ack Payload feature, so please enable it
//radio.enableDynamicPayloads();					  // Ack payloads are dynamic payloads
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
//  radio.writeAckPayload( 1, &message_count, sizeof(message_count) );  // Add an ack packet for the next time around.  This is a simple
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


void SetRF_ModeM(bool pWorkingMode)
{
	    if(pWorkingMode)// sender
			{
			
		 	 if(role != role_sender)
		 	 	{
				  role=role_sender;
				  RF_resetM();
		 	 	}
			}
		 else // receiver
			{

			 if(role != role_receiver)
			 	{
				    role = role_receiver;
				 	RF_resetM();
					
			 	}
			}


}

void RF_24L01_Frequency_Hopping(int channel)//channel must from 1-6 !!
{


//  Mirf.spi = &MirfHardwareSpi;
//  Mirf.init();
//  Mirf.setRADDR((byte *)"LAVAJ"); 
//  Mirf.payload = sizeof(data);
//  Mirf.channel = BASE_FREQUNCY-channel*15;
//  Mirf.config();
//  Mirf.configRegister(RF_SETUP,SPEED_DATA_RATES);

}
void RF_24L01_address_Hopping(int address)//channel must from 1-6 !!
{


//  Mirf.spi = &MirfHardwareSpi;
//  Mirf.init();
//  switch((int) address)
//  {
//  case 1:   
//    Mirf.setRADDR((byte *)"LAVAVA");
//    break;
//  
//  case 2:   
//    Mirf.setRADDR((byte *)"LBVBVB");
//    break;
//  
//  case 3:   
//    Mirf.setRADDR((byte *)"LCVCVC");
//    break;
//  
//  case 4:   
//    Mirf.setRADDR((byte *)"LDVDVD");
//    break;
//  
//  case 5:   
//    Mirf.setRADDR((byte *)"LEVEVE");
//    break;
//  
//  default:    
//    Mirf.setRADDR((byte *)"LFVFVF");
//    break;
//              

//  }
//  Mirf.payload = sizeof(data);
//  Mirf.channel = BASE_FREQUNCY;          
//  Mirf.config();
//  Mirf.configRegister(RF_SETUP,SPEED_DATA_RATES);

}
void III_Rf_Init(int paramter)
{
	
	RF_24L01_InitM();
//	RF_24L01_address_Hopping(paramter);
//	RF_24L01_Frequency_Hopping(paramter);

}

bool bt_pairing()// ture connected; false disconnect
{
  timeout = 0;
  comdata = "";
  
  LED_blink();

  delay(300);
   III_TrunOFF_All_LED();   
  delay(300);
  	
Serial.begin(BAUD_RATE);
  while (timeout < PAIRING_TIMEOUT)
  {
    delay(2);
    timeout++;
    while (Serial.available() > 0)
    {
      comdata += char(Serial.read());
      delay(2);

    }

    if (comdata.length() > 0)
    {
      if ((comdata[0] == '@') && (comdata[1] == 'P'))
      {
        String backdata;
        backdata.concat("ACK#");

        ID = comdata.substring(3, 9);
		comdata="";
        backdata.concat(ID);
        backdata.concat("#");

        Serial.println(backdata);
		Serial.end();

        return true;
      }
	  else if((comdata[0] == '@') && (comdata[1] == 'V'))
	  {
		  String backdata;
		  backdata.concat("#");
		  backdata.concat(VERION);				
		  backdata.concat("#");

		  Serial.println(backdata);

		  comdata="";
	      continue;
	  }
	  else if((comdata[0] == '@') && (comdata[1] == 'R'))//v198
	  {
		  comdata="";
		  system_reset();
	      return;

	  }
      else
      {
        comdata = "";
      }
    }
	
  }
  Serial.end();
  return false;
}


bool bt_Select_Mode()// 0 means no respond.
{
SELECTBIGIN:
  String backdata = "";
  bool first_talk = 0;
  timeout = 0;
  comdata = "";
  Serial.begin(BAUD_RATE);
  while (timeout < PAIRING_TIMEOUT)
  {
    delay(2);
    timeout++;
    if (timeout < 2)
    {
    backdata = "";
      backdata.concat("#");	  
      backdata.concat(SectionSelect);

      Serial.println(backdata);

    }

    while (Serial.available() > 0)
    {
      comdata += char(Serial.read());
      delay(2);

    }

    if (comdata.length() > 0)
    {
      if ((comdata[0] == 'A') && (comdata[1] == 'C'))
      {
      comdata = "";
        first_talk = 1;
        break;
      }
	  else if((comdata[0] == '@') && (comdata[1] == 'R'))//v198
	  {
		  comdata="";
		  Serial.end();
		  system_reset();
	      return false;

	  }
      else if ((comdata[0] == '@') && (comdata[1] == 'P'))//V192
      {
		ID = comdata.substring(3, 9);
		comdata="";
    	SectionSelect = get_who_is_online();
//		backdata = "";
//		backdata.concat("#");	  
//      backdata.concat(SectionSelect);
//      Serial.println(backdata);
		delay(2000);
		goto SELECTBIGIN;	   
      }
      else
      {
        comdata = "";
      }

    }
  }
  //get the command for APP
#ifdef STEP2STEP_DEBUG

  Serial.println("@S##");

#endif
  if (first_talk)
  {
    timeout = 0;
    comdata = "";
    while (timeout < PAIRING_TIMEOUT)
    {
      delay(2);
      timeout++;

      while (Serial.available() > 0)
      {
        comdata += char(Serial.read());
        delay(2);

      }

      if (comdata.length() > 0)
      {
        if ((comdata[0] == '@') && (comdata[1] == 'S'))
        {
          char ss1 = comdata[3];
		  comdata="";
          SectionSelect = (byte)ss1;//ss-48;
          backdata = "";
          backdata.concat("ACK#OK#");

          Serial.println(backdata);

          return true;
        }
	   else if((comdata[0] == '@') && (comdata[1] == 'R'))//v198
	  {
		  comdata="";
		  system_reset();
	      return false;

	  }
	  else if ((comdata[0] == '@') && (comdata[1] == 'P'))//V192
      {
		ID = comdata.substring(3, 9);
		comdata = "";
		SectionSelect = get_who_is_online();
//		backdata = "";
//		backdata.concat("#");	  
//      backdata.concat(SectionSelect);
//      Serial.println(backdata);
		delay(2000);
		goto SELECTBIGIN;	   
      }
        else
        {
          comdata = "";
        }

      }
    }
  }
  Serial.end();
  return false;


}
bool bt_upload_data()// 0 means no respond.
{
  String backdata;
  char backrecord[5];
  timeout = 0;
  comdata = "";
  Serial.begin(BAUD_RATE);
  while (timeout < UP_RESULT_CONNECT_CHECK_TIMEOUT)
  {
    delay(2);
    timeout++;
    if (timeout < 2)
    {

      backdata = "";
      backdata.concat("#R#");//v197
      itoa(total_record, backrecord, 10);
      backdata.concat(backrecord);
      backdata.concat("#");

      Serial.println(backdata);

    }


    while (Serial.available() > 0)
    {
      comdata += char(Serial.read());
      delay(2);

    }

    if (comdata.length() > 0)
    {
      if ((comdata[0] == 'A') && (comdata[1] == 'C'))
      {
		comdata = "";
	
        return true;
      }
	  else if((comdata[0] == '@') && (comdata[1] == 'S')) // tbd it will be confused with select mode?
      {
		comdata = "";
		return Force_share_information_action();
      }//v197 add
	  else if((comdata[0] == '@') && (comdata[1] == 'R'))//v198
	  {
		comdata="";
		Serial.end();
		system_reset();
		return false;
	
	  }

    }
  }

  return false;


}

bool Force_share_information_action()// 1 have shared 0 exit //v197 add
{
	timeout=0;
	II_Play_S26_Share();
		delay(2);
	while(timeout<FORCE_SHARE_INFORMATION_TIMEOUT)
	{
		
		   timeout++;
		  
	
		   while (Serial.available() > 0)
		   {
			 comdata += char(Serial.read());
			 delay(2);
		
		   }

		   if (comdata.length() > 0)
		   {
			 if ((comdata[0] == '@') && (comdata[1] == 'N'))//@NEXT
			 {
			   comdata = "";
			   return true;
			 }
			 else if((comdata[0] == '@') && (comdata[1] == 'R')) //@RESET			 	
			 {
			      comdata = "";
			   return false;
			  
			 }
		
		   }

	delay(2);

	}

	  return false;// over 60s reset


}
void bt_upload_state_withoutack()// 0 means no respond.
{
  String backdata;
  backdata = "";
  backdata.concat("#ACKTION#");
Serial.begin(BAUD_RATE);
  Serial.println(backdata);
Serial.end();


}
void bt_upload_ready_withoutack()// 0 means no respond.
{
  String backdata;
  backdata = "";
  backdata.concat("#READY#");
Serial.begin(BAUD_RATE);
  Serial.println(backdata);
Serial.end();


}

void bt_upload_begin_withoutack()// 0 means no respond.
{
  String backdata;
    backdata = "";
      backdata.concat("#START#");
Serial.begin(BAUD_RATE);
      Serial.println(backdata);
Serial.end();



}
void bt_upload_next_withoutack()// 0 means no respond.
{
  String backdata;
    backdata = "";
      backdata.concat("#NEXT#");
Serial.begin(BAUD_RATE);
      Serial.println(backdata);
Serial.end();
 


}
void bt_upload_select_withoutack()// 0 means no respond.
{
  String backdata;
    backdata = "";
      backdata.concat("#SELECTMODE#");
Serial.begin(BAUD_RATE);
      Serial.println(backdata);
Serial.end();
 


}

void bt_Throwout_Error_withoutack()// 0 means no respond.//v1.8
{
  String backdata;
  char backrecord[5];
  timeout = 0;
  comdata = "";
  Serial.begin(BAUD_RATE);

  while (timeout < CONNECT_CHECK_TIMEOUT)
  {
    delay(2);
    timeout++;
    if (timeout < 2)
    {

      backdata = "";
      backdata.concat("#RESET#");

      Serial.println(backdata);

    }

  }
	Serial.end();



}
bool bt_upload_LowPower()// 0 means no respond.
{
  String backdata;
  char backrecord[5];
  timeout = 0;
  comdata = "";
  II_Play_SX_ReselectMode();

  while (timeout < CONNECT_CHECK_TIMEOUT)
  {
    delay(2);
    timeout++;
    if (timeout < 2)
    {

      backdata = "";
      backdata.concat("#LOWPOWER#");
Serial.begin(BAUD_RATE);
      Serial.println(backdata);
Serial.end();
    }

Serial.begin(BAUD_RATE);
    while (Serial.available() > 0)
    {
      comdata += char(Serial.read());
      delay(2);

    }
Serial.end();
    if (comdata.length() > 0)
    {
      if ((comdata[0] == 'A') && (comdata[1] == 'C'))
      {

        return true;
      }

    }
  }
  return false;


}


bool bt_upload_state()// 0 means no respond.
{
  String backdata;
  char backrecord[5];
  timeout = 0;
  comdata = "";

  while (timeout < CONNECT_CHECK_TIMEOUT)
  {
    delay(2);
    timeout++;
    if (timeout < 2)
    {

      backdata = "";
      backdata.concat("#ACKTION#");
Serial.begin(BAUD_RATE);
      Serial.println(backdata);
Serial.end();
    }

Serial.begin(BAUD_RATE);
    while (Serial.available() > 0)
    {
      comdata += char(Serial.read());
      delay(2);

    }
Serial.end();
    if (comdata.length() > 0)
    {
      if ((comdata[0] == 'A') && (comdata[1] == 'C'))
      {
		comdata = "";
        return true;
      }
	  else if((comdata[0] == '@') && (comdata[1] == 'R'))//v198
	  {
		  comdata="";
		  system_reset();
	      return false;

	  }

    }
  }
  return false;


}

void bt_uploadstate_forLoop()
{
  int i;
  for ( i = 0; i < 3; i++)
  {
    if (bt_upload_state())
    {
      break;
    }
    if (i == 2)
    {
      break;//system_reset();

    }
    delay(40);
  }

}
void begin_traning()
{
//  II_Play_S7_startword();//v260
  //work_state=STATE_TRAINNING;// v15
  Clean_InputDataM();

  run_Script(RUN_SCRIPT_TIME);


  work_state = STATE_IDEA;
  II_Play_S14_cycleEnd();
  if (bt_upload_data())
  {
    II_Play_S15_BT_SendOver_dididi();

  }
  else
  {
   system_reset();
  }//v197

  calc_result();


}
void test_bt_upload()
{
  total_record = 249;
  if (bt_upload_data())
  {
Serial.begin(BAUD_RATE);
    Serial.println("uploaded");
Serial.end();
    comdata = "";
    // delay(5000);
  }
  else
  {
Serial.begin(BAUD_RATE);
    Serial.println("ul_disconnected");
Serial.end();
  }
}
void test_bt_pairing()
{

  if (bt_pairing())
  {
Serial.begin(BAUD_RATE);
    Serial.println("connected");
Serial.end();
    comdata = "";
    // delay(5000);
  }
  else
  {
Serial.begin(BAUD_RATE);
    Serial.println("disconnected");
Serial.end();
  }
}
void test_bt_upload_state()
{
  if (bt_upload_state())
  {
Serial.begin(BAUD_RATE);
    Serial.println("upload state ok");
Serial.end();
  }
  else
Serial.begin(BAUD_RATE);
    Serial.println("upload state fail.");
Serial.end();

}
void test_bt_selectMode()
{
Serial.begin(BAUD_RATE);
  SectionSelect = 0x00;
  if (bt_Select_Mode())
  {
    Serial.print("ss=");
    Serial.println(SectionSelect);
    comdata = "";
  }

  else
    Serial.println("SelectModeERROR.");

  if (SectionSelect & 0x0f == 0)
    Serial.println(" MODE ERROR");
  else
  {
    if ((SectionSelect & 0x08) && (SectionSelect & 0xf0))
      Serial.println("Round MODE");
    else
      Serial.println("Single MODE");
  }
Serial.end();


}

bool Bluetooth_test()
{
  //  test_bt_pairing();
  //test_bt_selectMode();
  test_bt_upload_state();
  //  test_bt_upload();
}
bool Traning_again_zhuque()
{
#if 1// v242 revivify
  
  int random_i;
 
  timeout = 0;
  III_TrunOFF_All_LED();
  Clean_InputDataM();	

  // II_Play_S6_start_QuanPuwords();


   
 #if 0 // remove this region 
  random_i = timeout % 7 + 1;
    random_i = timeout % 7 + 1;
    if (gCheck_Section_Select(random_i))
      III_Switch_LED(true, random_i);
    else
      III_TrunOFF_All_LED();
#else
	III_TrunON_All_LED();

#endif
	SetRF_ModeM(READING_MODE);

	while ((rBuffer[1]==0) && timeout < 1200)
	{
	  timeout++;
	  delay(10);
	}
	
    //byte data[Mirf.payload];
    if(rBuffer[1]!=0)
    {     
      int i;
      String Temp;
      for (i = 0; i < 2; i++)
      {
        Temp += char(rBuffer[i]);
      }
#ifdef ONLINE_DEBUG
	  Serial.begin(BAUD_RATE);

      Serial.println(Temp);
#endif

      if ((rBuffer[0] == '$') )
      {
#ifdef ONLINE_DEBUG
        Serial.println(">Beat Bird!");
		Serial.end();

#endif
        II_Play_beat1();
        III_TrunOFF_All_LED();

		bt_upload_next_withoutack();//v242

        return true;

      }


    }

  else
#endif
  return false;
}

bool Traning_again()
{
#if 1// v242 revivify 

  int  random_i;
  byte Temp_SS = 0;
 // byte data[Mirf.payload];
  Clean_InputDataM();	

  timeout = 0;
   comdata = "";
  random_i = 0;
  
  Temp_SS = get_who_is_online();//v1.8
    if (SectionSelect != Temp_SS)
    {
      bt_upload_LowPower();
      delay(2000);
      goto ReselectMode;
    }
	else
	{
#ifdef ONLINE_DEBUG
		Serial.begin(BAUD_RATE);

		Serial.print("##");
		Serial.println(SectionSelect);
		  Serial.end();
#endif
	}
#ifdef STEP2STEP_DEBUG
	  Serial.begin(BAUD_RATE);
	  Serial.println("Traning_again?");
  Serial.end();

#endif

  II_Play_S25_again();


 #if 0 // remove this region 
    random_i = timeout % 7 + 1;
    if (gCheck_Section_Select(random_i))
      III_Switch_LED(true, random_i);
    else
      III_TrunOFF_All_LED();
#else
	LED_blink();
	 

	III_TrunON_All_LED();
	 
#endif
  
//	w_Send_oneSignal(BT,99);
	SetRF_ModeM(READING_MODE);

	while ((rBuffer[1]==0) && timeout < TRY_AGAIN_TIMEO)
	{
	  timeout++;
	  
//		w_Send_oneSignal(BT,99);
//	if(timeout%3000==0)
//	  bt_upload_next_withoutack();//v192
	  while (Serial.available() > 0)
		 {
		   comdata += char(Serial.read());
		   delay(2);
	  
		 }
		 if (comdata.length() > 0)
		  {
			if ((comdata[0] == '@') && (comdata[1] == 'M'))
			{
	  ReselectMode:
			  comdata="";
//			  SectionSelect = get_who_is_online();//v1.8
			  SectionSelect=Temp_SS;

			  II_Play_S24_Select_mode();
			  III_TrunOFF_All_LED();
			  while (!bt_Select_Mode())
			  {
				//SectionSelect=btdata;
				II_Play_S24_Select_mode();
	  
	  
			  }
			  work_state = STATE_PRE_TRAINNING_S5; // break the while
			 check_targetOnline_withVoice();// gSection_Select_outspaker();
			  return Traning_again_zhuque();
			}
				else if((comdata[0] == '@') && (comdata[1] == 'R'))//v198
			{
				comdata="";
				system_reset();
				return false;
	  
			}
			else
			{
			  comdata = "";
			}
	  
		  }


	  delay(2);
	}
    if (rBuffer[1]!=0)
    {
      int i;
      String Temp;
      for (i = 0; i < 2; i++)
      {
        Temp += char(rBuffer[i]);
      }
#ifdef ONLINE_DEBUG
	  Serial.begin(BAUD_RATE);
      Serial.println(Temp);
	  Serial.end();
#endif

      if ((rBuffer[0] == '$') )
      {
#ifdef ONLINE_DEBUG
		Serial.begin(BAUD_RATE);
        Serial.println(">Beat Again!");
		Serial.end();
#endif
        II_Play_beat1();
        III_TrunOFF_All_LED();
		
		bt_upload_next_withoutack();//v242

        return true;

      }


    }

 
	delay(1);


  
#endif
  return false;
}


////////////%%%%%%%%%%%%%%%%%%%%%%  Originnal Funcion part
void setup() {
  int silince_tot;

  gpio_init();
//   attachInterrupt( digitalPinToInterrupt(2), D2onChange, FALLING);  
  randomSeed(analogRead(A7));

  
   ss.begin(BAUD_RATE);
 
  Serial.begin(BAUD_RATE);
  get_rf_frequncy();

  III_Rf_Init(0);
  
#ifdef STEP2STEP_DEBUG
  Serial.println("S2Step Open");
#ifndef DEBUG
  Serial.print("--------Ver");
  Serial.println(VERION);
#endif

#endif



#ifdef DEBUG
  Serial.println("KongFu DebugMode");
  Serial.print("--------Ver");
  Serial.println(VERION);

#endif



  III_AMP(1);
  III_BT(1);

  set_volume();



  //////Parameter initilzation
  SectionSelect = 0x00;
  work_state = STATE_UNPARING_S1; // NO.1 STATE
  GameMode = GAME_MODE_ERROR;
  /////////////
#ifndef DEBUG  // WORKING MODE
  II_Play_S1_start();
  silince_tot = 0;
  bt_upload_begin_withoutack();
#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	  Serial.println("input @P#123456#");
Serial.end();
#endif

  while (!bt_pairing() && work_state == STATE_UNPARING_S1)
  {
    if (silince_tot < 30000)
      silince_tot++;

    delay(1000);

    if (silince_tot < 2)
      II_Play_S1_start();


  }

  LED_blink();
  II_Play_S2_BT_connectOK();

  work_state = STATE_UNSELECTMODE_S24;




#endif


	Serial.end();


                                                                       
}
void _test()
{

//Serial.begin(BAUD_RATE);
//Serial.println("stS");
//=========test who is online
//w_Send_oneSignal(CK, 1);// Direct bit target
//delay(3000);
//Serial.println("2");

//w_Send_oneSignal(CK, 2);// Direct bit target
//delay(3000);
//Serial.println("3");

//w_Send_oneSignal(CK, 3);// Direct bit target
//delay(3000);
//Serial.println("4");

//w_Send_oneSignal(CK, 4);// Direct bit target
//delay(3000);
//Serial.println("5");   
//w_Send_oneSignal(CK, 5);// Direct bit target
// delay(3000);
//Serial.println("6");   
//w_Send_oneSignal(CK, 6);// Direct bit target
//delay(3000);

   
//Serial.end();
 // get_who_is_online();

//  String backdata = "";	

//	   SectionSelect = get_who_is_online();
//	 backdata.concat("#");
//		  backdata.concat(SectionSelect);
//	Serial.begin(BAUD_RATE);
//		  Serial.println(backdata);
//	Serial.end();

//test_all_target();

//total_record=0;

//LED_blink();
//delay(1000);
 beat_bird_toSTART();

delay(1000);

//run_Script_test();
//delay(3000);
//Serial.print("Record:");
//Serial.println(total_record);

//if(beat_bird_toSTART())
//Serial.println("s");

//else
//Serial.println("f");

//Serial.println("==LED TEST");
//v2_GreenLED(true);
//delay(3000);
//v2_GreenLED(false);
//delay(3000);

//Serial.println("==SW TEST");
//v2_Get_Channel_Switch_debug();
//delay(2000);

//Serial.println("==SoundCard TEST");
//III_AMP(true);

//II_PlayWave(snd_1,200);
//II_PlayWave(snd_2,200);
//III_AMP(false);

//II_PlayWave(snd_3,2000 + ANT_POPC);
//III_AMP(false);

//II_PlayWave(snd_3,2000 + ANT_POPC);


//delay(2000);


//=============test beats
//run_Script_test();
//delay(5000);

///===========test bluetooth
//III_BT(1);

//    while (Serial.available() > 0)
//    {
//      comdata += char(Serial.read());
//      delay(2);

//    }
//    if (comdata.length() > 0)
//    {
//    	Serial.println("got data! ");
//		Serial.println(comdata);
//		comdata="";
//    }

//========test channel select
//	III_Get_Channel();
//	DEBUG_sndtest();

//int _ii;
//		 Serial.println("==Next test item ADC state==");
//			for(_ii=0;_ii<50;_ii++)
//			{
//			if(III_Get_Battery_State())
//				Serial.println("ok");
//			else
//				Serial.println("low power");
//			delay(100);
//			}


}
void tt()//only for moniter programe
{
		 Serial.begin(BAUD_RATE);

		 if(role == role_sender)
 		SetRF_ModeM(READING_MODE);

	if (rCount>0)
	   {
		 int i;
			 String Temp;
		  for (i = 0; i < 2; i++)
	      {
	        Temp += char(rBuffer[i]);
	      }

		 
	      Serial.println(Temp);
	
		  
	 Clean_InputDataM();
	 }

}
void loop() {

#ifdef DEBUG   ///>>>>DEBUG MODE
_test();

//  _test();
  // delay(5000);

#else   ///>>>>WORKING MODE
  int bbts_timeout = 0;
  ;
  if (work_state == STATE_UNSELECTMODE_S24)
  {
    int pptimeout = 0;

    II_Play_S24_Select_mode();
    SectionSelect = get_who_is_online();
    if (!(SectionSelect & 0x0f))
    {
      II_Play_SX_Error_TargetPower();
#ifdef  ONLINE_DEBUG
Serial.begin(BAUD_RATE);
	  Serial.print("SectionSelect=");
      Serial.println(SectionSelect);
      Serial.println("CantOpenTargetPower");
Serial.end();
#endif
      delay(3000);
      system_reset();

    }
#ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	Serial.println("input ACK#OK#");
Serial.end();
#endif
    while (!bt_Select_Mode())
    {
      SectionSelect = get_who_is_online();
      if (pptimeout < 3)
        II_Play_S24_Select_mode();
      else
        system_reset();

      pptimeout++;
    }
    //Serial.print("SectionSelect=");
    //Serial.println(SectionSelect);//debug

    work_state = STATE_PRE_TRAINNING_S5; // break the while
//    gSection_Select_outspaker();
    check_targetOnline_withVoice();


  }

  if ((work_state == STATE_PRE_TRAINNING_S5) && (GameMode != GAME_MODE_ERROR))
  {
    bbts_timeout = 0;
	II_Play_S5_welcome();//v260
	II_Play_S7_startword();//v260
    while ((!beat_bird_toSTART()) && (bbts_timeout < ONEBIRD_START_TIMEOUT))
    {
      bbts_timeout++;
      delay(5000);// with 0.5 minter
    }
    if (bbts_timeout >= ONEBIRD_START_TIMEOUT)
    {
    #ifdef STEP2STEP_DEBUG
Serial.begin(BAUD_RATE);
	Serial.println("time out ONEBIRD_START_TIMEOUT");
Serial.end();
#endif
      system_reset();
    }
    else
    {

//      II_Play_S5_welcome();
//      delay(800);  // V260

      bt_uploadstate_forLoop();


      begin_traning();
      bbts_timeout = 0;
      while ((work_state == STATE_IDEA) && (bbts_timeout < TRY_AGAIN_TIMEOUT))
      {

        if (Traning_again())
        {

//          II_Play_S6_start_QuanPuwords();// v260
         // delay(1000);

          II_Play_S5_welcome();
		  II_Play_S7_startword();//v260
          delay(800);
          bt_uploadstate_forLoop();
          begin_traning();

        }
        bbts_timeout++;
        delay(100);


      }

      system_reset();


    }






  }
  else
  {
    //#error can't enter trainning mode ,target link error!!
Serial.begin(BAUD_RATE);
    Serial.print("GAME_MODE_ERROR");
Serial.end();
    system_reset();

  }


  //run_Script




#endif
}





