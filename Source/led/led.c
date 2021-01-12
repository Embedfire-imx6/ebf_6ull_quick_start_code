
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define RLED_DEV_PATH "/sys/class/leds/red/brightness"


int main(int argc,int *argv[])
{
    int r_fd;

    r_fd = open(RLED_DEV_PATH,O_WRONLY);
    if(r_fd < 0)
    {
        perror(RLED_DEV_PATH);
        exit(1);
    }

    while(1)
    {
        write(r_fd,"255",3);
        sleep(1);
        write(r_fd,"0",3);
        sleep(1);
    }

    close(r_fd);
}
 