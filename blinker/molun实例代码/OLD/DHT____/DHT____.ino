/* *****************************************************************
 *
 *  名称：DHT11温湿度计
 *  简介：DHT11温湿度传感器数据引脚接入NodeMCU D4(GPIO2)引脚
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * 
 * *****************************************************************/

#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "6825c3f2a773"; // Blinker APP中添加设备时生成的Secret Key
char ssid[] = "tymishop"; // WIFI名称
char pswd[] = "15581822425"; // WIFI密码

BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");

// Download Adafruit DHT-sensor-library library here:
// https://github.com/adafruit/DHT-sensor-library
#include <DHT.h>

#define DHTPIN 2

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

uint32_t read_time = 0;

float humi_read, temp_read;

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}

void heartbeat()
{
    HUMI.print(humi_read);
    TEMP.print(temp_read);
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachHeartbeat(heartbeat);
    
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
