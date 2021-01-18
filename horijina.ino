/*
*Horijina 1.1 中医物联网加湿器
*版本说明：
*1.设置湿度，低于该湿度时进行加湿
*2.接入esp8266，尝试接入天猫精灵（算了先不接了）
*
*五花八门研究组 2021年1月13日
*欢迎交流讨论：irfanwang@163.com
*
**************转载请标明出处，仅供学习交流使用，不可用于商业。作者不对代码及硬件的安全性负责。****************
*/
#define BLINKER_WIFI
#include <Blinker.h>
#include <DHT.h>
#define DHTPIN D7
#define DHTTYPE DHT11
#define relay D0
#define Slider_1 "ran-ds5"

DHT dht(DHTPIN, DHTTYPE);
BlinkerButton Button1("btn-abc");
BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");
BlinkerSlider Slider1(Slider_1);

char auth[] = "5131a6bb593f";
char ssid[] = "WHBM";
char pswd[] = "whbm106106106";



int i=0;
float humSet = 30;//湿度预设值，请在这里修改
float t,h,temp_read,humi_read;
int MODE = 2;
int AutoMode = 2;
//函数声明
void relay_high();
void t_read(void);
void h_read(float h);

void button1_callback(const String& state) 
{ 
  BLINKER_LOG("get button state: ", state); 
  if(AutoMode == 2){
    if (state=="on"){
      digitalWrite(relay,LOW);
      Button1.print("on");
      Serial.println(MODE);
      MODE = 1;
    }else if(state=="off"){
      digitalWrite(relay,HIGH);
      Button1.print("off");
      MODE = 2;
    }
  }
  if(AutoMode == 1){
    if (state=="on"){
      digitalWrite(relay,HIGH);
      Button1.print("on");
      Serial.println(MODE);
      MODE = 2;
    }else if(state=="off"){
      digitalWrite(relay,LOW);
      Button1.print("off");
      MODE = 1;
    }
  }
}

void slider1_callback(int32_t value)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("get slider value: ", value);
    humSet = value;
    Serial.print(humSet);
}

void heartbeat()
{
  HUMI.print(humi_read);
  TEMP.print(temp_read);
  
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachHeartbeat(heartbeat);
    dht.begin();
    pinMode(relay,OUTPUT);
    digitalWrite(relay,LOW);
    Button1.attach(button1_callback);
    Slider1.attach(slider1_callback);
}

void loop()
{
    Blinker.run();
    Serial.print(MODE);
    Serial.print(humSet);
    h = dht.readHumidity();
    t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        BLINKER_LOG("Failed to read from DHT sensor!");
    }
    else
    {
        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        humi_read = h;
        temp_read = t;
        if (MODE == 2){
          relay_high();
        }    
    }
}


void relay_high(){
  if(h < humSet){
    digitalWrite(relay,HIGH);
    AutoMode = 2;
  }
  else if(h >= humSet){
    digitalWrite(relay,LOW);
    AutoMode = 1;
  }
}
