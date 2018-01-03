//0.6 Debug mode ,base onKongFu 0.0.41
//0.7
///1029 Prerelease version
//1.0 Nov-04 new rule ,and fixed button fake press,and reindex all postionof keys
//1.5 Follow btverison v1.5
//1.6 remove link check and add silence mode
//1.7 only for new PCB v3
//1.8 ADD RESET CMD for PCB V3
//1.9 add #START#//  2018-Jan-02
//1.92 follow wild wind protocol 13
#include <SoftwareSerial.h>
#define VERION    192
//#define DEBUG 11
//#define ONLINE_DEBUG 12

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

#define VOTLAGE_VALUE  292 //351
#define ADC_GATE_VALUE 300

#define R0 3
#define R1 4
#define R2 5
#define C0 11
#define C1 12
#define C2 7
#define AMP 14 //A0
#define BTP 15 //A1


#define ANT_POPA 300
#define ANT_POPB 500
#define ANT_POPC 1000
#define DEBONDSTIME 20

#ifdef ONLINE_DEBUG
#define PAIRING_TIMEOUT 30000
#define CONNECT_CHECK_TIMEOUT 500
#define TRY_AGAIN_TIMEOUT 5
#define TRY_AGAIN_TIMEO 36000
#define RUN_SCRIPT_TIME 10
#define ONEBIRD_START_TIMEOUT 1
#else
#define PAIRING_TIMEOUT 30000
#define CONNECT_CHECK_TIMEOUT 500
#define TRY_AGAIN_TIMEOUT 15
#define TRY_AGAIN_TIMEO 36000
#define RUN_SCRIPT_TIME 90
#define ONEBIRD_START_TIMEOUT 10

#endif

#include "LedControl.h"

//#include <Keypad.h>

SoftwareSerial ss(13, 6);
//byte rowPins[3] = {3, 4, 5}; //connect to the row pinouts of the keypad
//byte colPins[3] = {11, 12, 7}; //connect to the column pinouts of the keypad
char hexaKeys[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}}; // the key value  NEED MODFIY BY SETUP
LedControl lc = LedControl(8, 9, 10, 1); // pin 8,9,10

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





int timeout;
int total_record;
int total_take_time;
int work_state;
String comdata = "";
String ID = "";
bool gSilence_Mode;


void(* resetFunc) (void) = 0;

void system_reset()
{
  //tbd
  II_Play_s22_BT_disconnect();
  bt_Throwout_Error_withoutack();//v1.8
  III_BT(0);
  delay(10000);
   III_BT(1);
  resetFunc();
  III_BT(1);


}
void III_AMP(bool sw)
{
  digitalWrite(AMP, sw);
}
void III_BT(bool sw)
{
  digitalWrite(BTP, !sw);
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
  //  II_PlayWave(snd_24,3138+500);
  II_PlayWave(snd_24, 3138 + ANT_POPC);

}
void II_Play_end()
{
  //II_PlayWave(, );
}
void II_Play_beat1()
{
  II_PlayWave(snd_1, 280);
}
void II_Play_beat2()
{
  II_PlayWave(snd_2, 200);
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
  II_PlayWave(snd_3, 12000 + ANT_POPC);
}

void II_Play_S7_startword()
{
  II_PlayWave(snd_11, 4000 + ANT_POPC);
}
void II_Play_lowpower()
{
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
void II_Play_S14_gameover()
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
  II_PlayWave(snd_10, 2297 + ANT_POPC);
}
void II_Play_S2_BT_connectOK()
{
  II_PlayWave(snd_12, 2000 + ANT_POPC);
}

void II_Play_s22_BT_disconnect()
{
  II_PlayWave(snd_13, 1962 + ANT_POPC);
}
void II_Play_S23_Closing()
{
  II_PlayWave(snd_15, 3281 + ANT_POPC);
}
void II_Play_S24_Select_mode()
{
  
  bt_upload_select_withoutack();

  II_PlayWave(snd_17, 2000 + ANT_POPC);
}
void II_Play_S6_start_zhuque()
{
   int result;
 
  result = random(1, 5);// 9 targets version
  switch(result)
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



////////############# Test Function Part
void LED_blink()
{
  lc.setRow(0, 2, 0xff); // d2 all
  delay(200);
  lc.setRow(0, 2, 0x00);
  delay(200);

  lc.setRow(0, 1, 0xff); // d6 all
  delay(200);
  lc.setRow(0, 1, 0x00);
  delay(200);

  lc.setRow(0, 0, 0xff); // d3 all
  delay(200);
  lc.setRow(0, 0, 0x00);
  delay(200);

}
void run_Script_test()
{
  work_state = STATE_TRAINNING;

  int i;
  int act_d;
  int delayt;

  set_volume();
  for (i = 1; i < 7; i++)
  {
    oneBeat(i);
    // delay(500);
  }
  //delay(1000);
  // set_playover();
  //  delay(100);
  // set_volume_silence();

}

void plug_test_getvalue()
{
  SectionSelect = get_who_is_online();
  Serial.println(SectionSelect);
  delay(3000);

}
void Plug_test()
{
  // byte state = (byte)get_who_is_online();
  // if command over state than report error to app and set state as real state.
  int a_result;
  Serial.print("A7:");
  a_result = analogRead(A7);
  if (a_result > ADC_GATE_VALUE)
  {
    II_Play_foot_L();
  }

  delay(500);
  Serial.println(a_result);
  Serial.print("A2:");
  a_result = analogRead(A2);
  if (a_result > ADC_GATE_VALUE)
  {
    II_Play_backward();
  }
  delay(500);
  Serial.println(a_result);
  Serial.print("A3:");
  a_result = analogRead(A3);
  if (a_result > ADC_GATE_VALUE)
  {
    II_Play_leftward();
  }
  delay(500);
  Serial.println(a_result);
  Serial.print("A4:");
  a_result = analogRead(A4);
  if (a_result > ADC_GATE_VALUE)
  {
    II_Play_foot_R();
  }
  delay(500);
  Serial.println(a_result);
  Serial.print("A5:");
  a_result = analogRead(A5);
  if (a_result > ADC_GATE_VALUE)
  {
    II_Play_rightward();
  }
  delay(500);
  Serial.println(a_result);
  Serial.print("A6:");
  a_result = analogRead(A6);
  if (a_result > ADC_GATE_VALUE)
  {
    II_Play_forward();

  }
  delay(500);
  Serial.println(a_result);


  //




}
void LED_All_ON_first()
{
  // lc.setRow(0, 3, 0xff); // d3 all
  // lc.setRow(0, 6, 0xff); // d6 all //k
  lc.setRow(0, 2, 0xff); // d2 all//k

  // lc.setRow(0, 4, 0xff); // d3 all
  // lc.setRow(0, 7, 0xff); // d6 all
  lc.setRow(0, 1, 0xff); // d2 all
  // lc.setRow(0, 5, 0xff); // d2 all
  lc.setRow(0, 0, 0xff); // d2 all

  delay(delaytimes);
  lc.setRow(0, 2, 0x00);
  lc.setRow(0, 3, 0x00);
  lc.setRow(0, 6, 0x00);

  lc.setRow(0, 4, 0x00); // d3 all
  lc.setRow(0, 7, 0x00); // d6 all
  lc.setRow(0, 1, 0x00); // d2 all
  lc.setRow(0, 5, 0x00); // d2 all
  lc.setRow(0, 0, 0x00); // d2 all
  delay(delaytimes);

}
void LED_AllONv16()
{
  lc.setRow(0, 1, 0xff); // d2 all
  lc.setRow(0, 0, 0xff); // d2 all
  lc.setRow(0, 2, 0xff); // d2 all

}
void LED_AllOFFv16()
{
  lc.setRow(0, 1, 0x00); // d2 all
  lc.setRow(0, 0, 0x00); // d2 all
  lc.setRow(0, 2, 0x00); // d2 all

}

void LED_Matrix_test() {
  /* here is the data for the characters */
  lc.setRow(0, 2, 0xff); // d2 all
  delay(delaytimes);
  lc.setRow(0, 2, 0x00);
  delay(delaytimes);

  lc.setRow(0, 2, 0x20); //1 d2-sa
  delay(delaytimes);
  lc.setRow(0, 2, 0x00);
  delay(delaytimes);


  lc.setRow(0, 2, 0x10); //3 d2-sb
  delay(delaytimes);
  lc.setRow(0, 2, 0x00);
  delay(delaytimes);




  lc.setRow(0, 1, 0xff); // d6 all
  delay(delaytimes);
  lc.setRow(0, 1, 0x00);
  delay(delaytimes);

  lc.setRow(0, 1, 0x20); //4 d6-sa
  delay(delaytimes);
  lc.setRow(0, 1, 0x00);
  delay(delaytimes);


  lc.setRow(0, 1, 0x10); //5 d6-sf
  delay(delaytimes);
  lc.setRow(0, 1, 0x00);
  delay(delaytimes);


  lc.setRow(0, 0, 0xff); // d3 all
  delay(delaytimes);
  lc.setRow(0, 0, 0x00);
  delay(delaytimes);

  lc.setRow(0, 0, 0x20); //7 d3-sa
  delay(delaytimes);
  lc.setRow(0, 0, 0x00);
  delay(delaytimes);


  lc.setRow(0, 0, 0x10); //8 d3-sf
  delay(delaytimes);
  lc.setRow(0, 0, 0x00);
  delay(delaytimes);


}
void key_test()
{

  int key;

  delay(100);



  key = get_key();


  if (key)
  {

    Serial.println(key);

  }

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
  II_Play_S14_gameover();
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

  int key;
  key = 0;
  timeout = 0;
  Open_LED(0);
  II_Play_S6_start_zhuque();




  while ((key == 0) && timeout < 1200)
  {
    timeout++;
    if (gCheck_Section_Select(timeout % 7 + 1))
      Open_LED(timeout % 7 + 1);
    else
      Open_LED(0);
   if(i%600==0)
	  bt_upload_state_withoutack();//v192
    key = key_scan();
#ifdef ONLINE_DEBUG
    if (key != 0)
      Serial.println(key);
#endif
    if (key != 0)
    {
      II_Play_beat1();
      Open_LED(0);
      return true;


    }
  }

  return false;


}
int key_scan() // case "n" n is sw . return value is led
{
  int rn;
  int result;
  int ckey ;
  ckey = get_key();//customKeypad.getKey();
  //Serial.println(ckey);

  switch (ckey)
  {
    case 5:
      result = 1;
      break;
    case 6:
      result = 2;
      break;
    case 2:
      result = 3;
      break;
    case 3:
      result = 4;
      break;
    case 8:
      result = 5;
      break;
    case 9:
      result = 6;
      break;
    default:
      result = 0;
  }
  // if(result!=0)
  // Serial.println(result);
  return result;
}
void Open_LED(int nb)//1~9 open 10 closed
{

  switch (nb)
  {
    case 1:
      lc.setRow(0, 0, 0x10); //1
      break;
    case 2:
      lc.setRow(0, 0, 0x20); //2
      break;
    case 3:
      lc.setRow(0, 1, 0x10); //3
      break;
    case 4:
      lc.setRow(0, 1, 0x20); //4
      break;
    case 5:
      lc.setRow(0, 2, 0x10); //5
      break;
    case 6:
      lc.setRow(0, 2, 0x20); //6
      break;

    default :
      lc.setRow(0, 0, 0x00);
      lc.setRow(0, 1, 0x00);
      lc.setRow(0, 2, 0x00);
      break;

  }

  delay(20);

}

////////////////////$$$$$$$$$$$$$$$ Base Function part over
/////////////////////^^^^^^^^^^^^ APP Function Parts
bool combintest(int i)// 1~6
{
  int rt;
  rt = 0;
  if (i < 10)
  {
    Open_LED(i);

    while (rt < 100)
    {
      if (key_scan() == i)
      {
        Open_LED(0);

        return true;

      }
      delay(10);
      rt++;
    }
    Open_LED(0);
    return false;
  }

}
void Mix_test()
{
  for (int i = 1; i < 7; i++)
  {
    combintest(i);
    delay(2000);

  }


}
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
  Serial.print("ss=");
  Serial.println(SectionSelect);
#endif
  if (!(SectionSelect & 0x0f))
    GameMode = GAME_MODE_ERROR;
  else
  {
    if ((SectionSelect & 0x08) || (SectionSelect & 0xf0))
      II_Play_S4_roundmode();
    else
      II_Play_S3_singlemode();
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
bool  oneBeat(int key_in)
{
  int direct;
  int rt;
  int vkey;
  rt = 0;
  vkey = 0;
  bool sndr;
  // delay(20);
  sndr = false;
  play_ss_ForTarget(key_in);

  if (key_in < 7)
  {
    Open_LED(0);
    Open_LED(key_in);
    delay(20);
    Open_LED(0);
    delay(10);
    Open_LED(key_in);
    while (rt < 100)
    {
      vkey = key_scan();
      if (vkey == key_in)
      {
        //set_volume();
        Open_LED(10);
        if (rt % 2 == 0)
          II_Play_beat1();
        else
          II_Play_beat2();

        //delay(300);
        total_record += 1;//25
        total_take_time += rt;
        //delay(1000);
        //set_playover();
        //delay(100);
        //set_volume_silence();
        return true;

      }
      else if ((vkey != key_in) && (vkey > 0))
      {
        if (!sndr)
        {
          sndr = true;
          // set_volume();
          if (rt % 2 == 0)
            II_Play_beat1();
          else
            II_Play_beat2();
          //delay(300);
          //Open_LED(10);
          //   set_playover();
          // delay(100);
          //set_volume_silence();
        }


      }

      delay(10);
      rt++;
    }
    Open_LED(0);

    //  delay(1000);
    // set_playover();
    //delay(100);
    //set_volume_silence();

    total_take_time += rt;
    return false;
  }

}
void bt_sendData(int data) //bt send part TBD
{
  work_state = STATE_BLUETOOTH;
  II_Play_btsend();

  Serial.print(data);
  work_state = STATE_IDEA;

}

void bt_receivedata()
{

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

}
void check_targetOnline_withVoice()
{
  byte state = (byte)get_who_is_online();
  if (SectionSelect == state)
    return;


  // if command over state than report error to app and set state as real state.
  if (!check_state_error(SectionSelect, state))
  {
    bt_sendError();
    SectionSelect = state; // Pluged < Selected
  }
  else
  {
    SectionSelect = SectionSelect; // Pluged > Selected
    delay(2000);
    gSection_Select_outspaker();
    // Serial.println(comdata);

  }

}
void bt_sendError()
{
  work_state = STATE_BLUETOOTH;
  Serial.print(0xff);
  byte state = (byte)get_who_is_online();

  // II_Play_btsend();

  Serial.print(state);
  work_state = STATE_IDEA;

}
void bt_sendState()
{
  work_state = STATE_BLUETOOTH;

  byte state = (byte)get_who_is_online();

  // II_Play_btsend();

  Serial.print(state);
  work_state = STATE_IDEA;


}
void check_battery_voltage()
{
  //  int valb;
  //  valb = analogRead(A0);
  //  if (valb <= 292)
  //Serial.println(valb);
  //   II_Play_lowpower();
  // delay(3000);
}
bool check_state_error(byte app, byte host) // true is right ,false is error
{
  bool result;
  result = false;

  if ((app & 0x04 == 1) && (host & 0x04 == 0))
  {
    return result;
  }
  if ((app & 0x02 == 1) && (host & 0x02 == 0))
  {
    return result;
  }
  if ((app & 0x08 == 1) && (host & 0x08 == 0))
  {
    return result;
  }
  if ((app & 0x10 == 1) && (host & 0x10 == 0))
  {
    return result;
  }
  if ((app & 0x20 == 1) && (host & 0x20 == 0))
  {
    return result;
  }
  if ((app & 0x40 == 1) && (host & 0x40 == 0))
  {
    return result;
  }

  result = true;

  return result;


}
int get_who_is_online()//
{
  int result = 0;
  if  (analogRead(A6) > ADC_GATE_VALUE) //front
  {
    result = 0x01;
  }
  delay(10);
  if  (analogRead(A7) > ADC_GATE_VALUE) //left foot
  {
    result |= 0x02;
  }
  delay(10);
  if  (analogRead(A4) > ADC_GATE_VALUE) //right foot
  {
    result |= 0x04;
  }
  if  (analogRead(A3) > ADC_GATE_VALUE) //left
  {
    result |= 0x08;
  }
  if  (analogRead(A5) > ADC_GATE_VALUE) //right
  {
    result |= 0x10;
  }

  if  (analogRead(A2) > ADC_GATE_VALUE) //behind
  {
    result |= 0x20;
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
  if(i%3==0)
    bt_upload_state_withoutack();

    act_d = get_random_number();

    oneBeat(act_d);


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
  work_state = STATE_IDEA;
#endif
}
/////////^^^^^^^^^^^^APP Function Part Over
int get_key()
{
  int result = 0;
  digitalWrite(C0, LOW);
  digitalWrite(C1, HIGH);
  digitalWrite(C2, HIGH);
  delay(2);
  if (digitalRead(R0) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R0) == LOW)
      result = 1;
  }

  else if (digitalRead(R1) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R1) == LOW)
      result = 2;
  }
  else if (digitalRead(R2) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R2) == LOW)
      result = 3;
  }

  digitalWrite(C0, HIGH);
  digitalWrite(C1, LOW);
  digitalWrite(C2, HIGH);
  delay(2);
  if (digitalRead(R0) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R0) == LOW)
      result = 4;
  }

  else if (digitalRead(R1) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R1) == LOW)
      result = 5;
  }
  else if (digitalRead(R2) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R2) == LOW)
      result = 6;
  }

  digitalWrite(C0, HIGH);
  digitalWrite(C1, HIGH);
  digitalWrite(C2, LOW);
  delay(2);
  if (digitalRead(R0) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R0) == LOW)
      result = 7;
  }

  else if (digitalRead(R1) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R1) == LOW)
      result = 8;
  }
  else if (digitalRead(R2) == LOW)
  {
    delay(DEBONDSTIME);
    if (digitalRead(R2) == LOW)
      result = 9;
  }

  return result;

}


void keyScan_init()
{

  pinMode(C0, OUTPUT);
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(R0, INPUT_PULLUP);
  pinMode(R1, INPUT_PULLUP);
  pinMode(R2, INPUT_PULLUP);

}
bool bt_pairing()// ture connected; false disconnect
{
  timeout = 0;
  comdata = "";
  while (timeout < PAIRING_TIMEOUT)
  {
    delay(2);
    timeout++;
    if (!(timeout % 5 == 0))
      LED_AllONv16();
    else
      LED_AllOFFv16();
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

        backdata.concat(ID);
        backdata.concat("#");
        Serial.println(backdata);
        return true;
      }
      else
      {
        comdata = "";
      }
    }
  }
  return false;
}


bool bt_Select_Mode()// 0 means no respond.
{
SELECTBIGIN:
  String backdata = "";
  bool first_talk = 0;
  timeout = 0;
  comdata = "";
  while (timeout < PAIRING_TIMEOUT)
  {
    delay(2);
    timeout++;
    if (timeout < 2)
    {
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
        first_talk = 1;
        break;
      }
      else if ((comdata[0] == '@') && (comdata[1] == 'P'))//V192
      {
		ID = comdata.substring(3, 9);
		goto SELECTBIGIN;	   
      }
      else
      {
        comdata = "";
      }

    }
  }
  //get the command for APP
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
          char ss = comdata[3];
          SectionSelect = (byte)ss;//ss-48;
          backdata = "";
          backdata.concat("ACK#OK#");
          Serial.println(backdata);
          return true;
        }
        else
        {
          comdata = "";
        }

      }
    }
  }
  return false;


}
bool bt_upload_data()// 0 means no respond.
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
      backdata.concat("@R#");
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

        return true;
      }

    }
  }
  return false;


}
void bt_upload_state_withoutack()// 0 means no respond.
{
  String backdata;
    backdata = "";
      backdata.concat("#ACKTION#");
      Serial.println(backdata);
 


}
void bt_upload_begin_withoutack()// 0 means no respond.
{
  String backdata;
    backdata = "";
      backdata.concat("#START#");
      Serial.println(backdata);



}
void bt_upload_next_withoutack()// 0 means no respond.
{
  String backdata;
    backdata = "";
      backdata.concat("#NEXT#");
      Serial.println(backdata);
 


}
void bt_upload_select_withoutack()// 0 means no respond.
{
  String backdata;
    backdata = "";
      backdata.concat("#SELECTMODE#");
      Serial.println(backdata);
 


}


void bt_Throwout_Error_withoutack()// 0 means no respond.//v1.8
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
      backdata.concat("#RESET#");
      Serial.println(backdata);
    }

  }



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

        return true;
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
  II_Play_S7_startword();
  //work_state=STATE_TRAINNING;// v15

  run_Script(RUN_SCRIPT_TIME);


  work_state = STATE_IDEA;
  II_Play_S14_gameover();
  if (bt_upload_data())
  {
    II_Play_S15_BT_SendOver_dididi();

  };

  calc_result();


}
void test_bt_upload()
{
  total_record = 249;
  if (bt_upload_data())
  {
    Serial.println("uploaded");
    comdata = "";
    // delay(5000);
  }
  else
  {
    Serial.println("ul_disconnected");
  }
}
void test_bt_pairing()
{

  if (bt_pairing())
  {
    Serial.println("connected");
    comdata = "";
    // delay(5000);
  }
  else
  {
    Serial.println("disconnected");
  }
}
void test_bt_upload_state()
{
  if (bt_upload_state())
  {
    Serial.println("upload state ok");
  }
  else
    Serial.println("upload state fail.");

}
void test_bt_selectMode()
{
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


}

bool Bluetooth_test()
{
  //  test_bt_pairing();
  //test_bt_selectMode();
  test_bt_upload_state();
  //  test_bt_upload();
}
bool Training_again_zhuque()
{

  int key;
  key = 0;
  timeout = 0;

 // II_Play_S6_start_zhuque();

  while ((key == 0) && timeout < 1200)
  {
    timeout++;
    if (gCheck_Section_Select(timeout % 7 + 1))
      Open_LED(timeout % 7 + 1);
    else
      Open_LED(0);
    key = key_scan();

    if (key != 0)
    {
      II_Play_beat1();
      Open_LED(0);
      return true;
    }


  }

  return false;
}

bool Training_again()
{

  int key;
  key = 0;
  timeout = 0;
  comdata = "";
  byte Temp_SS;
  Temp_SS = get_who_is_online();//v1.8
   if (SectionSelect != Temp_SS)
   {
     
     delay(2000);
     goto ReselectMode;
   }

  II_Play_S25_again();

  while ((key == 0) && timeout < TRY_AGAIN_TIMEO)
  {
    timeout++;
	
	if(i%3000==0)
	  bt_upload_next_withoutack();//v192
	
    if (gCheck_Section_Select(timeout % 7 + 1))
      Open_LED(timeout % 7 + 1);
    else
      Open_LED(0);
    key = key_scan();

    if (key != 0)
    {
      II_Play_beat1();
      Open_LED(0);
      return true;
    }
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
       SectionSelect = get_who_is_online();//v1.8
        II_Play_S24_Select_mode();
        Open_LED(0);
        while (!bt_Select_Mode())
        {
          //SectionSelect=btdata;
          II_Play_S24_Select_mode();


        }
        work_state = STATE_PRE_TRAINNING_S5; // break the while
        gSection_Select_outspaker();
        return Training_again_zhuque();
      }
      else
      {
        comdata = "";
      }

    }


  }

  return false;
}


////////////%%%%%%%%%%%%%%%%%%%%%%  Originnal Funcion part
void setup() {
  int silince_tot;
  ss.begin(9600);
  Serial.begin(9600);


#ifdef DEBUG
  Serial.println("KongFu Debug");
  Serial.print("--------ver");
  Serial.println(VERION);
  //Serial.println("LED ALL ON");
  //Serial.println("BLUETOOTH DEBUG");
  // Serial.println("SOUND ALL PLAY");
  // Serial.println("PLUG IN TEST");
  //Serial.println("LED MATRIX TEST");
  //Serial.println("key board TEST");
  Serial.println("first sndtest TEST");
#endif

  pinMode(AMP, OUTPUT);
  pinMode(BTP, OUTPUT);
  III_AMP(1);
  III_BT(1);
  keyScan_init();

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  set_volume();
  randomSeed(analogRead(A7));

  //////Parameter initilzation
  SectionSelect = 0x00;
  work_state = STATE_UNPARING_S1; // NO.1 STATE
  GameMode = GAME_MODE_ERROR;
  /////////////
#ifndef DEBUG  // WORKING
  II_Play_S1_start();
  silince_tot = 0;
  bt_upload_begin_withoutack();

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





}

void loop() {

#ifdef DEBUG   ///>>>>DEBUG MODE

  //LED_All_ON_first();
  // Bluetooth_test();
  DEBUG_sndtest();
  // II_Play_levelE();
   //Plug_test();
  //plug_test_getvalue();
  //LED_Matrix_test();
  //  run_Script_test();
  //SectionSelect=0x01;

  //run_Script(20);
  //
  //delay(5000);
  // key_test();
  // delay(500);
  //first_sndtest_debug();
  //Mix_test();
  // delay(5000);

#else   ///>>>>WORKING MODE
  int bbts_timeout = 0;

  if (work_state == STATE_UNSELECTMODE_S24)
  {
    int pptimeout = 0;

    II_Play_S24_Select_mode();
    SectionSelect = get_who_is_online();
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
    gSection_Select_outspaker();

  }

  if ((work_state == STATE_PRE_TRAINNING_S5) && (GameMode != GAME_MODE_ERROR))
  {
    bbts_timeout = 0;
    while ((!beat_bird_toSTART()) && (bbts_timeout < ONEBIRD_START_TIMEOUT))
    {
      bbts_timeout++;
      delay(5000);// with 0.5 minter
    }
    if (bbts_timeout >= ONEBIRD_START_TIMEOUT)
    {
      system_reset();
    }
    else
    {

      II_Play_S5_welcome();
      delay(800);

      bt_uploadstate_forLoop();


      begin_traning();
      bbts_timeout = 0;
      while ((work_state == STATE_IDEA) && (bbts_timeout < TRY_AGAIN_TIMEOUT))
      {

        if (Training_again())
        {
          II_Play_S6_start_zhuque();
          delay(1000);
      
      II_Play_S5_welcome();
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
    Serial.print("GAME_MODE_ERROR");
    system_reset();

  }


  //run_Script




#endif
}






