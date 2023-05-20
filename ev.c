#include "ev.h"
#include "bmp.h"
#include "game.h"


int Get_ev(int *x, int *y)
{
	int flag = 0;
	int flag1 = 3;
	int fd = open("/dev/input/event0", O_RDONLY);
	if (-1 == fd)
	{
		perror("open error");
		return -1;
	}
	struct input_event ev;
	int x1, y1;

	while (1)
	{
		read(fd, &ev, sizeof(ev));
		printf("ev_type = %d code = %d value = %d\n", ev.type, ev.code, ev.value);
		if (ev.type == EV_ABS)
		{
			if (ev.code == 0)//x
			{
				x1 = ev.value * 800 / 1024;

			}
			else
			{
				y1 = ev.value * 480 / 600;
			}
		}
		if (ev.type == EV_KEY && ev.code == 330 && ev.value == 1)//±£¥Ê≥ı º◊Û±ﬂ
		{
			*x = x1;
			*y = y1;
		}
		if (ev.type == EV_KEY && ev.code == 330 && ev.value == 0)
		{
			if (*x == x1 && *y == y1)//µ„ª˜
			{
				printf("dianji\n");
				if (flag1 == 1)
					Game_Change(x1, y1);
				if (flag1 != 1)
				{
					Dis_pan();
					flag1 = 1;
				}
			}
			if (x1 > *x)//”“ª¨
			{
				printf("youhua\n");

				flag1 = 2;
			}
			if (x1 < *x)//◊Ûª¨
			{
				printf("zuohua\n");

				flag1 = 3;
			}
			Game_Over();
		}
	}
}