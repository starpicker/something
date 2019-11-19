# -*- coding: UTF-8 -*-

import sys
import sqlite3
import os
import sys
import ftplib
import tempfile

##if sys.platform == 'win32':
##    dbpath = 'hos.db'
##elif sys.platform == 'linux':
##    dbpath = '/data/data/com.termux/files/home/wx/hos.db'

class myFtp:
    ftp = ftplib.FTP()
 
    def __init__(self, host, port=20):
        self.ftp.connect(host, port)
 
    def Login(self, user, passwd):
        self.ftp.login(user, passwd)
##        print(self.ftp.welcome)
 
    def DownLoadFile(self, RemoteFile, LocalFile):
        file_handler = open(LocalFile, 'wb')
##        print(file_handler)
        # self.ftp.retrbinary("RETR %s" % (RemoteFile), file_handler.write)
        self.ftp.retrbinary('RETR ' + RemoteFile, file_handler.write)
        file_handler.close()
        return True
 
    def DownLoadFileTree(self, LocalDir, RemoteDir):
##        print("remoteDir:", RemoteDir)
        if not os.path.exists(LocalDir):
            os.makedirs(LocalDir)
        self.ftp.cwd(RemoteDir)
        RemoteNames = self.ftp.nlst()
##        print("RemoteNames", RemoteNames)
        for file in RemoteNames:
            Local = os.path.join(LocalDir, file)
##            print(self.ftp.nlst(file))
            if file.find(".") == -1:
                if not os.path.exists(Local):
                    os.makedirs(Local)
                self.DownLoadFileTree(Local, file)
            else:
                self.DownLoadFile(Local, file)
        self.ftp.cwd("..")
        return
 
    def close(self):
        self.ftp.quit()

        
tmpfd, tempfilename = tempfile.mkstemp()
os.close(tmpfd)
        
ftp = myFtp('0.0.0.0', 3001)
ftp.Login('administrator', 'xxxxxxxx')
ftp.DownLoadFile('xxxxx/x/xxx.xx', tempfilename)
        
ftp.close()


conn = sqlite3.connect(tempfilename)
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
os.unlink(tempfilename)
