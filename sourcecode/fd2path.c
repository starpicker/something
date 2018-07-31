#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int MAXSIZE = 0xFFF;
    char proclnk[0xFFF];
    char filename[0xFFF];
    FILE *fp;
    int fno;
    ssize_t r;

    // test.txt created earlier
    fp = fopen("test.txt", "r");
    while(1);
    if (fp != NULL)
    {
        fno = fileno(fp);
        sprintf(proclnk, "/proc/self/fd/%d", fno);
        r = readlink(proclnk, filename, MAXSIZE);
        if (r < 0)
        {
            printf("failed to readlink\n");
            exit(1);
        }
        filename[r] = '\0';
        printf("fp -> fno -> filename: %p -> %d -> %s\n",
                fp, fno, filename);
    }
    return 0;
}