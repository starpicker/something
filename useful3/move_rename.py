import os
import uuid
import shutil

filter=[".jpg", ".jpeg"] #设置过滤后的文件类型 当然可以设置多个类型

def all_path(dirname):

    result = []#所有的文件

    for maindir, subdir, file_name_list in os.walk(dirname):

        # print("1:",maindir) #当前主目录
        # print("2:",subdir) #当前主目录下的所有目录
        # print("3:",file_name_list)  #当前主目录下的所有文件

        for filename in file_name_list:
            apath = os.path.join(maindir, filename)#合并成一个完整路径
            ext = os.path.splitext(apath)[1]  # 获取文件后缀 [0]获取的是除了文件名以外的内容

            if ext in filter:
                result.append(apath)

    return result

#print(all_path("."))
count = 100
'''
for i in all_path("."):
	filename = i.split("\\")
	count += 1
	filename, extname = os.path.splitext(filename[-1])
	print(filename + str(count) + extname)
'''
target_dirname = ".\\20200305"
if not os.path.exists(target_dirname):  # 判断文件夹是否存在，如果不存在：
	os.mkdir(target_dirname)  # 创建一个文件夹

l = all_path(".")
for i in l:
	filename = i.split("\\")
	count += 1
	filename, extname = os.path.splitext(filename[-1])
	try:
		print("move", i)
		shutil.move(i, target_dirname+filename + str(count) + extname)
	except:
		continue
