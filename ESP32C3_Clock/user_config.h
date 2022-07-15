#ifndef _USER_CONDIG_
#define _USER_CONDIG_

// wifi 账号密码最大长度  
#define WIFI_STR_LEN    255

// 热点名
#define AP_SSID         "ESP32C3"
#define AP_PASSWORD     ""

// IP获取本地地理位置、时区 及 天气URL 
#define WEATHER_URL     "http://1.15.242.194/getweather"

// 时间同步服务器 
#define NTP_HOST        "pool.ntp.org"

// 时区 
#define TIME_ZONE        8
#define LOCAL_HTTP_MIN   2     // 本地http间隔2分钟  
#define NETWOEK_WEATHER_MIN    35   // 网络上天气间隔时间35分 

// 屏幕区域 
#define  TFT_WIDTH      240 
#define  TFT_HEIGHT     240 
#define  SUPERIOR       60   // 上边高度 
#define  SUPERIOR_LEFT  155  // 上面左边区域宽度 
#define  SUPERIOR_RIGHT 72   // 上面右边区域宽度 
#define  MIDIUM         135  // 中间高度 
#define  INFERIOR       45   // 下面高度 
#define  INFERIOR_LEFT  125  // 下面高度 
#define  INFERIOR_RIGHT 115   // 下面高度 
#define  LINE_WIDTH     3    // 线宽度 

// 等宽字体 
#define  MSYHL25        25 
#define  NI7SEG45       45 

// 板载小灯 
#define  LED1           12  
#define  LED2           13

// 配置wifi页面 
String FrontHtml = "<!DOCTYPE html><head><meta charset='UTF-8'>"\
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<title>config wifi</title>"
    "<style type='text/css'>body{background-color:rgb(225,250,250);font-size:18px;font-weight:600;}"
    ".formbox{position:relative;height:200px;width:350px;top:150px;margin:0 auto;background:#fff;border:5px solid #ccc;}"
    ".lineblock{display:block;}"\
    ".middle{max-width:400px; text-align:center;}"\
    ".inputbox{display:inline-block;margin:10px 20px;height:60px;text-align:left;}"\
    ".ssidinput{margin:15px 0px;}"\
    ".submitbtn{margin:-30px auto;}"\
    ".connectBtn{width:50%;height:40px;border-radius:5px;background:#4ad0f1;font-size:20px;font-weight:600;}</style>"\
    "<script>function SelectWifi(){var select=document.getElementById('SelectSSID');var tsw=document.getElementById('tsw');if (select.options[select.selectedIndex].value=='Other'){tsw.style.display='inline-block';}else{tsw.style.display='none';}}</script>"\
    "</head><body onload='SelectWifi();'><div class='formbox'>"\
    "<form name='input' action='/' method='POST'><div class='lineblock middle inputbox'>"\
    "<div class=''>选择Wifi: <select id='SelectSSID' name='SelectSSID' onchange='SelectWifi();'>";

String AfterHtml = "<option value='Other'>隐藏的网络</option></select></div>"\
    "<div id='tsw' class='ssidinput'>SSID: <input id='wifiname' type='text' name='ssid'>"\
    "</div></div><div class='lineblock middle inputbox'>密码: <input type='text' name='password'>"\
    "</div><div class='lineblock middle submitbtn'><input class='connectBtn' type='submit' value='连接'>"\
    "</div></form></div></body></html>";

#endif // _USER_CONDIG_ 