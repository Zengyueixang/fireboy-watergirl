#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <math.h>
#include <stdlib.h>
#include <time.h>

//7.0版本更新内容：1.加入完整暂停菜单系统，可通过ESC暂停与继续游戏
//                 2.加入Game Over失败界面，并加入死亡次数限制机制
//                 3.加入重新开始与退出功能，完善游戏整体流程
//                 4.优化暂停界面UI，采用居中弹窗设计并保留游戏背景
//                 5.加入主菜单鼠标点击交互与按钮音效
//                 6.加入分数系统、钻石收集系统与死亡扣分机制
//                 7.加入随机死亡音效、死亡粒子特效与嘲讽文字系统
//                 8.加入胜利动画、烟花特效与胜利音效播放控制
//                 9.优化角色绘制与平台美术表现，增强整体游戏氛围
//                 10.完善机关按钮与机关墙联动逻辑，提升关卡互动性
//
//7.0版本存在的问题：火人与水人的逻辑代码重复量依然较大，后续应考虑角色类封装；
//                  地图与机关数据仍采用数组硬编码方式，关卡扩展难度较高；
//                  暂停菜单与失败界面仍为键盘操作，尚未加入鼠标按钮交互；
//                  角色缺少行走、跳跃等动态动画表现；
//                  游戏目前仍为单关卡结构，尚未实现关卡切换系统



// 游戏状态
// 0 = 主菜单
// 1 = 正式游戏
// 2 = 暂停菜单
// 3 = 游戏失败
int gameState = 0;

// 绘制主菜单界面
void DrawMainMenu()
{
    BeginBatchDraw();

    // 深蓝背景
    setbkcolor(RGB(20, 25, 45));
    cleardevice();

    // ===== 背景装饰 =====
    setfillcolor(RGB(40, 60, 120));

    for (int i = 0; i < 20; i++)
    {
        solidcircle(rand() % 800, rand() % 600, 2);
    }

    // ===== 游戏标题 =====
    setbkmode(TRANSPARENT);

    settextstyle(60, 0, _T("微软雅黑"));

    const TCHAR* title = _T("冰火双人闯关");

    // 自动计算文字宽高
    int tw = textwidth(title);
    int th = textheight(title);

    // 真正居中
    int tx = (800 - tw) / 2;
    int ty = 140;

    // 阴影
    settextcolor(RGB(60, 60, 60));
    outtextxy(tx + 5, ty + 5, title);

    // 正式标题
    settextcolor(RGB(255, 220, 120));
    outtextxy(tx, ty, title);

    // ===== 开始按钮 =====
    int bx = 275;
    int by = 320;
    int bw = 250;
    int bh = 70;

    // 按钮底框
    setfillcolor(RGB(70, 120, 255));
    solidroundrect(bx, by, bx + bw, by + bh, 20, 20);

    // 按钮高光
    setfillcolor(RGB(120, 170, 255));
    solidroundrect(bx + 5, by + 5, bx + bw - 5, by + 30, 20, 20);

    // 按钮文字
    settextcolor(WHITE);
    settextstyle(36, 0, _T("微软雅黑"));

    const TCHAR* startText = _T("开始游戏");

    int sw = textwidth(startText);
    int sh = textheight(startText);

    outtextxy(
        bx + (bw - sw) / 2,
        by + (bh - sh) / 2 - 3,
        startText
    );

    // ===== 操作提示 =====
    settextcolor(RGB(180, 180, 200));
    settextstyle(22, 0, _T("微软雅黑"));

    const TCHAR* tip = _T("火人：WASD        水人：方向键");

    // 自动居中
    int tipW = textwidth(tip);

    outtextxy(
        (800 - tipW) / 2,
        470,
        tip
    );

    FlushBatchDraw();
}

// 绘制暂停菜单
void DrawPauseMenu()
{
    setbkmode(TRANSPARENT);

    // ===== 中间黑色窗口 =====
    setfillcolor(RGB(40, 40, 40));

    solidroundrect(180, 120, 620, 500, 25, 25);

    // ===== 外边框 =====
    setlinecolor(WHITE);

    roundrect(180, 120, 620, 500, 25, 25);

    // ===== 标题 =====
    settextcolor(RGB(255, 220, 120));

    settextstyle(60, 0, _T("微软雅黑"));

    outtextxy(260, 170, _T("游戏暂停"));

    // ===== 提示 =====
    settextcolor(WHITE);

    settextstyle(30, 0, _T("微软雅黑"));

    outtextxy(260, 300, _T("ESC 继续游戏"));

    outtextxy(260, 360, _T("R 重新开始"));

    outtextxy(260, 420, _T("Q 退出游戏"));
}

// 绘制失败界面
void DrawGameOver()
{
    cleardevice();

    setbkmode(TRANSPARENT);

    // GAME OVER
    settextcolor(RED);
    settextstyle(70, 0, _T("微软雅黑"));

    outtextxy(180, 140, _T("GAME OVER"));

    // 提示
    settextcolor(WHITE);
    settextstyle(30, 0, _T("微软雅黑"));

    outtextxy(220, 300, _T("死亡次数过多"));

    outtextxy(220, 360, _T("R 重新开始"));
    outtextxy(220, 420, _T("Q 退出游戏"));
}

int main()
{
    MOUSEMSG m;

    initgraph(800, 600);

    // 新加：美化背景
    setbkcolor(RGB(30, 30, 50));
    cleardevice();

    srand((unsigned)time(NULL));

    //火人
    int x1 = 0, y1 = 430, vy1 = 0;

    //水人
    int x2 = 0, y2 = 500, vy2 = 0;

    //================ 平台（无左右边框） =================
    const int platformCount = 27;

    int px[platformCount] = {
        0, 0,300,375, 500,
        575,700,450,375,275,250,250,250,250,250,175,0,175,740,400,0,300,390,390,390,390,390
    };

    int py[platformCount] = {
        500, 580, 580,580,580,
        580,480,400,400,400,400,380,360,340,320,300,400,400,300,230,200,100,0,20,40,60,80
    };

    int pw[platformCount] = {
        200, 350, 75,125,75,
        225,100,112,75,100,25,25,25,25,25,100,60,75,60,210,100,200,20,20,20,20,20
    };

    int type[platformCount] = {
        1, 0,1,0,2,
        0,0,0,3,0,0,0,0,0,0,0,2,2,0,0,1,0,0,0,0,0,0
    };

    //按钮数组
    const int buttonCount = 5;
    int ax[buttonCount] = {
        200,500,760,40,470
    };
    int ay[buttonCount] = {
        380,380,280,180,80
    };
    int aw[buttonCount] = {
        20,20,20,20,20
    };
    int atype[buttonCount] = {
        4,4,4,4,4
    };

    //机关墙数组
    const int presswallCount = 5;
    int wx[presswallCount] = {
        400,675,50,650,200
    };
    int wy[presswallCount] = {
        250,300,400,140,140
    };
    int ww[presswallCount] = {
        20,65,125,50,50
    };
    int wtype[presswallCount] = {
        0,1,1,1,1
    };

    // 双金色终点
    int end1_x = 340, end1_y = 50, end1_w = 50, end1_h = 50; //火人终点
    int end2_x = 410, end2_y = 50, end2_w = 50, end2_h = 50; //水人终点

    // 分数 + 钻石系统
    int score = 0;

    // 死亡次数
    int deathCount = 0;

    // 钻石数量
    const int diamondCnt = 8;
    // 钻石坐标+是否被吃掉
    int diaX[diamondCnt] = { 120,420,600,220,520,100,350,700 };
    int diaY[diamondCnt] = { 460,300,520,200,150,280,180,420 };
    bool diaAlive[diamondCnt] = { true,true,true,true,true,true,true,true };

    //嘲讽文本数组
    const TCHAR* taunt[] = { _T("人机"),_T("菜"),_T("咋又死了"),_T("扣分") };

    int particleCnt = 0;
    //粒子坐标+生命+辐射速度
    float px_[50] = { 0 };
    float py_[50] = { 0 };
    float vx_[50] = { 0 };  // <-- 新增
    float vy_[50] = { 0 };  // <-- 新增
    int pLife[50] = { 0 };

    // 死亡嘲讽显示时间
    int showTaunt1 = 0;
    int showTaunt2 = 0;
    // 放在 showTaunt1、showTaunt2 旁边
    int lastTaunt1 = 0;
    int lastTaunt2 = 0;

    bool jump1_last = false;
    bool jump2_last = false;
    bool lastPress[buttonCount] = { false };

    //================ 主菜单循环 ================
    while (gameState == 0)
    {
        DrawMainMenu();

        // 鼠标消息
        if (MouseHit())
        {
            m = GetMouseMsg();

            // 左键点击
            if (m.uMsg == WM_LBUTTONDOWN)
            {
                // 判断是否点到“开始游戏”
                if (m.x >= 275 && m.x <= 525 &&
                    m.y >= 320 && m.y <= 390)
                {
                    gameState = 1;

                    // 点击音效（可选）
                    PlaySound(
                        TEXT("button.wav"),
                        NULL,
                        SND_FILENAME | SND_ASYNC
                    );
                }
            }
        }

        FlushBatchDraw();
        Sleep(16);
    }

    //================ 正式游戏循环 ================
    while (true)
    {
        //================ 暂停菜单 =================
        if (gameState == 2)
        {
            BeginBatchDraw();

            DrawPauseMenu();

            // ESC继续
            if (GetAsyncKeyState(VK_ESCAPE) & 1)
            {
                gameState = 1;
            }

            // R重新开始
            if (GetAsyncKeyState('R') & 1)
            {
                // 重置火人
                x1 = 0;
                y1 = 430;
                vy1 = 0;

                // 重置水人
                x2 = 0;
                y2 = 500;
                vy2 = 0;

                // 分数归零
                score = 0;
                deathCount = 0;

                // 钻石刷新
                for (int i = 0; i < diamondCnt; i++)
                {
                    diaAlive[i] = true;
                }

                gameState = 1;
            }

            // Q退出
            if (GetAsyncKeyState('Q') & 1)
            {
                closegraph();
                return 0;
            }

            FlushBatchDraw();
            Sleep(10);

            continue;
        }

        //================ 游戏失败 =================
        if (gameState == 3)
        {
            BeginBatchDraw();

            DrawGameOver();

            // R重新开始
            if (GetAsyncKeyState('R') & 1)
            {
                // 重置火人
                x1 = 0;
                y1 = 430;
                vy1 = 0;

                // 重置水人
                x2 = 0;
                y2 = 500;
                vy2 = 0;

                // 分数归零
                score = 0;
                deathCount = 0;

                // 钻石刷新
                for (int i = 0; i < diamondCnt; i++)
                {
                    diaAlive[i] = true;
                }

                gameState = 1;
            }

            // Q退出
            if (GetAsyncKeyState('Q') & 1)
            {
                closegraph();
                return 0;
            }

            FlushBatchDraw();
            Sleep(10);

            continue;
        }

        BeginBatchDraw();
        setbkmode(TRANSPARENT);
        cleardevice();

        // ESC暂停
        if (GetAsyncKeyState(VK_ESCAPE) & 1)
        {
            gameState = 2;
        }

        //================ 按钮状态（放这里！） =================
        bool press[buttonCount];
        for (int i = 0; i < buttonCount; i++)
            press[i] = false;

        for (int i = 0; i < buttonCount; i++)
        {
            //火人
            if (x1 + 50 > ax[i] && x1 < ax[i] + aw[i] &&
                y1 + 50 >= ay[i] && y1 <= ay[i] + 20)
                press[i] = true;

            //水人
            if (x2 + 50 > ax[i] && x2 < ax[i] + aw[i] &&
                y2 + 50 >= ay[i] && y2 <= ay[i] + 20)
                press[i] = true;
        }
        // ===== 按钮音效 =====
        for (int i = 0; i < buttonCount; i++)
        {
            // 上一帧没按，这一帧按下
            if (!lastPress[i] && press[i])
            {
                PlaySound(
                    TEXT("button.wav"),
                    NULL,
                    SND_FILENAME | SND_ASYNC
                );
            }

            // 更新状态
            lastPress[i] = press[i];
        }

        //================ 墙状态 =================
        bool wallActive[presswallCount];

        // 默认：墙0显示，墙1隐藏，墙2隐藏，墙3隐藏
        wallActive[0] = true;
        wallActive[1] = false;
        wallActive[2] = false;
        wallActive[3] = false;
        wallActive[4] = false;

        // 按钮0（200）：墙1 出现
        if (press[0])
        {
            wallActive[1] = true;
        }

        // 按钮1（500）：墙0 消失
        if (press[1])
        {
            wallActive[0] = false;
        }

        // 按钮2（760）：墙2 出现
        if (press[2])
        {
            wallActive[2] = true;
        }

        // 按钮3（40）：墙3 出现
        if (press[3])
        {
            wallActive[3] = true;
        }
        // 按钮4：墙4 出现
        if (press[4])
        {
            wallActive[4] = true;
        }

        //================ 火人 =================
        int oldY1 = y1;
        vy1 += 1;
        if (vy1 > 18) vy1 = 18;
        y1 += vy1;

        bool onground1 = false;

        for (int i = 0; i < platformCount; i++)
        {
            if (vy1 >= 0 &&
                oldY1 + 50 <= py[i] &&
                y1 + 50 >= py[i] &&
                x1 + 50 > px[i] && x1 < px[i] + pw[i])
            {
                y1 = py[i] - 50;
                vy1 = 0;
                onground1 = true;
            }

            if (vy1 < 0 &&
                oldY1 >= py[i] + 20 &&
                y1 <= py[i] + 20 &&
                x1 + 50 > px[i] && x1 < px[i] + pw[i])
            {
                y1 = py[i] + 20;
                vy1 = 0;
                break;
            }
        }
        //机关墙碰撞
        for (int i = 0; i < presswallCount; i++)
        {
            if (!wallActive[i]) continue; //消失就不碰撞

            //落地
            if (vy1 >= 0 &&
                oldY1 + 50 <= wy[i] &&
                y1 + 50 >= wy[i] &&
                x1 + 50 > wx[i] && x1 < wx[i] + ww[i])
            {
                y1 = wy[i] - 50;
                vy1 = 0;
                onground1 = true;
            }

            //顶头
            if (vy1 < 0 &&
                oldY1 >= (i == 0 ? wy[i] + 100 : wy[i] + 20) &&
                y1 <= (i == 0 ? wy[i] + 100 : wy[i] + 20) &&
                x1 + 50 > wx[i] && x1 < wx[i] + ww[i])
            {
                y1 = (i == 0 ? wy[i] + 100 : wy[i] + 20);
                vy1 = 0;
            }
        }

        int vx1 = 0;
        if (GetAsyncKeyState('A') && !GetAsyncKeyState('D')) vx1 = -5;
        if (GetAsyncKeyState('D') && !GetAsyncKeyState('A')) vx1 = 5;

        int oldX1 = x1;
        x1 += vx1;

        for (int i = 0; i < platformCount; i++)
        {
            if (oldX1 + 50 <= px[i] &&
                x1 + 50 >= px[i] &&
                y1 + 50 > py[i] && y1 < py[i] + 20)
                x1 = px[i] - 50;

            if (oldX1 >= px[i] + pw[i] &&
                x1 <= px[i] + pw[i] &&
                y1 + 50 > py[i] && y1 < py[i] + 20)
                x1 = px[i] + pw[i];
        }
        //机关墙碰撞（左右）
        for (int i = 0; i < presswallCount; i++)
        {
            if (!wallActive[i]) continue;

            int wh = (i == 0) ? 100 : 20;

            if (oldX1 + 50 <= wx[i] &&
                x1 + 50 >= wx[i] &&
                y1 + 50 > wy[i] && y1 < wy[i] + wh)
                x1 = wx[i] - 50;

            if (oldX1 >= wx[i] + ww[i] &&
                x1 <= wx[i] + ww[i] &&
                y1 + 50 > wy[i] && y1 < wy[i] + wh)
                x1 = wx[i] + ww[i];
        }

        //左右限制（不会掉出屏幕）
        if (x1 < 0) x1 = 0;
        if (x1 > 750) x1 = 750;

        if (y1 > 600)
        {
            x1 = 0; y1 = 430; vy1 = 0;
        }

        bool j1 = GetAsyncKeyState('W');

        if (j1 && !jump1_last && onground1)
        {
            vy1 = -15;

            PlaySound(
                TEXT("jump.wav"),
                NULL,
                SND_FILENAME | SND_ASYNC | SND_NOSTOP
            );
        }

        jump1_last = j1;
        // ====== 火人最终死亡检测（修复版）======
        for (int i = 0; i < platformCount; i++)
        {
            if (x1 + 50 > px[i] && x1 < px[i] + pw[i] &&
                y1 + 50 >= py[i] && y1 + 50 <= py[i] + 5)
            {
                if (type[i] == 2 || type[i] == 3)
                {
                    // 先保存死亡位置
                    float deadX = (float)(x1 + 25);
                    float deadY = (float)(y1 + 25);

                    // 复活
                    x1 = 0;
                    y1 = 430;
                    vy1 = 0;
                    score -= 5;
                    deathCount++;
                    int r = rand() % 4;

                    if (r == 0)
                    {
                        PlaySound(
                            TEXT("dead1.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );
                    }

                    else if (r == 1)
                    {
                        PlaySound(
                            TEXT("dead2.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );
                    }

                    else if (r == 2)
                    {
                        PlaySound(
                            TEXT("dead3.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );
                    }

                    else if (r == 3)
                    {
                        PlaySound(
                            TEXT("dead4.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );
                    }
                    // 辐射炸开粒子（无下落）
                    for (int k = 0; k < 30; k++)
                    {
                        float angle = (rand() % 360) * 3.14159f / 180.0f;
                        float speed = 2.0f + (rand() % 3);

                        px_[particleCnt] = deadX;
                        py_[particleCnt] = deadY;
                        vx_[particleCnt] = cos(angle) * speed;
                        vy_[particleCnt] = sin(angle) * speed;
                        pLife[particleCnt] = 40;

                        particleCnt = (particleCnt + 1) % 50;
                    }
                    showTaunt1 = 50;
                    lastTaunt1 = rand() % 4;  // <-- 加这一句
                }
            }
        }

        //================ 水人 =================
        int oldY2 = y2;
        vy2 += 1;
        if (vy2 > 18) vy2 = 18;
        y2 += vy2;

        bool onground2 = false;

        for (int i = 0; i < platformCount; i++)
        {
            if (vy2 >= 0 &&
                oldY2 + 50 <= py[i] &&
                y2 + 50 >= py[i] &&
                x2 + 50 > px[i] && x2 < px[i] + pw[i])
            {
                y2 = py[i] - 50;
                vy2 = 0;
                onground2 = true;
            }
            if (vy2 < 0 &&
                oldY2 >= py[i] + 20 &&
                y2 <= py[i] + 20 &&
                x2 + 50 > px[i] && x2 < px[i] + pw[i])
            {
                y2 = py[i] + 20;
                vy2 = 0;
                break;
            }
        }

        //机关墙
        for (int i = 0; i < presswallCount; i++)
        {
            if (!wallActive[i]) continue;

            if (vy2 >= 0 &&
                oldY2 + 50 <= wy[i] &&
                y2 + 50 >= wy[i] &&
                x2 + 50 > wx[i] && x2 < wx[i] + ww[i])
            {
                y2 = wy[i] - 50;
                vy2 = 0;
                onground2 = true;
            }

            if (vy2 < 0 &&
                oldY2 >= (i == 0 ? wy[i] + 100 : wy[i] + 20) &&
                y2 <= (i == 0 ? wy[i] + 100 : wy[i] + 20) &&
                x2 + 50 > wx[i] && x2 < wx[i] + ww[i])
            {
                y2 = (i == 0 ? wy[i] + 100 : wy[i] + 20);
                vy2 = 0;
            }
        }

        int vx2 = 0;
        if (GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT)) vx2 = -5;
        if (GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT)) vx2 = 5;

        int oldX2 = x2;
        x2 += vx2;

        for (int i = 0; i < platformCount; i++)
        {
            if (oldX2 + 50 <= px[i] &&
                x2 + 50 >= px[i] &&
                y2 + 50 > py[i] && y2 < py[i] + 20)
                x2 = px[i] - 50;

            if (oldX2 >= px[i] + pw[i] &&
                x2 <= px[i] + pw[i] &&
                y2 + 50 > py[i] && y2 < py[i] + 20)
                x2 = px[i] + pw[i];
        }
        //机关墙
        for (int i = 0; i < presswallCount; i++)
        {
            if (!wallActive[i]) continue;

            int wh = (i == 0) ? 100 : 20;

            if (oldX2 + 50 <= wx[i] &&
                x2 + 50 >= wx[i] &&
                y2 + 50 > wy[i] && y2 < wy[i] + wh)
                x2 = wx[i] - 50;

            if (oldX2 >= wx[i] + ww[i] &&
                x2 <= wx[i] + ww[i] &&
                y2 + 50 > wy[i] && y2 < wy[i] + wh)
                x2 = wx[i] + ww[i];
        }

        //左右限制
        if (x2 < 0) x2 = 0;
        if (x2 > 750) x2 = 750;

        if (y2 > 600)
        {
            x2 = 0; y2 = 500; vy2 = 0;
        }

        bool j2 = GetAsyncKeyState(VK_UP);

        if (j2 && !jump2_last && onground2)
        {
            vy2 = -15;

            PlaySound(
                TEXT("jump.wav"),
                NULL,
                SND_FILENAME | SND_ASYNC | SND_NOSTOP
            );
        }

        jump2_last = j2;
        // ====== 水人最终死亡检测（修复版）======
        for (int i = 0; i < platformCount; i++)
        {
            if (x2 + 50 > px[i] && x2 < px[i] + pw[i] &&
                y2 + 50 >= py[i] && y2 + 50 <= py[i] + 5)  // ?只检测脚
            {
                if (type[i] == 1 || type[i] == 3)
                {
                    // 先保存死亡位置
                    float deadX = (float)(x2 + 25);
                    float deadY = (float)(y2 + 25);

                    // 复活
                    x2 = 0;
                    y2 = 500;
                    vy2 = 0;
                    score -= 5;
                    deathCount++;
                    int r = rand() % 4;

                    if (r == 0)
                    {
                        PlaySound(
                            TEXT("dead1.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );
                    }

                    else if (r == 1)
                    {
                        PlaySound(
                            TEXT("dead2.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );
                    }

                    else if (r == 2)
                    {
                        PlaySound(
                            TEXT("dead3.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );
                    }

                    else if (r == 3)
                    {
                        PlaySound(
                            TEXT("dead4.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );
                    }
                    // 辐射炸开粒子（无下落）
                    for (int k = 0; k < 30; k++)
                    {
                        float angle = (rand() % 360) * 3.14159f / 180.0f;
                        float speed = 2.0f + (rand() % 3);

                        px_[particleCnt] = deadX;
                        py_[particleCnt] = deadY;
                        vx_[particleCnt] = cos(angle) * speed;
                        vy_[particleCnt] = sin(angle) * speed;
                        pLife[particleCnt] = 40;

                        particleCnt = (particleCnt + 1) % 50;
                    }
                    showTaunt2 = 50;
                    lastTaunt2 = rand() % 4;  // <-- 加这一句
                }
            }
        }



        // 钻石拾取加分
        for (int i = 0; i < diamondCnt; i++)
        {
            if (!diaAlive[i]) continue;

            //火人拾取
            if (x1 + 50 > diaX[i] && x1 < diaX[i] + 20 &&
                y1 + 50 > diaY[i] && y1 < diaY[i] + 20)
            {
                diaAlive[i] = false;
                score += 5;
                PlaySound(
                    TEXT("diamond.wav"),
                    NULL,
                    SND_FILENAME | SND_ASYNC
                );
            }

            //水人拾取
            if (x2 + 50 > diaX[i] && x2 < diaX[i] + 20 &&
                y2 + 50 > diaY[i] && y2 < diaY[i] + 20)
            {
                diaAlive[i] = false;
                score += 5;
                PlaySound(
                    TEXT("diamond.wav"),
                    NULL,
                    SND_FILENAME | SND_ASYNC
                );
            }
        }

        //================ 画平台 =================
        for (int i = 0; i < platformCount; i++)
        {
            // 草地
            if (type[i] == 0)
            {
                setfillcolor(RGB(90, 180, 90));
                solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);

                // 上边高光
                setlinecolor(RGB(180, 255, 180));
                line(px[i], py[i], px[i] + pw[i], py[i]);
            }

            // 火池
            if (type[i] == 1)
            {
                setfillcolor(RGB(220, 60, 60));
                solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);

                setlinecolor(YELLOW);
                line(px[i], py[i], px[i] + pw[i], py[i]);
            }

            // 水池
            if (type[i] == 2)
            {
                setfillcolor(RGB(50, 120, 255));
                solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);

                setlinecolor(WHITE);
                line(px[i], py[i], px[i] + pw[i], py[i]);
            }
            // 紫毒池
            if (type[i] == 3)
            {
                setfillcolor(RGB(160, 60, 200));
                solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);

                // 紫色高光
                setlinecolor(RGB(255, 120, 255));
                line(px[i], py[i], px[i] + pw[i], py[i]);
            }
        }

        //画按钮
        setfillcolor(RGB(200, 200, 200));
        //画按钮
        for (int i = 0; i < buttonCount; i++)
        {
            if (press[i])
            {
                setfillcolor(RGB(255, 130, 0)); // 按下橙
                solidrectangle(ax[i], ay[i] + 8, ax[i] + aw[i], ay[i] + 20);
            }
            else
            {
                setfillcolor(RGB(255, 200, 0)); // 常态金黄
                solidrectangle(ax[i], ay[i], ax[i] + aw[i], ay[i] + 20);
            }
        }

        // 绘制钻石
        for (int i = 0; i < diamondCnt; i++)
        {
            if (diaAlive[i])
            {
                setfillcolor(RGB(0, 200, 255));
                solidrectangle(diaX[i], diaY[i], diaX[i] + 20, diaY[i] + 20);
            }
        }

        // 绘制分数
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        setfont(24, 0, _T("黑体"));
        TCHAR str[50];
        _stprintf_s(str, _countof(str), _T("分数：%d"), score);
        outtextxy(15, 15, str);

        //两个金色终点
        setfillcolor(RGB(255, 235, 100));
        solidrectangle(end1_x, end1_y, end1_x + end1_w, end1_y + end1_h);
        solidrectangle(end2_x, end2_y, end2_x + end2_w, end2_y + end2_h);

        //画机关墙（只画存在的）
        setfillcolor(RGB(50, 50, 50));
        for (int i = 0; i < presswallCount; i++)
        {
            if (wallActive[i]) // 只有状态为true才画
            {
                if (i == 0)
                {
                    // 墙0：高度80
                    solidrectangle(wx[i], wy[i], wx[i] + ww[i], wy[i] + 100);
                }
                else if (i == 1)
                {
                    // 墙1：高度20
                    solidrectangle(wx[i], wy[i], wx[i] + ww[i], wy[i] + 20);
                }
                else if (i == 2)
                {
                    // 墙2：高度20
                    solidrectangle(wx[i], wy[i], wx[i] + ww[i], wy[i] + 20);
                }
                else if (i == 3)
                {
                    // 墙3：高度20
                    solidrectangle(wx[i], wy[i], wx[i] + ww[i], wy[i] + 20);
                }
                else if (i == 4)
                {
                    // 墙4：高度20
                    solidrectangle(wx[i], wy[i], wx[i] + ww[i], wy[i] + 20);
                }
            }
        }

        //画人
        //================ 火人 =================

        // 身体
        setfillcolor(RGB(255, 120, 60));
        solidroundrect(x1, y1, x1 + 50, y1 + 50, 12, 12);

        // 脸部渐变
        setfillcolor(RGB(255, 180, 120));
        solidcircle(x1 + 25, y1 + 22, 13);

        // 眼白
        setfillcolor(WHITE);
        solidcircle(x1 + 17, y1 + 18, 4);
        solidcircle(x1 + 33, y1 + 18, 4);

        // 黑眼珠
        setfillcolor(BLACK);
        solidcircle(x1 + 17, y1 + 18, 2);
        solidcircle(x1 + 33, y1 + 18, 2);

        // 嘴巴
        arc(x1 + 15, y1 + 22, x1 + 35, y1 + 38, 3.14, 0);

        // 火焰头
        setfillcolor(YELLOW);
        solidcircle(x1 + 25, y1 - 5, 8);

        setfillcolor(RGB(255, 100, 0));
        solidcircle(x1 + 25, y1, 5);

        //================ 水人 =================

// 身体
        setfillcolor(RGB(80, 160, 255));
        solidroundrect(x2, y2, x2 + 50, y2 + 50, 12, 12);

        // 脸部高光
        setfillcolor(RGB(180, 220, 255));
        solidcircle(x2 + 25, y2 + 22, 13);

        // 眼白
        setfillcolor(WHITE);
        solidcircle(x2 + 17, y2 + 18, 4);
        solidcircle(x2 + 33, y2 + 18, 4);

        // 黑眼珠
        setfillcolor(BLACK);
        solidcircle(x2 + 17, y2 + 18, 2);
        solidcircle(x2 + 33, y2 + 18, 2);

        // 嘴巴
        arc(x2 + 15, y2 + 22, x2 + 35, y2 + 38, 3.14, 0);

        // 水滴头
        setfillcolor(WHITE);
        solidellipse(x2 + 20, y2 - 8, x2 + 30, y2 + 5);


        // ========= 正确双终点判定（人物完全在金块内） =========
        bool ok1 = (x1 >= end1_x && x1 + 50 <= end1_x + end1_w &&
            y1 >= end1_y && y1 + 50 <= end1_y + end1_h);

        bool ok2 = (x2 >= end2_x && x2 + 50 <= end2_x + end2_w &&
            y2 >= end2_y && y2 + 50 <= end2_y + end2_h);

        static int winFrame = 0;
        static bool winPlayed = false;

        if (ok1 && ok2)
        {
            if (!winPlayed)
            {
                PlaySound(
                    TEXT("win.wav"),
                    NULL,
                    SND_FILENAME | SND_ASYNC | SND_NOSTOP
                );

                winPlayed = true;
            }

            winFrame++;

            // 画面定格：不再更新人物物理
            vy1 = 0;
            vy2 = 0;
            vx1 = 0;
            vx2 = 0;

            // ========= 随机烟花特效 =========
            for (int i = 0; i < 5; i++)
            {
                int rX = rand() % 800;
                int rY = rand() % 600;
                int colorR = rand() % 255;
                int colorG = rand() % 255;
                int colorB = rand() % 255;
                setfillcolor(RGB(colorR, colorG, colorB));
                solidcircle(rX, rY, 3);
            }

            // ========= 文字从小到大动画 =========
            int size = 20 + winFrame * 2;
            if (size > 90) size = 90;

            setbkmode(TRANSPARENT);
            settextcolor(RGB(255, 215, 0));
            setfont(size, 0, _T("黑体"));

            // 文字居中
            const TCHAR* winStr = _T("成功过关");
            int offset = (int)(size * 1.8);
            outtextxy(400 - offset, 260, winStr);

            // 5秒后关闭游戏
            if (winFrame > 250)
            {
                closegraph();
                return 0;
            }
        }

        //===== 辐射烟花粒子（无下落）=====
        for (int i = 0; i < 50; i++)
        {
            if (pLife[i] > 0)
            {
                // 只向外飞，不下落
                px_[i] += vx_[i];
                py_[i] += vy_[i];

                // 慢慢减速
                vx_[i] *= 0.93f;
                vy_[i] *= 0.93f;

                pLife[i]--;

                setfillcolor(WHITE);
                solidcircle(px_[i], py_[i], 2);
            }
        }

        //===== 死亡嘲讽文字 =====
        setbkmode(TRANSPARENT);
        setfont(32, 0, _T("微软雅黑"));   // 字体变大
        settextcolor(WHITE);               // 白色字体

        if (showTaunt1 > 0)
        {
            showTaunt1--;
            // 火人头顶居中
            int tx1 = x1 + 10;
            int ty1 = y1 - 30;
            outtextxy(tx1, ty1, taunt[lastTaunt1]);
        }
        if (showTaunt2 > 0)
        {
            showTaunt2--;
            // 水人头顶居中
            int tx2 = x2 + 10;
            int ty2 = y2 - 30;
            outtextxy(tx2, ty2, taunt[lastTaunt2]);
        }

        // 死亡次数过多
        if (deathCount >= 10)
        {
            gameState = 3;
        }

        FlushBatchDraw();
        Sleep(10);
    }
    closegraph();
    return 0;
}
