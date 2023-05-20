/****************************************************************************************
 *设    计:蒋亮
 *日    期:2018-8-26
 *说	明:头文件
****************************************************************************************/
#ifndef __SHOW_BMP_H__
#define __SHOW_BMP_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <strings.h>

/*
图片特效：
	1）“圆形扩散”    2）”圆形收缩“   
	3）”向下飞入“    4）”向上飞入“   5）”向左飞入“   6）”向右飞入“
	7）”横百叶窗“    8）”竖百叶窗“    9）“左右相合”   10）“中间展开”
	11）“斜方块”
*/

/*
	功能：显示800*480大小的bmp图片。
	参数：
		char *pathname：要显示图片的名字。
		int *show_1152000bmp_fd：被映射区的指针。
	返回值：成功  0
			失败  -1
*/
int show_1152000bmp(char *pathname, int *show_1152000bmp_fd);

/*
	功能：（不超过800*480显示屏大小）显示任意位置任意大小的bmp图片
	形参：
		char *pathname：要显示图片的名字
		int lcd_x_coordinates：lcd屏的x轴坐标
		int lcd_y_coordinates：lcd屏的y轴坐标（通过x、y轴坐标确定显示图片起始显示坐标）	
		int wide：所显示图片的宽度
		int height：所显示图片的高度
		int *show_bmp_lcd_mmap：被映射区的指针
	返回值：成功  0
			失败  -1
	
*/
int show_location_bmp(char *pathname,int lcd_x_coordinates, int lcd_y_coordinates, int wide, int height,int *show_bmp_lcd_mmap);

/*
	功能：切换BMP图片特效“圆形扩散”。
	参数：
		char *pathname：图片的路径。
		int *pic_circular_spread_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_circular_spread(char *pathname, int* pic_circular_spread_fd);

/*
	功能：切换BMP图片特效”圆形收缩“。
	参数：	
		char *pathname：图片的路径。
		int *pic_circular_shrink_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_circular_shrink(char *pathname, int* pic_circular_shrink_fd);

/*
	功能：切换BMP图片特效”向下飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_down_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_down(char *pathname, int* pic_down_fd);

/*
	功能：切换BMP图片特效”向上飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_up_fd：被映射区的指针。
	返回值：成功  0
			失败  -1
*/
int pic_up(char *pathname, int* pic_up_fd);

/*
	功能：切换BMP图片特效”向左飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_left_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_left(char *pathname, int* pic_left_fd);

/*
	功能：切换BMP图片特效”向右飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_right_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_right(char *pathname, int* pic_right_fd);

/*
	功能：切换BMP图片特效”横百叶窗“。
	参数：
		char *pathname：图片的路径。
		int *pic_transverse_blinds_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_transverse_blinds(char *pathname, int* pic_transverse_blinds_fd);

/*
	功能：切换BMP图片特效”竖百叶窗“。
	参数：
		char *pathname：图片的路径。
		int *pic_vertical_blinds_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_vertical_blinds(char *pathname, int* pic_vertical_blinds_fd);

/*
	功能：切换BMP图片特效“左右相合”。
	参数：	
		char *pathname：图片的路径。
		int *pic_left_right_coincidence_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_left_right_coincidence(char *pathname, int* pic_left_right_coincidence_fd);

/*
	功能：切换BMP图片特效“中间展开”。
	参数：	
		char *pathname：图片的路径。
		int *pic_mid_spread_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_mid_spread(char *pathname, int *pic_mid_spread_fd);

/*
	功能：切换BMP图片特效“斜方块”。
	参数：		
		char *pathname：图片的路径。
		int *pic_oblique_block_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_oblique_block(char *pathname, int *pic_oblique_block_fd);

/*
	功能：显示任意位置任意大小的lcd屏
	参数：	
		int color：要显示的颜色（RGB表示的十六进制）
		int lcd_x_coordinates：lcd屏幕的x轴坐标
		int lcd_y_coordinates：lcd屏幕的y轴坐标（通过x、y轴坐标确定起始显示颜色的坐标）
		int wide：所显示颜色的宽度
		int height：所显示颜色的高度（通过wide、height确定所显示颜色的面积）
		int *lcd_mmap：被映射区的指针
	返回值：成功  0
			失败  -1		
*/
int clear_lcd_screen(int color, int lcd_x_coordinates, int lcd_y_coordinates, int wide, int height,int *lcd_mmap);

#endif