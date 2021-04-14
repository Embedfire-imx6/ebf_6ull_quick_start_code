

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <termios.h>


const char default_path[] = "/dev/ttymxc2";

/*
 *串口配置成功返回0，失败返回-1；
 */
int set_uart(int fd,int nSpeed, int nBits, char nEvent, int nStop);

int main(int argc,char *argv[])
{
    int fd;
    int res;
    char *path;
    char buf[1024] = "Embedfire tty send test.\n";

    if(argc > 1)
    {
        path = argv[1];
    }
    else
    {
        path = (char *)default_path;
    }

    fd = open(path,O_RDWR);
    if(fd < 0)
    {
        perror(path);
        exit(-1);
    }

    if( set_uart(fd,115200,8,'n',1) )
    {
        printf("set serial error\n");
    }

    while(1)
    {
        write(fd, buf, strlen(buf));
        //printf("send res = %d bytes data: %s",strlen(buf), buf);
        memset(buf,0,1024);
        res = read(fd, buf, 1024);
        if(res > 0)
        {
            printf("Receive res = %d bytes data: %s",res, buf);
        }
    }
    close(fd);
    return 0;
}

int set_uart(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios opt;

    //清空串口接收缓冲区
    tcflush(fd, TCIOFLUSH);

    //获取串口配置参数
    tcgetattr(fd, &opt);

    opt.c_cflag &= (~CBAUD);    //清除数据位设置
    opt.c_cflag &= (~PARENB);   //清除校验位设置

    //opt.c_iflag |= IGNCR;       //忽略接收数据中的'\r'字符，在windows中换行为'\r\n'
    opt.c_iflag &= (~ICRNL);    //不将'\r'转换为'\n'
    opt.c_lflag &= (~ECHO);     //不使用回显

    //设置波特率
    switch(nSpeed)
    {
        case 2400: 
            cfsetspeed(&opt,B2400);
            break;

        case 4800:
            cfsetspeed(&opt,B4800);
            break;

        case 9600: 
            cfsetspeed(&opt,B9600);
            break;

        case 38400: 
            cfsetspeed(&opt,B38400);
            break;
            
        case 115200:
            cfsetspeed(&opt,B115200);
            break;

        default:
            return -1;
    }

    //设置数据位
    switch(nBits)
    {
        case 7: 
            opt.c_cflag |= CS7;
            break;

        case 8: 
            opt.c_cflag |= CS8;
            break;

        default:
            return -1;          
    }

    //设置校验位
    switch(nEvent)
    {
        //无奇偶校验
        case 'n':
        case 'N': 
            opt.c_cflag &= (~PARENB);   
            break;

        //奇校验
        case 'o':
        case 'O':  
            opt.c_cflag |= PARODD;
            break;
        
        //偶校验
        case 'e':
        case 'E':  
            opt.c_cflag |= PARENB;
            opt.c_cflag &= (~PARODD);
            break;

        default:
            return -1;
    }

    //设置停止位
    switch(nStop)
    {
        case 1:
            opt.c_cflag &= ~CSTOPB;
            break;
        case 2:
            opt.c_cflag |= CSTOPB; 
            break;
        default:
            return -1;
    }
    //设置串口
    tcsetattr(fd,TCSANOW,&opt);

    return 0;
}
