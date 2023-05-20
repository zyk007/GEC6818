#include "main.h"

//音乐播放
/*
    #输入：音乐文件路径
    #输出：音乐播放
    #返回：成功返回0，失败返回-1
*/
int MusicPlay(char musicpath[])
{
    show_1152000bmp("./picture/music_play.bmp", p_lcd);
    //打开音乐文件
    system("killall -9 mplayer");
    char Music[255];
    sprintf(Music, "mplayer %s &", musicpath);
    system(Music);
    int nTouch = project_touch(&posx, &posy);
    //操作视频
    int posx, posy;
    int choice;
    int MusicMode = 1;
    do{

        int nTouch = project_touch(&posx, &posy);
        
        if(posx > 300 && posx < 490 && posy > 100 && posy < 300) choice = 1; //暂停或继续音乐
        else if(posx > 250 && posx < 540 && posy > 330 && posy < 430) choice = 0; //退出音乐
        else if(posx > 60 && posx < 160 && posy > 330 && posy < 430) choice = 2; //音量—
        else if(posx > 630 && posx < 730 && posy > 330 && posy < 430) choice = 3; //音量+
        else if(posx > 0 && posx < 130 && posy > 100 && posy < 300) choice = 4; //快退
        else if(posx > 660 && posx < 800 && posy > 100 && posy < 300) choice = 5; //快进

        switch(choice)
        {
            case 0:
                system("killall -SIGKILL mplayer");
                break;
            case 1:
                if(MusicMode == 1)
                {
                    system("killall -SIGSTOP mplayer");
                    MusicMode = 0;
                    show_1152000bmp("./picture/music_stop.bmp", p_lcd);
                }
                else
                {
                    system("killall -SIGCONT mplayer");
                    MusicMode = 1;
                    show_1152000bmp("./picture/music_play.bmp", p_lcd);
                }
                break;
            case 2:
                Send_Cmd("volume -10\n");
                break;
            case 3:
                Send_Cmd("volume +10\n");
                break;
            case 4:
                Send_Cmd("seek -10\n");
                break;
            case 5:
                Send_Cmd("seek +10\n");
                break;
            default:
                printf("Error!\n");
                break;
        }
        
    }while(choice != 0);

    return 0;
}
