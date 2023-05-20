#include "bmp.h"
#include "ev.h"

int Dis_pic(char *pic)
{
	int fd = open(pic, O_RDONLY);
	if (-1 == fd)
	{
		perror("open error");
		return -1;
	}
	int width, height;
	short depth;
	lseek(fd, 0x12, SEEK_SET);
	read(fd, &width, 4);
	read(fd, &height, 4);

	lseek(fd, 0x1c, SEEK_SET);
	read(fd, &depth, 2);
	printf("%d %d %d\n", width, height, depth);
	int laizi = (4 - (width * depth / 8) % 4) % 4;
	unsigned char color_buf[height*(width*depth / 8 + laizi)];//32 24
	char color_a = 0, color_r, color_g, color_b;//颜色分量
	unsigned int color;//像素点的颜色
	unsigned char *p = color_buf;
	lseek(fd, 0x36, SEEK_SET);

	int r;
	r = read(fd, color_buf, height*(width*depth / 8 + laizi));
	printf("%d %d\n", r, height*(width*depth / 8 + laizi));
	for (int i = height - 1; i >= 0; i--)
	{
		for (int j = 0; j < width; j++)
		{
			color_b = *p++;//b颜色
			color_g = *p++;
			color_r = *p++;
			if (32 == depth)
			{
				color_a = *p++;
			}
			color = color_a << 24 | color_r << 16 | color_g << 8 | color_b;//屏幕需要的颜色a r g b
			Display(color, j, i);
		}
		p += laizi;
	}
}

void Dis_pan()//棋盘
{
	for (int i = 0; i < 480; i++)
	{
		for (int j = 0; j < 800; j++)
		{
			if (i % 40 == 0 || j % 40 == 0)
				Display(0x00, j, i);
			else
			{
				Display(0xffff, j, i);
			}
		}
	}
}

/*void Draw_Ck(int x0,int y0,int color)
{
	int flag;
	for(int i= 0;i<480;i++)
		{
			for(int j=0;j<800;j++)
			{
				if((j-x0)*(j-x0)+(i-y0)*(i-y0)<=10*10)
				{
					if(flag)
					Display(0x0000ff,j,i);
					else
					Display(0xffffff, j, i);
				}
			}
		}
}*/