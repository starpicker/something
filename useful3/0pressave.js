while (true) {
    var save = className("android.widget.TextView").text("保存图片").findOne(1000)

    if (save == null) {
        continue
    }

    save.click()
    sleep(50)
    if (scrollDown() == false) {
        sleep(100)
        var val = className("android.widget.ImageView").clickable(true).selected(true).findOne(500)
        if (val != null) {
            val.click()
            sleep(50)
            back()
            sleep(10)
        } else {
            back()
        }
    } else {
        sleep(1000)
        press(device.width / 2, device.height / 2, 1000)
    }
}
