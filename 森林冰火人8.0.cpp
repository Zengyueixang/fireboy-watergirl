#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <math.h>
#include <stdlib.h>
#include <time.h>

//8.0版本更新内容：1.全面优化主菜单界面，加入动态背景、标题美化、开始按钮悬停反馈
//                 2.优化暂停菜单界面，加入居中弹窗、时间显示、按钮悬停反馈和鼠标点击操作
//                 3.优化失败结算界面，显示最终分数、死亡次数、钻石数量和坚持时间
//                 4.优化胜利结算界面，加入通关数据统计、评级系统和胜利动画效果
//                 5.加入完整计时系统，可记录玩家通关时间和失败时坚持时间
//                 6.加入通关评级系统，根据时间、死亡次数、钻石数量和分数评定S/A/B/C等级
//                 7.优化游戏内HUD，将分数、时间、死亡次数和钻石数量集中显示
//                 8.优化角色建模，将火人和水人改为更完整的Q版人物形象
//                 9.加入角色移动与跳跃时的简单动态效果，增强人物表现力
//                 10.优化平台、火池、水池、毒池、按钮、机关墙、钻石和终点门的美术表现
//                 11.加入钻石发光动画和统一菱形外观，使收集目标更加明显
//                 12.调整钻石位置，让钻石不仅用于加分，也成为提高通关难度的挑战点
//                 13.加入终点门靠近高亮反馈，两名角色进入终点时有更明显的视觉提示
//                 14.加入机关触发提示，玩家踩下按钮后会显示“启动”或“关闭”提示
//                 15.优化死亡反馈，保留扣分、死亡音效、粒子效果、震动和嘲讽文字
//                 16.修复死亡检测可能重复扣分的问题，提高游戏稳定性
//                 17.统一重新开始逻辑，减少暂停、失败、胜利界面中的重复代码
//                 18.统一钻石统计和评级计算逻辑，使结算数据更加稳定
//
//相对7.0的主要提升：8.0不只是增加功能，而是对游戏的完整体验进行了整体升级。
//                  7.0主要解决了游戏流程问题，例如暂停、失败、重开、胜利和基础音效；
//                  8.0则重点优化了界面表现、交互反馈、结算系统、角色美术和代码稳定性。
//                  现在游戏已经从“能完整运行的版本”提升到了“有较完整体验的成品雏形”。
//
//8.0版本仍存在的问题：代码整体仍集中在main函数中，后续可以继续进行函数封装；
//                  火人和水人的移动、碰撞、死亡逻辑仍有大量重复；
//                  地图、按钮、机关墙、钻石等数据仍使用数组硬编码，后续制作多关卡时不够方便；
//                  当前仍是单关卡结构，后续可以考虑加入选关界面和关卡切换系统；
//                  音效、界面、胜利失败流程已经较完整，但整体代码结构还可以继续优化。



// 游戏状态
// 0 = 主菜单
// 1 = 正式游戏
// 2 = 暂停菜单
// 3 = 游戏失败
int gameState = 0;

// 绘制主菜单界面
void DrawMainMenu(bool startHover)
{
    BeginBatchDraw();

    static int menuFrame = 0;
    menuFrame++;

    //================ 背景 =================
    setbkcolor(RGB(18, 24, 45));
    cleardevice();
    setbkmode(TRANSPARENT);

    // 深色渐变背景
    for (int y = 0; y < 600; y += 20)
    {
        int c = 20 + y / 35;
        setfillcolor(RGB(18, 24, 45 + c / 2));
        solidrectangle(0, y, 800, y + 20);
    }

    // 背景星点，不随机闪烁
    for (int i = 0; i < 45; i++)
    {
        int sx = (i * 97 + 80) % 800;
        int sy = (i * 53 + menuFrame / 2) % 600;

        setfillcolor(RGB(90, 130, 200));
        solidcircle(sx, sy, 1 + i % 2);
    }

    //================ 左右元素装饰 =================

    // 左侧火光
    setfillcolor(RGB(90, 35, 25));
    solidcircle(105, 165, 65);

    setfillcolor(RGB(190, 70, 35));
    solidcircle(105, 165, 42);

    setfillcolor(RGB(255, 165, 60));
    solidcircle(105, 165, 20);

    // 右侧水光
    setfillcolor(RGB(25, 45, 90));
    solidcircle(695, 165, 65);

    setfillcolor(RGB(45, 115, 210));
    solidcircle(695, 165, 42);

    setfillcolor(RGB(150, 225, 255));
    solidcircle(695, 165, 20);

    //================ 游戏标题 =================

    const TCHAR* title = _T("冰火双人闯关");

    // 标题字体改成更有游戏感的字体
    settextstyle(70, 0, _T("华文琥珀"));

    int titleW = textwidth(title);
    int titleX = (800 - titleW) / 2;
    int titleY = 125;

    // 标题厚阴影
    settextcolor(RGB(35, 25, 25));
    outtextxy(titleX + 5, titleY + 6, title);

    // 标题外层暗红边
    settextcolor(RGB(150, 65, 35));
    outtextxy(titleX + 2, titleY + 2, title);

    // 标题主体
    settextcolor(RGB(255, 225, 115));
    outtextxy(titleX, titleY, title);

    // 标题下划线
    setlinecolor(RGB(255, 210, 90));
    line(250, 220, 550, 220);

    setlinecolor(RGB(90, 140, 220));
    line(290, 230, 510, 230);

    //================ 开始按钮 =================

    int bx = 275;
    int by = 320;
    int bw = 250;
    int bh = 70;

    // 按钮阴影
    setfillcolor(RGB(12, 16, 28));
    solidroundrect(bx + 6, by + 8, bx + bw + 6, by + bh + 8, 22, 22);

    // 按钮主体：鼠标悬停时变亮
    if (startHover)
    {
        setfillcolor(RGB(90, 150, 255));
    }
    else
    {
        setfillcolor(RGB(65, 120, 245));
    }

    solidroundrect(bx, by, bx + bw, by + bh, 22, 22);

    // 按钮高光：鼠标悬停时更亮
    if (startHover)
    {
        setfillcolor(RGB(160, 205, 255));
    }
    else
    {
        setfillcolor(RGB(125, 180, 255));
    }

    solidroundrect(bx + 8, by + 8, bx + bw - 8, by + 32, 18, 18);

    // 按钮边框
    setlinecolor(RGB(190, 220, 255));
    roundrect(bx, by, bx + bw, by + bh, 22, 22);

    // 鼠标悬停时增加外发光边框
    if (startHover)
    {
        setlinecolor(RGB(230, 245, 255));
        roundrect(bx - 3, by - 3, bx + bw + 3, by + bh + 3, 24, 24);
    }

    // 按钮文字
    const TCHAR* startText = _T("开始游戏");

    settextstyle(36, 0, _T("黑体"));

    int startW = textwidth(startText);
    int startH = textheight(startText);

    // 按钮文字阴影
    settextcolor(RGB(40, 60, 120));
    outtextxy(
        bx + (bw - startW) / 2 + 2,
        by + (bh - startH) / 2 + 1,
        startText
    );

    // 按钮文字主体
    settextcolor(WHITE);
    outtextxy(
        bx + (bw - startW) / 2,
        by + (bh - startH) / 2 - 1,
        startText
    );

    //================ 操作说明面板 =================

    int panelW = 430;
    int panelH = 75;
    int panelX = (800 - panelW) / 2;   // 自动居中
    int panelY = 435;

    // 面板主体
    setfillcolor(RGB(28, 35, 62));
    solidroundrect(
        panelX,
        panelY,
        panelX + panelW,
        panelY + panelH,
        18,
        18
    );

    // 面板边框
    setlinecolor(RGB(80, 100, 150));
    roundrect(
        panelX,
        panelY,
        panelX + panelW,
        panelY + panelH,
        18,
        18
    );

    // 操作文字
    settextstyle(22, 0, _T("黑体"));

    const TCHAR* tip1 = _T("火人操作：W A S D");
    const TCHAR* tip2 = _T("水人操作：方向键");

    // 计算两行文字整体高度
    int tip1W = textwidth(tip1);
    int tip2W = textwidth(tip2);
    int tipH = textheight(tip1);

    int gap = 8;
    int totalTextH = tipH * 2 + gap;

    // 横向居中
    int tip1X = panelX + (panelW - tip1W) / 2;
    int tip2X = panelX + (panelW - tip2W) / 2;

    // 纵向整体居中
    int tip1Y = panelY + (panelH - totalTextH) / 2;
    int tip2Y = tip1Y + tipH + gap;

    // 第一行：火人
    settextcolor(RGB(255, 175, 110));
    outtextxy(tip1X, tip1Y, tip1);

    // 第二行：水人
    settextcolor(RGB(150, 215, 255));
    outtextxy(tip2X, tip2Y, tip2);

    //================ 版本号 =================

    const TCHAR* versionText = _T("Version 8.0");

    settextstyle(18, 0, _T("黑体"));
    settextcolor(RGB(130, 140, 170));

    int versionW = textwidth(versionText);

    outtextxy(800 - versionW - 20, 565, versionText);
}

// 在矩形区域内居中绘制文字
void DrawTextCenter(int left, int top, int right, int bottom, const TCHAR* text)
{
    int tw = textwidth(text);
    int th = textheight(text);

    outtextxy(
        left + (right - left - tw) / 2,
        top + (bottom - top - th) / 2,
        text
    );
}

// 绘制暂停菜单
void DrawPauseMenu(int gameTimeSec, int hoverButton)
{
    setbkmode(TRANSPARENT);

    //================ 外层面板 =================
    setfillcolor(RGB(20, 24, 40));
    solidroundrect(170, 95, 630, 525, 28, 28);

    setlinecolor(RGB(255, 220, 120));
    roundrect(170, 95, 630, 525, 28, 28);

    //================ 内层面板 =================
    setfillcolor(RGB(35, 42, 70));
    solidroundrect(185, 110, 615, 510, 24, 24);

    //================ 标题条 =================
    setfillcolor(RGB(65, 75, 115));
    solidroundrect(205, 130, 595, 190, 18, 18);

    const TCHAR* pauseTitle = _T("游戏暂停");

    setfont(48, 0, _T("微软雅黑"));

    int titleW = textwidth(pauseTitle);

    settextcolor(RGB(55, 55, 65));
    outtextxy((800 - titleW) / 2 + 3, 142 + 3, pauseTitle);

    settextcolor(RGB(255, 225, 120));
    outtextxy((800 - titleW) / 2, 142, pauseTitle);

    //================ 分割线 =================
    setlinecolor(RGB(255, 220, 120));
    line(220, 215, 580, 215);

    setlinecolor(RGB(100, 115, 155));
    line(240, 220, 560, 220);

    //================ 当前时间 =================
    TCHAR pauseTimeStr[50];

    int pauseMinute = gameTimeSec / 60;
    int pauseSecond = gameTimeSec % 60;

    _stprintf_s(
        pauseTimeStr,
        _countof(pauseTimeStr),
        _T("当前时间：%02d:%02d"),
        pauseMinute,
        pauseSecond
    );

    setfont(24, 0, _T("微软雅黑"));
    settextcolor(RGB(220, 240, 255));
    DrawTextCenter(255, 230, 545, 260, pauseTimeStr);

    //================ 按钮 1：继续游戏 =================
    if (hoverButton == 1)
        setfillcolor(RGB(95, 155, 255));
    else
        setfillcolor(RGB(70, 130, 255));

    solidroundrect(255, 280, 545, 335, 16, 16);

    setlinecolor(RGB(150, 195, 255));
    roundrect(255, 280, 545, 335, 16, 16);

    if (hoverButton == 1)
    {
        setlinecolor(RGB(230, 245, 255));
        roundrect(250, 275, 550, 340, 18, 18);
    }

    setfont(28, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    DrawTextCenter(255, 280, 545, 335, _T("ESC 继续游戏"));

    //================ 按钮 2：重新开始 =================
    if (hoverButton == 2)
        setfillcolor(RGB(255, 190, 85));
    else
        setfillcolor(RGB(255, 170, 65));

    solidroundrect(255, 355, 545, 410, 16, 16);

    setlinecolor(RGB(255, 220, 140));
    roundrect(255, 355, 545, 410, 16, 16);

    if (hoverButton == 2)
    {
        setlinecolor(RGB(255, 245, 190));
        roundrect(250, 350, 550, 415, 18, 18);
    }

    setfont(28, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    DrawTextCenter(255, 355, 545, 410, _T("R 重新开始"));

    //================ 按钮 3：退出游戏 =================
    if (hoverButton == 3)
        setfillcolor(RGB(230, 100, 100));
    else
        setfillcolor(RGB(210, 80, 80));

    solidroundrect(255, 430, 545, 485, 16, 16);

    setlinecolor(RGB(255, 160, 160));
    roundrect(255, 430, 545, 485, 16, 16);

    if (hoverButton == 3)
    {
        setlinecolor(RGB(255, 210, 210));
        roundrect(250, 425, 550, 490, 18, 18);
    }

    setfont(28, 0, _T("微软雅黑"));
    settextcolor(WHITE);
    DrawTextCenter(255, 430, 545, 485, _T("Q 退出游戏"));
}

int main()
{
    MOUSEMSG m;

    initgraph(800, 600);

    // 新加：美化背景
    setbkcolor(RGB(30, 30, 50));
    cleardevice();

    srand((unsigned)time(NULL));

    //================ 背景粒子 =================
    const int bgParticleCnt = 40;

    float bgx[bgParticleCnt];
    float bgy[bgParticleCnt];
    float bgspeed[bgParticleCnt];

    // 初始化背景粒子
    for (int i = 0; i < bgParticleCnt; i++)
    {
        bgx[i] = rand() % 800;
        bgy[i] = rand() % 600;
        bgspeed[i] = 0.5f + rand() % 3;
    }

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

    //================ 8.0 计时系统 =================
    DWORD gameStartTick = 0;     // 游戏开始的时间点
    DWORD pauseStartTick = 0;    // 暂停开始的时间点
    DWORD pauseTotalTick = 0;    // 总共暂停了多久
    int gameTimeSec = 0;         // 当前游戏时间，单位：秒
    int finalTimeSec = 0;        // 结束时的最终时间
    bool timerRunning = false;   // 是否正在计时

    //================ 8.0-5 胜利评分系统 =================
    int finalDiamondCount = 0;       // 最终收集钻石数量
    const TCHAR* finalRank = _T("C"); // 最终评级

    // 钻石数量
    const int diamondCnt = 8;

    // 钻石坐标+是否被吃掉
    // 8.0-17 困难版钻石布局：钻石用于增加操作难度
    int diaX[diamondCnt] = { 40, 210, 250, 405, 520, 755, 470, 315 };
    int diaY[diamondCnt] = { 360, 260, 295, 360, 365, 250, 190, 65 };

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

    // 钻石动画帧
    int diamondFrame = 0;

    //================ 8.0-2 分数飘字 =================
    const int scoreTextCnt = 20;
    float scoreTextX[scoreTextCnt] = { 0 };
    float scoreTextY[scoreTextCnt] = { 0 };
    int scoreTextLife[scoreTextCnt] = { 0 };
    int scoreTextValue[scoreTextCnt] = { 0 };   // 新增：记录 +5 或 -5

    auto AddScoreText = [&](float x, float y, int value)
        {
            for (int i = 0; i < scoreTextCnt; i++)
            {
                if (scoreTextLife[i] <= 0)
                {
                    scoreTextX[i] = x;
                    scoreTextY[i] = y;
                    scoreTextLife[i] = 45;
                    scoreTextValue[i] = value;
                    break;
                }
            }
        };

    // 死亡嘲讽显示时间
    int showTaunt1 = 0;
    int showTaunt2 = 0;
    // 放在 showTaunt1、showTaunt2 旁边
    int lastTaunt1 = 0;
    int lastTaunt2 = 0;

    //================ 8.0-3 屏幕震动 =================
    int shakeTime = 0;      // 震动剩余时间
    int shakePower = 0;     // 震动强度

    bool jump1_last = false;
    bool jump2_last = false;
    bool lastPress[buttonCount] = { false };

    //================ 8.0-21 机关触发提示 =================
    int actionTipLife = 0;
    int actionTipX = 0;
    int actionTipY = 0;
    TCHAR actionTip[50] = _T("");

    //================ 8.0-16 清空临时状态 =================
    auto ClearTempEffects = [&]()
        {
            // 清空死亡粒子
            for (int i = 0; i < 50; i++)
            {
                px_[i] = 0;
                py_[i] = 0;
                vx_[i] = 0;
                vy_[i] = 0;
                pLife[i] = 0;
            }

            particleCnt = 0;

            // 清空分数飘字
            for (int i = 0; i < scoreTextCnt; i++)
            {
                scoreTextX[i] = 0;
                scoreTextY[i] = 0;
                scoreTextLife[i] = 0;
                scoreTextValue[i] = 0;
            }

            // 清空嘲讽文字
            showTaunt1 = 0;
            showTaunt2 = 0;
            lastTaunt1 = 0;
            lastTaunt2 = 0;

            // 清空震动
            shakeTime = 0;
            shakePower = 0;

            // 清空按键残留
            jump1_last = false;
            jump2_last = false;

            for (int i = 0; i < buttonCount; i++)
            {
                lastPress[i] = false;
            }

            // 清空机关提示
            actionTipLife = 0;
            actionTip[0] = 0;
        };

    //================ 8.0-28 统一重新开始逻辑 =================
    auto ResetGame = [&]()
        {
            // 重置火人
            x1 = 0;
            y1 = 430;
            vy1 = 0;

            // 重置水人
            x2 = 0;
            y2 = 500;
            vy2 = 0;

            // 分数和死亡次数归零
            score = 0;
            deathCount = 0;

            // 钻石刷新
            for (int i = 0; i < diamondCnt; i++)
            {
                diaAlive[i] = true;
            }

            // 重新开始时重置计时
            gameStartTick = GetTickCount();
            pauseStartTick = 0;
            pauseTotalTick = 0;
            gameTimeSec = 0;
            finalTimeSec = 0;
            timerRunning = true;

            // 重置评分数据
            finalDiamondCount = 0;
            finalRank = _T("C");

            // 清空飘字、粒子、震动、嘲讽、机关提示等临时状态
            ClearTempEffects();
        };

    //================ 8.0-29 统一统计钻石数量 =================
    auto CountDiamonds = [&]() -> int
        {
            int count = 0;

            for (int i = 0; i < diamondCnt; i++)
            {
                if (!diaAlive[i])
                {
                    count++;
                }
            }

            return count;
        };

    //================ 8.0-29 统一计算最终评级 =================
    auto CalculateRank = [&]() -> const TCHAR*
        {
            if (finalTimeSec <= 60 &&
                deathCount <= 5 &&
                finalDiamondCount == 8 &&
                score >= 15)
            {
                return _T("S");
            }
            else if (finalTimeSec <= 100 &&
                deathCount <= 10 &&
                finalDiamondCount >= 6 &&
                score >= -20)
            {
                return _T("A");
            }
            else if (finalTimeSec <= 150 &&
                deathCount <= 15 &&
                finalDiamondCount >= 4 &&
                score >= -55)
            {
                return _T("B");
            }
            else
            {
                return _T("C");
            }
        };

    //================ 主菜单循环 ================
    while (gameState == 0)
    {
        POINT mousePos;
        GetCursorPos(&mousePos);
        ScreenToClient(GetHWnd(), &mousePos);

        bool startHover =
            mousePos.x >= 275 && mousePos.x <= 525 &&
            mousePos.y >= 320 && mousePos.y <= 390;

        DrawMainMenu(startHover);

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

                    // 8.0：开始计时
                    gameStartTick = GetTickCount();
                    pauseStartTick = 0;
                    pauseTotalTick = 0;
                    gameTimeSec = 0;
                    finalTimeSec = 0;
                    timerRunning = true;

                    // 8.0-5：重置评分数据
                    finalDiamondCount = 0;
                    finalRank = _T("C");

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

            POINT pauseMouse;
            GetCursorPos(&pauseMouse);
            ScreenToClient(GetHWnd(), &pauseMouse);

            int pauseHoverButton = 0;

            if (pauseMouse.x >= 255 && pauseMouse.x <= 545 &&
                pauseMouse.y >= 280 && pauseMouse.y <= 335)
            {
                pauseHoverButton = 1;
            }
            else if (pauseMouse.x >= 255 && pauseMouse.x <= 545 &&
                pauseMouse.y >= 355 && pauseMouse.y <= 410)
            {
                pauseHoverButton = 2;
            }
            else if (pauseMouse.x >= 255 && pauseMouse.x <= 545 &&
                pauseMouse.y >= 430 && pauseMouse.y <= 485)
            {
                pauseHoverButton = 3;
            }

            DrawPauseMenu(gameTimeSec, pauseHoverButton);

            //================ 8.0-22 暂停菜单鼠标点击 =================
            if (MouseHit())
            {
                m = GetMouseMsg();

                if (m.uMsg == WM_LBUTTONDOWN)
                {
                    // 点击“ESC 继续游戏”按钮区域
                    if (m.x >= 255 && m.x <= 545 &&
                        m.y >= 280 && m.y <= 335)
                    {
                        if (pauseStartTick != 0)
                        {
                            pauseTotalTick += GetTickCount() - pauseStartTick;
                            pauseStartTick = 0;
                        }

                        PlaySound(
                            TEXT("button.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );

                        gameState = 1;
                    }

                    // 点击“R 重新开始”按钮区域
                    else if (m.x >= 255 && m.x <= 545 &&
                        m.y >= 355 && m.y <= 410)
                    {
                        ResetGame();

                        PlaySound(
                            TEXT("button.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );

                        gameState = 1;
                    }

                    // 点击“Q 退出游戏”按钮区域
                    else if (m.x >= 255 && m.x <= 545 &&
                        m.y >= 430 && m.y <= 485)
                    {
                        closegraph();
                        return 0;
                    }
                }
            }

            // ESC继续
            if (GetAsyncKeyState(VK_ESCAPE) & 1)
            {
                // 8.0：把本次暂停时间加入总暂停时间
                if (pauseStartTick != 0)
                {
                    pauseTotalTick += GetTickCount() - pauseStartTick;
                    pauseStartTick = 0;
                }

                gameState = 1;
            }

            // R重新开始
            if (GetAsyncKeyState('R') & 1)
            {
                ResetGame();
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

            //================ 8.0-7 失败结算界面美化 =================
            cleardevice();
            setbkmode(TRANSPARENT);

            // 深色背景
            setfillcolor(RGB(20, 18, 28));
            solidrectangle(0, 0, 800, 600);

            // 外层红色边框
            setlinecolor(RGB(255, 90, 90));
            roundrect(145, 75, 655, 540, 28, 28);

            // 主面板
            setfillcolor(RGB(45, 35, 45));
            solidroundrect(150, 80, 650, 535, 28, 28);

            // 顶部暗红高光条
            setfillcolor(RGB(90, 45, 55));
            solidroundrect(165, 95, 635, 150, 20, 20);

            //================ 标题 =================
            const TCHAR* overStr = _T("GAME OVER");

            setfont(64, 0, _T("微软雅黑"));

            int overW = textwidth(overStr);

            // 标题阴影
            settextcolor(RGB(80, 20, 20));
            outtextxy((800 - overW) / 2 + 4, 105 + 4, overStr);

            // 标题正文
            settextcolor(RGB(255, 80, 80));
            outtextxy((800 - overW) / 2, 105, overStr);

            // 分割线
            setlinecolor(RGB(255, 100, 100));
            line(190, 175, 610, 175);

            setlinecolor(RGB(110, 80, 90));
            line(210, 180, 590, 180);

            //================ 失败原因 =================
            setfont(28, 0, _T("微软雅黑"));
            settextcolor(RGB(255, 190, 190));
            outtextxy(260, 205, _T("死亡次数达到20次"));

            //================ 结算数据 =================
            TCHAR resultStr[100];

            int failMinute = finalTimeSec / 60;
            int failSecond = finalTimeSec % 60;

            setfont(28, 0, _T("微软雅黑"));
            settextcolor(WHITE);

            // 最终分数
            _stprintf_s(resultStr, _countof(resultStr), _T("最终分数：%d"), score);
            outtextxy(235, 265, resultStr);

            // 死亡次数
            _stprintf_s(resultStr, _countof(resultStr), _T("死亡次数：%d"), deathCount);
            outtextxy(235, 310, resultStr);

            // 钻石收集
            _stprintf_s(
                resultStr,
                _countof(resultStr),
                _T("钻石收集：%d / %d"),
                finalDiamondCount,
                diamondCnt
            );
            outtextxy(235, 355, resultStr);

            // 坚持时间
            _stprintf_s(
                resultStr,
                _countof(resultStr),
                _T("坚持时间：%02d:%02d"),
                failMinute,
                failSecond
            );
            outtextxy(235, 400, resultStr);

            // 鼓励语
            setfont(24, 0, _T("微软雅黑"));
            settextcolor(RGB(220, 180, 180));
            outtextxy(260, 440, _T("再试一次，下一把一定能过！"));

            //================ 8.0-26 失败界面按钮悬停判断 =================
            POINT failMouse;
            GetCursorPos(&failMouse);
            ScreenToClient(GetHWnd(), &failMouse);

            int failHoverButton = 0;

            // 鼠标在“R 重新挑战”按钮上
            if (failMouse.x >= 205 && failMouse.x <= 370 &&
                failMouse.y >= 475 && failMouse.y <= 525)
            {
                failHoverButton = 1;
            }
            // 鼠标在“Q 退出游戏”按钮上
            else if (failMouse.x >= 430 && failMouse.x <= 595 &&
                failMouse.y >= 475 && failMouse.y <= 525)
            {
                failHoverButton = 2;
            }

            //================ 底部操作按钮 =================

            // R重新挑战按钮
            if (failHoverButton == 1)
            {
                setfillcolor(RGB(95, 150, 255));
            }
            else
            {
                setfillcolor(RGB(70, 120, 255));
            }

            solidroundrect(205, 475, 370, 525, 16, 16);

            setlinecolor(RGB(150, 190, 255));
            roundrect(205, 475, 370, 525, 16, 16);

            // 鼠标悬停外发光
            if (failHoverButton == 1)
            {
                setlinecolor(RGB(230, 245, 255));
                roundrect(200, 470, 375, 530, 18, 18);
            }

            setfont(24, 0, _T("微软雅黑"));
            settextcolor(WHITE);
            DrawTextCenter(205, 475, 370, 525, _T("R 重新挑战"));


            // Q退出按钮
            if (failHoverButton == 2)
            {
                setfillcolor(RGB(225, 100, 100));
            }
            else
            {
                setfillcolor(RGB(200, 80, 80));
            }

            solidroundrect(430, 475, 595, 525, 16, 16);

            setlinecolor(RGB(255, 160, 160));
            roundrect(430, 475, 595, 525, 16, 16);

            // 鼠标悬停外发光
            if (failHoverButton == 2)
            {
                setlinecolor(RGB(255, 220, 220));
                roundrect(425, 470, 600, 530, 18, 18);
            }

            setfont(24, 0, _T("微软雅黑"));
            settextcolor(WHITE);
            DrawTextCenter(430, 475, 595, 525, _T("Q 退出游戏"));

            //================ 8.0-23 失败界面鼠标点击 =================
            if (MouseHit())
            {
                m = GetMouseMsg();

                if (m.uMsg == WM_LBUTTONDOWN)
                {
                    // 点击“R 重新挑战”按钮
                    if (m.x >= 205 && m.x <= 370 &&
                        m.y >= 475 && m.y <= 525)
                    {
                        ResetGame();

                        PlaySound(
                            TEXT("button.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );

                        gameState = 1;
                    }

                    // 点击“Q 退出游戏”按钮
                    else if (m.x >= 430 && m.x <= 595 &&
                        m.y >= 475 && m.y <= 525)
                    {
                        closegraph();
                        return 0;
                    }
                }
            }

            // R重新开始
            if (GetAsyncKeyState('R') & 1)
            {
                ResetGame();
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

        diamondFrame++;

        //================ 8.0 更新时间 =================
        if (timerRunning)
        {
            DWORD nowTick = GetTickCount();
            gameTimeSec = (nowTick - gameStartTick - pauseTotalTick) / 1000;
        }

        //================ 8.0-18 游戏背景远景层优化：干净版 =================

        //================ 深色渐变背景 =================
        for (int y = 0; y < 600; y += 20)
        {
            int c = 20 + y / 28;

            setfillcolor(RGB(18, 24, 42 + c / 2));
            solidrectangle(0, y, 800, y + 20);
        }

        //================ 远处山影：第一层 =================
        POINT farMountain1[5] =
        {
            { 0, 270 },
            { 130, 165 },
            { 300, 270 },
            { 520, 155 },
            { 800, 270 }
        };

        setfillcolor(RGB(24, 30, 52));
        solidpolygon(farMountain1, 5);

        //================ 远处山影：第二层 =================
        POINT farMountain2[7] =
        {
            { 0, 345 },
            { 90, 245 },
            { 210, 345 },
            { 360, 225 },
            { 520, 345 },
            { 690, 250 },
            { 800, 345 }
        };

        setfillcolor(RGB(30, 38, 65));
        solidpolygon(farMountain2, 7);

        //================ 远处小星点 =================
        for (int s = 0; s < 35; s++)
        {
            int sx = (s * 97 + 40) % 800;
            int sy = (s * 53 + 25) % 220;

            int twinkle = 150 + (int)(sin((diamondFrame + s * 18) * 0.08f) * 45);

            setfillcolor(RGB(twinkle, twinkle, 210));
            solidcircle(sx, sy, 1);
        }

        //================ 背景粒子 =================
        for (int i = 0; i < bgParticleCnt; i++)
        {
            bgy[i] += bgspeed[i] * 0.35f;

            if (bgy[i] > 600)
            {
                bgy[i] = 0;
                bgx[i] = rand() % 800;
            }

            int glow = 150 + (int)(sin((diamondFrame + i * 12) * 0.08f) * 35);

            setfillcolor(RGB(120, glow, 235));
            solidcircle((int)bgx[i], (int)bgy[i], 1);
        }

        //================ 底部暗色压影，让平台更突出 =================
        setfillcolor(RGB(12, 16, 28));
        solidrectangle(0, 560, 800, 600);

        // ESC暂停
        if (GetAsyncKeyState(VK_ESCAPE) & 1)
        {
            gameState = 2;

            // 8.0：记录暂停开始时间
            pauseStartTick = GetTickCount();
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
        // ===== 按钮音效 + 机关提示 =====
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

                // 8.0-21：按钮上方显示机关提示
                actionTipLife = 45;
                actionTipX = ax[i] + aw[i] / 2;
                actionTipY = ay[i] - 32;

                if (i == 1)
                {
                    _stprintf_s(
                        actionTip,
                        _countof(actionTip),
                        _T("关闭")
                    );
                }
                else
                {
                    _stprintf_s(
                        actionTip,
                        _countof(actionTip),
                        _T("启动")
                    );
                }
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
                    AddScoreText(deadX, deadY - 25, -5);
                    // 8.0-3：死亡时屏幕震动
                    shakeTime = 12;
                    shakePower = 5;
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
                    lastTaunt1 = rand() % 4;

                    // 8.0-16：死亡后立刻退出检测循环，防止同一帧重复扣分
                    break;
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
                    AddScoreText(deadX, deadY - 25, -5);
                    // 8.0-3：死亡时屏幕震动
                    shakeTime = 12;
                    shakePower = 5;
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
                    lastTaunt2 = rand() % 4;

                    // 8.0-16：死亡后立刻退出检测循环，防止同一帧重复扣分
                    break;
                }
            }
        }



        // 钻石拾取加分
        for (int i = 0; i < diamondCnt; i++)
        {
            if (!diaAlive[i]) continue;

            // 火人拾取
            if (x1 + 50 > diaX[i] && x1 < diaX[i] + 20 &&
                y1 + 50 > diaY[i] && y1 < diaY[i] + 20)
            {
                diaAlive[i] = false;
                score += 5;

                // 8.0-2：出现 +5 飘字
                AddScoreText((float)diaX[i] + 10, (float)diaY[i] - 15, 5);

                PlaySound(
                    TEXT("diamond.wav"),
                    NULL,
                    SND_FILENAME | SND_ASYNC
                );

                continue;
            }

            // 水人拾取
            if (x2 + 50 > diaX[i] && x2 < diaX[i] + 20 &&
                y2 + 50 > diaY[i] && y2 < diaY[i] + 20)
            {
                diaAlive[i] = false;
                score += 5;

                // 8.0-2：出现 +5 飘字
                AddScoreText((float)diaX[i] + 10, (float)diaY[i] - 15, 5);

                PlaySound(
                    TEXT("diamond.wav"),
                    NULL,
                    SND_FILENAME | SND_ASYNC
                );
            }
        }

        //================ 8.0 画平台美术升级 =================
        for (int i = 0; i < platformCount; i++)
        {
            //================ 普通草地平台 =================
            if (type[i] == 0)
            {
                // 土块底部
                setfillcolor(RGB(115, 75, 40));
                solidrectangle(px[i], py[i] + 6, px[i] + pw[i], py[i] + 20);

                // 草地顶部
                setfillcolor(RGB(80, 190, 85));
                solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 8);

                // 草地亮边
                setlinecolor(RGB(170, 255, 160));
                line(px[i], py[i], px[i] + pw[i], py[i]);

                // 小草尖
                if (py[i] > 5)
                {
                    setlinecolor(RGB(120, 240, 120));

                    for (int g = 5; g < pw[i]; g += 16)
                    {
                        line(px[i] + g, py[i] + 2, px[i] + g + 3, py[i] - 4);
                        line(px[i] + g + 6, py[i] + 2, px[i] + g + 3, py[i] - 3);
                    }
                }

                // 土块里的小石子
                setfillcolor(RGB(80, 55, 35));

                for (int s = 12; s < pw[i]; s += 32)
                {
                    solidcircle(px[i] + s, py[i] + 14, 2);
                }
            }

            //================ 火池 =================
            if (type[i] == 1)
            {
                // 火池底色
                setfillcolor(RGB(180, 45, 35));
                solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);

                // 顶部高亮
                setlinecolor(RGB(255, 210, 40));
                line(px[i], py[i], px[i] + pw[i], py[i]);

                // 动态火苗
                for (int f = 5; f < pw[i]; f += 18)
                {
                    int flameH = 6 + (int)(sin((diamondFrame + f + i * 10) * 0.15f) * 4);

                    POINT flame[3] =
                    {
                        { px[i] + f, py[i] + 18 },
                        { px[i] + f + 6, py[i] + 18 },
                        { px[i] + f + 3, py[i] + 6 - flameH }
                    };

                    setfillcolor(RGB(255, 180, 30));
                    solidpolygon(flame, 3);

                    POINT innerFlame[3] =
                    {
                        { px[i] + f + 2, py[i] + 18 },
                        { px[i] + f + 5, py[i] + 18 },
                        { px[i] + f + 3, py[i] + 10 - flameH / 2 }
                    };

                    setfillcolor(RGB(255, 80, 20));
                    solidpolygon(innerFlame, 3);
                }
            }

            //================ 水池 =================
            if (type[i] == 2)
            {
                // 水池底色
                setfillcolor(RGB(40, 105, 230));
                solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);

                // 水面亮边
                setlinecolor(RGB(180, 230, 255));
                line(px[i], py[i], px[i] + pw[i], py[i]);

                // 动态水纹
                setlinecolor(WHITE);

                for (int w = 0; w < pw[i]; w += 18)
                {
                    int waveY = py[i] + 7 + (int)(sin((diamondFrame + w + i * 8) * 0.12f) * 2);

                    line(px[i] + w, waveY, px[i] + w + 8, waveY);
                }

                // 水底暗部
                setfillcolor(RGB(25, 70, 180));
                solidrectangle(px[i], py[i] + 15, px[i] + pw[i], py[i] + 20);
            }

            //================ 紫毒池 =================
            if (type[i] == 3)
            {
                // 毒池底色
                setfillcolor(RGB(130, 45, 170));
                solidrectangle(px[i], py[i], px[i] + pw[i], py[i] + 20);

                // 毒液高亮
                setlinecolor(RGB(245, 120, 255));
                line(px[i], py[i], px[i] + pw[i], py[i]);

                // 毒液波纹
                setlinecolor(RGB(230, 150, 255));

                for (int w = 4; w < pw[i]; w += 20)
                {
                    int waveY = py[i] + 8 + (int)(sin((diamondFrame + w + i * 12) * 0.1f) * 2);
                    line(px[i] + w, waveY, px[i] + w + 10, waveY);
                }

                // 冒泡效果
                for (int b = 10; b < pw[i]; b += 28)
                {
                    int bubbleY = py[i] + 14 - (diamondFrame + b + i * 7) % 12;

                    setlinecolor(RGB(255, 180, 255));
                    circle(px[i] + b, bubbleY, 3);
                }

                // 底部暗紫色
                setfillcolor(RGB(90, 25, 120));
                solidrectangle(px[i], py[i] + 16, px[i] + pw[i], py[i] + 20);
            }
        }

        //================ 8.0-13 机关按钮美术升级 =================
        for (int i = 0; i < buttonCount; i++)
        {
            int bx = ax[i];
            int by = ay[i];
            int bw = aw[i];

            // 按钮底座，比实际碰撞稍宽一点，但不影响判定
            setfillcolor(RGB(70, 55, 35));
            solidroundrect(bx - 5, by + 10, bx + bw + 5, by + 22, 6, 6);

            setlinecolor(RGB(120, 95, 55));
            roundrect(bx - 5, by + 10, bx + bw + 5, by + 22, 6, 6);

            if (press[i])
            {
                // 按下状态：按钮变矮，颜色更深
                setfillcolor(RGB(230, 95, 25));
                solidroundrect(bx - 3, by + 8, bx + bw + 3, by + 18, 6, 6);

                // 顶部高光
                setfillcolor(RGB(255, 155, 60));
                solidroundrect(bx + 1, by + 9, bx + bw - 1, by + 12, 4, 4);

                // 按下发光
                setlinecolor(RGB(255, 210, 90));
                roundrect(bx - 4, by + 7, bx + bw + 4, by + 19, 6, 6);
            }
            else
            {
                // 未按下状态：按钮更高
                setfillcolor(RGB(255, 190, 45));
                solidroundrect(bx - 3, by, bx + bw + 3, by + 17, 7, 7);

                // 顶部高光
                setfillcolor(RGB(255, 235, 110));
                solidroundrect(bx + 1, by + 3, bx + bw - 1, by + 8, 5, 5);

                // 边框
                setlinecolor(RGB(255, 230, 120));
                roundrect(bx - 3, by, bx + bw + 3, by + 17, 7, 7);
            }
        }

        //================ 8.0-20 钻石统一风格美术优化 =================
        for (int i = 0; i < diamondCnt; i++)
        {
            if (!diaAlive[i]) continue;

            int cx = diaX[i] + 10;
            int cy = diaY[i] + 10;

            // 呼吸闪烁
            int glow = (int)(sin((diamondFrame + i * 15) * 0.08f) * 35 + 210);

            //================ 统一外圈光环 =================
            int ringR = 15 + (int)(sin((diamondFrame + i * 20) * 0.1f) * 2);

            setlinecolor(RGB(90, 190, 255));
            circle(cx, cy, ringR);

            //================ 统一小光点 =================
            for (int p = 0; p < 4; p++)
            {
                int sparkX = cx + (int)(cos((diamondFrame + p * 90 + i * 20) * 0.05f) * 18);
                int sparkY = cy + (int)(sin((diamondFrame + p * 90 + i * 20) * 0.05f) * 18);

                setfillcolor(RGB(130, 220, 255));
                solidcircle(sparkX, sparkY, 1);
            }

            //================ 钻石主体：外层 =================
            POINT outer[4] =
            {
                { cx,      cy - 10 },
                { cx + 10, cy },
                { cx,      cy + 10 },
                { cx - 10, cy }
            };

            setfillcolor(RGB(80, glow, 255));
            solidpolygon(outer, 4);

            //================ 钻石主体：内层 =================
            POINT inner[4] =
            {
                { cx,     cy - 5 },
                { cx + 5, cy },
                { cx,     cy + 5 },
                { cx - 5, cy }
            };

            setfillcolor(WHITE);
            solidpolygon(inner, 4);

            //================ 顶部小高光 =================
            setfillcolor(RGB(220, 250, 255));
            solidcircle(cx - 3, cy - 4, 2);
        }


        //================ 8.0-12 游戏内迷你 HUD =================

        // 统计当前已经收集的钻石数量
        int currentDiamondCount = 0;

        for (int i = 0; i < diamondCnt; i++)
        {
            if (!diaAlive[i])
            {
                currentDiamondCount++;
            }
        }

        // 时间换算
        int minute = gameTimeSec / 60;
        int second = gameTimeSec % 60;

        TCHAR hudStr[80];

        setbkmode(TRANSPARENT);

        //================ 左上角迷你信息条 =================

        // 小阴影
        setfillcolor(RGB(10, 14, 25));
        solidroundrect(12, 12, 308, 46, 12, 12);

        // 主体
        setfillcolor(RGB(25, 32, 55));
        solidroundrect(10, 10, 306, 44, 12, 12);

        // 边框
        setlinecolor(RGB(75, 100, 150));
        roundrect(10, 10, 306, 44, 12, 12);

        // 分数
        setfont(18, 0, _T("黑体"));

        _stprintf_s(hudStr, _countof(hudStr), _T("分数 %d"), score);
        settextcolor(WHITE);
        outtextxy(22, 18, hudStr);

        // 时间
        _stprintf_s(hudStr, _countof(hudStr), _T("时间 %02d:%02d"), minute, second);
        settextcolor(RGB(190, 230, 255));
        outtextxy(105, 18, hudStr);

        // 死亡
        _stprintf_s(hudStr, _countof(hudStr), _T("死亡 %d/20"), deathCount);
        settextcolor(RGB(255, 150, 150));
        outtextxy(205, 18, hudStr);


        //================ 右上角钻石信息 =================

        // 小阴影
        setfillcolor(RGB(10, 14, 25));
        solidroundrect(656, 12, 788, 46, 12, 12);

        // 主体
        setfillcolor(RGB(25, 32, 55));
        solidroundrect(654, 10, 786, 44, 12, 12);

        // 边框
        setlinecolor(RGB(75, 120, 170));
        roundrect(654, 10, 786, 44, 12, 12);

        // 钻石菱形小图标
        POINT smallDia[4] =
        {
            { 672, 17 },
            { 682, 27 },
            { 672, 37 },
            { 662, 27 }
        };

        setfillcolor(RGB(90, 210, 255));
        solidpolygon(smallDia, 4);

        setfillcolor(WHITE);
        solidcircle(672, 24, 2);

        // 钻石文字
        _stprintf_s(hudStr, _countof(hudStr), _T("%d / %d"), currentDiamondCount, diamondCnt);

        setfont(20, 0, _T("黑体"));
        settextcolor(RGB(150, 230, 255));
        outtextxy(696, 17, hudStr);

        //================ 8.0-14 开局目标提示 =================
// 只在开局前 7 秒显示，不影响后面游玩
        if (gameTimeSec <= 7)
        {
            // 小面板位置：放在左上 HUD 下面，不挡终点门、不挡角色
            int tipX = 12;
            int tipY = 58;
            int tipW = 310;
            int tipH = 125;

            // 阴影
            setfillcolor(RGB(8, 12, 22));
            solidroundrect(
                tipX + 3,
                tipY + 3,
                tipX + tipW + 3,
                tipY + tipH + 3,
                14,
                14
            );

            // 主体
            setfillcolor(RGB(25, 32, 55));
            solidroundrect(
                tipX,
                tipY,
                tipX + tipW,
                tipY + tipH,
                14,
                14
            );

            // 边框
            setlinecolor(RGB(95, 130, 190));
            roundrect(
                tipX,
                tipY,
                tipX + tipW,
                tipY + tipH,
                14,
                14
            );

            // 标题条
            setfillcolor(RGB(45, 60, 95));
            solidroundrect(
                tipX + 8,
                tipY + 8,
                tipX + tipW - 8,
                tipY + 34,
                10,
                10
            );

            setbkmode(TRANSPARENT);

            // 标题
            setfont(18, 0, _T("黑体"));
            settextcolor(RGB(255, 225, 120));
            DrawTextCenter(
                tipX + 8,
                tipY + 8,
                tipX + tipW - 8,
                tipY + 34,
                _T("目标提示")
            );

            // 提示文字
            setfont(17, 0, _T("黑体"));

            settextcolor(RGB(255, 170, 110));
            outtextxy(tipX + 22, tipY + 45, _T("火人进入红色传送门"));

            settextcolor(RGB(145, 215, 255));
            outtextxy(tipX + 22, tipY + 70, _T("水人进入蓝色传送门"));

            settextcolor(RGB(220, 235, 255));
            outtextxy(tipX + 22, tipY + 95, _T("收集钻石可以提高评分"));
        }

        //================ 8.0-19 终点门交互反馈版 =================

        // 门的基础呼吸光效
        int doorPulse = (int)(sin(diamondFrame * 0.08f) * 20 + 200);

        // 计算角色和对应门的中心点距离
        int fireCenterX = x1 + 25;
        int fireCenterY = y1 + 25;

        int waterCenterX = x2 + 25;
        int waterCenterY = y2 + 25;

        int fireDoorCenterX = end1_x + end1_w / 2;
        int fireDoorCenterY = end1_y + end1_h / 2;

        int waterDoorCenterX = end2_x + end2_w / 2;
        int waterDoorCenterY = end2_y + end2_h / 2;

        // 靠近对应门时触发高亮
        bool fireNearDoor =
            abs(fireCenterX - fireDoorCenterX) <= 75 &&
            abs(fireCenterY - fireDoorCenterY) <= 75;

        bool waterNearDoor =
            abs(waterCenterX - waterDoorCenterX) <= 75 &&
            abs(waterCenterY - waterDoorCenterY) <= 75;

        // 判断是否已经完全进入门内
        bool fireInDoorNow =
            x1 >= end1_x && x1 + 50 <= end1_x + end1_w &&
            y1 >= end1_y && y1 + 50 <= end1_y + end1_h;

        bool waterInDoorNow =
            x2 >= end2_x && x2 + 50 <= end2_x + end2_w &&
            y2 >= end2_y && y2 + 50 <= end2_y + end2_h;

        // 门的额外亮度
        int fireDoorLight = fireNearDoor ? 35 : 0;
        int waterDoorLight = waterNearDoor ? 35 : 0;

        // 两人都进门时，额外闪烁
        int bothDoorLight = 0;

        if (fireInDoorNow && waterInDoorNow)
        {
            bothDoorLight = (int)(sin(diamondFrame * 0.25f) * 30 + 30);
        }

        int doorRing = 21 + (int)(sin(diamondFrame * 0.12f) * 2);


        //================ 火人终点门 =================

        // 靠近时，门内部出现小光环，不外扩，不覆盖平台
        if (fireNearDoor)
        {
            setlinecolor(RGB(255, 210, 90));
            circle(
                end1_x + 25,
                end1_y + 25,
                doorRing
            );
        }

        // 门框，只画在 50×50 内部，不外扩
        setfillcolor(RGB(120 + fireDoorLight, 45 + fireDoorLight / 3, 35));
        solidroundrect(
            end1_x + 2,
            end1_y + 2,
            end1_x + end1_w - 2,
            end1_y + end1_h - 2,
            10,
            10
        );

        // 内部暗色入口
        setfillcolor(RGB(45 + fireDoorLight / 4, 18, 18));
        solidroundrect(
            end1_x + 7,
            end1_y + 7,
            end1_x + end1_w - 7,
            end1_y + end1_h - 7,
            8,
            8
        );

        // 火焰核心
        setfillcolor(RGB(255, 110 + fireDoorLight, 45));
        solidellipse(
            end1_x + 15,
            end1_y + 10,
            end1_x + 35,
            end1_y + 42
        );

        // 中间亮光
        setfillcolor(RGB(255, doorPulse + fireDoorLight, 80 + bothDoorLight));
        solidellipse(
            end1_x + 20,
            end1_y + 18,
            end1_x + 30,
            end1_y + 34
        );

        // 底部火光
        setfillcolor(RGB(255, 70 + fireDoorLight, 25));
        solidroundrect(
            end1_x + 12,
            end1_y + 38,
            end1_x + 38,
            end1_y + 46,
            5,
            5
        );

        // 小火星，只在门内部飘
        for (int p = 0; p < 3; p++)
        {
            int fx = end1_x + 14 + p * 9;
            int fy = end1_y + 36 - (diamondFrame + p * 11) % 22;

            if (fy > end1_y + 6)
            {
                setfillcolor(RGB(255, 190 + fireDoorLight, 70));
                solidcircle(fx, fy, fireNearDoor ? 3 : 2);
            }
        }

        // 火门内边框
        setlinecolor(RGB(255, 170 + fireDoorLight, 70));
        roundrect(
            end1_x + 2,
            end1_y + 2,
            end1_x + end1_w - 2,
            end1_y + end1_h - 2,
            10,
            10
        );


        //================ 水人终点门 =================

        // 靠近时，门内部出现小光环，不外扩，不覆盖平台
        if (waterNearDoor)
        {
            setlinecolor(RGB(160, 230, 255));
            circle(
                end2_x + 25,
                end2_y + 25,
                doorRing
            );
        }

        // 门框，只画在 50×50 内部，不外扩
        setfillcolor(RGB(35, 85 + waterDoorLight / 2, 160 + waterDoorLight));
        solidroundrect(
            end2_x + 2,
            end2_y + 2,
            end2_x + end2_w - 2,
            end2_y + end2_h - 2,
            10,
            10
        );

        // 内部暗色入口
        setfillcolor(RGB(15, 32 + waterDoorLight / 4, 55 + waterDoorLight / 3));
        solidroundrect(
            end2_x + 7,
            end2_y + 7,
            end2_x + end2_w - 7,
            end2_y + end2_h - 7,
            8,
            8
        );

        // 水流核心
        setfillcolor(RGB(80, 170 + waterDoorLight, 255));
        solidellipse(
            end2_x + 15,
            end2_y + 10,
            end2_x + 35,
            end2_y + 42
        );

        // 中间亮光
        setfillcolor(RGB(190, doorPulse + waterDoorLight, 255));
        solidellipse(
            end2_x + 20,
            end2_y + 18,
            end2_x + 30,
            end2_y + 34
        );

        // 水纹
        setlinecolor(WHITE);
        line(end2_x + 14, end2_y + 31, end2_x + 23, end2_y + 31);
        line(end2_x + 27, end2_y + 37, end2_x + 36, end2_y + 37);

        // 小水滴，只在门内部飘
        for (int p = 0; p < 3; p++)
        {
            int wxDrop = end2_x + 14 + p * 9;
            int wyDrop = end2_y + 12 + (diamondFrame + p * 9) % 22;

            if (wyDrop < end2_y + 44)
            {
                setfillcolor(RGB(170, 230, 255));
                solidcircle(wxDrop, wyDrop, waterNearDoor ? 3 : 2);
            }
        }

        // 水门内边框
        setlinecolor(RGB(140, 220 + waterDoorLight, 255));
        roundrect(
            end2_x + 2,
            end2_y + 2,
            end2_x + end2_w - 2,
            end2_y + end2_h - 2,
            10,
            10
        );

        //================ 8.0-13 机关墙美术升级 =================
        for (int i = 0; i < presswallCount; i++)
        {
            if (!wallActive[i]) continue;

            int wallH = 20;

            if (i == 0)
            {
                wallH = 100;
            }

            int x = wx[i];
            int y = wy[i];
            int w = ww[i];
            int h = wallH;

            // 墙体阴影
            setfillcolor(RGB(25, 25, 30));
            solidrectangle(x + 4, y + 4, x + w + 4, y + h + 4);

            // 墙体主体
            setfillcolor(RGB(70, 75, 90));
            solidrectangle(x, y, x + w, y + h);

            // 左侧暗边
            setfillcolor(RGB(45, 48, 60));
            solidrectangle(x, y, x + 5, y + h);

            // 顶部高光
            setfillcolor(RGB(120, 130, 150));
            solidrectangle(x, y, x + w, y + 5);

            // 外边框
            setlinecolor(RGB(160, 170, 190));
            rectangle(x, y, x + w, y + h);

            // 内部机械线条
            setlinecolor(RGB(45, 50, 65));

            for (int yy = y + 12; yy < y + h; yy += 18)
            {
                line(x + 4, yy, x + w - 4, yy);
            }

            // 铆钉
            setfillcolor(RGB(180, 185, 200));

            if (w >= 20)
            {
                solidcircle(x + 6, y + 6, 2);
                solidcircle(x + w - 6, y + 6, 2);
                solidcircle(x + 6, y + h - 6, 2);
                solidcircle(x + w - 6, y + h - 6, 2);
            }

            // 黄色警示条，只在宽一点的墙上画
            if (w >= 45)
            {
                setfillcolor(RGB(230, 180, 45));
                solidrectangle(x + 8, y + h / 2 - 4, x + w - 8, y + h / 2 + 4);

                setlinecolor(RGB(70, 60, 40));

                for (int s = x + 10; s < x + w - 10; s += 14)
                {
                    line(s, y + h / 2 + 4, s + 8, y + h / 2 - 4);
                }
            }
        }

        //画人
        //================ 8.0-9 人物建模重做版 =================

        //================ 火人 =================
        int fireDrawX = x1;
        int fireDrawY = y1;

        // 轻微走路起伏
        if (onground1 && vx1 != 0)
        {
            fireDrawY += (int)(sin(diamondFrame * 0.35f) * 2);
        }

        // 跳跃时轻微上提
        if (!onground1)
        {
            fireDrawY -= 2;
        }

        // 身体主色
        COLORREF fireBody = RGB(255, 110, 65);
        COLORREF fireDark = RGB(215, 70, 40);
        COLORREF fireSkin = RGB(255, 210, 170);

        // 腿部
        setfillcolor(fireDark);
        solidroundrect(fireDrawX + 10, fireDrawY + 34, fireDrawX + 20, fireDrawY + 48, 4, 4);
        solidroundrect(fireDrawX + 30, fireDrawY + 34, fireDrawX + 40, fireDrawY + 48, 4, 4);

        // 脚
        setfillcolor(RGB(120, 40, 30));
        solidellipse(fireDrawX + 7, fireDrawY + 44, fireDrawX + 22, fireDrawY + 50);
        solidellipse(fireDrawX + 28, fireDrawY + 44, fireDrawX + 43, fireDrawY + 50);

        // 身体
        setfillcolor(fireBody);
        solidroundrect(fireDrawX + 8, fireDrawY + 16, fireDrawX + 42, fireDrawY + 42, 10, 10);

        // 身体高光
        setfillcolor(RGB(255, 145, 95));
        solidroundrect(fireDrawX + 12, fireDrawY + 19, fireDrawX + 38, fireDrawY + 28, 8, 8);

        // 手臂
        setfillcolor(fireBody);
        solidroundrect(fireDrawX + 3, fireDrawY + 20, fireDrawX + 12, fireDrawY + 34, 4, 4);
        solidroundrect(fireDrawX + 38, fireDrawY + 20, fireDrawX + 47, fireDrawY + 34, 4, 4);

        // 手
        setfillcolor(fireSkin);
        solidcircle(fireDrawX + 7, fireDrawY + 32, 4);
        solidcircle(fireDrawX + 43, fireDrawY + 32, 4);

        // 头
        setfillcolor(fireSkin);
        solidcircle(fireDrawX + 25, fireDrawY + 14, 12);

        // 眼白
        setfillcolor(WHITE);
        solidcircle(fireDrawX + 20, fireDrawY + 11, 3);
        solidcircle(fireDrawX + 30, fireDrawY + 11, 3);

        // 眼珠朝向
        int eyeMove1 = 0;
        if (vx1 < 0) eyeMove1 = -1;
        if (vx1 > 0) eyeMove1 = 1;

        setfillcolor(BLACK);
        solidcircle(fireDrawX + 20 + eyeMove1, fireDrawY + 11, 1);
        solidcircle(fireDrawX + 30 + eyeMove1, fireDrawY + 11, 1);

        // 嘴
        setlinecolor(BLACK);
        arc(fireDrawX + 19, fireDrawY + 14, fireDrawX + 31, fireDrawY + 20, 3.14, 0);

        // 火焰头发
        int flameSwing = (int)(sin(diamondFrame * 0.25f) * 2);

        POINT fireFlame1[3] =
        {
            { fireDrawX + 15, fireDrawY + 4 },
            { fireDrawX + 25, fireDrawY - 12 + flameSwing },
            { fireDrawX + 24, fireDrawY + 6 }
        };
        setfillcolor(RGB(255, 220, 80));
        solidpolygon(fireFlame1, 3);

        POINT fireFlame2[3] =
        {
            { fireDrawX + 23, fireDrawY + 5 },
            { fireDrawX + 34, fireDrawY - 10 - flameSwing },
            { fireDrawX + 35, fireDrawY + 7 }
        };
        setfillcolor(RGB(255, 140, 40));
        solidpolygon(fireFlame2, 3);

        POINT fireFlame3[3] =
        {
            { fireDrawX + 20, fireDrawY + 5 },
            { fireDrawX + 28, fireDrawY - 4 + flameSwing },
            { fireDrawX + 30, fireDrawY + 8 }
        };
        setfillcolor(RGB(255, 80, 20));
        solidpolygon(fireFlame3, 3);


        //================ 水人 =================
        int waterDrawX = x2;
        int waterDrawY = y2;

        // 轻微走路起伏
        if (onground2 && vx2 != 0)
        {
            waterDrawY += (int)(sin(diamondFrame * 0.35f) * 2);
        }

        // 跳跃时轻微上提
        if (!onground2)
        {
            waterDrawY -= 2;
        }

        // 身体主色
        COLORREF waterBody = RGB(80, 165, 255);
        COLORREF waterDark = RGB(45, 110, 220);
        COLORREF waterSkin = RGB(210, 240, 255);

        // 腿部
        setfillcolor(waterDark);
        solidroundrect(waterDrawX + 10, waterDrawY + 34, waterDrawX + 20, waterDrawY + 48, 4, 4);
        solidroundrect(waterDrawX + 30, waterDrawY + 34, waterDrawX + 40, waterDrawY + 48, 4, 4);

        // 脚
        setfillcolor(RGB(30, 80, 180));
        solidellipse(waterDrawX + 7, waterDrawY + 44, waterDrawX + 22, waterDrawY + 50);
        solidellipse(waterDrawX + 28, waterDrawY + 44, waterDrawX + 43, waterDrawY + 50);

        // 身体
        setfillcolor(waterBody);
        solidroundrect(waterDrawX + 8, waterDrawY + 16, waterDrawX + 42, waterDrawY + 42, 10, 10);

        // 身体高光
        setfillcolor(RGB(145, 210, 255));
        solidroundrect(waterDrawX + 12, waterDrawY + 19, waterDrawX + 38, waterDrawY + 28, 8, 8);

        // 手臂
        setfillcolor(waterBody);
        solidroundrect(waterDrawX + 3, waterDrawY + 20, waterDrawX + 12, waterDrawY + 34, 4, 4);
        solidroundrect(waterDrawX + 38, waterDrawY + 20, waterDrawX + 47, waterDrawY + 34, 4, 4);

        // 手
        setfillcolor(waterSkin);
        solidcircle(waterDrawX + 7, waterDrawY + 32, 4);
        solidcircle(waterDrawX + 43, waterDrawY + 32, 4);

        // 头
        setfillcolor(waterSkin);
        solidcircle(waterDrawX + 25, waterDrawY + 14, 12);

        // 眼白
        setfillcolor(WHITE);
        solidcircle(waterDrawX + 20, waterDrawY + 11, 3);
        solidcircle(waterDrawX + 30, waterDrawY + 11, 3);

        // 眼珠朝向
        int eyeMove2 = 0;
        if (vx2 < 0) eyeMove2 = -1;
        if (vx2 > 0) eyeMove2 = 1;

        setfillcolor(BLACK);
        solidcircle(waterDrawX + 20 + eyeMove2, waterDrawY + 11, 1);
        solidcircle(waterDrawX + 30 + eyeMove2, waterDrawY + 11, 1);

        // 嘴
        setlinecolor(BLACK);
        arc(waterDrawX + 19, waterDrawY + 14, waterDrawX + 31, waterDrawY + 20, 3.14, 0);

        // 水滴头发
        int dropSwing = (int)(sin(diamondFrame * 0.2f) * 2);

        setfillcolor(RGB(180, 235, 255));
        solidellipse(
            waterDrawX + 20,
            waterDrawY - 10 + dropSwing,
            waterDrawX + 30,
            waterDrawY + 5 + dropSwing
        );

        // 水滴高光
        setfillcolor(WHITE);
        solidcircle(waterDrawX + 24, waterDrawY - 2 + dropSwing, 2);

        //================ 8.0-2 绘制分数飘字：+5 / -5 =================
        setbkmode(TRANSPARENT);
        setfont(46, 0, _T("微软雅黑"));

        for (int i = 0; i < scoreTextCnt; i++)
        {
            if (scoreTextLife[i] > 0)
            {
                // 往上飘
                scoreTextY[i] -= 1.2f;

                // 生命周期减少
                scoreTextLife[i]--;

                TCHAR addText[20];

                // %+d 可以显示 +5 或 -5
                _stprintf_s(addText, _countof(addText), _T("%+d"), scoreTextValue[i]);

                int tw = textwidth(addText);

                // 黑色阴影
                settextcolor(BLACK);
                outtextxy(
                    (int)scoreTextX[i] - tw / 2 + 3,
                    (int)scoreTextY[i] + 3,
                    addText
                );

                // 正式文字颜色
                if (scoreTextValue[i] > 0)
                {
                    // 加分：金黄色
                    settextcolor(RGB(255, 230, 80));
                }
                else
                {
                    // 扣分：红色
                    settextcolor(RGB(255, 80, 80));
                }

                outtextxy(
                    (int)scoreTextX[i] - tw / 2,
                    (int)scoreTextY[i],
                    addText
                );
            }
        }


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
                    SND_FILENAME | SND_ASYNC
                );

                // 8.0：记录最终通关时间
                finalTimeSec = gameTimeSec;
                timerRunning = false;

                // 8.0-29：统一统计钻石和评级
                finalDiamondCount = CountDiamonds();
                finalRank = CalculateRank(); 

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

            //================ 8.0-6 胜利结算界面美化 =================

// 胜利动画标题大小
            int size = 36 + winFrame * 2;
            if (size > 72) size = 72;

            setbkmode(TRANSPARENT);

            // 半透明感深色遮罩：EasyX 没有直接透明，这里用深色矩形模拟压暗背景
            setfillcolor(RGB(15, 18, 35));
            solidrectangle(0, 0, 800, 600);

            // 外层发光边框
            setlinecolor(RGB(255, 220, 100));
            roundrect(145, 75, 655, 540, 28, 28);

            // 主面板
            setfillcolor(RGB(35, 40, 65));
            solidroundrect(150, 80, 650, 535, 28, 28);

            // 面板顶部高光条
            setfillcolor(RGB(65, 75, 115));
            solidroundrect(165, 95, 635, 150, 20, 20);

            //================ 标题 =================
            const TCHAR* winStr = _T("成功过关");

            setfont(size, 0, _T("黑体"));

            int winW = textwidth(winStr);

            // 标题阴影
            settextcolor(RGB(90, 65, 20));
            outtextxy((800 - winW) / 2 + 4, 105 + 4, winStr);

            // 标题正文
            settextcolor(RGB(255, 225, 90));
            outtextxy((800 - winW) / 2, 105, winStr);

            // 装饰分割线
            setlinecolor(RGB(255, 220, 100));
            line(190, 175, 610, 175);

            setlinecolor(RGB(100, 110, 150));
            line(210, 180, 590, 180);

            //================ 结算数据 =================
            TCHAR resultStr[100];

            int finalMinute = finalTimeSec / 60;
            int finalSecond = finalTimeSec % 60;

            setfont(28, 0, _T("微软雅黑"));

            // 左侧小标题
            settextcolor(RGB(180, 210, 255));
            outtextxy(205, 215, _T("通关数据"));

            // 最终分数
            settextcolor(WHITE);
            _stprintf_s(resultStr, _countof(resultStr), _T("最终分数：%d"), score);
            outtextxy(205, 260, resultStr);

            // 死亡次数
            _stprintf_s(resultStr, _countof(resultStr), _T("死亡次数：%d"), deathCount);
            outtextxy(205, 305, resultStr);

            // 钻石收集
            _stprintf_s(
                resultStr,
                _countof(resultStr),
                _T("钻石收集：%d / %d"),
                finalDiamondCount,
                diamondCnt
            );
            outtextxy(205, 350, resultStr);

            // 通关时间
            _stprintf_s(
                resultStr,
                _countof(resultStr),
                _T("通关时间：%02d:%02d"),
                finalMinute,
                finalSecond
            );
            outtextxy(205, 395, resultStr);

            //================ 评级区域 =================

            // 评级背景圆牌
            COLORREF rankColor;

            if (finalRank[0] == _T('S'))
            {
                rankColor = RGB(255, 220, 70);
            }
            else if (finalRank[0] == _T('A'))
            {
                rankColor = RGB(100, 200, 255);
            }
            else if (finalRank[0] == _T('B'))
            {
                rankColor = RGB(120, 255, 160);
            }
            else
            {
                rankColor = RGB(210, 210, 210);
            }

            // 右侧评级框
            setfillcolor(RGB(25, 28, 48));
            solidroundrect(455, 220, 610, 405, 24, 24);

            setlinecolor(rankColor);
            roundrect(455, 220, 610, 405, 24, 24);

            // 评级文字
            setfont(26, 0, _T("微软雅黑"));
            settextcolor(RGB(220, 220, 230));
            outtextxy(500, 245, _T("评级"));

            setfont(90, 0, _T("微软雅黑"));
            settextcolor(rankColor);

            int rankW = textwidth(finalRank);
            outtextxy(532 - rankW / 2, 285, finalRank);

            // 评级评价语
            const TCHAR* rankComment;

            if (finalRank[0] == _T('S'))
            {
                rankComment = _T("完美表现！");
            }
            else if (finalRank[0] == _T('A'))
            {
                rankComment = _T("非常优秀！");
            }
            else if (finalRank[0] == _T('B'))
            {
                rankComment = _T("顺利通关！");
            }
            else
            {
                rankComment = _T("继续加油！");
            }

            setfont(22, 0, _T("微软雅黑"));
            settextcolor(rankColor);

            int commentW = textwidth(rankComment);
            outtextxy(532 - commentW / 2, 375, rankComment);

            //================ 8.0-27 胜利界面按钮悬停判断 =================
            POINT winMouse;
            GetCursorPos(&winMouse);
            ScreenToClient(GetHWnd(), &winMouse);

            int winHoverButton = 0;

            // 鼠标在“R 重新开始”按钮上
            if (winMouse.x >= 205 && winMouse.x <= 370 &&
                winMouse.y >= 465 && winMouse.y <= 515)
            {
                winHoverButton = 1;
            }
            // 鼠标在“Q 退出游戏”按钮上
            else if (winMouse.x >= 430 && winMouse.x <= 595 &&
                winMouse.y >= 465 && winMouse.y <= 515)
            {
                winHoverButton = 2;
            }

            //================ 底部操作按钮 =================

            // R重新开始按钮
            if (winHoverButton == 1)
            {
                setfillcolor(RGB(95, 150, 255));
            }
            else
            {
                setfillcolor(RGB(70, 120, 255));
            }

            solidroundrect(205, 465, 370, 515, 16, 16);

            setlinecolor(RGB(150, 190, 255));
            roundrect(205, 465, 370, 515, 16, 16);

            // 鼠标悬停外发光
            if (winHoverButton == 1)
            {
                setlinecolor(RGB(230, 245, 255));
                roundrect(200, 460, 375, 520, 18, 18);
            }

            setfont(24, 0, _T("微软雅黑"));
            settextcolor(WHITE);
            DrawTextCenter(205, 465, 370, 515, _T("R 重新开始"));


            // Q退出按钮
            if (winHoverButton == 2)
            {
                setfillcolor(RGB(225, 100, 100));
            }
            else
            {
                setfillcolor(RGB(200, 80, 80));
            }

            solidroundrect(430, 465, 595, 515, 16, 16);

            setlinecolor(RGB(255, 160, 160));
            roundrect(430, 465, 595, 515, 16, 16);

            // 鼠标悬停外发光
            if (winHoverButton == 2)
            {
                setlinecolor(RGB(255, 220, 220));
                roundrect(425, 460, 600, 520, 18, 18);
            }

            setfont(24, 0, _T("微软雅黑"));
            settextcolor(WHITE);
            DrawTextCenter(430, 465, 595, 515, _T("Q 退出游戏"));

            //================ 通关后手动操作 =================

//================ 8.0-23 胜利界面鼠标点击 =================
            if (MouseHit())
            {
                m = GetMouseMsg();

                if (m.uMsg == WM_LBUTTONDOWN)
                {
                    // 点击“R 重新开始”按钮
                    if (m.x >= 205 && m.x <= 370 &&
                        m.y >= 465 && m.y <= 515)
                    {
                        PlaySound(NULL, NULL, 0);

                        ResetGame();

                        winFrame = 0;
                        winPlayed = false;

                        PlaySound(
                            TEXT("button.wav"),
                            NULL,
                            SND_FILENAME | SND_ASYNC
                        );

                        gameState = 1;
                    }

                    // 点击“Q 退出游戏”按钮
                    else if (m.x >= 430 && m.x <= 595 &&
                        m.y >= 465 && m.y <= 515)
                    {
                        PlaySound(NULL, NULL, 0);
                        closegraph();
                        return 0;
                    }
                }
            }

            // R重新开始
            if (GetAsyncKeyState('R') & 1)
            {
                PlaySound(NULL, NULL, 0);

                ResetGame();

                winFrame = 0;
                winPlayed = false;

                gameState = 1;
            }

            // Q退出
            if (GetAsyncKeyState('Q') & 1)
            {
                PlaySound(NULL, NULL, 0);
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

        // 死亡次数达到20次，游戏失败
        if (gameState == 1 && deathCount >= 20 && !(ok1 && ok2))
        {
            finalTimeSec = gameTimeSec;
            timerRunning = false;

            // 8.0-29：失败时统一统计钻石数量
            finalDiamondCount = CountDiamonds();

            // 进入失败界面前，清空临时效果
            ClearTempEffects();

            gameState = 3;
        }

        //================ 8.0-3 屏幕震动效果 =================
        if (shakeTime > 0)
        {
            IMAGE screenImg;

            // 把当前画面保存下来
            getimage(&screenImg, 0, 0, 800, 600);

            // 随机偏移
            int offsetX = rand() % (shakePower * 2 + 1) - shakePower;
            int offsetY = rand() % (shakePower * 2 + 1) - shakePower;

            // 清屏后，把刚才的画面偏移画出来
            setbkcolor(RGB(20, 25, 45));
            cleardevice();

            putimage(offsetX, offsetY, &screenImg);

            shakeTime--;

            if (shakeTime <= 0)
            {
                shakePower = 0;
            }
        }

        //================ 8.0-21 机关触发小提示：最上层显示 =================
        if (actionTipLife > 0)
        {
            actionTipLife--;

            setfont(18, 0, _T("黑体"));

            int textW = textwidth(actionTip);
            int textH = textheight(actionTip);

            int tipW = textW + 24;
            int tipH = textH + 12;

            // 比按钮再高一点，避免和人物重叠
            int tipX = actionTipX - tipW / 2;
            int tipY = actionTipY - 20 - (45 - actionTipLife) / 3;

            // 防止跑出屏幕
            if (tipX < 5) tipX = 5;
            if (tipX + tipW > 795) tipX = 795 - tipW;
            if (tipY < 55) tipY = 55;

            // 小阴影
            setfillcolor(RGB(8, 12, 22));
            solidroundrect(
                tipX + 2,
                tipY + 2,
                tipX + tipW + 2,
                tipY + tipH + 2,
                10,
                10
            );

            // 主体
            setfillcolor(RGB(32, 40, 65));
            solidroundrect(
                tipX,
                tipY,
                tipX + tipW,
                tipY + tipH,
                10,
                10
            );

            // 边框
            setlinecolor(RGB(130, 180, 240));
            roundrect(
                tipX,
                tipY,
                tipX + tipW,
                tipY + tipH,
                10,
                10
            );

            settextcolor(RGB(225, 240, 255));
            DrawTextCenter(
                tipX,
                tipY,
                tipX + tipW,
                tipY + tipH,
                actionTip
            );
        }

        FlushBatchDraw();
        Sleep(10);
    }
    closegraph();
    return 0;
}