#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        printf("usage: %s input.grey output.dat rate\n", argv[0]);

        return -1;
    }

    const char* input = argv[1];
    const char* output = argv[2];
    float rate = atof(argv[3]);

    FILE* fp = fopen(input, "r");
    FILE* fp2 = fopen(output, (rate < 1.0)?"a":"w");
    char c;
    float f;

    while(fread(&c, 1, 1, fp))
    {
        f = c;
        fprintf(fp2, "%.4f ", f);
    }

    fprintf(fp2, "%.4f\n", rate);

    fclose(fp);
    fclose(fp2);

    return 0;
}
