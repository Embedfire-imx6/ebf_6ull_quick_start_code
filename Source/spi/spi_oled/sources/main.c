
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdint.h>


#include "../includes/spi_oled_app.h"

/*SPI 接收 、发送 缓冲区*/
unsigned char tx_buffer[100] = "hello the world !";
unsigned char rx_buffer[100];


/* 设备文件描述符 */
extern int data_or_command; // D/C控制引脚的设备文件描述符
extern int fd;				// SPI控制引脚的设备文件描述符

/*程序中使用到的点阵 字库*/
extern const unsigned char BMP1[];

int main(int argc,char * argv[])
{
	int i = 0; //用于循环
    int ret;
	ret = oled_init();
    if(ret == -1)
    {
        printf("oled_init error\n");
    }
	OLED_Fill(0xFF);
	sleep(1);

	OLED_Fill(0x00); //清屏

	while (1)
	{

		OLED_ShowStr(0, 3, (unsigned char *)"Wildfire Tech", 1);  //测试6*8字符
		OLED_ShowStr(0, 4, (unsigned char *)"Hello wildfire", 2); //测试8*16字符
		sleep(2);
		OLED_Fill(0x00); //清屏

		for (i = 0; i < 4; i++)
		{
			OLED_ShowCN(22 + i * 16, 0, i); //测试显示中文
		}
		sleep(1);
		OLED_Fill(0x00); //清屏

		OLED_DrawBMP(0, 0, 128, 8, (unsigned char *)BMP1); //测试BMP位图显示
		sleep(1);
		OLED_Fill(0x00); //清屏
	}

	close(fd);
    close(data_or_command);

    return 0;
}


