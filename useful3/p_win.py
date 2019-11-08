# -*- coding: UTF-8 -*-

import sys
import sqlite3

dbpath = 'hos.db'

if sys.platform == 'win32':
    dbpath = 'hos.db'
elif sys.platform == 'linux':
    dbpath = '/data/data/com.termux/files/home/wx/hos.db'
    
conn = sqlite3.connect(dbpath)
c = conn.cursor()

def getphone(cursor, queryword):
    query = "select * from phone where abbr like '%"       + queryword + "%'"   + "or " \
                                    + "phone like '%"      + queryword + "%'"   + "or " \
                                    + "location like '%"   + queryword + "%'"   + "or " \
                                    + "district like '%"   + queryword + "%'"
    c = cursor.execute(query)
    tplt = "{0:<10}{1:{5}<8}\t{2:<10}\t{3:{5}<5}\t{4:{5}<10}"
    
    for row in c:
        m = ""
        if queryword in row[3] and "(" not in row[3] and row[5] == "西区":
            l = row[3].split(' ')
            if len(l) >= 2 and l[1] != "0":
                if queryword in l[0]:
                    m = " 医生办"
                elif queryword in l[1]:
                    m = " 护士办"
        print(tplt.format(row[1], row[2]+m, row[3], row[4], row[5], chr(12288)))

if (len(sys.argv) > 1):
    getphone(c, sys.argv[1])

else:
    while (True):
        queryinput = input("请输入科室拼音简写或者电话号码：")
        if(queryinput == ''):
            break
        getphone(c, queryinput)
    
conn.close()
