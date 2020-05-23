while (true) {
    WidthOne = random(300, 800);
    HeightOne = random(1600, 1800);
    WidthTwo = random(300, 800);
    HeightTwo = random(500, 700);
    timeGo = random(500, 1000);
    timeNext = random(4000, 7000);
    swipe(WidthOne, HeightOne, WidthTwo, HeightTwo, timeGo);
    sleep(timeNext);
}
