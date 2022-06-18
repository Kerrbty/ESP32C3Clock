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
            
            sprintf(gFormat_Str, "%s°C", WeatherData.temperature);
            // 整个过程加载字体 
            tft.loadFont(msyhl25); 
            tft.setTextColor(TFT_BLACK, background_color, true);
            // 定位 
            tft.drawString(parea, 5, 10);
            // 天气(不定长) 
            tft.drawString(WeatherData.weather, 5, 40);
            // 温度 
            tft.drawString(gFormat_Str, SUPERIOR_LEFT+LINE_WIDTH+10, 24);
            // 星期(3个字符) 
            tft.drawString(WeatherData.week, 143, SUPERIOR+MIDIUM+LINE_WIDTH*2+15);
            // 风向(不定长) 
            int nWindLen = GetUtf8LetterNumber(WeatherData.wind.c_str(), WeatherData.wind.length());
            tft.drawString(WeatherData.wind, TFT_WIDTH-(MSYHL25*nWindLen+5), SUPERIOR+LINE_WIDTH+MIDIUM-50);
            // 湿度(3个字符) 
            tft.drawString(WeatherData.humidity, TFT_WIDTH-(MSYHL25*2+5), SUPERIOR+LINE_WIDTH+MIDIUM-26);
            // 显示空气质量(1个字符) 
            tft.fillCircle(aqi_r+left_space, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*2-2, aqi_r, color);
            tft.fillRect(aqi_r+left_space, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*3-2, len, aqi_r*2, color);
            tft.fillCircle(aqi_r+len, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*2-2, aqi_r, color);
            tft.setTextColor(TFT_BLACK, color, true);
            tft.drawString(chdisplay, left_space+(len+aqi_r*2-MSYHL25)/2, SUPERIOR+LINE_WIDTH+MIDIUM-aqi_r*3+1);
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

void setup() {
    Serial.begin(115200);

    // 初始化tft屏幕240*240 
    tft.init();

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
    delay(1000);

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

    // 更新时间 
    ShowTime();
}
