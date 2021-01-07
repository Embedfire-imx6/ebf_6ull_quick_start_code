
#生成可执行文件的名称
Target = libgpiod-beep

#编译的源文件
SRC=beep.c

#存放libgpiod库的路径
lib_dir = libgpiod

#编译指定的库
LIBS=-lgpiod

ARCH ?= arm
#编译器CC
#根据传入的参数ARCH，确定使用的编译器
#默认使用gcc编译器
#make ARCH=arm 时使用ARM-GCC编译器
ifeq ($(ARCH), x86)
	CC = gcc
else
	CC = arm-linux-gnueabihf-gcc
endif

#指定头文件的路径
CFLAGS = $(patsubst %, -I%, $(lib_dir))

#指定动态库的路径
LIB_CFLAGS = $(patsubst %, -L%, $(lib_dir))

all:
	$(CC) -o $(Target) $(SRC) $(LIBS) $(LIB_CFLAGS) $(CFLAGS) 

clean:
	rm $(Target)
