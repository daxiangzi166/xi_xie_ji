#define BLYNK_PRINT Serial

// 添加模板ID和名称
#define BLYNK_TEMPLATE_ID "TMPL6ae6ZQ2VV"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

#define LED_PIN     5 // 定义连接到FastLED的引脚
#define NUM_LEDS    30 // 定义灯带中的LED灯数量
#define BRIGHTNESS  50 // 设置亮度
#define LED_TYPE    WS2812B // 定义LED的型号
#define COLOR_ORDER GRB // 定义色彩顺序

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "FastLED.h"

char auth[] = "mcPWGqdRHuCvFjCNQ0uk0XJ_toAZxCPt"; // 替换为你的Blynk授权令牌
char ssid[] = "科创工作室"; // 替换为你的WiFi SSID
char pass[] = "xsj666666"; // 替换为你的WiFi密码

int counter = 0;
int vpin_value;


// 定义电机控制引脚
int motor1Pin1 = 17;
int motor1Pin2 = 18;
int enable1Pin1 = 14;

// 定义水泵控制引脚
int waterpin1 = 39;
int waterpin2 = 40;
int enable1Pin2 = 32;

//定义超声波控制的继电器的引脚
int trigPin = 12; // 超声波触发引脚


int buzzerPin1 = 40; // 蜂鸣器引脚
int buzzerPin2 = 39; // 蜂鸣器引脚

// PWM设置参数
const int frep = 30000; // 频率
const int pwmchannel = 0; // PWM通道
const int resolution = 8; // 分辨率
int dutCycle = 200; // 占空比

CRGB leds[NUM_LEDS]; // 创建LED数组

void LEDRGB(){
  delay(3000); // 延迟3秒
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // 初始化FastLED库并设置
  FastLED.setBrightness(BRIGHTNESS); // 设置全局亮度
}

void buzzerpin(){ 
  pinMode(buzzerPin1, OUTPUT);
  pinMode(buzzerPin2, OUTPUT);
}

void buzzerhigh(){
  digitalWrite(buzzerPin1, HIGH);
  digitalWrite(buzzerPin2, LOW);
}

void buzzerlow(){
  digitalWrite(buzzerPin1, LOW);
  digitalWrite(buzzerPin2, LOW);
}

void buzzerrun(){
  buzzerhigh();
  delay(2000);
  buzzerlow();
}

// 初始化电机控制引脚并配置PWM
void motorPin(){
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin1, OUTPUT);

  ledcSetup(pwmchannel, frep, resolution); // 设置PWM通道
  ledcAttachPin(enable1Pin1, pwmchannel); // 将使能引脚附加到PWM通道
}

// 电机高速运转
void motorhigh(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
}

// 停止电机
void motorlow(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
}

// 按钮1回调函数
BLYNK_WRITE(V0){ // 假设按钮1连接到虚拟引脚V1 
  vpin_value = param.asInt();
  if(vpin_value == 1) // 如果按钮被按下
  {
    Serial.println(vpin_value); // 打印按钮状态到串口
    buzzerrun(); // 启动蜂鸣器
    motorhigh(); // 启动电机
  }
  else
  {
    buzzerrun(); // 启动蜂鸣器
    motorlow(); // 停止电机
  }
}


// 初始化水泵控制引脚并配置PWM
void waterPin(){
  pinMode(waterpin1, OUTPUT);
  pinMode(waterpin2, OUTPUT);
  pinMode(enable1Pin1, OUTPUT);

  ledcAttachPin(enable1Pin1, pwmchannel);
  ledcSetup(pwmchannel, frep, resolution);
}

// 泵水启动
void waterhigh(){
  digitalWrite(waterpin1, HIGH);
  digitalWrite(waterpin2, LOW);
}

// 停止泵水
void waterlow(){
  digitalWrite(waterpin1, LOW);
  digitalWrite(waterpin2, LOW);
}

// 自动运行泵水程序
void waterrun(){
  waterhigh();
  delay(10000); // 运行10秒
  waterlow();
  delay(1000); // 停止1秒
}

// 按钮2回调函数
BLYNK_WRITE(V1){
  vpin_value = param.asInt(); // 获取虚拟引脚V1的值
  Serial.println(vpin_value); // 打印按钮状态到串口
  if(vpin_value == 1)
  {
    Serial.println(vpin_value); // 打印按钮状态到串口
    buzzerrun(); // 启动蜂鸣器
    waterrun(); // 启动泵水程序
  }
}

//继电器引脚为输出模式
void trigpin(){
  pinMode(trigPin, OUTPUT);
}

void trighigh(){
  digitalWrite(trigPin, HIGH);
}

void triglow(){
  digitalWrite(trigPin, LOW);
}

BLYNK_WRITE(V2){ // 假设按钮3连接到虚拟引脚V2
  vpin_value = param.asInt();
  if(vpin_value == 1)
  {
    Serial.println(vpin_value); // 打印按钮状态到串口
    buzzerrun(); // 启动蜂鸣器
    trighigh(); // 启动继电器
  }
  else
  {
    Serial.println(vpin_value); // 打印按钮状态到串口
    buzzerrun(); // 启动蜂鸣器
    triglow(); // 停止继电器
  }
}

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void OLED(){
  u8g2.setFont(u8g2_font_wqy12_t_chinese1);
  u8g2.setFontPosTop();
  u8g2.setCursor(0,20);
  u8g2.print("你好");
  delay(1000);
  u8g2.clearDisplay();
  u8g2.setFont(u8g2_font_wqy12_t_chinese1);
  u8g2.setFontPosTop();
  u8g2.setCursor(0,20);
  u8g2.print("你好");
  delay(1000);
}

// 系统初始化
void setup(){
    // 初始化串口通信
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass); // 使用正确的初始化方式

    motorPin(); // 初始化电机控制
    waterPin(); // 初始化水泵控制
    trigpin(); // 初始化超声波继电器控制
    buzzerpin(); // 初始化蜂鸣器控制
    LEDRGB(); // 初始化LED灯带

    u8g2.setI2CAddress(0x3C*2);
    u8g2.begin();
    u8g2.enableUTF8Print();

    // 初始化内置LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

// 主循环
void loop(){
  Blynk.run(); // 运行Blynk库

  u8g2.firstPage();
  do
  {
    OLED();
  } while(u8g2.nextPage()); // 显示OLED

  for (int i = 0; i < 256; i++) { // 从0变化到255
    int r = i; // 红色分量
    int g = 255 - i; // 绿色分量
    int b = 0; // 蓝色分量为0
    fill_solid(leds, NUM_LEDS, CHSV(r, 255, g)); // 设置HSV颜色
    FastLED.show();
    delay(10); // 短暂延迟以减慢变化速度
  }
  // 可以通过改变上面的循环条件或颜色函数来创建其他动态效果
}
