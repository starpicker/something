import os
import sys
import ftplib
import tempfile
 
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
 
 
if __name__ == "__main__":
    tmpfd, tempfilename = tempfile.mkstemp()
    os.close(tmpfd)
    
    ftp = myFtp('10.0.0.7', 3001)
    ftp.Login('administrator', 'abc@1234')
    ftp.DownLoadFile('malei/u/hos.db', tempfilename)
    
    ftp.close()

    os.unlink(tempfilename)
    print(tempfilename)
##    print("ok!")
