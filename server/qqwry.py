#!/usr/bin/env python
# coding: UTF-8

# 解析 QQWry 库

'''Python查 询纯真IP库

qqwry.dat 的格式如下:

+----------+
|  文件头  |  (8字节)
+----------+
|  记录区  |  (不定长) 
+----------+
|  索引区  |  (大小由文件头决定) 
+----------+

文件头: 4字节开始索引偏移值+4字节结尾索引偏移值

记录区:  每条IP记录格式 ==> IP地址[国家信息][地区信息]

   对于国家记录,可以有三种表示方式: 

       字符串形式(IP记录第5字节不等于0x01和0x02的情况),
       重定向模式1(第5字节为0x01),则接下来3字节为国家信息存储地的偏移值
       重定向模式(第5字节为0x02),

   对于地区记录,可以有两种表示方式:  字符串形式和重定向

   我们称IP记录的第5字节,以及所有重定向后的信息的第1字节,为 falg,依
   据 flag 值,我们有判定规则: 

      1. 0x01 时,＂国家记录/地区信息＂都为 offset 指向的新地址
      2. 0x02 时,仅＂国家记录＂为 offset 指向的新地址,＂地区信息＂在4字节后
      3. 0x01, 0x02的情况下,重定向后的信息 (新或旧) 的首字节如果为 : 
         - 0,表示无记录 (也读不到字符串的) 
         - 0x02,其后3字节整数值为新的 offset 地址

索引区:  每条索引记录格式 ==> 4字节起始IP地址 + 3字节指向IP记录的偏移值

   索引区的IP和它指向的记录区一条记录中的IP构成一个IP范围。查询信息是这个
   范围内IP的信息

术语: 

每条IP记录的信息有＂国家＂和＂地区＂两部分,但＂国家＂不一定表示＂国
家＂．我们仅使用这两名词表示信息位置．

纯真IP的问题: 

1. 地区信息中,如果最后一位字符值是 '\x96',可能表示"?"．因为QQWry的
   Windows查询程序就是这样显示的．目前(2014年8月5日纯真IP库)一共有7组
   IP段是这样．详情见 decode_str 函数．

   目前发现有这个问题的IP段有: 

   1.5.0.0         日本, 港区?
   120.138.128.0   日本 愛知県名古屋市?
   133.129.0.0     日本 株式会社大林組?
   133.131.0.0     日本 ?
   133.142.0.0     日本 株式会社三菱?
   133.196.0.0     日本 株式会社?
   133.255.0.0     日本 ?

2. 记录中其有70处记录中含"?"字符,原因可能是gbk编码表示不了这样字符,纯
   真IP库生成时就有这个问题 (也有可能是用户提交原因) .举例: 

     218.93.194.50   218.93.194.50 江苏省宿迁市沭阳县,??街娱乐中心
   221.199.154.100 221.199.154.100 内蒙古赤峰市,赤峰学院机场路?虎网吧
    221.203.111.12  221.203.111.12 辽宁省本溪市平山区,佰?网络世界)
    221.224.55.234  221.224.55.234 江苏省苏州市,浪淘沙连锁网吧鑫轮?直店
     221.233.1.137   221.233.1.137 湖北省荆州市,??网吧
   221.236.163.158 221.236.163.158 四川省内江市,?木镇顺发网吧
     221.239.1.203   221.239.1.203 天津市,依?网吧
   222.170.192.213 222.170.192.213 黑龙江省双鸭山市宝清县,新?网吧
     222.175.52.10   222.175.52.10 山东省枣庄市,薛城区?伟网苑
     222.203.146.0 222.203.147.255 广东省广州市天河区,长?小学

本命令行程序用法示例: 

查询多个IP信息: 

    python qqwry.py -q 91.121.92.177 88.198.17.4 101.1.25.145

查询 1.list, 2.list 文件中的 IP 信息: 

    python qqwry.py --query-file 1.list 2.list

输出所有IP信息到文件 /tmp/ip.txt: 

    python qqwry.py --dump -o /tmp/ip.txt


最后: 

不应完全依赖IP数据库的查询信息,原因有: 

1. IP数据库的信息收集有可能不准确
2. 为了隐藏真实信息,有些IP信息提供方会使用混淆技术

'''

import socket
import struct

skipstring = ['电信', '移动', '联通']

class CzIp:
    def __init__(self, db_file='qqwry.dat'):
        self.f_db = open(db_file, "rb")
        bs = self.f_db.read(8)
        (self.first_index, self.last_index) = struct.unpack('II', bs)
        self.index_count = int((self.last_index - self.first_index) / 7 + 1)
        self.cur_start_ip = None
        self.cur_end_ip_offset = None
        self.cur_end_ip = None
        # print(self.get_version(), " 纪录总数: %d 条 "%(self.index_count)) 

    def get_version(self):
        '''
        获取版本信息，最后一条IP记录 255.255.255.0-255.255.255.255 是版本信息
        :return: str
        '''
        s = self.get_addr_by_ip(0xffffff00)
        # print(s)
        return s

    def _get_area_addr(self, offset=0):
        if offset:
            self.f_db.seek(offset)
        bs = self.f_db.read(1)
        (byte,) = struct.unpack('B', bs)
        if byte == 0x01 or byte == 0x02:
            p = self.getLong3()
            if p:
                return self.get_offset_string(p)
            else:
                return ""
        else:
            self.f_db.seek(-1, 1)
            return self.get_offset_string(offset)

    def _get_addr(self, offset):
        '''
        获取offset处记录区地址信息(包含国家和地区)
        如果是中国ip，则是 "xx省xx市 xxxxx地区" 这样的形式
        (比如:"福建省 电信", "澳大利亚 墨尔本Goldenit有限公司")
        :param offset:
        :return:str
        '''
        self.f_db.seek(offset + 4)
        bs = self.f_db.read(1)
        (byte,) = struct.unpack('B', bs)
        if byte == 0x01:    # 重定向模式1
            country_offset = self.getLong3()
            self.f_db.seek(country_offset)
            bs = self.f_db.read(1)
            (b,) = struct.unpack('B', bs)
            if b == 0x02:
                country_addr = self.get_offset_string(self.getLong3())
                self.f_db.seek(country_offset + 4)
            else:
                country_addr = self.get_offset_string(country_offset)
            area_addr = self._get_area_addr()
        elif byte == 0x02:  # 重定向模式2
            country_addr = self.get_offset_string(self.getLong3())
            area_addr = self._get_area_addr(offset + 8)
        else:   # 字符串模式
            country_addr = self.get_offset_string(offset + 4)
            area_addr = self._get_area_addr()
        return country_addr + " " + area_addr

    def dump(self, first, last):
        '''
        打印数据库中索引为first到索引为last(不包含last)的记录
        :param first:
        :param last:
        :return:
        '''
        if last > self.index_count:
            last = self.index_count
        for index in range(first, last):
            offset = self.first_index + index * 7
            self.f_db.seek(offset)
            buf = self.f_db.read(7)
            (ip, of1, of2) = struct.unpack("IHB", buf)
            address = self._get_addr(of1 + (of2 << 16))
            print("%d %s %s" % (index, self.ip2str(ip), address))

    def _set_ip_range(self, index):
        offset = self.first_index + index * 7
        self.f_db.seek(offset)
        buf = self.f_db.read(7)
        (self.cur_start_ip, of1, of2) = struct.unpack("IHB", buf)
        self.cur_end_ip_offset = of1 + (of2 << 16)
        self.f_db.seek(self.cur_end_ip_offset)
        buf = self.f_db.read(4)
        (self.cur_end_ip,) = struct.unpack("I", buf)

    def get_addr_by_ip(self, ip):
        '''
        通过ip查找其地址
        :param ip: (int or str)
        :return: str
        '''
        if type(ip) == str:
            ip = self.str2ip(ip)
        L = 0
        R = self.index_count - 1
        while L < R - 1:
            M = int((L + R) / 2)
            self._set_ip_range(M)
            if ip == self.cur_start_ip:
                L = M
                break
            if ip > self.cur_start_ip:
                L = M
            else:
                R = M
        self._set_ip_range(L)
        # version information, 255.255.255.X, urgy but useful
        if ip & 0xffffff00 == 0xffffff00:
            self._set_ip_range(R)
        if self.cur_start_ip <= ip <= self.cur_end_ip:
            address = self._get_addr(self.cur_end_ip_offset)
        else:
            address = "未找到该IP的地址"
        return address

    def get_ip_range(self, ip):
        '''
        返回ip所在记录的IP段
        :param ip: ip(str or int)
        :return: str
        '''
        if type(ip) == str:
            ip = self.str2ip(ip)
        self.get_addr_by_ip(ip)
        range = self.ip2str(self.cur_start_ip) + ' - ' \
                + self.ip2str(self.cur_end_ip)
        return range

    def get_offset_string(self, offset=0):
        '''
        获取文件偏移处的字符串(以'\0'结尾)
        :param offset: 偏移
        :return: str
        '''
        if offset:
            self.f_db.seek(offset)
        bs = b''
        ch = self.f_db.read(1)
        (byte,) = struct.unpack('B', ch)
        while byte != 0:
            bs += ch
            ch = self.f_db.read(1)
            (byte,) = struct.unpack('B', ch)
        return bs.decode('gbk')

    def ip2str(self, ip):
        '''
        整数IP转化为IP字符串
        :param ip:
        :return:
        '''
        return str(ip >> 24) + '.' + str((ip >> 16) & 0xff) + '.' + str((ip >> 8) & 0xff) + '.' + str(ip & 0xff)

    def str2ip(self, s):
        '''
        IP字符串转换为整数IP
        :param s:
        :return:
        '''
        (ip,) = struct.unpack('I', socket.inet_aton(s))
        return ((ip >> 24) & 0xff) | ((ip & 0xff) << 24) | ((ip >> 8) & 0xff00) | ((ip & 0xff00) << 8)

    def getLong3(self, offset=0):
        '''
        3字节的数值
        :param offset:
        :return:
        '''
        if offset:
            self.f_db.seek(offset)
        bs = self.f_db.read(3)
        (a, b) = struct.unpack('HB', bs)
        return (b << 16) + a 
    
def get_location_by_ip(ipaddr):
    cz = CzIp("qqwry.dat")
    addr = cz.get_addr_by_ip(ipaddr)
    return str(addr.split(' ')[0])

if __name__ == '__main__':
    ip = '192.168.1.1'
    print(get_location_by_ip(ip))
