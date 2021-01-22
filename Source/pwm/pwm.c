
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#define PWM_PATH "/sys/class/pwm/pwmchip2/"

int main(int argc,char * argv[])
{
    int fd;
    ssize_t ret;
    long period,duty_cycle;

    //判断参数合法性
    if( argc < 3)
    {
        printf("Please enter period and duty_cycle!\n ");
        exit(-1);
    }
    period = atol(argv[1]);
    duty_cycle = atol(argv[2]);
    if ( (period < duty_cycle)  || (strspn(argv[1], "0123456789")!= strlen(argv[1]) ) || (strspn(argv[2], "0123456789")!= strlen(argv[2])) )
    {
        printf("Period or duty_cycle parameter error!\n ");
        exit(-1);
    }
    

    //导出用户空间
    fd = open(PWM_PATH"export",O_WRONLY);
    if( fd < 0 )
    {
        perror(PWM_PATH"export");
        goto err1;
    }

    ret = write( fd,"0",1);
    if( ret < 1 )
    {
        perror(PWM_PATH"export");
        goto err1;
    }
    close(fd);

    //设置pwm周期
    fd = open(PWM_PATH"/pwm0/period",O_RDWR);
    if( fd < 0 )
    {
        perror(PWM_PATH"/pwm0/period");
        goto err1;
    }
    ret = write( fd,argv[1],strlen(argv[1]));
    if( ret < strlen(argv[1]) )
    {
        perror(PWM_PATH"/pwm0/period");
        goto err1;
    }
    close(fd);

    //设置占空比
    fd = open(PWM_PATH"/pwm0/duty_cycle",O_RDWR);
    if( fd < 0 )
    {
        perror(PWM_PATH"/pwm0/duty_cycle");
        goto err1;
    }
    ret = write( fd,argv[2],strlen(argv[2]));
    if( ret < strlen(argv[2]) )
    {
        perror(PWM_PATH"/pwm0/duty_cycle");
        goto err1;
    }
    close(fd);

    //设置极性
    fd = open(PWM_PATH"/pwm0/polarity",O_RDWR);
    if( fd < 0 )
    {
        perror(PWM_PATH"/pwm0/polarity");
        goto err1;
    }
    ret = write( fd,"normal",strlen("normal"));
    if( ret < strlen("normal") )
    {
        perror(PWM_PATH"polarity");
        goto err1;
    }
    close(fd);

    //使能pwm
    fd = open(PWM_PATH"/pwm0/enable",O_RDWR);
    if( fd < 0 )
    {
        perror(PWM_PATH"/pwm0/enable");
        goto err1;
    }
    ret = write( fd,"1",1);
    if( ret < 1 )
    {
        perror(PWM_PATH"/pwm0/enable");
        goto err1;
    }
    close(fd);

   
    printf("Set pwm success! \n");

    //从用户空间删除
    fd = open(PWM_PATH"unexport",O_WRONLY);
    if( fd < 0 )
    {
        perror(PWM_PATH"unexport");
        goto err1;
    }

    ret = write( fd,"0",1);
    if( ret < 1 )
    {
        perror(PWM_PATH"unexport");
        goto err1;
    }
    close(fd);

    return 0;

err1:

    close(fd);
    return -1;
}
