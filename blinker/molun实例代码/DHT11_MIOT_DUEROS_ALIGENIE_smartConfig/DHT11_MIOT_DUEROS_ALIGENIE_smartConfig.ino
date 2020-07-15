/* *****************************************************************
 *
 *  名称：DHT11温湿度计
 *  简介：DHT11温湿度传感器DATA引脚接入NodeMCU D4(GPIO2)引脚,接入小爱同学、
 *       小度音箱、天猫精灵，增加历史数据存储与图表查看数据的功能
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_SENSOR
#define BLINKER_DUEROS_SENSOR
#define BLINKER_ALIGENIE_SENSOR
#define BLINKER_PRINT Serial

#include <Blinker.h>
#include <DHT.h>
#include <ESP8266WiFi.h>    //WIFI库，配网必需

//int counter = 0;

int count = 0;     //时间计数
bool WIFI_Status = true;   //WIFI状态标志位

char auth[] = "6825c3f2a773"; // Blinker APP中添加设备时生成的Secret Key
//char ssid[] = "tymishop"; // WIFI名称
//char pswd[] = "15581822425"; // WIFI密码

/* 微信智能配网 */
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

/*连接网络*/
void blinkerConnect()
{
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
          smartConfig();  //微信智能配网
        }
    }

    Serial.println("连接成功");  
    Serial.print("IP:");
    Serial.println(WiFi.localIP());
}

BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");

#define DHTPIN D4

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

uint32_t read_time = 0;

float humi_read, temp_read;

void heartbeat()
{
    HUMI.print(humi_read);
    TEMP.print(temp_read);
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.temp(temp_read);
            BlinkerMIOT.humi(humi_read);
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.temp(20);
            BlinkerMIOT.humi(20);
            BlinkerMIOT.print();
            break;
    }
}

void dataStorage()
{
    Blinker.dataStorage("temp", temp_read);
    Blinker.dataStorage("humi", humi_read);
}

void duerQuery(int32_t queryCode)
{
    BLINKER_LOG("DuerOS Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_HUMI_NUMBER :
            BLINKER_LOG("DuerOS Query HUMI");
            BlinkerDuerOS.humi(humi_read);
            BlinkerDuerOS.print();
            break;
        case BLINKER_CMD_QUERY_TEMP_NUMBER :
            BLINKER_LOG("DuerOS Query TEMP");
            BlinkerDuerOS.temp(temp_read);
            BlinkerDuerOS.print();
            break;
        default :
            BlinkerDuerOS.temp(20);
            BlinkerDuerOS.humi(20);
            BlinkerDuerOS.print();
            break;
    }
}

void aligenieQuery(int32_t queryCode)
{
    BLINKER_LOG("AliGenie Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("AliGenie Query All");
            BlinkerAliGenie.temp(temp_read);
            BlinkerAliGenie.humi(humi_read);
            BlinkerAliGenie.print();
            break;
        default :
            BlinkerAliGenie.temp(20);
            BlinkerAliGenie.humi(20);
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
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    //网络连接
    blinkerConnect();
  
    // 初始化blinker
    Blinker.begin(auth,WiFi.SSID().c_str(),WiFi.psk().c_str()); //根据配网得到的WIFI信息和设备秘钥，连接到Blinker云   
//    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    Blinker.attachDataStorage(dataStorage);
    BlinkerMIOT.attachQuery(miotQuery);
    BlinkerDuerOS.attachQuery(duerQuery);
    BlinkerAliGenie.attachQuery(aligenieQuery);   
    dht.begin();
}

void loop()
{
    Blinker.run();

    if (read_time == 0 || (millis() - read_time) >= 2000)
    {
        read_time = millis();

        float h = dht.readHumidity();
        float t = dht.readTemperature();        

        if (isnan(h) || isnan(t)) {
            BLINKER_LOG("Failed to read from DHT sensor!");
            return;
        }

        float hic = dht.computeHeatIndex(t, h, false);

        humi_read = h;
        temp_read = t;

        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        BLINKER_LOG("Heat index: ", hic, " *C");
    }
}
