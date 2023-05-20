# 使用说明书

## 硬件要求

粤嵌GEC6818开发板

## 编译命令

linux终端输入：
`arm-linux-gcc ./src/*.c -I ./inc -o ./main.out`

## 环境搭建

1. 使用arm-linux-gcc进行C语言代码编译
2. 使用VScode进行C语言代码编写

## 项目目录

1. bin文件夹：存放编译后的.out二进制文件

2. inc文件夹：存放引用的头文件

3. lib文件夹：存放引用的库文件

4. src文件夹：存放.c代码文件

## 素材文件

1. 图片文件
   
   1. 开始界面
      `./picture/Photo_tip.bmp`
   
   2. 菜单界面
      `./picture/background.bmp`
   
   3. 电子相册
      `./picture/1.bmp`
      `./picture/2.bmp`
      `./picture/3.bmp`
      `./picture/4.bmp`
      `./picture/5.bmp`
   
   4. 音乐播放界面
      `./picture/kjashdk.bmp`
      `./picture/music_play.bmp`
      `./picture/music_stop.bmp`

2. 音频文件
   `./music/Thisstreet.mp3`

3. 视频文件
   `./video/kungfu.avi`

## 功能操作

#### 菜单操作

开机动画为提示界面，大约6秒后进入菜单。

选择屏幕下方四个按钮进入相应界面

#### 音乐播放

* 点击”||“暂停播放
* 点击">"继续播放
* 点击”返回菜单“退出音乐播放返回主界面
* 点击”>>"快进
* 点击“<<"快退
* 点击”-“音量减
* 点击”+“音量加

#### 视频播放

* 点击屏幕：暂停视频播放
* 上滑：退出视频播放

#### 电子相册

* 下一张：向左滑动或点击屏幕右半边
* 上一张：向右滑动或点击屏幕左半边
* 退出：上滑

#### 游戏

一个棋盘，下棋顺序先黑后白，可作为围棋和五子棋棋盘，功能未完善。

当一方获胜时，需要重启系统
