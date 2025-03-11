#define BLINKER_PRINT Serial
#define BLINKER_WIFI

#include <Blinker.h>
#include <Arduino.h>

char auth[] = "e0ac32aaa464";
char ssid[] = "科创工作室";
char pswd[] = "xsj666666";

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

void Button2_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  if(state == F("on"))
  {
   waterhigh();
   Button2.print("on");
  }
  else if (state == F("off"))
  {
    waterlow();
    Button2.print("off");
  }
}

void setup() {
    // 初始化串口
    Serial.begin(115200);
    
    motorPin();
    waterPin();

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
}