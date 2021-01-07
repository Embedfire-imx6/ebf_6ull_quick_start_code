
#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    int i;
    int ret;

    struct gpiod_chip * chip;      //GPIO控制器句柄
    struct gpiod_line * line;      //GPIO引脚句柄


    /*获取GPIO控制器*/
    chip = gpiod_chip_open("/dev/gpiochip0");
    if(chip == NULL)
    {
        printf("gpiod_chip_open error\n");
        return -1;
    }

    /*获取GPIO引脚*/
    line = gpiod_chip_get_line(chip, 19);
    if(line == NULL)
    {
        printf("gpiod_chip_get_line error\n");
        goto release_line; 
    }

    /*设置GPIO为输出模式*/
    ret = gpiod_line_request_output(line,"beep",0);
    if(ret < 0)
    {
        printf("gpiod_line_request_output error\n");
        goto release_chip; 
    }

    for(i = 0;i<10;i++)
    {
        gpiod_line_set_value(line,1);
        usleep(500000);  //延时0.5s
        gpiod_line_set_value(line,0);
        usleep(500000);
    }

    
release_line:  
    /*释放GPIO引脚*/
    gpiod_line_release(line);

release_chip:
    /*释放GPIO控制器*/
    gpiod_chip_close(chip);


    return 0;
}



/*
 //获取GPIO控制器(GPIO组)
 struct gpiod_chip *gpiod_chip_open(const char *path);

 //获取GPIO引脚
 struct gpiod_line * gpiod_chip_get_line(struct gpiod_chip *chip, unsigned int offset);

 //设置引脚方向为输入模式
 int gpiod_line_request_input(struct gpiod_line *line,const char *consumer);

 //读取引脚状态
 int gpiod_line_get_value(struct gpiod_line *line);

 //释放GPIO引脚
 void gpiod_line_release(struct gpiod_line *line);

 //关闭GPIO组句柄并释放所有分配的资源。
 void gpiod_chip_close(struct gpiod_chip *chip);

 */