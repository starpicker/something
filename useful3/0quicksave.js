while (true) {
    while (click("保存")) {
        if (scrollDown() == false) {
            back()
            sleep(100)
            var val = className("android.widget.ImageView").clickable(true).selected(true).findOne(500)
            if (val != null) {
                val.click()
                sleep(50)
                back()
                sleep(10)
            } else {
                back()
                sleep(10)
            }
        }
        sleep(100)
    }   }