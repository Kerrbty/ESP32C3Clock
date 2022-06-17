#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os
from flask import Flask, request, Response
from weather import get_weather_by_ip


app = Flask(__name__)
app.config['FLASK_DEBUG'] = True


@app.route('/')
def hello_world():
    return 'Hello World!'

# 根据IP地理位置获取天气 
@app.route('/getweather')
def get_weathrer():
    if request.headers.has_key('X-Forwarded-For'):
        return get_weather_by_ip(str(request.headers['X-Forwarded-For']))
    return 'ip error'

# 获取ip地址 
@app.route('/getip')
def get_ip():
    if request.headers.has_key('X-Forwarded-For'):
        return str(request.headers['X-Forwarded-For'])
    return 'ip error'

if __name__ == '__main__':
    app.run(host='0.0.0.0', port='5001', debug=True)
