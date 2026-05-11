#include <graphics.h>
#include <conio.h>
#include <Windows.h>

//2.0版本加入了“跳跃系统和地面检测”

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

        //跳跃系统
        bool onfloor = (y = 50 >= ground);//用来检测人物是否在地面上
        if (GetAsyncKeyState(VK_SPACE) && onfloor)//保证人物只能在地面上起跳
        {
            vy = -15; // 向上跳
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