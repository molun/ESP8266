/* *****************************************************************  
 * @author  GuoFuqiang 
 * @version 1.0.0
 * @date    2021.03.22
 * @brief   主程序
 * 程序功能：blinker+小爱同学+本地按键（点触自复位）控制插座4路插孔/开关
 * 1.使用开发板：nodemcu-ESP8266(模块ESP-12F)
 * 2.程序功能1：使用Blinker 手机APP添加设备并绑定，通过Blinker APP或者小爱同学，
 *   远程控制开发板相连的继电器开关
 * 3.程序功能2：使用本地开关控制继电器开关，并反馈继电器状态到Blinker控制按钮
 * 4.例程文档：小爱类：https://www.diandeng.tech/doc/xiaoai
 *            SmartConfig自动配网：https://blog.csdn.net/u014091490/article/details/105178037
 *            Blinker心跳包：https://github.com/blinker-iot/blinker-doc/wiki/Blinker-Arduino-%E5%BA%93%E4%BD%BF%E7%94%A8%E6%89%8B%E5%86%8C
 * 5.程序参考：blinker库例程--blinker-library-master\examples\Blinker_MIOT\MIOT_MULTI_OUTLET
 * *****************************************************************/
 
#define BLINKER_WIFI               //支持wifi
#define BLINKER_ESP_SMARTCONFIG    //Smartconfig或者手机微信扫码自动配网，代码配网请注释此条
#define BLINKER_MIOT_MULTI_OUTLET  //支持多路插座，最多4个插孔
#define BLINKER_PRINT Serial       //串口协议库
#define BLINKER_WITHOUT_SSL        //blinker默认使用加密方式进行远程通信，但通信加密会消耗大量的RAM，如果您对通信安全性无要求
                                   //可以添加宏BLINKER_WITHOUT_SSL用以获得更多可用RAM,BLINKER_WITHOUT_SSL目前仅可用于ESP8266，其他设备的RAM足以进行加密通信
 
#include <Blinker.h>               //使用第三方Blinker库

char auth[] = "64ebf3fa9e74";      //Blinker APP（连接小爱同学必须阿里云服务器）中获取到的Secret Key(密钥)
//代码配网用这段
//char ssid[] = "tymishop";          //自己wifi名称
//char pswd[] = "15581822425";       //自己wifi密码
 
//定义继电器信号
#define Relay_1 5         //定义继电器1信号接入GPIO5，D1
#define Relay_2 4         //定义继电器2信号接入GPIO4，D2
#define Relay_3 0         //定义继电器3信号接入GPIO0，D3
#define Relay_4 2         //定义继电器4信号接入GPIO2，D4

//定义本地开关信号
#define KG_1 14         //定义开关1信号接入GPIO14，D5
#define KG_2 12         //定义开关2信号接入GPIO12，D6
#define KG_3 13         //定义开关3信号接入GPIO13，D7
#define KG_4 3          //定义开关4信号接入GPIO3，RX（GPIO15/D8持续低电平,后更改为GPIO3/RX接口）
 
// 新建Blinker软件组件对象
BlinkerButton Button("MainSwitch");     //组件对象,要和APP组件中的“数据键名”一致,总开关
BlinkerButton Button1("Switch1");       //组件对象,要和APP组件中的“数据键名”一致，开关1
BlinkerButton Button2("Switch2");       //组件对象,要和APP组件中的“数据键名”一致，开关2
BlinkerButton Button3("Switch3");       //组件对象,要和APP组件中的“数据键名”一致，开关3
BlinkerButton Button4("Switch4");       //组件对象,要和APP组件中的“数据键名”一致，开关4
BlinkerButton Button5("Refresh");       //APP端按钮状态刷新

//定义插座状态，用于小爱同学状态反馈
bool oState[5] = { false };
#define OUTLET_ALL   0  //所有插孔
#define OUTLET_NO_1  1  //插座插孔一
#define OUTLET_NO_2  2  //插座插孔二
#define OUTLET_NO_3  3  //插座插孔三
#define OUTLET_NO_4  4  //插座插孔四

//反馈继电器状态函数
void RelayState(int num)
{
  switch(num)
  {
    case 1:   //插座插孔一状态
        if(digitalRead(Relay_1)==LOW)
        {
          Button1.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
          Button1.text("插座1开");          //设置app按键注释“开”
          Button1.print("on");
          oState[1] = true;
        }
        else if(digitalRead(Relay_1==HIGH))
        {
          Button1.color("#808080");   //设置app按键是灰色，16进制颜色码
          Button1.text("插座1关");          //设置app按键注释“关”
          Button1.print("off");
          oState[1] = false;
        }
        break;
    case  2:
        if(digitalRead(Relay_2)==LOW)
        {
          Button2.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
          Button2.text("插座2开");          //设置app按键注释“开”
          Button2.print("on");
          oState[2] = true;
        }
        else if(digitalRead(Relay_2==HIGH))
        {
          Button2.color("#808080");   //设置app按键是灰色，16进制颜色码
          Button2.text("插座2关");          //设置app按键注释“关”
          Button2.print("off");
          oState[2] = false;
        }
        break;
    case  3:
        if(digitalRead(Relay_3)==LOW)
        {
          Button3.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
          Button3.text("插座3开");          //设置app按键注释“开”
          Button3.print("on");
          oState[3] = true;
        }
        else if(digitalRead(Relay_3==HIGH))
        {
          Button3.color("#808080");   //设置app按键是灰色，16进制颜色码
          Button3.text("插座3关");          //设置app按键注释“关”
          Button3.print("off");
          oState[3] = false;
        }
        break;
    case  4:
        if(digitalRead(Relay_4)==LOW)
        {
          Button4.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
          Button4.text("插座4开");          //设置app按键注释“开”
          Button4.print("on");
          oState[4] = true;
        }
        else if(digitalRead(Relay_4==HIGH))
        {
          Button4.color("#808080");   //设置app按键是灰色，16进制颜色码
          Button4.text("插座4关");          //设置app按键注释“关”
          Button4.print("off");
          oState[4] = false;
        }
        break;
        default:
            break;
  }
}
 
//小爱同学控制插座多个插孔
void ctrl_multi_outlet(uint8_t num, uint8_t io_level)
{
    switch (num)
        {
            case 0:  //所有插孔
                digitalWrite(Relay_1, io_level);//控制继电器1
                digitalWrite(Relay_2, io_level);//控制继电器2
                digitalWrite(Relay_3, io_level);//控制继电器3
                digitalWrite(Relay_4, io_level);//控制继电器4
                break;
            case 1:  //插座插孔一
                digitalWrite(Relay_1, io_level);//控制继电器1
                break;
            case 2:  //插座插孔二
                digitalWrite(Relay_2, io_level);//控制继电器2
                break;
            case 3:  //插座插孔三
                digitalWrite(Relay_3, io_level);//控制继电器3
                break;
            case 4:  //插座插孔四
                digitalWrite(Relay_4, io_level);//控制继电器4
                break;
            default:
                break;
        }
}
 
//小爱电源类回调，例如：“打开插座”、“打开插座插孔一”、“打开插座插孔二”
void miotPowerState(const String & state, uint8_t num)
{
    BLINKER_LOG("need set outlet: ", num, ", power state: ", state);
 
    if (state == BLINKER_CMD_ON) 
    {
        ctrl_multi_outlet(num, LOW);//打开继电器，num表示是多少路（继电器低电平出发）
        BlinkerMIOT.powerState("on", num);
        BlinkerMIOT.print();
        RelayState(num);
 
        oState[num] = true;
    }
   else if (state == BLINKER_CMD_OFF) 
    {
        ctrl_multi_outlet(num, HIGH);//关闭继电器，num表示是多少路
 
        BlinkerMIOT.powerState("off", num);
        BlinkerMIOT.print();
        RelayState(num);
 
        oState[num] = false;
    }
}

//小爱设备查询的回调函数，查询设备状态，例如：“插座插孔一状态”
void miotQuery(int32_t queryCode,uint8_t num)
{
    BLINKER_LOG("插孔",num,"状态",",codes:", queryCode);

    switch (num)
    {
         case 0 :
            BLINKER_LOG("状态：");
            BlinkerMIOT.powerState(oState[1] ? "on" : "off");
            BlinkerMIOT.powerState(oState[2] ? "on" : "off");
            BlinkerMIOT.powerState(oState[3] ? "on" : "off");
            BlinkerMIOT.powerState(oState[4] ? "on" : "off"); 
            BlinkerMIOT.print();
            break;
         case 1 :
            BLINKER_LOG("插孔1状态：");
            BlinkerMIOT.powerState(oState[1] ? "on" : "off");
            BlinkerMIOT.print();
            break;
         case 2 :
            BLINKER_LOG("插孔2状态：");
            BlinkerMIOT.powerState(oState[2] ? "on" : "off");
            BlinkerMIOT.print();
            break;
         case 3 :
            BLINKER_LOG("插孔3状态：");
            BlinkerMIOT.powerState(oState[3] ? "on" : "off");
            BlinkerMIOT.print();
            break;
         case 4 :
            BLINKER_LOG("插孔4状态：");
            BlinkerMIOT.powerState(oState[4] ? "on" : "off");
            BlinkerMIOT.print();
            break;
         default :
            BlinkerMIOT.powerState(oState[1] ? "on" : "off");
            BlinkerMIOT.powerState(oState[2] ? "on" : "off");
            BlinkerMIOT.powerState(oState[3] ? "on" : "off");
            BlinkerMIOT.powerState(oState[4] ? "on" : "off"); 
            BlinkerMIOT.print();
            break;
    }
 }

// 在APP控制，按下MainSwitch按键即会执行该函数
void button_callback(const String & state)
{
    BLINKER_LOG("操作了MainSwitch: ", state);//APP中的Monitor控件打印的信息
    if (state=="on") 
    {
        ctrl_multi_outlet(OUTLET_ALL, LOW);//打开继电器--所有
        // 反馈继电器状态
        Button.color("#FFFF00");   //设置app按键是纯黄色，16进制颜色码
        Button.text("开");          //设置app按键注释“开”
        Button.print("on");
    } else if(state=="off")
    {
        ctrl_multi_outlet(OUTLET_ALL, HIGH);//关闭继电器--所有
        // 反馈继电器状态
        Button.color("#808080");   //设置app按键是纯黄色，16进制颜色码
        Button.text("关");          //设置app按键注释“开”
        Button.print("off");
    }
}
 
// 在APP控制，按下Switch1按键即会执行该函数--第1路开关
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
    if (state=="on") 
    {
        ctrl_multi_outlet(OUTLET_NO_1, LOW);//打开继电器--第1路
        // 反馈继电器1状态
        RelayState(1);    //调用继电器反馈程序
    } else if(state=="off")
    {
        ctrl_multi_outlet(OUTLET_NO_1, HIGH);//关闭继电器--第1路
        // 反馈继电器状态
        RelayState(1);    //调用继电器反馈程序
    }
}
 
// 在APP控制，按下Switch2按键即会执行该函数--第2路开关
void button2_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
    if (state=="on") 
    {
        ctrl_multi_outlet(OUTLET_NO_2, LOW);//打开继电器--第2路
        // 反馈继电器状态
        RelayState(2);    //调用继电器反馈程序
    } else if(state=="off")
    {
        ctrl_multi_outlet(OUTLET_NO_2, HIGH);//关闭继电器--第2路
        // 反馈继电器状态
        RelayState(2);    //调用继电器反馈程序
    }
}
 
 // 在APP控制，按下Switch3按键即会执行该函数--第3路开关
 void button3_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
    if (state=="on") 
    {
        ctrl_multi_outlet(OUTLET_NO_3, LOW);//打开继电器--第3路
        // 反馈继电器状态
        RelayState(3);    //调用继电器反馈程序
    } else if(state=="off")
    {
        ctrl_multi_outlet(OUTLET_NO_3, HIGH);//关闭继电器--第3路
        // 反馈继电器状态
        RelayState(3);    //调用继电器反馈程序
    }
}

// 在APP控制，按下Switch4按键即会执行该函数--第4路开关 
 void button4_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);//APP中的Monitor控件打印的信息
    if (state=="on") 
    {
        ctrl_multi_outlet(OUTLET_NO_4, LOW);//打开继电器--第4路
        // 反馈继电器状态
        RelayState(4);    //调用继电器反馈程序
    } else if(state=="off")
    {
        ctrl_multi_outlet(OUTLET_NO_4, HIGH);//关闭继电器--第4路
        // 反馈继电器状态
        RelayState(4);    //调用继电器反馈程序
    }
}

//APP端状态手动刷新按钮
void button5_callback(const String & state)
{
  for(int i=0;i<5;i++)
  {
    RelayState(i);
  }
}

//心跳包刷新状态
 void heartbeat()
 {
  for(int i=0;i<5;i++)
  {
    RelayState(i);
  }
 }
 
 //本地开关控制继电器程序
 void bdms()
 {
  if(digitalRead(KG_1)==LOW)
  {
    digitalWrite(Relay_1, !digitalRead(Relay_1));
    //反馈继电器1状态
    RelayState(1);    //调用继电器反馈程序
    Blinker.delay(1000);    //必须加延时,否则灯一直切换状态
  }
  if(digitalRead(KG_2)==LOW)
  {
    digitalWrite(Relay_2, !digitalRead(Relay_2));
    RelayState(2);    //调用继电器反馈程序
    Blinker.delay(1000); 
  }
  if(digitalRead(KG_3)==LOW)
  {
    digitalWrite(Relay_3, !digitalRead(Relay_3));
    RelayState(3);    //调用继电器反馈程序
    Blinker.delay(1000);
  }
 if(digitalRead(KG_4)==LOW)
  {
    digitalWrite(Relay_4, !digitalRead(Relay_4));
    RelayState(4);    //调用继电器反馈程序
    Blinker.delay(1000);
  }
 }
 
void setup()
{
    // 初始化串口，用于调试，后期可删除
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    
    // 初始化有继电器的IO
    pinMode(Relay_1, OUTPUT);
    pinMode(Relay_2, OUTPUT);
    pinMode(Relay_3, OUTPUT);
    pinMode(Relay_4, OUTPUT);
    //初始化继电器初始状态
    digitalWrite(Relay_1, HIGH);       //继电器为低电平触发，初始化为HIGH
    digitalWrite(Relay_2, HIGH);
    digitalWrite(Relay_3, HIGH);
    digitalWrite(Relay_4, HIGH);
    //初始化本地按键状态
    pinMode(KG_1, INPUT_PULLUP);   //本地开关输入上拉
    pinMode(KG_2, INPUT_PULLUP);   //本地开关输入上拉
    pinMode(KG_3, INPUT_PULLUP);   //本地开关输入上拉
    pinMode(KG_4, INPUT_PULLUP);   //本地开关输入上拉
    
    //初始化blinker
    Blinker.begin(auth);              //手机配网用这段
    //Blinker.begin(auth, ssid, pswd);//代码配网用这段
    Button.attach(button_callback);   //绑定按键回调
    Button1.attach(button1_callback); //绑定按键回调
    Button2.attach(button2_callback); //绑定按键回调
    Button3.attach(button3_callback); //绑定按键回调
    Button4.attach(button4_callback); //绑定按键回调
    Button5.attach(button5_callback); //绑定按键回调
    
    //小爱同学注册回调
    BlinkerMIOT.attachPowerState(miotPowerState); //注册小爱电源回调
    BlinkerMIOT.attachQuery(miotQuery);           //小爱设备查询的回调函数
    
    //心跳包，初始化
    Blinker.attachHeartbeat(heartbeat);           //app定时向设备发送心跳包, 设备收到心跳包后会返回设备当前状态
}
 
void loop() 
{
  bdms();           //本地开关模式
  Blinker.run();
}
