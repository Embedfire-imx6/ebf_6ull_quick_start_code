
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/spi/spidev.h>


#define spi3_oled_DEV_path        "/dev/spidev2.0"




static int fd;                          // SPI 控制引脚的设备文件描述符

static uint32_t mode = SPI_MODE_2;      //用于保存 SPI 工作模式
static uint8_t bits = 8;                // 接收、发送数据位数
static uint32_t speed = 500000;         // 发送速度
static uint16_t delay;                  //保存延时时间

/*SPI 接收 、发送 缓冲区*/
static unsigned char tx_buffer[100] = "hello the world !";
static unsigned char rx_buffer[100];


//spi初始化
static int spi_init(void);
//spi发送数据
static int transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len);


int main(int argc,char * argv[])
{
    int ret;

    ret = spi_init();
    if( -1 == ret  )
    {
        printf("spi_init error\n");
        exit(-1);
    }

	ret = transfer(fd, tx_buffer, rx_buffer, sizeof(tx_buffer));
    if (-1 == ret  )
    {
        printf("transfer error...\n");
    }
    

	/*打印 tx_buffer 和 rx_buffer*/
	printf("tx_buffer: \n %s\n ", tx_buffer);
	printf("rx_buffer: \n %s\n ", rx_buffer);



    return 0;
}

int transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len)
{
    int ret;

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long )tx,
        .rx_buf = (unsigned long )rx,
        .len = len,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word =bits,
    };

    ret = ioctl(fd,SPI_IOC_MESSAGE(1),&tr);
    if( ret == -1 )
    {
       return -1;
    }

    return 0;

}

int spi_init(void)
{
    int ret;
    fd = open(spi3_oled_DEV_path,O_RDWR);
    if(fd < 0)
    {
        perror("/dev/spidev2.0");
        return -1;
    }

    //设置spi工作模式
    ret = ioctl(fd,SPI_IOC_RD_MODE,&mode);
    if( ret == -1)
    {
        printf("SPI_IOC_RD_MODE error......\n ");
        goto fd_close;
    }
    ret = ioctl(fd,SPI_IOC_WR_MODE,&mode);
    if( ret == -1)
    {
        printf("SPI_IOC_WR_MODE error......\n ");
        goto fd_close;
    }

    //设置SPI通信的字长
    ret = ioctl(fd,SPI_IOC_WR_BITS_PER_WORD,&bits);
    if( ret == -1)
    {
        printf("SPI_IOC_WR_BITS_PER_WORD error......\n ");
        goto fd_close;
    }
    ret = ioctl(fd,SPI_IOC_RD_BITS_PER_WORD,&bits);
    if( ret == -1)
    {
        printf("SPI_IOC_RD_BITS_PER_WORD error......\n ");
        goto fd_close;
    }

    //设置SPI最高工作频率
    ret = ioctl(fd,SPI_IOC_WR_MAX_SPEED_HZ,&speed);
    if( ret == -1)
    {
        printf("SPI_IOC_WR_MAX_SPEED_HZ error......\n ");
        goto fd_close;
    }
    ret = ioctl(fd,SPI_IOC_RD_MAX_SPEED_HZ,&speed);
    if( ret == -1)
    {
        printf("SPI_IOC_RD_MAX_SPEED_HZ error......\n ");
        goto fd_close;
    }

    printf("spi mode: 0x%x\n", mode);
    printf("bits per word: %d\n", bits);
    printf("max speed: %d Hz (%d KHz)\n", speed, speed / 1000);

    return 0;

fd_close:

    close(fd);
    return -1;
}
