/*
    接入贝壳物联，控制两路继电器开合。
    下载https://github.com/bigiot/bigiotArduino/archive/master.zip，
    需要将libraries/aJson文件夹全部复制到Arduino IDE的库文件夹内。
    我的继电器默认高电频关闭，所以在初始化时都初始化为高电频，
    play关闭开启，stop关闭关闭，输入1-2打开或关闭对应的引脚。
    源码来源https://github.com/bigiot/bigiotArduino/tree/master/examples/ESP8266/ESP8266WIFI/control4
        
*/

#include <ESP8266WiFi.h>
#include <aJSON.h>

//=============  此处必须修该============
String DEVICEID="18842"; // 你的设备编号   ==
String  APIKEY = "ecb34f917"; // 设备密码==
//=======================================
unsigned long lastCheckInTime = 0; //记录上次报到时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次

const char* ssid     = "tymishop";//无线名称
const char* password = "15581822425";//无线密码

const char* host = "www.bigiot.net";
const int httpPort = 8181;

int pins[2] = {D1,D2};
int state[2] = {HIGH,HIGH};
int arr_len = sizeof(pins)/sizeof(pins[0]);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, password);
  //默认输出关闭电频
  for(int i=0;i<arr_len;i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], state[i]);
  }
  
}

WiFiClient client;

void loop() {

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Use WiFiClient class to create TCP connections
  if (!client.connected()) {
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      delay(5000);
      return;
    }
  }

  if(millis() - lastCheckInTime > postingInterval || lastCheckInTime==0) {
    checkIn();
  }
  
  // Read all the lines of the reply from server and print them to Serial
  if (client.available()) {
    String inputString = client.readStringUntil('\n');
    inputString.trim();
    Serial.println(inputString);
    int len = inputString.length()+1;
    if(inputString.startsWith("{") && inputString.endsWith("}")){
      char jsonString[len];
      inputString.toCharArray(jsonString,len);
      aJsonObject *msg = aJson.parse(jsonString);
      processMessage(msg);
      aJson.deleteItem(msg);          
    }
  }
}

void processMessage(aJsonObject *msg){
  aJsonObject* method = aJson.getObjectItem(msg, "M");
  aJsonObject* content = aJson.getObjectItem(msg, "C");     
  aJsonObject* client_id = aJson.getObjectItem(msg, "ID");
  if (!method) {
    return;
  }
    String M = method->valuestring;
    if(M == "say"){
      String C = content->valuestring;
      String F_C_ID = client_id->valuestring;
      if(C == "play"){
        for(int i=0;i<arr_len;i++){
          state[i] = LOW;
          digitalWrite(pins[i], state[i]);
        }
        sayToClient(F_C_ID,"LED All on!");    
      }else if(C == "stop"){
        for(int i=0;i<arr_len;i++){
          state[i] = HIGH;
          digitalWrite(pins[i], state[i]);
        }
        sayToClient(F_C_ID,"LED All off!");    
      }else{
        int pin = C.toInt();
        if(pin > 0 && pin <= arr_len){
          pin--;
          state[pin] = !state[pin];
          digitalWrite(pins[pin], state[pin]);
        }
        sayToClient(F_C_ID,"LED pin:"+pin); 
      }
    }
}

void checkIn() {
    String msg = "{\"M\":\"checkin\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
    client.print(msg);
    lastCheckInTime = millis(); 
}

void sayToClient(String client_id, String content){
  String msg = "{\"M\":\"say\",\"ID\":\"" + client_id + "\",\"C\":\"" + content + "\"}\n";
  client.print(msg);
  lastCheckInTime = millis();
}
