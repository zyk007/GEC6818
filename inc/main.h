//#ifdef _MAIN_H_

#define _MAIN_H_

/*头文件*/
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <linux/input.h>

#include "show_bmp.h"
#include"ev.h"
#include"bmp.h"
#include"game.h"
#include"light.h"

/*全局变量*/
#define PIXELS 800*480//屏幕像素点
int fd_lcd;//LCD文件
int fd_touch;//touch文件
int* p_lcd;//LCD映射内存
int posx, posy;    //存放坐标
int fd_fifo;//管道文件
int x, y;
/*结构体定义*/
struct input_event buf;//触摸屏结构体
/*宏定义*/

/*联合体*/

/*枚举*/

/*函数声明*/
int project_init(void);//程序初始化
int project_ui(void);//程序主界面
int project_destroy(void);//程序销毁
int project_touch(int*, int*);//触摸屏操作函数

int Send_Cmd(char*);//写入管道文件
//#endif