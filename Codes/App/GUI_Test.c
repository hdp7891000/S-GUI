﻿#include "GUI_Test.h"
#include "GUI.h"
#include "GUI_GL_AA.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Bitmap.h"
#include "game.h"

#define WINDOW1      (WM_USER_ID + 0x0010)
#define WINDOW2      (WM_USER_ID + 0x0020)
#define WINDOW3      (WM_USER_ID + 0x0030)
#define WINDOW4      (WM_USER_ID + 0x0040)
#define WINDOW5      (WM_USER_ID + 0x0050)

#define WIN1_BTN1    (WINDOW1 + 0x0001)
#define WIN1_BTN2    (WINDOW1 + 0x0002)

#define WIN2_BTN1    (WINDOW2 + 0x0001)
#define WIN2_BTN2    (WINDOW2 + 0x0002)
#define WIN2_BTN3    (WINDOW3 + 0x0002)

void Create_Window1(void);

static char _Str[30];
static u_16 _FpsVal; /* 帧率 */
static u_8 _CPUUsage; /* CPU占用率 */

static void _RootWinPaint(WM_HWIN hWin)
{
    /* 绘制背景 */
    GUI_SetFGColor(0x00C0C0C0);
    //GUI_FillRect(0, 0, GUI_GetScreenWidth(), GUI_GetScreenHeight());
    GUI_DrawBitmap(0, 0, 480, 320, &bmpic_rootwin);
    GUI_DispString(10, 300, _Str);
}

static void _RootWinTimer(WM_HWIN hWin)
{
    //GUI_RECT Rect = { 10, 300, 150, 320 };

#ifdef _MSC_VER
    sprintf_s(_Str, sizeof(_Str), "FPS: %d, CPU: %d%%", _FpsVal, _CPUUsage);
#else
    sprintf(_Str, "FPS: %d, CPU: %d%%", _FpsVal, _CPUUsage);
#endif // _WIN32
    
    //WM_InvalidateRect(_hRootWin, &Rect);
    WM_Invalidate(_hRootWin);
    _FpsVal = 0; /* 帧率清零 */
    printf("Mem usage: %d bytes\n", GUI_GetMemUsage());
}

/* GUI测试 */
void GUI_Test(void)
{
    GUI_Init();
    /* 设置根窗口回调函数 */
    RootWinPaint_Cb = _RootWinPaint;
    RootWinTimer_Cb = _RootWinTimer;
    GUI_SetRootWindowTimer(1000);
    Create_Window1();
    GUI_SetFont(&GUI_FontUI17_4pp);
    while (1) {
        GUI_Delay(20);
        ++_FpsVal; /* 统计帧率 */
    }
}

void Window2_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hWin, hItem;

    hWin = WM_GetClientWindow(pMsg->hWin);
    switch (pMsg->MsgId) {
    case WM_CREATED:
        hItem = BUTTON_Create(12, 20, 70, 20, hWin, WIN2_BTN2, 0);
        BUTTON_SetTitle(hItem, "Button 1");
        hItem = BUTTON_Create(108, 20, 70, 20, hWin, WIN2_BTN3, 0);
        BUTTON_SetTitle(hItem, "Button 2");
        hItem = BUTTON_Create(70, 70, 70, 20, hWin, WIN2_BTN1, 0);
        BUTTON_SetTitle(hItem, "Exit");
        break;
    case WM_BUTTON_RELEASED:
        if (WM_GetDialogId(pMsg->hWinSrc) == WIN2_BTN1) {
            WM_DeleteWindow(pMsg->hWin);
        }
        break;
    case WM_KEYDOWN:
        switch (pMsg->Param) {
        case KEY_ESC:
            WM_DeleteWindow(pMsg->hWin);
            break;
        }
        break;
    }
}

void Create_Window2(void)
{
    WM_HWIN hWin;

    hWin = WINDOW_Create(7, 10, 200, 140, NULL, WINDOW2, WM_WS_MOVE, Window2_Cb);
    WINDOW_SetTitle(hWin, "Hello World!");
}

void Create_Window3(void)
{
    WINDOW_Create(0, 0, GUI_GetScreenWidth(),
        GUI_GetScreenHeight(), NULL, WINDOW3, WM_WS_MOVE, GameWin_Cb);
}

void Window1_Cb(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem, hWin = pMsg->hWin, hClient;

    switch (pMsg->MsgId) {
    case WM_CREATED:
        hClient = WM_GetClientWindow(hWin);
        hItem = BUTTON_Create(5, 5, 100, 20, hClient, WIN1_BTN1, 0);
        BUTTON_SetTitle(hItem, "Button Test");
        hItem = BUTTON_Create(5, 30, 100, 20, hClient, WIN1_BTN2, 0);
        BUTTON_SetTitle(hItem, "Cube Field");
        BUTTON_SetFont(hItem, &GUI_FontUI17_4pp);
        break;
    case WM_BUTTON_RELEASED:
        /* 根据点击的按键创建窗口 */
        switch (WM_GetDialogId(pMsg->hWinSrc)) {
        case WIN1_BTN1:
            Create_Window2();
            break;
        case WIN1_BTN2:
            Create_Window3();
            break;
        }
        break;
    }
}

void Create_Window1(void)
{
    GUI_HWIN hWin;

    hWin = WINDOW_Create(20, 20, 120, 80, NULL, WINDOW1, WM_WS_MOVE, Window1_Cb);
    WINDOW_SetTitle(hWin, "S-GUI Demo");
    WINDOW_SetFont(hWin, &GUI_FontUI17_4pp);
}
