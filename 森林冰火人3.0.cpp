#include <graphics.h>
#include <conio.h>
#include <Windows.h>

//3.0版本更新内容：1.加入了两个平台 2.加入了地面碰撞检测 3.优化了跳跃系统，使角色可以在任意平台上跳动
//                 4.给角色加入了地图边界碰撞检测 5.解决了画面闪烁的问题
//3.0版本存在的问题：角色跳跃时会发生“穿模”，直接从平台下方穿过去，跳到平台上方，穿模还导致跳跃碰撞和地面碰撞有冲突，使角色在跳跃过程中接触到平台后穿模并触发“二段跳”

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

    //定义平台
    const int platformCount = 3;//一共三个平台，其中一个是地面
    int px[platformCount] = { 0, 200, 400 };   // 用一个数组储存三个平台的x位置
    int py[platformCount] = { 500, 400, 300 }; // 用一个数组储存三个平台的y高度
    int pw[platformCount] = { 800, 150, 150 }; // 用一个数组储存三个平台的宽度
    while (true)
    {
        //防闪烁
        BeginBatchDraw(); 

        //清空屏幕
        cleardevice();

        //重力模块
        vy += 1;
        y += vy;

        //地面碰撞判断
        bool onground = false;

        for (int i = 0; i < platformCount; i++)
        {
            if (y + 50 >= py[i] && y + 50 <= py[i] + 20 &&//人物站在平台上
                x + 50 > px[i] && x < px[i] + pw[i])//人物在平台的左右边缘之内
            {
                y = py[i] - 50;
                vy = 0;
                onground = true;
            }
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

        //边界限制
        if (x < 0) x = 0;
        if (x + 50 > 800) x = 800 - 50;

        //跳跃系统
        if (GetAsyncKeyState(VK_SPACE) && onground)//保证人物只能在地面上起跳
        {
            vy = -15; // 向上跳
        }

        //绘制地面
        setfillcolor(RGB(100, 200, 100));//上色

        for (int i = 0; i < platformCount; i++)//用矩形绘制平台
        {
            solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);
        }

        //绘制玩家
        setfillcolor(RED);
        solidrectangle(x, y, x + 50, y + 50);

        //一次性显示
        FlushBatchDraw(); 

        //控制帧率
        Sleep(10);
    }
    closegraph();
    return 0;
}