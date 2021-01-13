/*
 *Horijina 1.0 中医物联网加湿器
 *版本说明：
 *1.设置湿度，低于该湿度时进行加湿
 *
 *教学说明：
 *1.继电器的使用
 *2.加湿模块的使用
 *
 *当前问题：
 *1.加湿模块较小，不能快速提高周围湿度（加湿速度的多档位调节）
 *2.将在后续版本增加充放电模块
 *3.DHT11摆放位置将影响湿度的检测
 *4.在设置湿度附近时频繁开关（确实需要多档调节）
 *
 *
 *五花八门研究组 2021年1月13日
 *
 *
 **************转载请标明出处，仅供学习交流使用，不可用于商业。作者不对代码及硬件的安全性负责。****************
 */

//DHT11调用、接口、定义
#include"DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11
//继电器接口
#define relay 2

//变量定义
float tempResult;//温度值
float humResult;//湿度值
int i=0;
float humSet = 15.0;//湿度预设值，请在这里修改

DHT dht(DHTPIN,DHTTYPE);

//函数声明
void relay_high();
void t_read(void);
void h_read(float h);

//开发板初始化
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);
}

//开发板主循环
void loop() {
  delay(10);
  i ++;
  //Serial.println(i);
  if(i > 100){
    i = 0;
    tempResult = dht.readTemperature();
    humResult = dht.readHumidity();
    Serial.println(humResult);
    relay_high();
  } 
}

//湿度调节函数
void relay_high(){
  if(humResult < humSet){
    digitalWrite(relay,HIGH);
  }
  else if(humResult >= humSet){
    digitalWrite(relay,LOW);
  }
}
