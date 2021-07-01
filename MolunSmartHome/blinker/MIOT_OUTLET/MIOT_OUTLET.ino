/* *****************************************************************
 *  名称：智能语音控制插座
 *  简介：硬件使用ESP-01S+继电器模块，继电器低电平触发，使用GPIO0控制通断，
 *       带按键反馈，采用ESPTOUCH配网，接入小爱、小度、天猫精灵语音控制。
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET
#define BLINKER_DUEROS_OUTLET
#define BLINKER_ALIGENIE_OUTLET
#define BLINKER_ESP_SMARTCONFIG
//#define BLINKER_APCONFIG

#include <Blinker.h>

char auth[] = "64ebf3fa9e74";
//char ssid[] = "tymishop";
//char pswd[] = "15581822425";

const int RELAY = 0; // 定义继电器输入引脚为GPIO0

// 新建组件对象
#define BUTTON_1 "ButtonKey"

BlinkerButton Button1(BUTTON_1);

bool switch_state = false;

bool oState = false;

// 按下按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);

    if (state == BLINKER_CMD_ON) {
        BLINKER_LOG("Toggle on!");

        Button1.icon("fal fa-light-switch-on");
        Button1.color("#0000FF");
        Button1.text("电风扇开");
        Button1.print("on");
        BUILTIN_SWITCH.print("on");

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");

        Button1.icon("fal fa-light-switch-off");
        Button1.color("#FF0000");
        Button1.text("电风扇关");
        Button1.print("off");
        BUILTIN_SWITCH.print("off");

        oState = false;
    }

    BlinkerMIOT.powerState(oState ? "on" : "off");
    BlinkerMIOT.print();

    BlinkerDuerOS.powerState(oState ? "on" : "off");
    BlinkerDuerOS.report();

    BlinkerAliGenie.powerState(oState ? "on" : "off");
    BlinkerAliGenie.print();
    
    digitalWrite(LED_BUILTIN, oState);
    digitalWrite(RELAY, oState);
}

// 按下按键即会执行该函数
void switch_callback(const String & state)
{
    BLINKER_LOG("get switch state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(RELAY, LOW);
        switch_state = true;
        BUILTIN_SWITCH.print("on");
        Button1.icon("fal fa-light-switch-on");
        Button1.color("#0000FF");
        Button1.text("电风扇开");
        Button1.print("on");
    }
    else {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RELAY, HIGH);
        switch_state = false;
        BUILTIN_SWITCH.print("off");
        Button1.icon("fal fa-light-switch-off");
        Button1.color("#FF0000");
        Button1.text("电风扇关");
        Button1.print("off");
    }
}

void heartbeat()
{
    BUILTIN_SWITCH.print(switch_state ? "on" : "off");
    Button1.icon(oState ? "fal fa-light-switch-on" : "fal fa-light-switch-off");
    Button1.color(oState ? "#0000FF" : "#FF0000");
    Button1.text(oState ? "电风扇开" : "电风扇关");
    Button1.print(oState ? "on" : "off");
}

// 小爱同学控制部分
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(RELAY, LOW);

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
        Button1.icon("fal fa-light-switch-on");
        Button1.color("#0000FF");
        Button1.text("电风扇开");
        Button1.print("on");
        BUILTIN_SWITCH.print("on");

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RELAY, HIGH);

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
        Button1.icon("fal fa-light-switch-off");
        Button1.color("#FF0000");
        Button1.text("电风扇关");
        Button1.print("off");
        BUILTIN_SWITCH.print("off");

        oState = false;
    }
}

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

// 小度控制部分
void duerPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(RELAY, LOW);

        BlinkerDuerOS.powerState("on");
        BlinkerDuerOS.print();
        Button1.icon("fal fa-light-switch-on");
        Button1.color("#0000FF");
        Button1.text("电风扇开");
        Button1.print("on");
        BUILTIN_SWITCH.print("on");

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RELAY, HIGH);

        BlinkerDuerOS.powerState("off");
        BlinkerDuerOS.print();
        Button1.icon("fal fa-light-switch-off");
        Button1.color("#FF0000");
        Button1.text("电风扇关");
        Button1.print("off");
        BUILTIN_SWITCH.print("off");

        oState = false;
    }
}

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

// 天猫精灵控制部分
void aligeniePowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(RELAY, LOW);

        BlinkerAliGenie.powerState("on");
        BlinkerAliGenie.print();
        Button1.icon("fal fa-light-switch-on");
        Button1.color("#0000FF");
        Button1.text("电风扇开");
        Button1.print("on");
        BUILTIN_SWITCH.print("on");

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RELAY, HIGH);

        BlinkerAliGenie.powerState("off");
        BlinkerAliGenie.print();
        Button1.icon("fal fa-light-switch-off");
        Button1.color("#FF0000");
        Button1.text("电风扇关");
        Button1.print("off");
        BUILTIN_SWITCH.print("off");

        oState = false;
    }
}

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

    BlinkerDuerOS.powerState("off");
    BlinkerDuerOS.report();
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

    // 初始化有继电器的IO
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, HIGH);

    // 初始化blinker
    Blinker.begin(auth);
//    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    
    BUILTIN_SWITCH.attach(switch_callback);
    Button1.attach(button1_callback);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);

    BlinkerDuerOS.attachPowerState(duerPowerState);
    BlinkerDuerOS.attachQuery(duerQuery);

    BlinkerAliGenie.attachPowerState(aligeniePowerState);
    BlinkerAliGenie.attachQuery(aligenieQuery);
}

void loop()
{
    Blinker.run();
}
