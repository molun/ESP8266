/* *****************************************************************
 *  简介：ESP-01/ESP-01S 接入小爱同学控制
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET

#include <Blinker.h>
#include <ESP8266WiFi.h>
int count = 0;
bool WIFI_Status = true;

#define pin 0

char auth[] = "cb5ff18ed1cf";

bool oState = false;

//新建组件对象
#define BUTTON_1 "btn-abc" //开关按钮

BlinkerButton Button1(BUTTON_1);

// 按键回调函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    if (state == BLINKER_CMD_BUTTON_TAP) { //轻触按键  短按
        BLINKER_LOG("Button tap!");

        // Button1.icon("icon_1");
        Button1.color("#FF0000");
        Button1.text("Your button name or describe");
        Button1.print();
    }
    else if (state == BLINKER_CMD_BUTTON_PRESSED) {  //轻触按键  长按
        BLINKER_LOG("Button pressed!");

        // Button1.icon("icon_1");
        Button1.color("#FFFF00");
        Button1.text("Your button name or describe");
        Button1.print();
    }
    else if (state == BLINKER_CMD_BUTTON_RELEASED) {//轻触按键  释放
        BLINKER_LOG("Button released!");

        // Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        // Button1.text("Your button name", "describe");
        Button1.print();
    }
    else if (state == BLINKER_CMD_ON) {//开关按键  打开
        BLINKER_LOG("Toggle on!");
        digitalWrite(pin, HIGH);
        oState = true;
        // Button1.icon("icon_1");
        Button1.color("#0000FF");
        Button1.text("ON");
        // Button1.text("Your button name", "describe");
        Button1.print("on");
    }
    else if (state == BLINKER_CMD_OFF) {//开关按键  关闭
        BLINKER_LOG("Toggle off!");
        digitalWrite(pin, LOW);
        oState = false;
        // Button1.icon("icon_1");
        Button1.color("#00FFFF");
        Button1.text("OFF");
        // Button1.text("Your button name", "describe");
        Button1.print("off");
    }
    else {
        BLINKER_LOG("Get user setting: ", state);

        // Button1.icon("icon_1");
        Button1.color("#FFFFFF");
        Button1.text("Your button name or describe");
        // Button1.text("Your button name", "describe");
        Button1.print();
    }
}
/* 智能配网模式 */
void smartConfig()
{
  WiFi.mode(WIFI_STA);//设置STA模式
  Serial.println("\r\nWait for Smartconfig...");//打印log信息
  WiFi.beginSmartConfig();//开始SmartConfig，等待手机端发出用户名和密码
  while(1)
  {
    Serial.println(".");
    digitalWrite(LED_BUILTIN,HIGH);//指示灯闪烁
    delay(1000);
    digitalWrite(LED_BUILTIN,LOW);//指示灯闪烁
    delay(1000);
    if(WiFi.smartConfigDone())//配网成功，接收到SSID和密码
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      break;      
      }
    }  
}

/*小米小爱电源类回调函数*/
void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(pin, HIGH);
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(pin, LOW);
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();

        oState = false;
    }
}

/*小米设备状态查询回调函数*/
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            if(oState)
            {
                Button1.color("#00FFFF");
                Button1.text("OFF");
                // Button1.text("Your button name", "describe");
                Button1.print("off");
            }
            else
            {
                Button1.color("#0000FF");
                Button1.text("ON");
                // Button1.text("Your button name", "describe");
                Button1.print("on"); 
            }
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            if(oState)
            {
                Button1.color("#00FFFF");
                Button1.text("OFF");
                // Button1.text("Your button name", "describe");
                Button1.print("off");
            }
            else
            {
                Button1.color("#0000FF");
                Button1.text("ON");
                // Button1.text("Your button name", "describe");
                Button1.print("on"); 
            }
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); 
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("\r\n正在连接WIFI...");

    while(WiFi.status()!=WL_CONNECTED)//判断是否连接WIFI成功
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
          smartConfig(); //微信智能配网
          }
      }

   Serial.println("连接成功");  
   Serial.print("IP:");
   Serial.println(WiFi.localIP());
   
   Blinker.begin(auth,WiFi.SSID().c_str(),WiFi.psk().c_str());
   Blinker.attachData(dataRead);
   
    BlinkerMIOT.attachPowerState(miotPowerState); //注册小爱电源回调函数
    BlinkerMIOT.attachQuery(miotQuery);
    Button1.attach(button1_callback); //绑定按键执行回调函数
}

void loop()
{
    Blinker.run();
}
