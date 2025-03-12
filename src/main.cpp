#define BLINKER_PRINT Serial
#define BLINKER_WIFI

// OLED显示屏的宽度和高度
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#include <Blinker.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Blinker设备认证信息和WiFi网络信息
char auth[] = "e0ac32aaa464";
char ssid[] = "密码是";
char pswd[] = "a&J8O3804";

// 创建OLED显示对象
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 定义Blinker按钮组件
BlinkerButton Button1("btn-on");
BlinkerButton Button2("btn-wtf");

int counter = 0;

// 定义电机控制引脚
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin1 = 14;

// 定义水泵控制引脚
int waterpin1 = 32;
int waterpin2 = 33;
int enable1Pin2 = 32;

// PWM设置参数
const int frep = 30000; // 频率
const int pwmchannel = 0; // PWM通道
const int resolution = 8; // 分辨率
int dutCycle = 200; // 占空比

// 初始化电机控制引脚并配置PWM
void motorPin()
{
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin1, OUTPUT);

  ledcSetup(pwmchannel, frep, resolution); // 设置PWM通道
  ledcAttachPin(enable1Pin1, pwmchannel); // 将使能引脚附加到PWM通道
}

// 电机高速运转
void motorhigh()
{
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
}

// 停止电机
void motorlow()
{
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
}

// 按钮1回调函数
void Button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  if(state == F("on"))
  {
    motorhigh(); // 启动电机
    Button1.print("on"); // 更新按钮状态
  }
  else if (state == F("off"))
  {
    motorlow(); // 停止电机
    Button1.print("off"); // 更新按钮状态
  }
}

// 初始化水泵控制引脚并配置PWM
void waterPin()
{
  pinMode(waterpin1, OUTPUT);
  pinMode(waterpin2, OUTPUT);
  pinMode(enable1Pin1, OUTPUT);

  ledcAttachPin(enable1Pin1, pwmchannel);
  ledcSetup(pwmchannel, frep, resolution);
}

// 泵水启动
void waterhigh()
{
  digitalWrite(waterpin1, HIGH);
  digitalWrite(waterpin2, LOW);
}

// 停止泵水
void waterlow()
{
  digitalWrite(waterpin1, LOW);
  digitalWrite(waterpin2, LOW);
}

// 自动运行泵水程序
void waterrun()
{
  waterhigh();
  delay(10000); // 运行10秒
  waterlow();
  delay(1000); // 停止1秒
}

// 按钮2回调函数
void Button2_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  if(state == F("on"))
  {
    waterrun(); // 启动泵水程序
    Button2.print("off"); // 更新按钮状态
  }
}

// 初始化OLED显示屏
void OLEDSetup()
{
  Wire.begin();
  display.begin();
  display.clearDisplay();
}

// 在OLED显示屏上打印消息
void OLEDPrint(String str)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(str);
  display.display();
  delay(1000);
}

// 循环调用OLEDPrint以显示不同的提示信息
void OLEDPr()
{
  OLEDPrint("欢迎使用刷鞋机");
  delay(5000);
  OLEDPrint("请将鞋子放入,并点击按钮开始");
  delay(5000);
  OLEDPrint("谢谢");
  delay(5000);
}

// 系统初始化
void setup() {
    // 初始化串口通信
    Serial.begin(115200);
    
    motorPin(); // 初始化电机控制
    waterPin(); // 初始化水泵控制
    OLEDSetup(); // 初始化OLED显示屏

    // 如果定义了BLINKER_PRINT，则启用调试输出
    #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
    #endif
    
    // 初始化内置LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    // 初始化Blinker服务
    Blinker.begin(auth, ssid, pswd);
    Button1.attach(Button1_callback); // 绑定按钮1回调函数
    Button2.attach(Button2_callback); // 绑定按钮2回调函数
}

// 主循环
void loop() 
{
  Blinker.run(); // 处理Blinker事件
  OLEDPr(); // 显示提示信息
}