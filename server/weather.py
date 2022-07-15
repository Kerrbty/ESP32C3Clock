#! /usr/bin/env python
# -*- coding:utf-8 -*-
import requests
import re
import os
import time
import json
import datetime
from getcityid import get_cityid_by_ip

http_headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36',
    'Referer': "http://www.weather.com.cn/",
}

week_list = ["星期一","星期二","星期三","星期四","星期五","星期六","星期日"]

weather_url = "http://d1.weather.com.cn/weather_index/{0}.html?_={1}"

# 日志打印时间
def logger(*nums):
    print(time.strftime("[%Y-%m-%d][%H:%M:%S] ", time.localtime()), end="")
    for argv in nums:
        print(argv, end=" ")
    print()
    sys.stdout.flush()

# 获取网页文本
def _get_html(url):
    try:
        # 获取网页
        http = requests.session()
        http.keep_alive = False
        r = http.get(url, timeout=3, headers=http_headers)
        # 设置网页编码
        if re.search('charset="gb2312"', r.text) or re.search('charset=gb2312', r.text):
            r.encoding = "gb2312"
        elif re.search('charset="gbk"', r.text) or re.search('charset=gbk', r.text):
            r.encoding = "gbk"
        elif re.search('charset="utf-8"', r.text) or re.search('charset=utf-8', r.text):
            r.encoding = "utf-8"
        return r.text
    except:
        return None

def get_html(url, retry = 3):
    times = 0
    html = None
    while times < retry:
        html = _get_html(url)
        if html:
            break
        times = times+1
    return html

# 获取网页文本
def post_data(url, data):
    try:
        # 获取网页
        http = requests.session()
        http.keep_alive = False
        r = http.post(url, timeout=3, headers=http_headers, data=data)
        # 设置网页编码
        if re.search('charset="gb2312"', r.text) or re.search('charset=gb2312', r.text):
            r.encoding = "gb2312"
        elif re.search('charset="gbk"', r.text) or re.search('charset=gbk', r.text):
            r.encoding = "gbk"
        elif re.search('charset="utf-8"', r.text) or re.search('charset=utf-8', r.text):
            r.encoding = "utf-8"
        # 返回页面数据
        return r.text
    except:
        return None

# 获取远程二进制数据(下载文件等)
def get_content(url):
    try:
        http = requests.session()
        http.keep_alive = False
        data = http.get(url, timeout=3, headers=http_headers)
        return data.content
    except:
        return None

# 下载大文件
def download_file(url, file):
    try:
        # 获取网页
        http = requests.session()
        http.keep_alive = False
        with http.get(url, headers=http_headers, stream=True) as rb:
            contentLength = int(rb.headers['content-length'])
            downSize = 0
            with open(file, 'wb') as fp:
                for chunk in rb.iter_content(8192):
                    if chunk:
                        fp.write(chunk)
                        downSize += len(chunk)
                    if downSize >= contentLength:
                        break
        return True
    except:
        return False

def get_weather_json(id):
    # 生成URL访问 
    current_time = int(round(time.time() * 1000))
    request_url = weather_url.format(str(id), str(current_time))

    # 访问数据 
    http = requests.session()
    http.keep_alive = False
    r = http.get(request_url, timeout=3, headers=http_headers)
    r.encoding = "utf-8"
    return r.text

def get_weather_by_ip(ip):
    data = {}

    # 获取区域，id 
    (area, id) = get_cityid_by_ip(ip)

    # 获取区域天气数据 
    cvdata = get_weather_json(id)

    # 解析数据，拼凑返回数据 
    index = cvdata.find('dataSK')
    if index != -1:
        current_weather = cvdata[index:]
        fi = current_weather.find('=')
        li = current_weather.find(';')
        if fi != -1 and li != -1:
            try:
                jvweather = json.loads(current_weather[fi+1:li])
                data['temp'] = jvweather['temp']  # 气温 
                data['weather'] = jvweather['weather']  # 天气情况 
                data['WD'] = jvweather['WD']  # 风向 
                data['WS'] = jvweather['WS']  # 风力 
                data['SD'] = jvweather['SD']  # 相对湿度 
                data['aqi'] = jvweather['aqi']  # 空气质量指数 
                data['area'] = area  # 地区
                data['date'] = str(datetime.datetime.now().strftime('%Y-%m-%d')) # 时间(年月日星期) 
                data['weekday'] = week_list[datetime.datetime.now().weekday()]
                dt = data['date'] + ' ' + jvweather['time'] + ':00'
                timeArray = time.strptime(dt, "%Y-%m-%d %H:%M:%S")
                data['updatetime'] =  int(time.mktime(timeArray)) # 更新时间 
                # 00点交接会发生差一天的情况 
                if data['updatetime'] > int(time.time()) :
                    data['updatetime'] = data['updatetime'] - 24*60*60
            except:
                pass
    return json.dumps(data)

if __name__== "__main__":
    ip = '115.196.137.252'
    print(get_weather_by_ip(ip))