/* *****************************************************************
 *
 *  名称：语音控制开关（插座）
 *  简介：硬件使用ESP-01S或者NodeMCU+继电器模块，继电器低电平触发，带按键反馈
 *       使用smartConfig配网
 *       ESP-01S使用GPIO0，NodeMCU使用D1（GPIO5）/D2（GPIO4）/D3(GPIO0)
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET
#define BLINKER_DUEROS_OUTLET
#define BLINKER_ALIGENIE_OUTLET

#include <Blinker.h>
#include <ESP8266WiFi.h> // WIFI库，配网必需

int count = 0; // 时间计数
bool WIFI_Status = true; // WIFI状态标志位

char auth[] = "64ebf3fa9e74"; // Blinker APP中添加设备时生成的Secret Key

/* 微信智能配网 */
void smartConfig()
{
  WiFi.mode(WIFI_STA); //设置STA模式
  Serial.println("\r\nWait for Smartconfig..."); // 打印log信息
  WiFi.beginSmartConfig(); // 开始SmartConfig，等待手机端发出用户名和密码
  while(1)
  {
    Serial.println(".");
    digitalWrite(LED_BUILTIN,HIGH); // 指示灯闪烁
    delay(1000);
    digitalWrite(LED_BUILTIN,LOW); // 指示灯闪烁
    delay(1000);
    if(WiFi.smartConfigDone()) // 配网成功，接收到SSID和密码
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      break;      
    }
  }  
}

/*连接网络*/
void blinkerConnect()
{
  Serial.println("\r\n正在连接WIFI...");

    while(WiFi.status()!=WL_CONNECTED)// 判断是否连接WIFI成功
    {
      if(WIFI_Status)
      {
          Serial.print(".");
          digitalWrite(LED_BUILTIN, HIGH);  
          delay(500);                       
          digitalWrite(LED_BUILTIN, LOW);    
          delay(500);                 
          count++;
          if(count>=5)//5s
          {
              WIFI_Status = false;
              Serial.println("WiFi连接失败，请用手机进行配网"); 
          }        
        }
        else
        {
          smartConfig(); // 微信智能配网
        }
    }

    Serial.println("连接成功");  
    Serial.print("IP:");
    Serial.println(WiFi.localIP());
}

const int GPIO = 0; // 定义继电器输入引脚为GPIO0

#define BUTTON_1 "btn-abc"
// 新建组件对象
BlinkerButton Button1(BUTTON_1);

bool oState = false;

// 按下按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");

        Button1.icon("fal fa-light-switch-on");
        Button1.color("#0000FF");
        Button1.text("ON");
        Button1.print("on");

        oState = true;
    } else if(state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");

        Button1.icon("fal fa-light-switch-off");
        Button1.color("#FF0000");
        Button1.text("OFF");
        Button1.print("off");

        oState = false;
    }
    
    BlinkerMIOT.powerState(oState ? "on" : "off");
    BlinkerMIOT.print();
    BlinkerDuerOS.powerState(oState ? "on" : "off");
    BlinkerDuerOS.print();
    BlinkerAliGenie.powerState(oState ? "on" : "off");
    BlinkerAliGenie.print();

//    digitalWrite(LED_BUILTIN, oState);
    digitalWrite(GPIO, oState);
}

void heartbeat()
{
    if (oState) Button1.print("on");
    else Button1.print("off");
}

// 小爱电源类的操作接口
// 用户自定义电源类操作的回调函数
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
//        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(GPIO, LOW);

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
//        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(GPIO, HIGH);

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();

        oState = false;
    }
}

// 小爱设备查询接口
// 用户自定义设备查询的回调函数
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}

// 小度电源类的操作接口
// 用户自定义电源类操作的回调函数
void duerPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
//        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(GPIO, LOW);

        BlinkerDuerOS.powerState("on");
        BlinkerDuerOS.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
//        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(GPIO, LOW);

        BlinkerDuerOS.powerState("off");
        BlinkerDuerOS.print();

        oState = false;
    }
}

// 小度设备查询接口
// 用户自定义设备查询的回调函数
void duerQuery(int32_t queryCode)
{
    BLINKER_LOG("DuerOS Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("DuerOS Query power state");
            BlinkerDuerOS.powerState(oState ? "on" : "off");
            BlinkerDuerOS.print();
            break;
        case BLINKER_CMD_QUERY_TIME_NUMBER :
            BLINKER_LOG("DuerOS Query time");
            BlinkerDuerOS.time(millis());
            BlinkerDuerOS.print();
            break;
        default :
            BlinkerDuerOS.powerState(oState ? "on" : "off");
            BlinkerDuerOS.print();
            break;
    }
}

// 天猫精灵电源类的操作接口
// 用户自定义电源类操作的回调函数
void aligeniePowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
//        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(GPIO, LOW);

        BlinkerAliGenie.powerState("on");
        BlinkerAliGenie.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
//        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(GPIO, HIGH);

        BlinkerAliGenie.powerState("off");
        BlinkerAliGenie.print();

        oState = false;
    }
}

// 天猫精灵设备查询接口
// 用户自定义设备查询的回调函数
void aligenieQuery(int32_t queryCode)
{
    BLINKER_LOG("AliGenie Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.powerState(oState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("AliGenie Query Power State");
            BlinkerAliGenie.powerState(oState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.powerState(oState ? "on" : "off");
            BlinkerAliGenie.print();
            break;
    }
}


// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();

    uint32_t BlinkerTime = millis();

    Blinker.print("millis", BlinkerTime);
}

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();

    // 初始化有LED的IO
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(GPIO, OUTPUT); // 设定GPIO0为输出引脚
    digitalWrite(GPIO, HIGH); // 设定GPIO0为高电位

    // 网络连接
    blinkerConnect();

    // 初始化blinker
    Blinker.begin(auth,WiFi.SSID().c_str(),WiFi.psk().c_str()); // 根据配网得到的WIFI信息和设备秘钥，连接到Blinker
    Blinker.attachData(dataRead);

    Button1.attach(button1_callback);
    Blinker.attachHeartbeat(heartbeat);
    BlinkerMIOT.attachPowerState(miotPowerState); // 注册小爱电源类操作回调函数
    BlinkerMIOT.attachQuery(miotQuery); // 注册小爱设备查询的回调函数
    BlinkerDuerOS.attachPowerState(duerPowerState); // 注册小度电源类操作回调函数
    BlinkerDuerOS.attachQuery(duerQuery);  // 注册小度设备查询的回调函数
    BlinkerAliGenie.attachPowerState(aligeniePowerState); // 注册天猫精灵电源类操作回调函数
    BlinkerAliGenie.attachQuery(aligenieQuery); // 注册天猫精灵设备查询的回调函数
}

void loop() {
    Blinker.run();
}
