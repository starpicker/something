"auto";

events.observeKey();

var task = task1;
var extu = null
var extu2 = null
events.onKeyDown("volume_down", function(event) {
    if (extu) {
        extu.getEngine().forceStop()
    }
    if (extu2) {
        extu2.getEngine().forceStop()
    }
    if (task == task1) {
        task = task2;
    } else if (task == task2) {
        task = task3;
    } else if (task == task3) {
        task = task1;
    }
    toast("switched");
    task()
});

events.onKeyDown("volume_up", function(event) {
    toast("exit");
    exit();
});

task();

function task1() {
    toast("swipe");
    extu = engines.execScriptFile("/sdcard/scripts/0quickmove.js");
    extu2 = engines.execScriptFile("/sdcard/scripts/0quicksave.js");
}

function task2() {
    toast("save");
    extu = engines.execScriptFile("/sdcard/scripts/0quicksave.js");
}

function task3() {
    toast("pressave");
    extu = engines.execScriptFile("/sdcard/scripts/0pressave.js");
}
