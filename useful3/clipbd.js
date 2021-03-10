var pst = getClip()
var path = "/storage/emulated/0/douyin/1.txt"
files.append(path, pst)

setInterval(
   function(){
       if (currentPackage() != "com.ss.android.ugc.aweme.lite") {
            exit()
        }
       var now = getClip()
       if (now != pst){
           files.append(path, now)
           pst = now
           }
       } ,
       5000
    )
