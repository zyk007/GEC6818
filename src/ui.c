#include "main.h"

//菜单选择
/*
    #输入：无
    #输出：显示菜单以及选择的菜单项
    #返回值：选择的菜单项 0~4
*/
int ChoiceSelect(void)
{
    int choices = 4;
    show_1152000bmp("./picture/background.bmp", p_lcd);//显示菜单
    

    //获取坐标
    int SlideMode = project_touch(&posx, &posy);
    printf("posx = %d, posy = %d\nSlideMode = %d\n", posx, posy, SlideMode); //test

    //返回choice
    // if(*posx > 240 && *posx < 480 && *posy > 0 && *posy > 800/choices)  return 1;
    // else if(*posx > 240 && *posx < 480 && *posy > 800/choices && *posy > 800/choices*2)  return 2;
    // else if(*posx > 240 && *posx < 480 && *posy > 800/choices*2 && *posy > 800/choices*3)  return 3;
    // else if(*posx > 240 && *posx < 480 && *posy > 800/choices*3 && *posy > 800/choices*4)  return 4;
    // else    return 0;
    if(posx > 0 && posx < 800/4 && posy > 240 && posy < 480)    return 1;
    else if(posx > 800/4 && posx < 800/4*2 && posy > 240 && posy < 480)    return 2;
    else if(posx > 800/4*2 && posx < 800/4*3 && posy > 240 && posy < 480)    return 3;
    else if(posx > 800/4*3 && posx < 800/4*4 && posy > 240 && posy < 480)    return 4;
    else    return 0;

}

//UI界面
int project_ui(void)
{
    /*函数声明*/
    int Polling(int);
    int MusicPlay(char[]);
    int VideoPlay(char[], int, int);

    //1.开机提示
    printf("Welcome to my project!!\n");
    printf("-------------------------\n");
    printf("PICTURE: 1.picture polling\n2.music playing\n3.video playing\n");
    show_1152000bmp("./picture/Photo_tip.bmp", p_lcd);//开机界面
    system("mplayer ./music/Thisstreet.mp3 &");
    sleep(4);
    system("killall -9 mplayer");
    sleep(3);
    show_1152000bmp("./picture/background.bmp", p_lcd);//显示菜单
    system("killall -9 mplayer");//防止复活

    //2.项目功能
    int choice;
    do{
        //提示选择要进行的项目
        choice = ChoiceSelect();
        printf("Menu load success!\n");
        switch(choice)
        {
            //(0)退出
            case 0:
                printf("Exit!\n");
                break;
            //(1)幻灯片播放
            case 1:
                Polling(3);
                break;
            //(2)音乐播放
            case 2:
                MusicPlay("./music/Thisstreet.mp3");
                break;
            //(3)视频播放
            case 3:
                VideoPlay("./video/kungfu.avi", 800, 400);
                break;
            //(4)未知功能
            case 4:
				Lcd_Init();
				Dis_pic("./picture/1.bmp");//封面图片
				Get_ev(&x, &y);//开始操作
				


                printf("What are you doing?\n");
                break;
            //输入错误
            default:
                printf("You want to destory me??\n");
                break;
        }
    }while(1);

    return 0;
}