/* *****************************************************************
 *
 *  名称：DHT11温湿度计
 *  简介：DHT11温湿度传感器DATA引脚接入NodeMCU D4(GPIO2)引脚,接入小爱同学，
 *       增加历史数据存储与图表查看数据的功能
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_SENSOR

#include <Blinker.h>
#include <DHT.h>

char auth[] = "6825c3f2a773"; // Blinker APP中添加设备时生成的Secret Key
char ssid[] = "tymishop"; // WIFI名称
char pswd[] = "15581822425"; // WIFI密码

BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");

#define DHTPIN D4

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

int humi_read = 0, temp_read = 0;

void heartbeat()
{
    HUMI.print(humi_read);
    TEMP.print(temp_read);
}

void dataStorage()
{
    Blinker.dataStorage("temp", temp_read);
    Blinker.dataStorage("humi", humi_read);
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
            break;
    }
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    // BLINKER_DEBUG.debugAll();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachHeartbeat(heartbeat);
    Blinker.attachDataStorage(dataStorage);
    BlinkerMIOT.attachQuery(miotQuery);
    dht.begin();
}

void loop()
{
    Blinker.run();

    int h = dht.readHumidity();
    int t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        BLINKER_LOG("Failed to read from DHT sensor!");
    }
    else
    {
        humi_read = h;
        temp_read = t;
    }

    Blinker.delay(2000);
}
