#include <graphics.h>
#include <conio.h>
#include <Windows.h>
//4.0版本更新内容：1.修复了角色跳跃时的“穿模”问题
//                 2.加入了平台顶部与底部碰撞检测
//                 3.加入了平台左右碰撞检测
//                 4.加入了危险平台（火、水）与死亡重生机制
//                 5.加入了平台类型系统，不同平台拥有不同颜色与功能
//                 6.优化了移动系统，使角色左右移动更加稳定
//
//4.0版本存在的问题：目前仅支持单角色操作，尚未实现双人协作玩法；
//                  平台与角色均为简单矩形，画面表现较为单调；
//                  暂未加入胜利判定与关卡系统

int main()
{
    initgraph(800, 600);//创建800*600的窗口

    //玩家的初始位置
    int x = 100;
    int y = 100;

    //定义垂直速度
    int vy = 0;

    //定义平台
    const int platformCount = 3;//一共三个平台，其中一个是地面
    int px[platformCount] = { 0, 200, 400 };   // 用一个数组储存三个平台的x位置
    int py[platformCount] = { 500, 400, 300 }; // 用一个数组储存三个平台的y高度
    int pw[platformCount] = { 800, 150, 150 }; // 用一个数组储存三个平台的宽度

    //定义平台类型
    int type[platformCount] = { 0, 1, 2 };
    // 0 = 普通
    // 1 = 火
    // 2 = 水

    while (true)
    {
        //防闪烁
        BeginBatchDraw();

        //清空屏幕
        cleardevice();

        //保存旧位置
        int oldY = y;

        //重力模块（向下为正方向）
        vy += 1;
        if (vy > 20) vy = 20; // 限速
        y += vy;

        //地面碰撞判断（站在平台上）
        bool onground = false;

        //死亡判断
        bool died = false;

        for (int i = 0; i < platformCount; i++)
        {
            //从上往下：落地
            if (vy >= 0 &&
                oldY + 50 <= py[i] &&   // 上一帧在平台上方
                y + 50 >= py[i] &&      // 当前帧已经穿到平台位置
                x + 50 > px[i] && x < px[i] + pw[i])
            {
                y = py[i] - 50;
                vy = 0;
                onground = true;
                //危险判定
                if (type[i] == 1 || type[i] == 2)
                {
                    // 重生
                    x = 100;
                    y = 100;
                    vy = 0;
                    died = true;
                    break;//重生后直接退出循环
                }
            }

            //从下往上：顶头
            if (vy < 0 &&
                oldY >= py[i] + 20 &&   // 上一帧在平台下面
                y <= py[i] + 20 &&      // 这一帧撞到平台底部
                x + 50 > px[i] && x < px[i] + pw[i])
            {
                y = py[i] + 20;  // 卡在平台底部
                vy = 0;
            }
        }

        //死亡以后直接进入下一帧
        if (died)
        {
            FlushBatchDraw();
            Sleep(10);
            continue;
        }

        // 左右移动输入
        int vx = 0;
        if (GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT)) vx = -5;
        if (GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT)) vx = 5;
        int oldX = x; //专门用于X碰撞
        x += vx;

        //平台左右碰撞判断
        for (int i = 0; i < platformCount; i++)
        {
            //从左撞墙
            if (oldX + 50 <= px[i] &&//上一帧在平台左侧
                x + 50 >= px[i] &&//这一帧在平台右侧
                oldY + 50 > py[i] && oldY < py[i] + 20)
            {
                x = px[i] - 50;//卡在平台左侧
            }

            //从右撞墙
            if (oldX >= px[i] + pw[i] &&//上一帧在平台右侧
                x <= px[i] + pw[i] &&//这一帧在平台左侧
                oldY + 50 > py[i] && oldY < py[i] + 20)
            {
                x = px[i] + pw[i];//卡在平台右侧
            }
        }

        //边界限制
        if (x < 0) x = 0;
        if (x + 50 > 800) x = 800 - 50;

        //跳跃系统
        if (GetAsyncKeyState(VK_SPACE) && onground)//保证人物只能在地面上起跳
        {
            vy = -15; // 向上跳
        }

        //绘制平台
        for (int i = 0; i < platformCount; i++)
        {
            if (type[i] == 0) setfillcolor(RGB(100, 200, 100)); // 普通
            if (type[i] == 1) setfillcolor(RED);               // 火
            if (type[i] == 2) setfillcolor(BLUE);              // 水

            solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);
        }

        //绘制玩家
        setfillcolor(WHITE);
        solidrectangle(x, y, x + 50, y + 50);

        //一次性显示
        FlushBatchDraw();

        //控制帧率
        Sleep(10);
    }
    closegraph();
    return 0;
}