/* *****************************************************************
 *
 *  名称：DHT11温湿度计（接入小爱同学）
 *  简介：DHT11温湿度传感器接入NodeMCU D4(GPIO2)引脚
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

BlinkerNumber TEMP("temp");
BlinkerNumber HUMI("humi");

#define DHTPIN D4

#define TEXTE_1 "tex-abc"

BlinkerText Text1(TEXTE_1);

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

void dataStorage()
{
    Blinker.dataStorage("temp", temp_read);
    Blinker.dataStorage("humi", humi_read);
}

void weatherData(const String & data)
{
    BLINKER_LOG("weather: ", data);

    DynamicJsonDocument jsonBuffer(1024);
    DeserializationError error = deserializeJson(jsonBuffer, data);
    JsonObject weather = jsonBuffer.as<JsonObject>();

    if (error)
    {
        BLINKER_LOG(data, " , not a Json buffer!");
    }

    String weather_text = weather["cond_txt"];
    int8_t weather_temp = weather["tmp"];

    BLINKER_LOG("Local weather is: ", weather_text, " ,temperature is: ", weather_temp, "℃");
}

void weatherForecastData(const String & data)
{
    BLINKER_LOG("weather: ", data);
}

void airData(const String & data)
{
    BLINKER_LOG("air: ", data);
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

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
    Text1.print("os time", BlinkerTime);
    
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
    Blinker.weather();
    Blinker.weatherForecast();
    Blinker.aqi();
    Blinker.wechat("Hello blinker!","state","message");
    
    Blinker.delay(60000);
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachWeather(weatherData);
    Blinker.attachWeatherForecast(weatherForecastData); 
    Blinker.attachAir(airData);   
    Blinker.attachHeartbeat(heartbeat);
    Blinker.attachDataStorage(dataStorage);
    BlinkerMIOT.attachQuery(miotQuery);  
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
        BLINKER_LOG("Temperature: ", t, " °C");
        BLINKER_LOG("Heat index: ", hic, " °C");
    }
}
