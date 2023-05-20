#include "light.h"
unsigned int *plcd = NULL;
#include "ev.h"


int Display(int color, int x, int y)
{
	if (x >= 0 && x <= 800 && y >= 0 && y <= 480)
	{
		*(plcd + y * 800 + x) = color;
	}
}


int Lcd_Init()
{
	int fd = open("/dev/fb0", O_RDWR);
	if (-1 == fd)
	{
		perror("open error");
		return -1;
	}
	plcd = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

void Dis_wh()
{
	for (int i = 0; i < 480; i++)
	{
		for (int j = 0; j < 800; j++)
		{
			Display(0xffffff, j, i);
		}
	}

}
//»­¾ØÐÎ
void Dis_qu()
{
	for (int i = 100; i < 300; i++)
	{
		for (int j = 100; j < 600; j++)
		{
			Display(0xffffff, j, i);
		}
	}
}

//Ô²ÐÎ
void Dis_cir()
{
	for (int i = 100; i < 480; i++)
	{
		for (int j = 100; j < 800; j++)
		{
			if ((j - 350)*(j - 350) + (i - 200)*(i - 200) <= 100 * 100)
				Display(0x00ffff, j, i);
			if ((j - 400)*(j - 400) + (i - 275)*(i - 275) <= 100 * 100)
				Display(0xff00ff, j, i);
			if ((j - 450)*(j - 450) + (i - 200)*(i - 200) <= 100 * 100)
				Display(0xffff00, j, i);
			if (((j - 350)*(j - 350) + (i - 200)*(i - 200) <= 100 * 100) && ((j - 400)*(j - 400) + (i - 275)*(i - 275) <= 100 * 100) && ((j - 450)*(j - 450) + (i - 200)*(i - 200) <= 100 * 100))
				Display(0xff0000, j, i);
		}
	}
}
void Dis_tri()
{
	for (int i = 0; i < 480; i++)
	{
		for (int j = 0; j < 800; j++)
		{
			if (i + j <= 400)
				Display(0xff00ff, j, i);
		}
	}
}

/*
void Dis_tri()
{
	for(int y = 0;y<480;y++)
		   {
			   for(int x = 0;x<800;x++)
			   {
					if(x>=100 && y<=300 && y>=100 && y<=-x+400&& y>=x)
					Display(0xff00ff,x,y);
			   }
		   }
}
*/

/*int main()
{
	Lcd_Init();
	 Dis_wh();
	//Dis_wh();
	//Dis_qu();
	//Dis_cir();
	Dis_tri();

	return 0;

}*/
