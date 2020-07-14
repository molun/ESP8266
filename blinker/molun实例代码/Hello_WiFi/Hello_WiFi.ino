/* *****************************************************************
 *
 *  简介：ESP-01S+Relay，继电器低电平触发，使用ESP-01S GPIO0引脚控制
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET




#include <Blinker.h>

char auth[] = "cb5ff18ed1cf"; // Blinker APP中添加设备时生成的Secret Key
char ssid[] = "tymishop"; // WIFI名称
char pswd[] = "15581822425"; // WIFI密码

// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerNumber Number1("num-abc");

int counter = 0;

int GPIO = 0; // 定义继电器输入引脚为GPIO/0

bool oState = false;

// 按下按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    digitalWrite(GPIO, !digitalRead(GPIO));
}

// 小爱电源类的操作接口
// 用户自定义电源类操作的回调函数
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

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
    Number1.print(counter);

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
    digitalWrite(LED_BUILTIN, HIGH);
    pinMode(GPIO, OUTPUT); // 设定GPIO0为输出引脚
    digitalWrite(GPIO, HIGH); // 设定GPIO0为高电位
    
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);

    Button1.attach(button1_callback);

    BlinkerMIOT.attachPowerState(miotPowerState); // 注册小爱电源类操作回调函数
    BlinkerMIOT.attachQuery(miotQuery); // 注册小爱设备查询的回调函数
}

void loop() {
    Blinker.run();
}
