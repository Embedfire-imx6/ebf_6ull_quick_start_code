


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/fb.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//RGB565 颜色
#define RED     0xF800	//红色
#define GREED   0x07E0	//绿色
#define BLUE	0x001f	//蓝色
#define WTITE   0xFFFF	//白色
#define BLACK   0x0000	//黑色

static struct fb_var_screeninfo var;	/* LCD可变参数 */
static unsigned char *fb_base;			/* Framebuffer映射基地址 */
static int screen_size;					/* 整个Framebuffer大小*/

void lcd_show_pixel_rgb565(int x,int y, unsigned short color);
void lcd_fill_rgb565(unsigned short color);

int main(int argc, char * argv[])
{
	int i;
	int fd_fb;							/* 文件句柄 */
	
	fd_fb =  open("/dev/fb0", O_RDWR); //打开LCD文件

	if(fd_fb < 0)
	{
		perror("/dev/fb0");
		exit(-1);
	}

	//获取屏幕可变参数相关信息
	if (ioctl(fd_fb,FBIOGET_VSCREENINFO,&var))
	{
		printf("can't get fb_var_screeninfo \n");
		goto err1;
	}

	printf("X:%d  Y:%d  bbp:%d\n",var.xres,var.yres,var.bits_per_pixel);

	screen_size = var.xres *var.yres *var.bits_per_pixel /8;  //整个Framebuffer大小

	//建立内存映射 方便控制
	fb_base = (unsigned char*)mmap(NULL,screen_size,PROT_READ|PROT_WRITE,MAP_SHARED, fd_fb,0);
	if(fb_base == NULL)
	{
		printf("can't mmap Framebuffer\n");
		goto err1;
	}
	//屏幕闪烁
	lcd_fill_rgb565(WTITE);
	lcd_fill_rgb565(RED);
	sleep(1);
	lcd_fill_rgb565(GREED);
	sleep(1);
	lcd_fill_rgb565(BLUE);
	sleep(1);
	lcd_fill_rgb565(WTITE);
	
	//显示线条
	for(i = 0;i<100;i++)
	{
		lcd_show_pixel_rgb565(var.xres/2,var.yres/2+i,RED);
	}

	//解除内存映射关系
	munmap(fb_base,screen_size);
	close(fd_fb);

	return 0;

err1:	

	close(fd_fb);

	return -1;
}

//画点函数
void lcd_show_pixel_rgb565(int x,int y, unsigned short color)  
{
	//确定坐标点所在的内存
	void *pen = fb_base + ((var.xres * y + x) * (var.bits_per_pixel/8));

	if( (x > var.xres) ||  (y > var.yres) )
	{
		return ;
	}
	else
	{
		*((unsigned short*)pen) = color;
	}
	
}

//屏幕填充函数
void lcd_fill_rgb565(unsigned short color)
{
	int i;

	unsigned short  *base = (unsigned short*)fb_base;
	for(i = 0 ;i < (screen_size/2); i++)
	{
		*(base+i) = color;
	}

}
