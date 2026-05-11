//#include <graphics.h>
//#include <conio.h>
//#include <Windows.h>
//
//int main()
//{
//    initgraph(800, 600);
//
//    //火人
//    int x1 = 100, y1 = 100, vy1 = 0;
//
//    //水人
//    int x2 = 200, y2 = 100, vy2 = 0;
//
//    //平台
//    const int platformCount = 3;
//    int px[platformCount] = { 0, 200, 400 };
//    int py[platformCount] = { 500, 400, 300 };
//    int pw[platformCount] = { 800, 150, 150 };
//    int type[platformCount] = { 0, 1, 2 };
//
//    //终点门
//    int door1_x = 650, door1_y = 440; //火门
//    int door2_x = 720, door2_y = 440; //水门
//
//    bool jump1_last = false;
//    bool jump2_last = false;
//
//    while (true)
//    {
//        BeginBatchDraw();
//        cleardevice();
//
//        //================ 火人 =================
//        int oldY1 = y1;
//        vy1 += 1;
//        if (vy1 > 18) vy1 = 18;
//        y1 += vy1;
//
//        bool onground1 = false;
//
//        for (int i = 0; i < platformCount; i++)
//        {
//            //落地
//            if (vy1 >= 0 &&
//                oldY1 + 50 <= py[i] &&
//                y1 + 50 >= py[i] &&
//                x1 + 50 > px[i] && x1 < px[i] + pw[i])
//            {
//                y1 = py[i] - 50;
//                vy1 = 0;
//                onground1 = true;
//
//                if (type[i] == 2) //火碰水
//                {
//                    x1 = 100; y1 = 100; vy1 = 0;
//                }
//                break;
//            }
//
//            //顶头（修复穿模）
//            if (vy1 < 0 &&
//                oldY1 >= py[i] + 20 &&
//                y1 <= py[i] + 20 &&
//                x1 + 50 > px[i] && x1 < px[i] + pw[i])
//            {
//                y1 = py[i] + 20;
//                vy1 = 0;
//                break;
//            }
//        }
//
//        //左右
//        int vx1 = 0;
//        if (GetAsyncKeyState('A') && !GetAsyncKeyState('D')) vx1 = -5;
//        if (GetAsyncKeyState('D') && !GetAsyncKeyState('A')) vx1 = 5;
//
//        int oldX1 = x1;
//        x1 += vx1;
//
//        for (int i = 0; i < platformCount; i++)
//        {
//            if (oldX1 + 50 <= px[i] &&
//                x1 + 50 >= px[i] &&
//                y1 + 50 > py[i] && y1 < py[i] + 20)
//                x1 = px[i] - 50;
//
//            if (oldX1 >= px[i] + pw[i] &&
//                x1 <= px[i] + pw[i] &&
//                y1 + 50 > py[i] && y1 < py[i] + 20)
//                x1 = px[i] + pw[i];
//        }
//
//        if (x1 < 0) x1 = 0;
//        if (x1 > 750) x1 = 750;
//
//        if (y1 > 600)
//        {
//            x1 = 100; y1 = 100; vy1 = 0;
//        }
//
//        //跳跃
//        bool j1 = GetAsyncKeyState('W');
//        if (j1 && !jump1_last && onground1)
//            vy1 = -15;
//        jump1_last = j1;
//
//        //================ 水人 =================
//        int oldY2 = y2;
//        vy2 += 1;
//        if (vy2 > 18) vy2 = 18;
//        y2 += vy2;
//
//        bool onground2 = false;
//
//        for (int i = 0; i < platformCount; i++)
//        {
//            if (vy2 >= 0 &&
//                oldY2 + 50 <= py[i] &&
//                y2 + 50 >= py[i] &&
//                x2 + 50 > px[i] && x2 < px[i] + pw[i])
//            {
//                y2 = py[i] - 50;
//                vy2 = 0;
//                onground2 = true;
//
//                if (type[i] == 1) //水碰火
//                {
//                    x2 = 200; y2 = 100; vy2 = 0;
//                }
//                break;
//            }
//
//            if (vy2 < 0 &&
//                oldY2 >= py[i] + 20 &&
//                y2 <= py[i] + 20 &&
//                x2 + 50 > px[i] && x2 < px[i] + pw[i])
//            {
//                y2 = py[i] + 20;
//                vy2 = 0;
//                break;
//            }
//        }
//
//        int vx2 = 0;
//        if (GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT)) vx2 = -5;
//        if (GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT)) vx2 = 5;
//
//        int oldX2 = x2;
//        x2 += vx2;
//
//        for (int i = 0; i < platformCount; i++)
//        {
//            if (oldX2 + 50 <= px[i] &&
//                x2 + 50 >= px[i] &&
//                y2 + 50 > py[i] && y2 < py[i] + 20)
//                x2 = px[i] - 50;
//
//            if (oldX2 >= px[i] + pw[i] &&
//                x2 <= px[i] + pw[i] &&
//                y2 + 50 > py[i] && y2 < py[i] + 20)
//                x2 = px[i] + pw[i];
//        }
//
//        if (x2 < 0) x2 = 0;
//        if (x2 > 750) x2 = 750;
//
//        if (y2 > 600)
//        {
//            x2 = 200; y2 = 100; vy2 = 0;
//        }
//
//        bool j2 = GetAsyncKeyState(VK_UP);
//        if (j2 && !jump2_last && onground2)
//            vy2 = -15;
//        jump2_last = j2;
//
//        //================ 门检测 =================
//        bool fireInDoor = (x1 + 50 > door1_x && x1 < door1_x + 40 &&
//            y1 + 50 > door1_y && y1 < door1_y + 60);
//
//        bool waterInDoor = (x2 + 50 > door2_x && x2 < door2_x + 40 &&
//            y2 + 50 > door2_y && y2 < door2_y + 60);
//
//        //================ 画平台 =================
//        for (int i = 0; i < platformCount; i++)
//        {
//            if (type[i] == 0) setfillcolor(RGB(100, 200, 100));
//            if (type[i] == 1) setfillcolor(RED);
//            if (type[i] == 2) setfillcolor(BLUE);
//
//            solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);
//        }
//
//        //画门
//        setfillcolor(RED);
//        solidrectangle(door1_x, door1_y, door1_x + 40, door1_y + 60);
//
//        setfillcolor(BLUE);
//        solidrectangle(door2_x, door2_y, door2_x + 40, door2_y + 60);
//
//        //画人
//        setfillcolor(RED);
//        solidrectangle(x1, y1, x1 + 50, y1 + 50);
//
//        setfillcolor(BLUE);
//        solidrectangle(x2, y2, x2 + 50, y2 + 50);
//
//        //胜利
//        if (fireInDoor && waterInDoor)
//        {
//            settextstyle(40, 0, _T("Consolas"));
//            outtextxy(260, 200, _T("YOU WIN!"));
//        }
//
//        FlushBatchDraw();
//        Sleep(10);
//    }
//
//    closegraph();
//    return 0;
//}