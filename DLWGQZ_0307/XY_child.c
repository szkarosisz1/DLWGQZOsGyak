#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    for(int i = 0; i < 10; i++)
    {
        printf("Szkarosi Szilard, DLWGQZ\n");
        sleep(2);
    }

    return 0;
}
