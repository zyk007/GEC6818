/****************************************************************************************
 *设    计:蒋亮
 *日    期:2018-8-26
 *说	明:常用函数
****************************************************************************************/
#include "show_bmp.h"
#include"main.h"

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
int show_1152000bmp(char *pathname, int *show_1152000bmp_fd)
{
	//1、打开BMP格式图片和lcd显示屏
	int fd_bmp = open(pathname,  O_RDWR);
	if(fd_bmp == -1)
	{
		perror("show_1152000bmp(), open bmp fail!\n");
		return -1;
	}
	
	//2、去掉54个字节
	int ret = lseek(fd_bmp, 54, SEEK_SET);
	if(ret == -1)
	{
		printf("show_1152000bmp(), lseek bmp fail!\n");
		return -1;		
	}
	
	//3、读取BMP数据到lcd屏幕中
	char bmp_buf[800*480*3];
	int lcd_buf[800*480];
	bzero(bmp_buf, 800*480*3);
	bzero(lcd_buf, 800*480);
	
	ret = read(fd_bmp, bmp_buf, 800*480*3);//BMP数据放到了bmp_buf中
	if(ret == -1)
	{
		printf("show_1152000bmp(), read bmp fail!\n");
		return -1;		
	}		

	//4、关闭文件，回收资源
	close(fd_bmp);
	
	//5、800*480个A像素变成800*480个B像素算法。
	int i;
	for(i=0; i<800*480; i++)
	{
		lcd_buf[i] = bmp_buf[i*3]<<0 | bmp_buf[i*3+1]<<8 | bmp_buf[i*3+2]<<16;
		//lcd_buf[0] = bmp_buf[0]<<0 | bmp_buf[1]<<8 | bmp_buf[2]<<16;
		//lcd_buf[1] = bmp_buf[3]<<0 | bmp_buf[4]<<8 | bmp_buf[5]<<16;
		//lcd_buf[2] = bmp_buf[6]<<0 | bmp_buf[7]<<8 | bmp_buf[8]<<16;
	}
	
	//6、lcd_buf数据（DDR2中的数据）通过mmap显示到LCD屏幕中。
/*
	for(i=0; i<800*480; i++)
	{
		*(lcd_mmap+i)=lcd_buf[i];
	}	
*/
	int x, y;
	for(y=0; y<480; y++)//优化算法，解决图片颠倒问题
	{
		for(x=0; x<800; x++)
		{
			*(show_1152000bmp_fd+800*(479-y)+x)= lcd_buf[800*y+x];
		}
	}
			
	return 0;
}

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
int show_location_bmp(char *pathname,int lcd_x_coordinates, int lcd_y_coordinates, int wide, int height,int *show_bmp_lcd_mmap) 
{
	int i;
	int ret;
	int x, y;
	
	char bmp_buf[wide*height*3];
	int  new_buf[wide*height];
	bzero(bmp_buf, sizeof(bmp_buf));
	bzero(new_buf, sizeof(new_buf));

	int bmp_fd = open(pathname, O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("show_location_bmp(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd,54,SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		printf("show_location_bmp(), lseek bmp failed\n");		
		return -1;
	}

    int *new_p = show_bmp_lcd_mmap + 800*lcd_y_coordinates + lcd_x_coordinates;//3、重新确定映射位置。
	
	ret = read(bmp_fd, bmp_buf, wide*height*3);//4、取读图片像素
	if(ret == -1)
	{
		printf("show_location_bmp(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片
	
	for(i=0; i<wide*height; i++)//6、24bits 转 32bits控制变量
	{
			new_buf[i] = bmp_buf[i*3]<<0  |  bmp_buf[(i*3)+1]<<8 | bmp_buf[(i*3)+2]<<16;		
	}
	
	for(y=0;y<height;y++)//7、解决图片上下颠倒问题
	{
		for(x=0;x<wide;x++)
		{
			*(new_p+(800*((height-1)-y))+x) = new_buf[wide*y+x];
		}
	}
	
	return 0;		
}

/*
	功能：切换BMP图片特效“圆形扩散”。
	参数：
		char *pathname：图片的路径。
		int *pic_circular_spread_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_circular_spread(char *pathname, int* pic_circular_spread_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname, O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_circular_spread(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_circular_spread(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_circular_spread(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	for(k=0; k<467; k+=3)//8、特殊动画“圆形扩散”效果算法
	{
		for(i=0; i<480; i++)
		{
			for(j=0; j<800; j++)
			{
				if((j-400)*(j-400)+(i-240)*(i-240) <= k*k)
				{
					pic_circular_spread_fd[800*i+j] = mi_buf[800*i+j];					
				}
			}
		}
	} 

	return 0;
}

/*
	功能：切换BMP图片特效”圆形收缩“。
	参数：	
		char *pathname：图片的路径。
		int *pic_circular_shrink_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_circular_shrink(char *pathname, int* pic_circular_shrink_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_circular_shrink(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_circular_shrink(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_circular_shrink(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画“圆形收缩”效果算法
	for(k=468; k>=0; k-=3)
	{
		for(i=0; i<480; i++)
		{
			for(j=0; j<800; j++)
			{
				if((j-400)*(j-400)+(i-240)*(i-240) >= k*k)
					pic_circular_shrink_fd[800*i+j] = mi_buf[800*i+j];
			}
		}
	}	
		
	return 0;
}

/*
	功能：切换BMP图片特效”向下飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_down_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_down(char *pathname, int* pic_down_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_down(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_down(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_down(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画“向下飞入”效果算法
	for(i=0; i<480; i++)
	{
		for(j=0; j<800; j++)
		{
			pic_down_fd[800*i+j] = mi_buf[800*i+j];	
		}
		usleep(1000);
	} 	

	return 0;
}

/*
	功能：切换BMP图片特效”向上飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_up_fd：被映射区的指针。
	返回值：成功  0
			失败  -1
*/
int pic_up(char *pathname, int* pic_up_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_up(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_up(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_up(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画“向上飞入”效果算法
	for(i=479; i>=0; i--)
	{
		for(j=0; j<800; j++)
		{
			pic_up_fd[800*i+j] = mi_buf[800*i+j];	
		}
		usleep(500);
	} 	
	
	return 0;
}

/*
	功能：切换BMP图片特效”向左飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_left_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_left(char *pathname, int* pic_left_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_left(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_left(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_left(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画“向左飞入”效果算法
	for(j=799; j>=0; j--)
	{
		for(i=0; i<480; i++)
		{
			pic_left_fd[800*i+j] = mi_buf[800*i+j];
		}
		usleep(500);
	} 
	
	return 0;
}

/*
	功能：切换BMP图片特效”向右飞入“。
	参数：
		char *pathname：图片的路径。
		int *pic_right_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_right(char *pathname, int* pic_right_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_right(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_right(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_right(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画“向右飞入”效果算法
	for(j=0; j<800; j++)
	{
		for(i=0; i<480; i++)
		{
			pic_right_fd[800*i+j] = mi_buf[800*i+j];
		}
		usleep(500);
	} 
		
	return 0;
}

/*
	功能：切换BMP图片特效”横百叶窗“。
	参数：
		char *pathname：图片的路径。
		int *pic_transverse_blinds_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_transverse_blinds(char *pathname, int* pic_transverse_blinds_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_transverse_blinds(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_transverse_blinds(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_transverse_blinds(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画“横百叶窗”效果算法
	for(j=0; j<800/8; j++)
	{
		for(i=0; i<480; i++)
		{
			for(k=0; k<8; k++)
			{
				pic_transverse_blinds_fd[800*i+j+k*800/8] = mi_buf[800*i+j+k*800/8];
			}
		}
		usleep(500);
	}	 
		
	return 0;
}

/*
	功能：切换BMP图片特效”竖百叶窗“。
	参数：
		char *pathname：图片的路径。
		int *pic_vertical_blinds_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_vertical_blinds(char *pathname, int* pic_vertical_blinds_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_vertical_blinds(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_vertical_blinds(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_vertical_blinds(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}

	//8、特殊动画竖百叶窗”效果算法
	for(i=0; i<480/8; i++)
	{
		for(j=0; j<800; j++)
		{
			for(k=0; k<8; k++)
			{
				pic_vertical_blinds_fd[800*(i+k*480/8)+j] = mi_buf[800*(i+k*480/8)+j];
			}
		}
		usleep(500);
	}		
		
	return 0;
}

/*
	功能：切换BMP图片特效“左右相合”。
	参数：	
		char *pathname：图片的路径。
		int *pic_left_right_coincidence_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_left_right_coincidence(char *pathname, int* pic_left_right_coincidence_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_left_right_coincidence(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_left_right_coincidence(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_left_right_coincidence(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}	

	for(block=0;block<10;block++)//8、特殊动画“左右相合”效果算法
	{
		for(i=40*block;i<(block+1)*40;i++)//刷左部分矩形
		{
			for(j=0;j<480;j++)
			pic_left_right_coincidence_fd[j*800+i]=mi_buf[j*800+i];
		}
		for(line=799-block*40;line>799-(block+1)*40;line--) //刷右部分矩形
		{
			for(j=0;j<480;j++)
			pic_left_right_coincidence_fd[j*800+line]=mi_buf[j*800+line];
		}
		usleep(100000);	
	}

	return 0;
}

/*
	功能：切换BMP图片特效“中间展开”。
	参数：	
		char *pathname：图片的路径。
		int *pic_mid_spread_fd：被映射区的指针。
	返回值：成功  0
			失败  -1		
*/
int pic_mid_spread(char *pathname, int *pic_mid_spread_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_mid_spread(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_mid_spread(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_mid_spread(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}	

	//8、特殊动画“中间展开”效果算法
	for(i=0; i<800/2; i++)
	{
		for(j=0; j<480; j++)
		{
			pic_mid_spread_fd[800/2+800*j+i] = mi_buf[800/2+800*j+i];
			pic_mid_spread_fd[800/2+800*j-i] = mi_buf[800/2+800*j-i];
		}
		usleep(500);
	}  

	return 0;
}

/*
	功能：切换BMP图片特效“斜方块”。
	参数：		
		char *pathname：图片的路径。
		int *pic_oblique_block_fd：被映射区的指针。
	返回值：成功  0
			失败  -1	
*/
int pic_oblique_block(char *pathname, int *pic_oblique_block_fd)
{	
	int ret=0;
	int line=0;
	int block=0;
	int i=0, j=0, k=0;
	int m=0, n=0;
	int bmp_fd=0;
	
	char bmp_buf[480*800*3];
	int mi_buf[480*800];	
	int temp_buf[480*800];	
	bzero(mi_buf,sizeof(mi_buf));
	bzero(bmp_buf,sizeof(bmp_buf));
	bzero(temp_buf,sizeof(temp_buf));

	bmp_fd = open(pathname , O_RDONLY);//1、打开BMP格式图片
	if(bmp_fd == -1)
	{
		printf("pic_oblique_block(), open bmp failed\n");
		return -1;
	}

	ret = lseek(bmp_fd, 54 , SEEK_SET);//2、跳过bmp图片的前54个位置
	if(ret == -1)
	{
		perror("pic_oblique_block(), lseek bmp failed\n");		
		return -1;
	}

	ret = read(bmp_fd , bmp_buf , sizeof(bmp_buf)); //4、取读图片像素
	if(ret == -1)
	{
		printf("pic_oblique_block(), read bmp failed\n");	
		return -1;
	}
	
	close(bmp_fd);//5、关闭图片

	for(i=0, j=0 ; i<800*480 ; i++, j+=3)//6、24bits 转 32bits控制变量
	{
		temp_buf[i] = bmp_buf[j+2]<<16 | bmp_buf[j+1]<<8 | bmp_buf[j] ;
	}

	for(line=0 ; line <=480 ;line++)//7、解决图片上下颠倒问题
	{
		for(i=0; i<=800 ; i++)
		mi_buf[800*line + i] = temp_buf[ (479-line)*800 + i ];
	}	

	//8、特殊动画“斜方块”效果算法
	for(k=0; k<=14; k++)
	{
		for(i=0; i<8; i++)
		{
			for(j=0; j<8; j++)
			{
				if(i+j <= k)
				{
					for(m=100*i; m<100*(i+1); m++)
					{
						for(n=60*j; n<60*(j+1); n++)
						{
							pic_oblique_block_fd[n*800+m] = mi_buf[n*800+m];
						}
					}
					usleep(200);
				}
			}
		}
	}

	return 0;
}

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
int clear_lcd_screen(int color, int lcd_x_coordinates, int lcd_y_coordinates, int wide, int height,int *lcd_mmap)
{
	int x, y;
	int *new_p = lcd_mmap + 800*lcd_y_coordinates + lcd_x_coordinates;
	for(y=0; y<height; y++)
	{
		for(x=0; x<wide; x++)
		{
			*(new_p+ 800*y+x) = color;
		}
	}
	return 0;	
}