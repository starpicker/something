#!/usr/bin/python

import sys
from PIL import Image
import os,time,shutil

"""
verison: 1.0.2.20170215
Author: ArcSoft
change log: only static the percent between different range.
"""

def get_brightness(photoPath, Module):
    """
    get the brightness of all of the photo in the photoPath, and return the dictory.
    """
    dic_photo={}

    for photo in os.listdir(photoPath):
        if photo[-3:] not in ["jpg","JPG"]:
            continue

        abs_photo_path = os.path.join(photoPath,photo)
        im = Image.open(abs_photo_path)

        if "_0.jpg" in photo or ("_1.jpg" in photo and not Module =="W+T"):        #Judge the photo type and resize or crop photo.
            im_temp = im.resize((320,240))   # resize photo to 320x240
        elif "_1.jpg"  in photo and Module =="W+T":
            box = (im.size[0]/4,im.size[1]*1/4,im.size[0]*3/4,im.size[1]*3/4)
            im_crop = im.crop(box)
            im_crop.save(abs_photo_path.replace("1.jpg","1_res.jpg"))
            im_temp = Image.open(abs_photo_path.replace("1.jpg","1_res.jpg")).resize((320,240))

        iml = im_temp.convert("YCbCr")
        w, h = iml.size
        ys,ns = 0,0

        for w0 in range(0,w):
            for  h0 in range(0,h):
				y,cb,cr = iml.getpixel((w0,h0))
				ys = ys + y
				ns = ns + 1
        ys = ys/ns

        PhotoName = photo.split("_")[1]

        print photo,ys

        dic_photo.setdefault(PhotoName, []).append(photo)
        dic_photo.setdefault(PhotoName, []).append(ys)

    return dic_photo

def get_percent(dic_photo):
    level1 = 0   # level 1 means less than [0,10%]
    level2 = 0   # level 2 means between (10%,20%]
    level3 = 0   # level 3 means more than 20%
    max_offset , offset_total=0,0

    for value in dic_photo.keys():
        offset = 0
        if len(dic_photo[value])<4:
            continue

        if(dic_photo[value][1]>dic_photo[value][3]):
            offset = float(dic_photo[value][1]-dic_photo[value][3])/dic_photo[value][3]
        else:
            offset = float(dic_photo[value][3]-dic_photo[value][1])/dic_photo[value][1]

        print value, "%.2f%%"%(offset*100)

        if max_offset < offset:
            max_offset = offset

        offset_total = offset_total+ offset

        if offset <0.1 or offset == 0.1:
            level1 += 1
        elif offset <0.2 or offset == 0.2:
            level2 += 1
        else:
            level3 += 1
    total = level1 + level2 + level3

    saveout = sys.stdout
    fcon = open('conclusion/brightness.txt', 'a')
    sys.stdout = fcon
    print "\n***********************************Conclusion******************************\n"

    print "The percent  between (0,10%%) is %.2f%%, account is %d. \n"%(float(level1)/total*100,level1)
    print "The percent  (10%% ,20%%)is %.2f%%, account is %d. \n"%(float(level2)/total*100,level2)
    print "The percent  more than 20%% is %.2f%%, account is %d\n"%(float(level3)/total*100,level3)

    print "Max offset is %.2f%%, and average offset is %.2f%%"%(max_offset*100,offset_total*100/total)

    print "\n***********************************Conclusion******************************\n"

    sys.stdout = saveout
    fcon.close()

if __name__=="__main__":
    time_start = time.clock()

    Module_class={1:"RGB+RGB", 2:"RGB+MONO",3:"W+T"}

    #index = int(argv[1])
    #photo_path = argv[2]

    index = 1
    photo_path = r"jpg" 

    print "module is %s, photo_path is %s" %(Module_class[index], photo_path)

    resDir = r"conclusion"
    os.system("mkdir -p " + resDir)

    brightness_dic = get_brightness(photo_path,Module_class[index])

    get_percent(brightness_dic)

    print "Cost total time is %f s"%(time.clock()-time_start)


