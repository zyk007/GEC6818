#include "main.h"


int project_touch(int *th_x, int *th_y)
{
    //获取坐标
/*
	输入：屏幕点击
	输出：点击的坐标(th_x, th_y)
	返回：异常-1,点击0，向右移动1，向左移动2,向下移动3,向上移动4
	注意：需要两个int指针存放坐标
*/
	while(1)
	{
		int old_x, old_y;
		read(fd_touch, &buf, sizeof(buf));

		if (buf.type == EV_ABS && buf.code == ABS_X)
			//*th_x = buf.value;
			*th_x = buf.value*800/1024;
		if (buf.type == EV_ABS && buf.code == ABS_Y)
			//*th_x = buf.value;
			*th_y = buf.value*480/600;
		//这是开发板驱动问题，开发板的屏幕像素是800*480，但驱动给的分辨率有误，驱动的分辨率是1024*600，所以需要乘相应的值
		//触控驱动正常的开发板无需乘这些值，即使用注释的代码，无需使用源代码
		if (buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 1)
		{
			old_x = *th_x;
			old_y = *th_y;
				
		}
		

		if (buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
		{
			if (old_x < *th_x && (*th_x - old_x >= 50)) return 1;//向右移动
			if (old_x > *th_x && (old_x - *th_x >= 50)) return 2;//向左移动
			if (old_y < *th_y && (*th_y - old_y >= 50)) return 3;//向下移动
			if (old_y > *th_y && (old_y - *th_y >= 50)) return 4;//向上移动
			break ;
		}

	}//end while

	return 0;
}
