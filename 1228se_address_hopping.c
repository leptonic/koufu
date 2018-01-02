//kongfu Target Firmware
//1.7 frequency hopping version
#define VERISON 185
#define CONFIGRATION 1
#define _DEBUG_LOWPOWER 1  //1 means true; 2 means false ; should remove this define without debuging
#define _DEBUG_SENDER 1

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



#if CONFIGRATION == 1
#define CHECK_BIT ZQ_T
#define KEY_BIT ZQ_O
#elif CONFIGRATION == 2
#define CHECK_BIT ZT_T
#define KEY_BIT ZT_O
#elif CONFIGRATION == 3
#define CHECK_BIT YT_T
#define KEY_BIT YT_O
#elif CONFIGRATION == 4
#define CHECK_BIT QL_T
#define KEY_BIT QL_O
#elif CONFIGRATION == 5
#define CHECK_BIT BH_T
#define KEY_BIT BH_O
#elif CONFIGRATION == 6
#define CHECK_BIT XW_T
#define KEY_BIT XW_O
#endif

#define VOTLAGE_VALUE  292
#define BASE_FREQUNCY   90
#define SPEED_DATA_RATES 0x0e  // 0x06 7 1M 0x0f 2M 

#include <avr/wdt.h>
#define TIMEOUT WDTO_8S 

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
void(* resetFunc) (void) = 0;
/*SONboard Base*/
int value;
int timeout;
byte data[2];


void RF_24L01_Init()
{


  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"LAVAJ"); //设置自己的地址（发送端地址），使用5个字符
  Mirf.payload = sizeof(data);
//  Mirf.channel = 90-CONFIGRATION*15;          //设置所用信道
  Mirf.channel = BASE_FREQUNCY;      //设置所用信道

  Mirf.config();
  Mirf.configRegister(RF_SETUP,SPEED_DATA_RATES);

}
void RF_24L01_ah_Init()//channel must from 1-6 !!
{

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  switch((int) CONFIGRATION)
  {
  case 1:   
    Mirf.setRADDR((byte *)"LAVAJ1");
    break;
  
  case 2:   
    Mirf.setRADDR((byte *)"LAVAJ2");
    break;
  
  case 3:   
    Mirf.setRADDR((byte *)"LAVAJ3");
    break;
  
  case 4:   
    Mirf.setRADDR((byte *)"LAVAJ4");
    break;
  
  case 5:   
    Mirf.setRADDR((byte *)"LAVAJ5");
    break;
  
  default:    
    Mirf.setRADDR((byte *)"LAVAJ6");
    break;
              

  }
//  Mirf.setRADDR((byte *)"LAVAJ"); 
  Mirf.payload = sizeof(data);
  Mirf.channel = BASE_FREQUNCY;          //设置所用信道
  Mirf.config();
  Mirf.configRegister(RF_SETUP,SPEED_DATA_RATES);

}
void III_Rf_Init()
{
  RF_24L01_Init();

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Sender "); 
  Serial.print(CONFIGRATION); 
  Serial.print(" ver "); 
  Serial.println(VERISON); 
  III_Rf_Init();
  pinMode(3, INPUT);  //TBD
  pinMode(5, OUTPUT); //TBD
 
 // wdt_enable(TIMEOUT);
  // attachInterrupt(0, Receive, FALLING);
}

void rf_Send(char *str)
{
  int lens;
  lens = strlen(str);

  char msg[lens];
  int i;
  for (i = 0; i < lens; i++)
  {
    msg[i] = int(str[i]);
  }
  Mirf.send((byte *)&msg);

  while (Mirf.isSending()) {}
}
void rf_Send_key()
{
  int i;
  char sd[2];
  sd[0] = '$';
  sd[1] = KEY_BIT;
  // Serial.println("get");
  //    for (i = 0; i < 1; i++)
//  {
//    delay(50);
//      timeout = 0;
//    while(Mirf.dataReady()&&timeout <20)
//      {
//      timeout++;
//         delay(2);
//      }
  III_Rf_Init();
    rf_Send(sd);
    delay(7);
  rf_Send(sd);
    delay(49);
  rf_Send(sd);
   
  //}

}
bool ADC_check() // false means low power
{
#ifdef _DEBUG_LOWPOWER
#if _DEBUG_LOWPOWER == 1
  return true;
#elif _DEBUG_LOWPOWER == 2
  return false;
#endif
#endif
  int valb;
  valb = analogRead(A7);
  delay(200);
  if (valb <= 292)
  {
    valb = analogRead(A7);
    if (valb <= 292)
    {
      return false;
    }
    else return true;

  }
  else return true;

}
void control_LED(bool sw)
{
  if (sw)
  {
    //Trun on
    digitalWrite(5, HIGH);
  }
  else
  {
    //Trun off
    digitalWrite(5, LOW);
  }

}
bool get_key()
{
  bool sre;
  if (digitalRead(3) == HIGH)
  {
    delay(150);
    if (digitalRead(3) == HIGH)
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
  control_LED(1);
  delay(3);
  timeout = 0;
  while (timeout < 199)
  {
    if (get_key())
    {
      control_LED(0);
      rf_Send_key();
    break;
    }
    timeout++;
    delay(5);
  }
  control_LED(0);

}

void loop()
{
  
  //wdt_reset();
 // III_Rf_Init();
  if (!Mirf.isSending() && Mirf.dataReady())
  {
  delay(3);
    Mirf.getData(data);

#if   1
    int i;  
    String Temp;
    for (i = 0; i <  Mirf.payload ; i++)
    {
      Temp += char(data[i]);
    }

#ifdef _DEBUG_SENDER
  //Serial.print("ss");

    Serial.println(Temp);
  
#endif

#endif


    if (data[0] == '%') //beat
    {

      if (data[1] == KEY_BIT)
      {
        run_training();
      }

    }
    else if (data[0] == '#') //Open led
    {

//      if (data[1] == KEY_BIT)
      {
        control_LED(1);
      }

    }
    else if (data[0] == '!') //Close led
    {

//      if (data[1] == KEY_BIT)
      {
        control_LED(0);
      }

    }
    
    else if ((char)data[0] == '+') //check online & power level
    {

      if ((char)data[1] == KEY_BIT)
      {
        char sd[2];
        control_LED(1);
        sd[0] = '$';
        if (!ADC_check())
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
      
         
          control_LED(0);

      }
    }// +++
    
  }

  else //for beat birds
  {
    if (get_key())
    {
#ifdef _DEBUG_SENDER
      Serial.println(KEY_BIT);
#endif
      control_LED(0);
  //  III_Rf_Init();
      rf_Send_key();
    // III_Rf_Init();
      delay(500);
    }


  }
  delay(2);
//  control_LED(0);// for idle mode

}


