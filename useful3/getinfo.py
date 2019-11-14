import wmi
import ftplib
import tempfile
import os
import datetime
import easygui

class myFtp:
    ftp = ftplib.FTP()
    ftp.set_pasv(False)
 
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
    
    def UploadFile(self, local_file, remote_file):
        buf_size = 1024
        file_handler = open(local_file, 'rb')
        try:
            self.ftp.storbinary('STOR %s' % remote_file, file_handler, buf_size)
            self.ftp.set_debuglevel(0)
        except Exception as e:
            print('Exception is: ',e)

        file_handler.close()
     
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
        
ftp = myFtp('10.0.0.7', 3001)
ftp.Login('administrator', 'abc@1234')
infofile = 'malei/info/cmter.txt'
ftp.DownLoadFile(infofile, tempfilename)

def system_info():
    c = wmi.WMI ()
    for sys in c.Win32_OperatingSystem():
##        print("Version :\t%s" % sys.Caption)
##        print("Vernum :\t%s" % sys.BuildNumber)
##        print("Installdate :\t%s" % sys.Installdate)
        return "system: " + sys.Caption + ", " + sys.BuildNumber + ", " + sys.Installdate[0:6]
        
def bios_info():
    c = wmi.WMI ()
    bioss = []
    for bios_id in c.Win32_BIOS():
        tmpmsg = {}
        tmpmsg['Manufacturer'] = bios_id.Manufacturer.strip()
        tmpmsg['ReleaseDate'] = bios_id.ReleaseDate
        bioss.append(tmpmsg)
        return "bios: " + tmpmsg['Manufacturer'] + ", " + tmpmsg['ReleaseDate'][0:6]
##    print("Bios :\t" + str(bioss))

def memory_info():
    c = wmi.WMI ()
    cs = c.Win32_ComputerSystem()
    MemTotal = int(cs[0].TotalPhysicalMemory)/1024/1024/1024
##    print("TotalPhysicalMemory :" + '\t' + '%.2f' % MemTotal + "G")
    return "memory: " + '%.2f' % MemTotal + "G"

def disk_info():
    c = wmi.WMI ()
    for physical_disk in c.Win32_DiskDrive():
        if physical_disk.Size:
##            print(str(physical_disk.Caption) + ' :\t' +
##            str(int(int(physical_disk.Size)/1024/1024/1024)) + "G")
            return "disk: " + str(int(int(physical_disk.Size)/1024/1024/1024)) + "G"

def cpu_info():
    tmpdict = {}
    tmpdict["CpuCores"] = 0
    c = wmi.WMI ()
    for cpu in c.Win32_Processor():
        tmpdict["CpuType"] = cpu.Name
        tmpdict["AddressWidth"] = cpu.AddressWidth
        try:
            tmpdict["CpuCores"] = cpu.NumberOfCores
        except:
            tmpdict["CpuCores"] += 1
            tmpdict["CpuClock"] = cpu.MaxClockSpeed
##    print('CpuType :\t' + str(tmpdict["CpuType"]))
##    print('CpuCores :\t' + str(tmpdict["CpuCores"]))
##    print('AddressWidth :\t' + str(tmpdict["AddressWidth"]))
        return "cpu: " + str(tmpdict["CpuType"]) + ", " + str(tmpdict["CpuCores"]) + \
    " cores, " + str(tmpdict["AddressWidth"]) + " bits"

def network_info():
    c = wmi.WMI ()
    tmplist = []
    for interface in c.Win32_NetworkAdapterConfiguration (IPEnabled=1):
        tmpdict = {}
        tmpdict["Description"] = interface.Description
        tmpdict["IPAddress"] = interface.IPAddress[0]
        tmpdict["IPSubnet"] = interface.IPSubnet[0]
        tmpdict["MAC"] = interface.MACAddress
        tmplist.append(tmpdict)

        for i in tmplist:
##            print(i["Description"])
##            print('\t' + "MAC :" + '\t' + i["MAC"])
##            print('\t' + "IPAddress :" + '\t' + i["IPAddress"])
##            print('\t' + "IPSubnet :" + '\t' + i["IPSubnet"])
            return "network: " + i["IPAddress"] + ", " + i["MAC"]

if __name__ == "__main__":
    sysinfo = system_info()
    biosinfo= bios_info()
    meminfo = memory_info()
    diskinfo = disk_info()
    cpuinfo = cpu_info()
    netinfo = network_info()

    loc = easygui.enterbox(msg="楼层科室护士站/医生办（例：6楼东骨二医生办）", title="位置")
    print(str(loc) + ":\n" + sysinfo, biosinfo, meminfo, diskinfo, cpuinfo, netinfo, sep = '\n')

    
    with open(tempfilename, 'a+') as f:
        f.write("\n" + str(datetime.datetime.now()) + ": ")
        f.write(str(loc) + ": ")
        f.write(sysinfo + " ")
        f.write(biosinfo + " ")
        f.write(meminfo + " ")
        f.write(diskinfo + " ")
        f.write(cpuinfo + " ")
        f.write(netinfo + " ")

    ftp.UploadFile(tempfilename, infofile)    
    ftp.close()
    os.unlink(tempfilename)
    os.system("pause")
