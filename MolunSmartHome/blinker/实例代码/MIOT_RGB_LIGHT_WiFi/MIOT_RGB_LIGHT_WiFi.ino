/* *****************************************************************
 *
 *  名称：RGB氛围灯
 *  简介：三色RGB灯共GND,R、G、B引脚分别接GPIO13、GPIO14、GPIO15，小爱同学控制
 *  作者：MOLUN
 *  抖音：tymishop
 *  博客：https://molun.cf/
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_ESP_SMARTCONFIG
#define BLINKER_MIOT_LIGHT

#include <Blinker.h>

char auth[] = "dd84e83d50b7";

const int PIN_LED_RED = 13;
const int PIN_LED_GREEN = 14;
const int PIN_LED_BLUE = 15;

#define RGB_1 "RGBKey"

BlinkerRGB RGB1(RGB_1);

uint8_t colorR, colorG, colorB, colorW;
bool wsState;
uint8_t wsMode = BLINKER_CMD_MIOT_DAY;

void rgb1_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("R value: ", r_value);
    BLINKER_LOG("G value: ", g_value);
    BLINKER_LOG("B value: ", b_value);
    BLINKER_LOG("Rrightness value: ", bright_value);

    colorR = r_value;
    colorG = g_value;
    colorB = b_value;
    colorW = bright_value;
 
    analogWrite(PIN_LED_RED, colorR*colorW*0.041);
    analogWrite(PIN_LED_GREEN, colorG*colorW*0.041);
    analogWrite(PIN_LED_BLUE, colorB*colorW*0.041);

}

uint32_t getColor()
{
    uint32_t color = colorR << 16 | colorG << 8 | colorB;

    return color;
}

void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();

        wsState = true;

        if (colorW == 0) colorW = 255;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();

        wsState = false;
    }
}

void miotColor(int32_t color)
{
    BLINKER_LOG("need set color: ", color);

    colorR = color >> 16 & 0xFF;
    colorG = color >>  8 & 0xFF;
    colorB = color       & 0xFF;

    BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);

    analogWrite(PIN_LED_RED, colorR*colorW*0.041);
    analogWrite(PIN_LED_GREEN, colorG*colorW*0.041);
    analogWrite(PIN_LED_BLUE, colorB*colorW*0.041);

    BlinkerMIOT.color(color);
    BlinkerMIOT.print();
}

void miotMode(uint8_t mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_MIOT_DAY) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_NIGHT) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COLOR) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_WARMTH) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_TV) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_READING) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COMPUTER) {
        // Your mode function
    }

    wsMode = mode;

    BlinkerMIOT.mode(mode);
    BlinkerMIOT.print();
}

void miotBright(const String & bright)
{
    BLINKER_LOG("need set brightness: ", bright);

    colorW = bright.toInt();

    BLINKER_LOG("now set brightness: ", colorW);

    analogWrite(PIN_LED_RED, colorR*colorW*0.041);
    analogWrite(PIN_LED_GREEN, colorG*colorW*0.041);
    analogWrite(PIN_LED_BLUE, colorB*colorW*0.041);

    BlinkerMIOT.brightness(colorW);
    BlinkerMIOT.print();
}

void miotColoTemp(int32_t colorTemp)
{
    BLINKER_LOG("need set colorTemperature: ", colorTemp);

    BlinkerMIOT.colorTemp(colorTemp);
    BlinkerMIOT.print();
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.color(0);
            BlinkerMIOT.mode(0);
            BlinkerMIOT.colorTemp(1000);
            BlinkerMIOT.brightness(1);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_COLOR_NUMBER :
            BLINKER_LOG("MIOT Query Color");
            BlinkerMIOT.color(0);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_MODE_NUMBER :
            BLINKER_LOG("MIOT Query Mode");
            BlinkerMIOT.mode(0);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_COLORTEMP_NUMBER :
            BLINKER_LOG("MIOT Query ColorTemperature");
            BlinkerMIOT.colorTemp(1000);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER :
            BLINKER_LOG("MIOT Query Brightness");
            BlinkerMIOT.brightness(1);
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(wsState ? "on" : "off");
            BlinkerMIOT.color(0);
            BlinkerMIOT.mode(0);
            BlinkerMIOT.colorTemp(1000);
            BlinkerMIOT.brightness(1);
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

    RGB1.brightness(random(0, 255));
    RGB1.print(random(0, 255), random(0, 255), random(0, 255));
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Blinker.begin(auth);
    Blinker.attachData(dataRead);

    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachColor(miotColor);
    BlinkerMIOT.attachMode(miotMode);
    BlinkerMIOT.attachBrightness(miotBright);
    BlinkerMIOT.attachColorTemperature(miotColoTemp);
    BlinkerMIOT.attachQuery(miotQuery);

    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_RED, HIGH);
    pinMode(PIN_LED_GREEN, OUTPUT);
    digitalWrite(PIN_LED_GREEN, HIGH);
    pinMode(PIN_LED_BLUE, OUTPUT);
    digitalWrite(PIN_LED_BLUE, HIGH);

    colorR = 255;
    colorG = 255;
    colorB = 255;
    colorW = 0;
    wsState = true;

    RGB1.attach(rgb1_callback);
}

void loop()
{
    Blinker.run();
}
