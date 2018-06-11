#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <termio.h>
#include <map>
using std::map;

map<int, int> g_kbmap;
#include "map.txt"

#define DEVNAME "/dev/input/event4"

#define KEYDOWN 1
#define KEYUP   0

struct termios stored_settings;
void initkeyboard(void)
{
    struct termios new_settings;
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ECHO);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
}

void unitkeyboard(void)
{
    tcsetattr(0, TCSANOW, &stored_settings);
}

void send_single_key(int k_fd, int key);
void send_left_shift_key(int k_fd, int key);
void send_left_ctrl_key(int k_fd, int key);
bool checkshift(char c);

int simulate_input_init(void);
int simulate_input_char(int kbfd, char c);
void simulate_input_unit(int kbfd);

int simulate_input_init(void)
{
    int k_fd;
    k_fd = open(DEVNAME, O_RDWR);

    if(k_fd < 0)
    {
        printf("open error!\n");
        return k_fd;
    }

    initkeyboard();
    initkbmap();
    return k_fd;
}

int simulate_input_char(int kbfd, char c)
{
    int k_fd = kbfd;
    int keycode;
    keycode = g_kbmap[c];

    if(keycode)
    {
        if(checkshift(c))
        {
            send_left_shift_key(k_fd, keycode);
        }
        else
        {
            send_single_key(k_fd, keycode);
        }
    }

    return 0;
}

void simulate_input_unit(int kbfd)
{
    int k_fd = kbfd;
    close(k_fd);
    unitkeyboard();
}

int reportkey(int fd, uint16_t type, uint16_t keycode,
              int32_t value)
{
    struct input_event event;
    event.type = type;
    event.code = keycode;
    event.value = value;
    gettimeofday(&event.time, 0);

    if(write(fd, &event, sizeof(struct input_event)) < 0)
    {
        printf("report key error!\n");
        return -1;
    }

    return 0;
}

void send_single_key(int k_fd, int keycode)
{
    reportkey(k_fd, EV_SYN, SYN_REPORT, 0);
    reportkey(k_fd, EV_KEY, keycode, KEYDOWN);
    reportkey(k_fd, EV_KEY, keycode, KEYUP);
    reportkey(k_fd, EV_SYN, SYN_REPORT, 0);
}

void send_left_shift_key(int k_fd, int keycode)
{
    reportkey(k_fd, EV_SYN, SYN_REPORT, 0);
    reportkey(k_fd, EV_KEY, KEY_LEFTSHIFT, KEYDOWN);
    reportkey(k_fd, EV_SYN, SYN_REPORT, 0);
    reportkey(k_fd, EV_KEY, keycode, KEYDOWN);
    reportkey(k_fd, EV_KEY, keycode, KEYUP);
    reportkey(k_fd, EV_SYN, SYN_REPORT, 0);
    reportkey(k_fd, EV_KEY, KEY_LEFTSHIFT, KEYUP);
    reportkey(k_fd, EV_SYN, SYN_REPORT, 0);
}

bool checkshift(char c)
{
    char sign[] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '(', '_', '+', '{', '}', ':', '\\', '~', '|', '<', '>', '?'};

    for(int i = 0; i < sizeof(sign); i++)
    {
        if(sign[i] == c)
        {
            return true;
        }
    }

    if(c <= 'Z' && c >= 'A')
    {
        return true;
    }

    return false;
}
