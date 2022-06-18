#include "./font/Ni7seg45.h"
#include "./font/msyhl25.h"
#include <time.h>
#include <WiFi.h>
#include <HTTPClient.h>
// tft 驱动库 
#include <TFT_eSPI.h>
// json解析库 
#include <ArduinoJson.h>
// 太空人图片  
#include "./img/taikonaut/i0.h" 
#include "./img/taikonaut/i1.h" 
#include "./img/taikonaut/i2.h" 
#include "./img/taikonaut/i3.h" 
#include "./img/taikonaut/i4.h" 
#include "./img/taikonaut/i5.h" 
#include "./img/taikonaut/i6.h" 
#include "./img/taikonaut/i7.h" 
#include "./img/taikonaut/i8.h" 
#include "./img/taikonaut/i9.h" 
// 湿度
#include "./img/humidity.h"
// 温度 
#include "./img/temperature.h"
// 天气
#include "./img/weather/sunny.h"      // 晴朗 
#include "./img/weather/overcast.h"   // 阴天 
#include "./img/weather/rain.h"      // 雨天 
#include "./img/weather/snow.h"      // 下雪 
#include "./img/weather/thunder.h"   // 打雷 
#include "./img/weather/cloudy.h"    // 多云 

#include "common_def.h"

HTTPClient http;
TFT_eSPI tft = TFT_eSPI();
weather_info WeatherData;
char gFormat_Str[64] = {0};
DynamicJsonDocument doc(1024);
uint32_t background_color;
uint32_t line_color;
const uint16_t* Taikonaut[] = { 
    gImage_i0, gImage_i1, gImage_i2, gImage_i3, gImage_i4, 
    gImage_i5, gImage_i6, gImage_i7, gImage_i8, gImage_i9
};

weather_img_list weather_img_data[] = {
    {"晴", gImage_sunny},
    {"阴", gImage_overcast},
    {"雨", gImage_rain},
    {"雪", gImage_snow},
    {"雷", gImage_thunder},
    {"云", gImage_cloudy},
};


// 统计UTF-8字符数 
int GetUtf8LetterNumber(const char *s, size_t len)
{
    if (s == NULL || len == 0)
    {
        return 0;
    }
    
    int nCount = 0;
    for (size_t i=0; i<len; i++)
    {
    	//if ((s[i] & 0b11000000) != 0b10000000) j++;
        if ((s[i] & 0xc0) != 0x80) 
        {
            nCount++;
        }
    }
    return nCount;
}

void ShowWeather()
{
    // 定位太长了，截断 
    const char* parea = WeatherData.area.c_str();
    if (GetUtf8LetterNumber(parea, WeatherData.area.length()) > 6)
    {
        const char* pcity = strchr(parea, '省');
        if (pcity)
        {
            parea = pcity + 1;
            if (GetUtf8LetterNumber(parea, strlen(parea)) > 6)
            {
                const char* pcity = strchr(parea, '市');
                if (pcity)
                {
                    parea = pcity + 1;
                } 
            }
        }                
    }

    // 计算空气质量显示数据 
    int left_space = 3;
    int aqi_r = 12;
    uint32_t color = TFT_GREEN;
    int len = 30;
    const char* chdisplay;
    uint32_t aqi = atoi(WeatherData.aqi.c_str());
    if (aqi < 50) {
        color = TFT_GREEN;
        chdisplay = "优";
    } else if (aqi<99) {
        color = TFT_YELLOW;
        chdisplay = "良";
    } else {
        color = TFT_RED;
        chdisplay = "差";
    }
    // 空气质量左边距 
    int aqi_left = 0;
    switch (WeatherData.aqi.length())
    {
    case 1:
        aqi_left = left_space+aqi_r+(aqi_r/2)-2;
        break;
    case 2:
        aqi_left = left_space+aqi_r-2;
        break;
    default:
        aqi_left = left_space+aqi_r/2-2;
        break;
    }

    // 天气图片 
    const uint16_t* PWeatherPic = gImage_sunny;
    for (int i = 0; i < sizeof(weather_img_data)/sizeof(weather_img_data[0]); i++)
    {
        if (strstr(WeatherData.weather.c_str(), weather_img_data[i].name) != NULL)
        {
            PWeatherPic = weather_img_data[i].pic;
            break;
        }
    }
    int weather_len = GetUtf8LetterNumber(WeatherData.weather.c_str(), WeatherData.weather.length());
    
    sprintf(gFormat_Str, "%s°C", WeatherData.temperature);
    // 整个过程加载字体 
    tft.loadFont(msyhl25); 
    tft.setTextColor(TFT_BLACK, background_color, true);
    // 定位 
    tft.drawString(parea, 4, 5);
    // 天气(不定长) 
    tft.drawString(WeatherData.weather, 5, 34);
    tft.pushImage(MSYHL25*weather_len+10, 30, 24, 24, PWeatherPic);
    // 温度 
    tft.pushImage(SUPERIOR_LEFT+LINE_WIDTH, 16, 24, 24, gImage_temp); 
    tft.drawString(gFormat_Str, SUPERIOR_LEFT+LINE_WIDTH+25, 20);
    // 星期(3个字符) 
    tft.drawString(WeatherData.week, 143, SUPERIOR+MIDIUM+LINE_WIDTH*2+10);
    // 风向(不定长) 
    int nWindLen = GetUtf8LetterNumber(WeatherData.wind.c_str(), WeatherData.wind.length());
    tft.drawString(WeatherData.wind, TFT_WIDTH-(MSYHL25*nWindLen+5), SUPERIOR+LINE_WIDTH+MIDIUM-49);
    // 湿度(3个字符) 
    tft.pushImage(TFT_WIDTH-(MSYHL25*2+33), SUPERIOR+LINE_WIDTH+MIDIUM-29, 24, 24, gImage_SD); 
    tft.drawString(WeatherData.humidity, TFT_WIDTH-(MSYHL25*2+5), SUPERIOR+LINE_WIDTH+MIDIUM-26);
    // 显示空气质量(1个字符) 
    tft.drawString(WeatherData.aqi, aqi_left, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*3-MSYHL25+2);
    tft.fillCircle(aqi_r+left_space, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*2-2, aqi_r, color);
    tft.fillRect(aqi_r+left_space, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*3-2, len, aqi_r*2, color);
    tft.fillCircle(aqi_r+len, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*2-2, aqi_r, color);
    tft.setTextColor(TFT_BLACK, color, true);
    tft.drawString(chdisplay, left_space+(len+aqi_r*2-MSYHL25)/2, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*3+1);
    // 卸载字体 
    tft.unloadFont();
}

void UpdateWeather()
{
    http.begin(WEATHER_URL);
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK) 
    {
        // 获取json数据 
        String pageData = http.getString();
        DeserializationError error = deserializeJson(doc, pageData);
        if (!error)
        {
            JsonObject obj = doc.as<JsonObject>();

            // 最新数据放入结构体中保存 
            WeatherData.temperature = obj["temp"].as<String>();
            WeatherData.weather = obj["weather"].as<String>();
            WeatherData.wind = obj["WD"].as<String>();
            WeatherData.windPower = obj["WS"].as<String>();
            WeatherData.humidity = obj["SD"].as<String>();
            WeatherData.aqi = obj["aqi"].as<String>();
            WeatherData.area = obj["area"].as<String>();
            WeatherData.week = obj["weekday"].as<String>();
            WeatherData.update_time = obj["updatetime"].as<uint32_t>();
            WeatherData.last_update_time = time(NULL);

            ShowWeather();

            // 调试日志 
            Serial.print("城市: ");
            Serial.println(WeatherData.area);
            Serial.print("天气: ");
            Serial.println(WeatherData.weather);
            Serial.print("气温: ");
            Serial.println(WeatherData.temperature);
        }
        else
        {
            Serial.println("Json Data analyze Error");
        }
    }
    else
    {
        Serial.println("GET Weather Error");
    }
    http.end();
}

void ShowTime(bool force_update = false)
{
    // 显示时间，画动画 
    time_t ltime = time(NULL);
    struct tm *gmt = localtime( &ltime );

    // 0时0秒到2秒更新日期 
    if (force_update || (gmt->tm_hour == 0 && gmt->tm_min == 0 && gmt->tm_sec<2) )
    {
        sprintf(gFormat_Str, "%02u月%02u日", 
            gmt->tm_mon+1, gmt->tm_mday);
        tft.loadFont(msyhl25);
        tft.setTextColor(TFT_BLACK, background_color, true);
        tft.drawString(gFormat_Str, 8, SUPERIOR+MIDIUM+LINE_WIDTH*2+10);
        tft.unloadFont();
    }

    if (force_update || gmt->tm_sec == 0)
    {
        // 更新：时-分 
        sprintf(gFormat_Str, "%02u:%02u", gmt->tm_hour, gmt->tm_min);
        tft.loadFont(Ni7seg45);
        tft.setTextColor(TFT_BLACK, background_color, true);
        tft.drawString(gFormat_Str, 9, SUPERIOR+LINE_WIDTH+12);
        tft.unloadFont();
    }

    // 更新：秒 
    sprintf(gFormat_Str, "%02u", gmt->tm_sec);
    tft.loadFont(msyhl25);
    tft.setTextColor(TFT_BLACK, background_color, true);
    tft.drawString(gFormat_Str, NI7SEG45*3+20, SUPERIOR+LINE_WIDTH+NI7SEG45-15);
    tft.unloadFont();
}

void create_partition()
{
    background_color = TFT_WHITE; // tft.color565(195, 230, 245);
    line_color = tft.color565(180, 180, 180);

    tft.fillScreen(background_color);
    // 上中下区域划分  
    tft.fillRect(0, SUPERIOR, TFT_WIDTH, LINE_WIDTH, line_color);
    tft.fillRect(0, SUPERIOR+MIDIUM, TFT_WIDTH, LINE_WIDTH, line_color);
    // 顶部左右区域划分 
    tft.fillRect(SUPERIOR_LEFT, 0, LINE_WIDTH, SUPERIOR, line_color); 
    // 底部左右区域划分 
    tft.fillRect(INFERIOR_LEFT, SUPERIOR+MIDIUM, LINE_WIDTH, INFERIOR, line_color);
}

void ConnectWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);

    tft.fillScreen(TFT_BLACK);
    tft.loadFont(msyhl25);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.drawString("正在连接网络...", 10, 60);
    tft.unloadFont();

    // 进度条 
    int index = 0;
    int x1 = 0;
    int y1 = 100;
    int height = 30;
    int r = height/2;
    tft.fillCircle(x1+r, y1+r, r, TFT_GREEN);
    while (WiFi.status() != WL_CONNECTED) {
        index++;
        delay(100);
        Serial.print(".");
        // 圆角进度条 
        int width = index*24/4;
        tft.fillRect(x1+r, y1, width, height, TFT_GREEN);
        tft.fillCircle(width+r, y1+r, r, TFT_GREEN);
    }
    Serial.print("\nWiFi connected, IP address:");
    Serial.println(WiFi.localIP());
    delay(500);
}

void SyncSystemTime()
{
    long gmtOffset_sec = TIME_ZONE*60*60;
    int daylightOffset_sec = 0;
    int index = 0;
    int x1 = 0;
    int y1 = 100;
    int height = 30;
    int r = height/2;
    uint32_t ltime;

    // 显示进度条 
    tft.fillScreen(TFT_BLACK);
    tft.loadFont(msyhl25);
    tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
    tft.drawString("正在同步时间...", 10, 60);
    tft.unloadFont();
    do {
        configTime(gmtOffset_sec, daylightOffset_sec, NTP_HOST);
        tft.fillRect(x1, y1, 240, height+2, TFT_BLACK);  // 进度条重新刷黑 
        for (int i = 0; i < 31; i++)
        {
            int width = 7*i;
            tft.fillRect(x1+r, y1, width, height, TFT_GREEN);
            tft.fillCircle(width+r, y1+r, r, TFT_GREEN);
            delay(150);
        }
        delay(500);
        ltime = time(NULL);
    } while (ltime<24*60*60);
}

void ShowTaikonaut()
{
    static uint8_t index = 0;
    tft.pushImage(65, SUPERIOR+MIDIUM+LINE_WIDTH-75, 70, 70, Taikonaut[index]); 
    index++;
    if (index >= sizeof(Taikonaut)/sizeof(Taikonaut[0]))
    {
        index = 0;
    }
}

void test()
{
    // 屏幕分区 
    create_partition();

    WeatherData.temperature = "33";
    WeatherData.weather = "多云";
    WeatherData.wind = "东南风";
    WeatherData.windPower = "2级",
    WeatherData.humidity = "51%";
    WeatherData.aqi = "80";
    WeatherData.area = "浙江省杭州市";
    WeatherData.week = "星期六";
    WeatherData.update_time = time(NULL);
    WeatherData.last_update_time = time(NULL);
    ShowWeather();

    // 显示时间 
    ShowTime(true);
    while(true)
    {
        ShowTaikonaut();
        delay(150); 
    }

    while(true)
    {
        delay(1000);
    }
}

void setup() {
    Serial.begin(115200);

    // 初始化tft屏幕240*240 
    tft.init();

//    test();

    // 连接wifi 
    ConnectWifi();

    // 同步时间 
    SyncSystemTime();

    // 屏幕分区 
    create_partition();

    // 获取天气数据 
    UpdateWeather();

    // 显示时间 
    ShowTime(true);
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(150);
    ShowTaikonaut();

    // 查看是否需要更新天气 
    uint32_t ltime = time(NULL);
    if ( ltime > (WeatherData.last_update_time+LOCAL_HTTP_MIN*60) )
    {
        if ( ltime > (WeatherData.last_update_time+NETWOEK_WEATHER_MIN*60) )
        {
            // 重新显示天气信息 
            UpdateWeather();  
        }
    }
    delay(150);
    ShowTaikonaut();

    // 更新时间 
    ShowTime();
    delay(150);
    ShowTaikonaut();
}
