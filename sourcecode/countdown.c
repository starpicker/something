#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

WINDOW *create_window(int height, int width, int starty, int startx, chtype ch, int has_box);
void destory_win(WINDOW *win);

#define D_WIDTH  5 // a digit's width
#define D_HEIGHT 5 // a digit's height
#define INFO_WIDTH 40
#define INFO_HEIGHT 5

WINDOW *digit_win[8], *info_win;

char digit[11][5][5] = {
    { { 1,1,1,1,1 }, /* 0 */
	  { 1,0,0,0,1 },
	  { 1,0,0,0,1 },
	  { 1,0,0,0,1 },
	  { 1,1,1,1,1 } },
	{ { 0,0,0,0,1 }, /* 1 */
	  { 0,0,0,0,1 },
	  { 0,0,0,0,1 },
	  { 0,0,0,0,1 },
	  { 0,0,0,0,1 } },
	{ { 1,1,1,1,1 }, /* 2 */
	  { 0,0,0,0,1 },
	  { 1,1,1,1,1 },
	  { 1,0,0,0,0 },
	  { 1,1,1,1,1 } },
	{ { 1,1,1,1,1 }, /* 3 */
	  { 0,0,0,0,1 },
	  { 1,1,1,1,1 },
	  { 0,0,0,0,1 },
	  { 1,1,1,1,1 } },
	{ { 1,0,0,0,1 }, /* 4 */
	  { 1,0,0,0,1 },
	  { 1,1,1,1,1 },
	  { 0,0,0,0,1 },
	  { 0,0,0,0,1 } },
	{ { 1,1,1,1,1 }, /* 5 */
	  { 1,0,0,0,0 },
	  { 1,1,1,1,1 },
	  { 0,0,0,0,1 },
	  { 1,1,1,1,1 } },
	{ { 1,1,1,1,1 }, /* 6 */
	  { 1,0,0,0,0 },
	  { 1,1,1,1,1 },
	  { 1,0,0,0,1 },
	  { 1,1,1,1,1 } },
	{ { 1,1,1,1,1 }, /* 7 */
	  { 0,0,0,0,1 },
	  { 0,0,0,0,1 },
	  { 0,0,0,0,1 },
	  { 0,0,0,0,1 } },
	{ { 1,1,1,1,1 }, /* 8 */
	  { 1,0,0,0,1 },
	  { 1,1,1,1,1 },
	  { 1,0,0,0,1 },
	  { 1,1,1,1,1 } },
	{ { 1,1,1,1,1 }, /* 9 */
	  { 1,0,0,0,1 },
	  { 1,1,1,1,1 },
	  { 0,0,0,0,1 },
	  { 1,1,1,1,1 } },
	{ { 0,0,0,0,0 }, /* : */
	  { 0,0,1,0,0 },
	  { 0,0,0,0,0 },
	  { 0,0,1,0,0 },
	  { 0,0,0,0,0 } }
};

void display_a_digit(WINDOW *win, char digit[5][5], char isnear)
{
    int y, x;
    int color;
    color = isnear?COLOR_PAIR(4):COLOR_PAIR(3);
    werase(win); // erase the window before re-put the number
    for(y = 0; y < 5; y++){
        for(x = 0; x < 5; x++){
            if (digit[y][x] == 1){
                wattron(win, color);
                mvwprintw(win, y, x, "a");
                wattroff(win, color);
            }
        }
    }
}

void create_clock_subwindow(WINDOW *win)
{
    int top_win_width, top_win_height;
    int starty, startx;
    int i;
    getmaxyx(win, top_win_height, top_win_width);
    if (top_win_height < D_HEIGHT || top_win_width < D_WIDTH){
        printf("window is too small, please relize the window\n");
        return;
    }
    starty = (top_win_height - D_HEIGHT) / 2;
    startx = (top_win_width - D_WIDTH*8-7) / 2;
    for(i = 0; i < 8; i++){
        digit_win[i] = subwin(win, D_HEIGHT, D_WIDTH, starty, startx+i*(D_WIDTH+1));
    }
}
void create_info_subwindow(WINDOW *win)
{
    int top_win_width, top_win_height;
    int starty, startx;
    getmaxyx(win, top_win_height, top_win_width);
    starty = top_win_height / 2 - 2*INFO_HEIGHT;
    startx = (top_win_width - INFO_WIDTH) / 2;
    info_win = subwin(win, INFO_HEIGHT, INFO_WIDTH, starty, startx);
}

void destory_all_win(WINDOW *win)
{
    int i;
    delwin(win);
    delwin(info_win);
    for(i = 0; i < 8; i++){
        delwin(digit_win[i]);
    }
}

int main(int argc, char *argv[])
{
    char isnear;
    int screen_width, screen_height;
    int i;
    int hour_tens, hour_units, min_tens, min_units, sec_tens, sec_units;
    time_t current_time;
    long left_time, end_seconds;
    char *str;
    struct tm *c_time;
    WINDOW *win1;

    str = malloc(256*sizeof(char));
    memset(str, '\0', sizeof(char)*256);
    if (argc < 2 || argc > 4){
        printf("Usage: %s [options] <seconds> [info strings]\n", argv[0]);
        return 0;
    }
    if (strcmp(argv[1], "-c")){
        left_time = atol(argv[1]);
        if(argc == 3)
            strcpy(str, argv[2]);
    }else if(!strcmp(argv[1], "-c")){
        left_time = atol(argv[2]);
        if (argc == 4)
            strcpy(str, argv[3]);
    }else{
        printf("Usage: %s [options] <seconds> [info strings]\n", argv[0]);
        return 0;
    }
    initscr();
    cbreak();
    noecho();
    /* keypad(stdscr, TRUE); */
    // init color
    start_color();
    init_color(COLOR_BLACK, 46, 52, 54);
    init_color(COLOR_BLUE, 52, 101, 164);
    init_color(COLOR_WHITE, 128, 128, 128);
    init_pair(1, COLOR_BLACK, COLOR_BLACK); // use for main win
    init_pair(2, COLOR_BLUE, COLOR_BLACK); // use for clock win
    init_pair(3, COLOR_BLUE, COLOR_BLUE); // use for clock digit
    init_pair(4, COLOR_RED, COLOR_RED); // use for clock digit
    init_pair(5, COLOR_WHITE, COLOR_BLACK); // use for clock digit
    // init main win
    getmaxyx(stdscr, screen_height, screen_width);
    win1 = newwin(screen_height, screen_width, 0, 0);
    wbkgd(win1, COLOR_PAIR(1));
    time(&current_time);
    end_seconds = current_time + left_time;
    while (left_time){
        sleep(1);
        time(&current_time);
        left_time = end_seconds - current_time;
        isnear = (left_time < 120)?TRUE:FALSE;
        if (strcmp(argv[1], "-c")){
            hour_tens = left_time/3600/10;
            hour_units = left_time/3600%10;
            min_tens = left_time/60%60/10;
            min_units = left_time/60%60%10;
            sec_tens = left_time%60/10;
            sec_units = left_time%60%10;
        }else{
            c_time = localtime(&current_time);
            hour_tens = c_time->tm_hour/10;
            hour_units = c_time->tm_hour%10;
            min_tens = c_time->tm_min/10;
            min_units = c_time->tm_min%10;
            sec_tens = c_time->tm_sec/10;
            sec_units = c_time->tm_sec%10;
        }
        werase(win1);
        create_info_subwindow(win1); // info window
        wattron(info_win, COLOR_PAIR(5));
        mvwprintw(info_win, 0, 0, "%s", str);
        wattron(info_win, COLOR_PAIR(5));
        create_clock_subwindow(win1);
        for(i = 0; i < 8; i++){
            wbkgd(digit_win[i], COLOR_PAIR(2));
        }
        display_a_digit(digit_win[0], digit[hour_tens], isnear);
        display_a_digit(digit_win[1], digit[hour_units], isnear);
        display_a_digit(digit_win[2], digit[10], isnear);
        display_a_digit(digit_win[3], digit[min_tens], isnear);
        display_a_digit(digit_win[4], digit[min_units], isnear);
        display_a_digit(digit_win[5], digit[10], isnear);
        display_a_digit(digit_win[6], digit[sec_tens], isnear);
        display_a_digit(digit_win[7], digit[sec_units], isnear);
        touchwin(win1);
        wrefresh(win1);
    }
    free(str);
    destory_all_win(win1);
    endwin();
    return 0;
}