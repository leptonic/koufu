#define DELAY_TIME 500
#define BEAT_PIN A5
#define BEAT_THRESHOLD_VAULE 125
#define BATTERY_PIN A7
#define BATTERY_THRESHOLD_VAULE 100
#define LED_PIN 9
#define LED2_PIN 10

void setup() {
  // put your setup code here, to run once:
#ifdef DEBUGMODE
Serial.begin(9600);
#endif
pinMode(LED_PIN,OUTPUT);
pinMode(LED2_PIN,OUTPUT);
III_Control_LED2(0);
III_Control_LED(0);
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
bool III_Get_KeyState()
{
  int result;
  result=analogRead(BEAT_PIN);
#ifdef DEBUGMODE
  Serial.print("get key state :");
  Serial.println(result);
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
void loop() {
  // put your main code here, to run repeatedly:
  int timeout;
  
while(!III_Get_KeyState()&&timeout<997)
{

  III_Control_LED(1);
  delay(1);
  timeout++;
}
if(timeout<996)
{
  III_Control_LED(0);
   delay(1000);
  }
  else
  timeout=0;


  
}