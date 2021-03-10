while (true) {
    if (currentPackage() != "com.ss.android.article.lite") {
            exit()
        }
    var save = className("android.widget.TextView").text("保存图片").findOne(1000)
    sleep(1000)
    if (save == null){
	continue
    }
    if (scrollDown() == false) {
        back()
    } else {
        sleep(1000)
        press(device.width / 2, device.height / 2, 1000)
    }
}
