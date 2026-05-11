#include <graphics.h>
#include <conio.h>
#include <Windows.h>

//1.0版本仅绘制了人物和地面，并且设置了左右移动的操作
int main()
{
    initgraph(800, 600);//创建800*600的窗口

    //玩家的初始位置
    int x = 100;
    int y = 100;

    //定义垂直速度
    int vy = 0;

    //地面高度
    int ground = 500;

    while (true)
    {
        cleardevice();//清空屏幕

        //重力模块
        vy += 1;
        y += vy;

        //地面碰撞判断
        if (y + 50 >= ground)
        {
            y = ground - 50;//接触地面
            vy = 0;//停止下落
        }

        //左右移动模块
        if (GetAsyncKeyState(VK_LEFT))//按下左键
        {
            x -= 5;
        }

        if (GetAsyncKeyState(VK_RIGHT))//按下右键
        {
            x += 5;
        }

        //绘制地面
        setfillcolor(RGB(100, 200, 100));//设置颜色
        solidrectangle(0, ground, 800, 600);//绘制一个矩形，左上角x=0,y=ground,右下角x=800,y=600

        //绘制玩家
        setfillcolor(RED);
        solidrectangle(x, y, x + 50, y + 50);

        //控制帧率
        Sleep(10);
    }
    closegraph();
    return 0;
}