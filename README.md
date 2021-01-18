# HoriJina 好瑞 中医物联网加湿器
**请注意：本项目开源仅供学习使用并请在转载时注明出处。请勿在任何商业用途中使用。本项目作者不对项目中的任何软件和硬件的安全性负责，作者暂不对后续增加的养生功能涉及的中草药及其他形式的疗法的安全性和有效性负责。
欢迎和作者交流学习，如有意见和建议请联系：irfanwang@163.com**
点灯·Blinker相关文件请浏览：[点灯科技·Blinker](https://www.diandeng.tech/home) 或[Blinker中文文档](https://github.com/blinker-iot/blinker-doc/wiki)
## 1.背景&功能说明：
*本项目作者为中医背景，会在后续的版本中会根据中医理论推出养生功能，敬请期待*
 ### 1.1 背景
 * 在北方的冬天，课题组中来自南方的同门纷纷表示太干燥了（快要质壁分离的那种）。
 * 还有同门因为实验需要在恒温箱里加入加湿器。
 * 于是，HoriJina就诞生了。
 ### 1.2 功能说明
 ***当前版本：1.1***
 #### HoriJina 1.1
1.接入Blinker
2.在blinker客户端加入了一个可用于设置加湿阈值的滑块
3.在blinker客户端现在可以显示当前的温度和湿度
3.在自动开始加湿时，可以通过blinker客户端上的开关开启或关闭加湿
4.在加湿自动关闭时，暂时还不能通过开关开启或关闭，可通过降低加湿阈值使加湿功能自动开启
![d73a494cd7440a9086b413ff2032fafd.jpeg](en-resource://database/605:1)
![6ac201e40b0da3ee3b6737e285c31688.jpeg](en-resource://database/607:1)

 #### HoriJina 1.0
* 温湿度模块检测环境温度和湿度。
* **如果当前湿度小于等于预设值，加湿模块进行加湿**

## 2.硬件选择：
 * 开发板：Arduino Mega2560（后续版本会可能会使用ESP系列）
 * 温湿度传感器：DHT11
 * 继电器、加湿模块等。
## 3.接线：
* DHT11 接Arduino A0
* 继电器信号接Arduino 2
![接线原理图](https://github.com/IrfanWang95/HoriJina/blob/main/horijina.png?raw=true)
+ 当前版本的开发板使用的是esp8266系列，接口为：
    + 继电器信号引脚：D0
    + DHT11信号引脚：D7
+ 其他接线方式不变，如修改信号引脚需修改对应代码。

## 4.代码说明
### 4.1 包含的库
+ DHT11模块
```
#include"DHT.h"
#define DHTPIN D7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
```
+ Blinker
```
#define BLINKER_WIFI
#include <Blinker.h>
BlinkerButton Button1("btn-abc");    //增加按钮（按钮的键名）
BlinkerNumber HUMI("humi");        //传输的数据（数据的键名）
BlinkerNumber TEMP("temp");
BlinkerSlider Slider1("ran-ds5");        //滑动条（滑动条的键名）
```
   （如果在Arduino报错请在菜单栏“工具”下的“管理库”中进行搜索下载）
### 4.2 声明
+ blinker的设备码和WiFi信息
```
char auth[] = "Your Device Secret Key";
char ssid[] = "Your WiFi network SSID or name";
char pswd[] = "Your WiFi network WPA password or WEP key";
```
+ 变量的声明
```
float humSet = 30;//湿度默认值，请在这里修改
float t,h,temp_read,humi_read;
int MODE = 2;      //
int AutoMode = 2;   //加湿器开关状态2默认打开
//函数声明
void relay_high();
void t_read(void);
void h_read(float h);
```
### 4.3 Blinker函数
+ 按键
```
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
```
+ 滑动条
```
void slider1_callback(int32_t value)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("get slider value: ", value);                   //这里的value就是滑动条的数值，将humSet赋值一下就可以设置了
    humSet = value;
    Serial.print(humSet);
}

```
+ 心跳包
```
void heartbeat()
{
  HUMI.print(humi_read);
  TEMP.print(temp_read);
}
```
    
**剩余的图片和说明会在即刻更新**

