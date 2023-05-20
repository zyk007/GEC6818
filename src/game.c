#include "ev.h"
#include "bmp.h"
#include "game.h"

int flag = 0;
unsigned int Game_buf[12][20] = { 0 };

int Game_Change(int x, int y)
{
	int i, j;
	i = x % 40;
	j = y % 40;
	if (i > 20)
	{
		i = x / 40 + 1;
	}
	else
	{
		i = x / 40;
	}
	if (j > 20)
	{
		j = y / 40 + 1;
	}
	else
	{
		j = y / 40;
	}
	x = i * 40;
	y = j * 40;
	if (Game_buf[j][i] == 0)
	{
		if (flag)
		{
			Game_buf[j][i] = 1;
			Dis_zi(y, x, flag);

		}
		else
		{
			Game_buf[j][i] = 2;
			Dis_zi(y, x, flag);
		}
		flag = ~flag;
	}


}
int clear()
{
	for (int a = 0; a < 12; a++)
	{
		for (int b = 0; b < 20; b++)
		{
			Game_buf[a][b] = 0;

		}
	}
}

int Dis_zi(int x, int y, int flag)
{
	int i, j;
	if (flag)
	{
		for (i = 0; i < 480; i++)
		{
			for (j = 0; j < 800; j++)
			{
				if ((i - x)*(i - x) + (j - y)*(j - y) <= 330)
				{
					Display(0x00, j, i);
				}
			}
		}
	}
	else
	{
		for (i = 0; i < 480; i++)
		{
			for (j = 0; j < 800; j++)
			{
				if ((i - x)*(i - x) + (j - y)*(j - y) <= 330)
				{
					Display(0x00ffffff, j, i);
				}
			}
		}
	}
}

int Game_Over()
{
	int a, b, h = 1, s = 1, x = 1, j = 1;
	for (a = 0; a < 12; a++)
	{
		for (b = 0; b < 20; b++)
		{
			if (Game_buf[a][b] != 0)
			{

				while (h != 5)
				{
					if (Game_buf[a][b] == Game_buf[a + h][b])
						h++;
					else
						break;
				}
				while (s != 5)
				{
					if (Game_buf[a][b] == Game_buf[a][b + s])
						s++;
					else
						break;
				}
				while (x != 5)
				{
					if (Game_buf[a][b] == Game_buf[a + x][b + x])
						x++;
					else
						break;
				}
				while (j != 5)
				{
					if (Game_buf[a][b] == Game_buf[a + j][b - j])
						j++;
					else
						break;
				}
				if (h == 5 || s == 5 || x == 5 || j == 5)
				{
					Dis_pic("/game/bucuoo.bmp");
					clear();
				}
			}
		}
	}
}