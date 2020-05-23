while (true) {
    className("android.widget.TextView").text("保存图片").findOne().click()
    sleep(1000)
    if (scrollDown() == false) {
        back()
    } else {
        sleep(1000)
        press(device.width / 2, device.height / 2, 1000)
    }
}