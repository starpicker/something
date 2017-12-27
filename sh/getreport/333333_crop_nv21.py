#!/usr/bin/python
import os

size_dict = {}
size_dict["4032x3000"] = "4000x3000"
size_dict["2624x1944"] = "2592x1944"
#size_dict["4672x3520"] = "4640x3480"
#size_dict["4032x2304"] = "4032x2268"

#dir = raw_input("Please Input Your NV21 Direct : ")
dir = "nv21"
dircrop = "nv21crop"
os.system("mkdir -p " + dircrop);

fileList = os.listdir(dir)

NV21_List = []
for file in fileList:
    if file.endswith("nv21" or "NV21" ):
        NV21_List.append(file)


for file in NV21_List:
    for size in size_dict:
        width_before_crop = size.split("x")[0]
        height_before_crop = size.split("x")[1]
        width_after_crop = size_dict[size].split("x")[0]
        height_after_crop = size_dict[size].split("x")[1]
        pixel_right_side = str(int(width_before_crop) - int(width_after_crop))
        pixel_bottom_side = str(int(height_before_crop) - int(height_after_crop))

        if file.find(width_before_crop and height_before_crop) !=-1:
            new_name = (file.replace(width_before_crop,width_after_crop)).replace(height_before_crop,height_after_crop)
            #print ("crop_nv21.sh"+ os.path.join(dir,file) + " " +os.path.join(dir,new_name) + " 0 0 " + pixel_right_side + " " + pixel_bottom_side)
            print ("crop_nv21.sh "+ os.path.join(dir,file) + " " +os.path.join(dircrop,new_name) + " 0 0 " + width_after_crop + " " + height_after_crop)
            #os.system ("crop_nv21.sh"+ os.path.join(dir,file)  + " " +os.path.join(dir,new_name) + " 0 0 " + pixel_right_side + " " + pixel_bottom_side)
            os.system ("crop_nv21.sh "+ os.path.join(dir,file)  + " " +os.path.join(dircrop,new_name) + " 0 0 " + width_after_crop + " " + height_after_crop)
            print "old name is " + file
            print "new name is " + new_name + "\n"




