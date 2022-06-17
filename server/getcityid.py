#! /usr/bin/env python
# -*- coding:utf-8 -*-
import requests
import re
import os
import time
import json
from qqwry import get_location_by_ip

http_headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36',
    "Referer" : "http://www.weather.com.cn/",
}
city_list_url = "https://j.i8tq.com/weather2020/search/city.js"

cached_file_name = 'city.js'

def find_cityid_in_json(data, addr):
    # find city code 
    find_city_id = ""
    find_city_name = ""
    try:
        ctjson = json.loads(data)
        for province in ctjson.keys():
            if province in addr:
                for city in ctjson[province].keys():
                    if city in addr:
                        for area in ctjson[province][city].keys():
                            if area in addr:
                                find_city_id = ctjson[province][city][area]['AREAID']
                                find_city_name = ctjson[province][city][area]['NAMECN']
                                break
                            else:
                                # 以防止一个都找不到情况 
                                find_city_id = ctjson[province][city][area]['AREAID']
    except:
        pass
    return find_city_id

def get_citylist():
    # get city list 
    city_json = ""
    http = requests.session()
    http.keep_alive = False
    r = http.get(city_list_url, timeout=3, headers=http_headers)
    if r.status_code == 200:
        r.encoding = "utf-8"
        city_json = r.text
    else:
        if os.path.exists(cached_file_name):
            with open(cached_file_name, 'rb') as fp:
                city_json = fp.read().decode('utf-8')
    index = city_json.find('=')
    if index != -1:
        index = index + 1
    return city_json[index:]

def get_cityid_by_ip(ip):
    # ip to addr 
    analyze_addr = get_location_by_ip(ip)
    jsondata = get_citylist()
    return (analyze_addr, find_cityid_in_json(jsondata, analyze_addr))
    
if __name__ == '__main__':
    ip = '115.196.137.252'
    print(get_cityid_by_ip(ip))