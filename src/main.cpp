#define BLINKER_PRINT Serial
#define BLINKER_WIFI


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#include <Blinker.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

char auth[] = "e0ac32aaa464";
char ssid[] = "科创工作室";
char pswd[] = "xsj666666";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 新建组件对象
BlinkerButton Button1("btn-on");
BlinkerButton Button2("btn-wtf");

int counter = 0;

int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin1 = 14;

int waterpin1 = 32;
int waterpin2 = 33;
int enable1Pin2 = 32;

const int frep = 30000;
const int pwmchannel = 0;
const int resolution = 8;
int dutCycle = 200;

void motorPin()
{
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin1, OUTPUT);

ledcSetup(pwmchannel, frep, resolution);
ledcAttachPin(enable1Pin1, pwmchannel);
}

void motorhigh()
{
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
}

void motorlow()
{
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
}

void Button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  if(state == F("on"))
  {
    motorhigh();
    Button1.print("on");
  }
  else if (state == F("off"))
  {
    motorlow();
    Button1.print("off");
  }
}

void waterPin()
{
  pinMode(waterpin1, OUTPUT);
  pinMode(waterpin2, OUTPUT);
  pinMode(enable1Pin1, OUTPUT);

  ledcAttachPin(enable1Pin1, pwmchannel);
  ledcSetup(pwmchannel, frep, resolution);
}

void waterhigh()
{
  digitalWrite(waterpin1, HIGH);
  digitalWrite(waterpin2, LOW);
}

void waterlow()
{
  digitalWrite(waterpin1, LOW);
  digitalWrite(waterpin2, LOW);
}


void waterrun()
{
  waterhigh();
  delay(10000);
  waterlow();
  delay(1000);
}

void Button2_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  if(state == F("on"))
  {
    waterrun();
    Button2.print("off");
  }
}



void OLEDSetup()
{
  Wire.begin();
  display.begin();
  display.clearDisplay();
}

void OLEDPrint(String str)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("欢迎使用刷鞋机");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("请将鞋子放入,并点击按钮开始");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("谢谢");
  display.display();
  delay(1000);
}

void OLEDPr()
{
  OLEDPrint("欢迎使用刷鞋机");
  delay(5000);
  OLEDPrint("请将鞋子放入,并点击按钮开始");
  delay(5000);
  OLEDPrint("谢谢");
  delay(5000);
}

void setup() {
    // 初始化串口
    Serial.begin(115200);
    
    motorPin();
    waterPin();
    OLEDSetup();

    #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
    #endif
    
    // 初始化有LED的IO
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Button1.attach(Button1_callback);
    Button2.attach(Button2_callback);
}

void loop() 
{
 Blinker.run();
 OLEDPr();
}