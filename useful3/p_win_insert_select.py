# -*- coding: UTF-8 -*-

import sys
import sqlite3

dbpath = 'hos.db'

conn = sqlite3.connect(dbpath)
cursor = conn.cursor()

## select
##query = "select * from phone"
##c = cursor.execute(query)
##for row in c:
##    print(row)


## insert delete update
##cursor.execute("insert into phone (abbr, duty, phone, location, district) \
##      values ('gyt', '国医堂', '2827107', '三楼', '西区')");
##cursor.execute("insert into phone (abbr, duty, phone, location, district) \
##      values ('hl', '护理公司', '3903270', '一楼', '西区')");
##cursor.execute("insert into phone (abbr, duty, phone, location, district) \
##      values ('wy', '物业公司', '3903265', '一楼', '北区')");
##cursor.execute("insert into phone (abbr, duty, phone, location, district) \
##        values ('bwk', '保卫科', '2826100', '一楼', '西区')");

##
##cursor.execute("delete from phone where abbr='bwk' and phone='2826100'")
##cursor.execute("delete from phone where abbr='gyt'")
##cursor.execute("update phone set phone = '2826019 2826039 682153' where abbr='ebhk'")
##cursor.execute("update phone set location = '门诊2楼' where abbr='nk'")
##cursor.execute("update phone set abbr = 'sss' where abbr='szs'")
##cursor.execute("update phone set abbr = 'bqsss' where abbr='bqszs'")

##cursor.execute("alter table phone add column id int auto_increment");


conn.commit()



conn.close()


##CREATE TABLE temp(id integer primary key autoincrement, abbr char(50) not null,\
                    ##duty char(50), phone char(50), location char(50), district char(15) not null);  

##insert into temp select null, * from phone;
##drop table phone;
##alter table temp rename to phone;

