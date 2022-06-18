#include "Ni7seg45.h"
#include "msyhl25.h"
#include <time.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "common_def.h"

HTTPClient http;
TFT_eSPI tft = TFT_eSPI();
weather_info WeatherData;
char gFormat_Str[64] = {0};
DynamicJsonDocument doc(1024);
uint32_t background_color;
uint32_t line_color;

void UpdateWeather()
{
    http.begin(WEATHER_URL);
    int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK) {
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

            const char* parea = WeatherData.area.c_str();
            const char* pcity = strchr(parea, '省');

            // 计算空气质量显示数据 
            int r = 13;
            uint32_t color = TFT_GREEN;
            int len = 40;
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
            
            sprintf(gFormat_Str, "%s°C", WeatherData.temperature);
            // 整个过程加载字体 
            tft.loadFont(msyhl25); 
            tft.setTextColor(TFT_BLACK, background_color, true);
            tft.drawString(gFormat_Str, SUPERIOR_LEFT+LINE_WIDTH+10, 24);
            tft.drawString(WeatherData.week, 143, SUPERIOR+MIDIUM+LINE_WIDTH*2+15);
            if (pcity)
            {
                tft.drawString(pcity+1, 5, 10);
            }
            else
            {
                tft.drawString(parea, 5, 10);
            }
            tft.drawString(WeatherData.weather, 5, 40);
            tft.drawString(WeatherData.week, 143, SUPERIOR+MIDIUM+LINE_WIDTH*2+15);
            tft.drawString(WeatherData.wind, TFT_WIDTH-80, SUPERIOR+LINE_WIDTH+MIDIUM-50);
            tft.drawString(WeatherData.humidity, TFT_WIDTH-55, SUPERIOR+LINE_WIDTH+MIDIUM-26);
            // 显示空气质量 
            tft.fillCircle(r+3, SUPERIOR+LINE_WIDTH+MIDIUM-r*2-2, r, color);
            tft.fillRect(r+3, SUPERIOR+LINE_WIDTH+MIDIUM-r*3-2, len, r*2, color);
            tft.fillCircle(r+len, SUPERIOR+LINE_WIDTH+MIDIUM-r*2-2, r, color);
            tft.setTextColor(TFT_BLACK, color, true);
            tft.drawString(chdisplay, r+9, SUPERIOR+LINE_WIDTH+MIDIUM-r*3+1);
            // 卸载字体 
            tft.unloadFont();

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
        tft.drawString(gFormat_Str, 8, SUPERIOR+MIDIUM+LINE_WIDTH*2+15);
        tft.unloadFont();
    }

    if (force_update || gmt->tm_sec == 0)
    {
        // 更新：时-分 
        sprintf(gFormat_Str, "%02u:%02u", gmt->tm_hour, gmt->tm_min);
        tft.loadFont(Ni7seg45);
        tft.setTextColor(TFT_BLACK, background_color, true);
        tft.drawString(gFormat_Str, 5, SUPERIOR+LINE_WIDTH+12);
        tft.unloadFont();
    }

    // 更新：秒 
    sprintf(gFormat_Str, "%02u", gmt->tm_sec);
    tft.loadFont(msyhl25);
    tft.setTextColor(TFT_BLACK, background_color, true);
    tft.drawString(gFormat_Str, SUPERIOR+85, SUPERIOR+LINE_WIDTH+31);
    tft.unloadFont();
}

void create_partition()
{
    background_color = tft.color565(195, 230, 245);
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

void setup() {
    Serial.begin(115200);

    // 初始化tft屏幕240*240  
    tft.init();
    tft.fillScreen(TFT_BLACK);

    // 连接wifi 
    WiFi.begin(WIFI_SSID, WIFI_PASSWD);

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

    // 服务器获取根据IP获取本地地理位置及时区 
    delay(800);
    long gmtOffset_sec = TIME_ZONE*60*60;
    int daylightOffset_sec = 0;

    // 配置本地时间 
    configTime(gmtOffset_sec, daylightOffset_sec, NTP_HOST);
    delay(4000);

    // 屏幕分区 
    create_partition();

    // 获取天气数据 
    UpdateWeather();

    // 显示时间 
    ShowTime(true);
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(1000);

    // 查看是否需要更新天气 
    uint32_t ltime = time(NULL);
    if ( ltime > (WeatherData.last_update_time+LOCAL_HTTP_MIN*60) )
    {
        if ( ltime > (WeatherData.last_update_time+NETWOEK_WEATHER_MIN*60) )
        {
            UpdateWeather();
            // 重新显示天气信息 
        }
    }

    ShowTime();
}
