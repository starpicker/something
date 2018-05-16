#include <ncurses.h>
#define ENTER                10
#define ESC                  27
#define STARTY               3
#define STARTX               4
#define MENU_ITEM_LENGTH     60
//change to add new items
#define MENU_ITEM_NUM        12 
#define GET_ARR_LEN(arr)     (sizeof(arr)/sizeof(arr[0]))

int menu(void)
{
    const char items[MENU_ITEM_NUM][MENU_ITEM_LENGTH] = {
		"Please select do what",
		"1 mount 5.0 ve mount_32bit",
		"2 mount 5.0 ve mount_64bit_yunmall",
		"3 mount sleekui",
		"4 umount all",
		"5 mount for_ants",
		"6 mount for_hitachi",
		"7 mount for_olympus",
		"8 mount for_cut_down_ve",
		"9 mount for_hls",
		"10 mount for_hitachi_delivery",
		"11 mount for_photo",
	};
	
	int choice, old_choice, ch, i;
	
	clear();
	mvprintw(STARTY, STARTX, items[0]);
	for (i=1;i<GET_ARR_LEN(items);++i){
		if(1==i)attron(A_REVERSE);
		mvprintw(STARTY+i, STARTX+2, "%s\n", items[i]);
		if(1==i)attroff(A_REVERSE);
	}
	
	choice = old_choice = 1;
	
	for(;;){
		ch=getch();
		switch(ch){
			case KEY_UP: choice = choice==1?MENU_ITEM_NUM:choice-1;break;
			case KEY_DOWN: choice = choice==MENU_ITEM_NUM?1:choice+1;break;
		//	case ESC: return 4;
			case ENTER: return choice;
			default: break;
		}
		
		mvprintw(STARTY + old_choice, STARTX + 2, items[old_choice]);
		attron(A_REVERSE);
		mvprintw(STARTY + choice, STARTX + 2, items[choice]);
		attroff(A_REVERSE);
		old_choice = choice;
	}
	getch();
	refresh();
}

do_work_one()
{
	system(". ~/mount3.txt");
}

do_work_two()
{
	system(". ~/mount5.txt");
}

do_work_three()
{
	system(". ~/mount_sleekui.txt");
}

do_work_fourth()
{
	//add here to umount
	system("sudo umount ~/mount_32bit ~/mount_64bit_yunmall/ ~/sleekui ~/mount_for_ants ~/mount_for_hitachi ~/mount_for_olympus ~/mount_for_hitachi_delivery ~/mount_for_photo 2>/dev/null");
}

do_work_fifth()
{
	system(". ~/mount_for_ants.txt");
}

do_work_sixth()
{
	system(". ~/mount_for_hitachi.txt");
}

//add here to new items
//
do_work_seventh()
{
	system(". ~/mount_for_olympus.txt");
}

do_work_eighth()
{
	system(". ~/mount_for_cut_down_ve.txt");
}

do_work_ninth()
{
	system(". ~/mount_for_hls.txt");
}

do_work_tenth()
{
	system(". ~/mount_for_hitachi_delivery.txt");
}

do_work_eleventh()
{
	system(". ~/mount_for_photo.txt");
}

int main(int argc, char** argv)
{
	int choice;
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	
	for(;;) {
		choice = menu();
		switch (choice) {
			case 1: mvprintw(15, 0, "workplace is mount_32bit\n");   do_work_one();   		 goto end;
			case 2: mvprintw(15, 0, "workplace is mount_64bit_yunmall\n");   do_work_two();          goto end;
			case 3: mvprintw(15, 0, "workplace is sleekui");
																				 do_work_three();        goto end;
			//add here to new items
			case 4: mvprintw(15, 0, "sudo umount mount_32bit mount_64bit_yunmall sleekui mount_for_ants mount_for_olympus mount_for_hls mount_for_hitachi mount_for_hitachi_delivery mount_for_photo\n");
		        do_work_fourth(); goto end;
			case 5: mvprintw(15, 0, "workplace is mount_for_ants\n");   			
		        do_work_fifth(); goto end;
			case 6: mvprintw(15, 0, "workplace is mount_for_hitachi\n");   			
		        do_work_sixth(); goto end;
			case 7: mvprintw(15, 0, "workplace is mount_for_olympus\n");   			
		        do_work_seventh(); goto end;
			case 8: mvprintw(15, 0, "workplace is mount_for_cut_down_ve\n");   			
		        do_work_eighth(); goto end;
			case 9: mvprintw(15, 0, "workplace is mount_for_hls\n");   			
		        do_work_ninth(); goto end;
			case 10: mvprintw(15, 0, "workplace is mount_for_hitachi_delivery\n");   			
		        do_work_tenth(); goto end;
			case 11: mvprintw(15, 0, "workplace is mount_for_photo\n");   			
		        do_work_eleventh(); goto end;
			default: break;
		}
	}
	
end:
	getch();
	endwin();
	return 0;
}

