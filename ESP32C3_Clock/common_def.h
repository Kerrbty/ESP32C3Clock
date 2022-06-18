#ifndef _COMMON_DEFINE_HEADER_
#define _COMMON_DEFINE_HEADER_
#include "user_config.h"

typedef struct _weather_info{
    String area;       // IP定位地区 
    String weather;    // 天气情况 
    String temperature;  // 温度 
    String wind;       // 风向 
    String windPower;   // 风力 
    String humidity;   // 湿度 
    String aqi;        // 空气质量指数 
    String week;       // 星期几 
    uint32_t update_time;  // 网络数据更新时间(网络30分钟更新一次) 
    uint32_t last_update_time;  // 本地更新时间(本地确保最少5分钟间隔) 
}weather_info, *pweather_info;

typedef struct _weather_img_list
{
    const char* name;    // 天气名 
    const uint16_t* pic;  // 图片 
}weather_img_list, *pweather_img_list;

#endif  // _COMMON_DEFINE_HEADER_