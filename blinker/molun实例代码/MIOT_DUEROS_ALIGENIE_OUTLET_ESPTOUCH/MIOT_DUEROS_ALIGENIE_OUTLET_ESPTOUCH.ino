/* *****************************************************************
 *  名称：智能语音控制插座
 *  简介：硬件使用ESP-01S+继电器模块，继电器低电平触发，使用GPIO0控制通断，
 *       带按键反馈，采用ESPTOUCH配网，接入小爱、小度、天猫精灵语音控制。
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_ESP_SMARTCONFIG
#define BLINKER_DUEROS_OUTLET
#define BLINKER_MIOT_OUTLET
#define BLINKER_ALIGENIE_OUTLET

#include <Blinker.h>

char auth[] = "64ebf3fa9e74";

const int GPIO = 0; // 定义继电器输入引脚为GPIO0

#define BUTTON_1 "btn-abc"

BlinkerButton Button1(BUTTON_1);

bool oState = false;

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
    }
    else if (state == BLINKER_CMD_OFF) {
        BLINKER_LOG("Toggle off!");

        Button1.icon("fal fa-light-switch-off");
        Button1.color("#FF0000");
        Button1.text("OFF");
        Button1.print("off");

        oState = false;
    }

    BlinkerDuerOS.powerState(oState ? "on" : "off");
    BlinkerDuerOS.report();
//    BlinkerMIOT.powerState(oState ? "on" : "off");
//    BlinkerMIOT.print();
//    BlinkerAliGenie.powerState(oState ? "on" : "off");
//    BlinkerAliGenie.print();
    
    digitalWrite(LED_BUILTIN, oState);
    digitalWrite(GPIO, oState);
}

//小度控制部分
void duerPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(GPIO, LOW);

        BlinkerDuerOS.powerState("on");
        BlinkerDuerOS.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(GPIO, HIGH);

        BlinkerDuerOS.powerState("off");
        BlinkerDuerOS.print();

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

//小爱同学控制部分
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(GPIO, LOW);

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(GPIO, HIGH);

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();

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

//天猫精灵控制部分
void aligeniePowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(GPIO, LOW);

        BlinkerAliGenie.powerState("on");
        BlinkerAliGenie.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(GPIO, HIGH);

        BlinkerAliGenie.powerState("off");
        BlinkerAliGenie.print();

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
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(GPIO, OUTPUT);
    digitalWrite(GPIO, HIGH);

    Blinker.begin(auth);
    Blinker.attachData(dataRead);

//    Blinker.setTimezone(8.0);

    Button1.attach(button1_callback);
//    Blinker.attachHeartbeat(heartbeat);
    
    BlinkerDuerOS.attachPowerState(duerPowerState);
    BlinkerDuerOS.attachQuery(duerQuery);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);
    BlinkerAliGenie.attachPowerState(aligeniePowerState);
    BlinkerAliGenie.attachQuery(aligenieQuery);
}

void loop()
{
    Blinker.run();
}
