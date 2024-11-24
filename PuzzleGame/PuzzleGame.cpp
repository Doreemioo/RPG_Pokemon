﻿// PuzzleGame.cpp : 定义应用程序的入口点。
//

#include "PuzzleGame.h"
using namespace std;
#include <string.h>
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HBITMAP bmp_StartButton;	//开始按钮图像资源
HBITMAP bmp_Player;			//玩家图像
HBITMAP bmp_NPC_MAN1;		//各位NPC图像
HBITMAP bmp_Background;		//生成的背景图像
HBITMAP bmp_map;			//地图砖块图像
HBITMAP bmp_dialog;			//对话框背景图像
HBITMAP bmp_monster1;		//怪物1图像
HBITMAP bmp_bloodbar;		//血条图像
HBITMAP bmp_hp_mp_box;		//血条蓝条背景框
HBITMAP bmp_mpbar;			//蓝条图像

Stage* currentStage = NULL; //当前场景状态
vector<NPC*> npcs;			//NPC列表
vector<Monster*> monsters;	//怪物列表
Player* player = NULL;		//玩家
vector<Button*> buttons;	//按钮	

int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;
bool keyUpDown = false;
bool keyDownDown = false;
bool keyLeftDown = false;
bool keyRightDown = false;
bool in_conversation = false;	//当前游戏处在对话状态
const wchar_t* converstaion_content = nullptr;	//当前对话的内容
int currentBloodBlocks = BLOOD_BLOCK_COUNT;
int currentmpBlocks = MP_BLOCK_COUNT;

//TODO 更多的全局变量


//帧
int PLAYER_FRAMES_HOLD[] = { 0 };
int PLAYER_FRAMES_HOLD_COUNT = sizeof(PLAYER_FRAMES_HOLD) / sizeof(int);
int NPC_FRAMES_HOLD[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 };
int NPC_FRAMES_HOLD_COUNT = sizeof(NPC_FRAMES_HOLD) / sizeof(int);
int FRAMES_WALK[] = {0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,};
int FRAMES_WALK_COUNT = sizeof(FRAMES_WALK) / sizeof(int);

//地图
//0空地 1草 2红花 3+7树 4/5/6/8/9/10/12/13/14土地 11蓝花 15路牌
int map_stage1[20][28] = {
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,},
	{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 5, 5, 5, 6, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 9, 9, 9,10, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 9, 9, 9,10, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 9, 9, 9,10, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,12,13,13,13,14, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 2, 2, 2, 2,11,11,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 2, 2, 2, 2,11,11,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 2, 2, 2, 2,11,11,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 2, 2, 2, 2,11,11,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 2, 2, 2, 2,11,11,11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,},
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,},
	{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,},
};
//第二个关卡地图
int map_stage2[20][28] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
};
int map[20][28] = { 0 };	//存储当前关卡的地图


// TODO: 在此添加其它全局变量

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PUZZLEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PUZZLEGAME));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PUZZLEGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PUZZLEGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, 
       WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化
       CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);
   SetMenu(hWnd, NULL);     //隐藏菜单栏
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // 初始化游戏窗体
        InitGame(hWnd, wParam, lParam);
        break;
    case WM_KEYDOWN:
        // 键盘按下事件
        KeyDown(hWnd, wParam, lParam);
        break;
    case WM_KEYUP:
        // 键盘松开事件
        KeyUp(hWnd, wParam, lParam);
        break;
    case WM_MOUSEMOVE:
        // 鼠标移动事件
        MouseMove(hWnd, wParam, lParam);
        break;
    case WM_LBUTTONDOWN:
        // 鼠标左键按下事件
        LButtonDown(hWnd, wParam, lParam);
        break;
    case WM_LBUTTONUP:
        // 鼠标左键松开事件
        LButtonUp(hWnd, wParam, lParam);
        break;
	case WM_TIMER:
		// 定时器事件
		if (currentStage != NULL && currentStage->timerOn) {
			TimerUpdate(hWnd, wParam, lParam);
			CheckCollision();
		}
		break;
    case WM_PAINT:
        // 绘图
        Paint(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//加载图像资源
	bmp_Background = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BACKGROUND));
	bmp_StartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_STARTBUTTON));
	bmp_Player = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_PLAYER));
	bmp_NPC_MAN1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_NPC_MAN1));
	bmp_map = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_MAP));
	bmp_dialog = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_DIALOG));
	bmp_monster1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_MONSTER1));
	bmp_bloodbar = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BLOODBAR));
	bmp_hp_mp_box = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_HP_MP_BOX));
	bmp_mpbar = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_MPBAR));
	
	//添加按钮
	Button* startButton = CreateButton(BUTTON_STARTGAME, bmp_StartButton, BUTTON_STARTGAME_WIDTH, BUTTON_STARTGAME_HEIGHT,
		(WINDOW_WIDTH - BUTTON_STARTGAME_WIDTH) / 2, (WINDOW_WIDTH - BUTTON_STARTGAME_HEIGHT) / 2);
	buttons.push_back(startButton);


	//初始化开始场景
	InitStage(hWnd, STAGE_STARTMENU);

	//初始化主计时器
	SetTimer(hWnd, TIMER_GAMETIMER, TIMER_GAMETIMER_ELAPSE, NULL);
}

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = true;
		break;
	case VK_DOWN:
		keyDownDown = true;
		break;
	case VK_LEFT:
		keyLeftDown = true;
		break;
	case VK_RIGHT:
		keyRightDown = true;
		break;
	case VK_SPACE:
		break;
	default:
		break;
	}
}

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// TODO
	switch (wParam)
	{
	case VK_UP:
		keyUpDown = false;
		break;
	case VK_DOWN:
		keyDownDown = false;
		break;
	case VK_LEFT:
		keyLeftDown = false;
		break;
	case VK_RIGHT:
		keyRightDown = false;
		break;
	case VK_SPACE:
		HandleConversationEvents(hWnd);
		break;
	default:
		break;
	}
}

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
}

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = true;

	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			if (button->x <= mouseX
				&& button->x + button->width >= mouseX
				&& button->y <= mouseY
				&& button->y + button->height >= mouseY)
			{
				switch (button->buttonID) {
				case BUTTON_STARTGAME:
				{
					//TODO：判断进入哪个关卡
					InitStage(hWnd, STAGE_1);
				}
				break;
				}
			}
		}
	}

}

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	mouseX = LOWORD(lParam);
	mouseY = HIWORD(lParam);
	mouseDown = false;
}

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{

	UpdatePlayer(hWnd);
	UpdateNPCs(hWnd);
	UpdateMonsters(hWnd);
	UpdateMaps(hWnd);
	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

//更新玩家状态
void UpdatePlayer(HWND hWnd) {
	//如果键盘按下，设置状态为WALK
	if (keyLeftDown || keyUpDown || keyRightDown || keyDownDown) {
		player->state = UNIT_STATE_WALK;
		player->frame_sequence = FRAMES_WALK;
		player->frame_count = FRAMES_WALK_COUNT;
		if (keyLeftDown) {
			player->direction = UNIT_DIRECT_LEFT;
		}
		else if (keyUpDown) {
			player->direction = UNIT_DIRECT_UP;
		}
		else if (keyRightDown) {
			player->direction = UNIT_DIRECT_RIGHT;
		}
		else if (keyDownDown) {
			player->direction = UNIT_DIRECT_DOWN;
		}
		player->frame_row = player->direction;
	}
	else {
		player->state = UNIT_STATE_HOLD;
		player->frame_sequence = PLAYER_FRAMES_HOLD;
		player->frame_count = PLAYER_FRAMES_HOLD_COUNT;
	}
	//更新位置
	if (player->state == UNIT_STATE_WALK) {
		int oldX = player->x;
		int oldY = player->y;

		switch (player->direction) {
		case UNIT_DIRECT_LEFT:
			player->x -= player->vx;
			break;
		case UNIT_DIRECT_UP:
			player->y -= player->vy;
			break;
		case UNIT_DIRECT_RIGHT:
			player->x += player->vx;
			break;
		case UNIT_DIRECT_DOWN:
			player->y += player->vy;
			break;
		default:
			break;
		};
		// 检查碰撞，如果碰撞则还原位置
		if (CheckCollision1(player->x / BLOCK_SIZE_X, player->y / BLOCK_SIZE_Y, player->direction, map_stage1)) {
			player->x = oldX;
			player->y = oldY;
		}
	}

	//动画运行到下一帧
	player->frame_id++;
	player->frame_id = player->frame_id % player->frame_count;
	player->frame_column = player->frame_sequence[player->frame_id];
}

//更新NPC状态
void UpdateNPCs(HWND hWnd) {

	//顺次更新每个npc
	for (int i = 0; i < npcs.size(); i++) {
		//动画运行到下一帧
		npcs[i]->frame_id++;
		npcs[i]->frame_id = npcs[i]->frame_id % npcs[i]->frame_count;
		npcs[i]->frame_column = npcs[i]->frame_sequence[npcs[i]->frame_id];
	}
}

//更新怪物状态
void UpdateMonsters(HWND hWnd)
{
	//顺次更新每个怪物
	for (int i = 0; i < monsters.size(); i++) {
		//动画运行到下一帧
		monsters[i]->frame_id++;
		monsters[i]->frame_id = monsters[i]->frame_id % monsters[i]->frame_count;
		monsters[i]->frame_column = monsters[i]->frame_sequence[monsters[i]->frame_id];
	}
}

// 地图切换逻辑
void UpdateMaps(HWND hWnd)
{
	//走到地图边界，切换到map2
	if (currentStage->stageID == STAGE_1 && player->y <= 0 && map[player->y / BLOCK_SIZE_Y][player->x / BLOCK_SIZE_Y] == 0)
	{
		InitStage(hWnd, STAGE_2);
	}
}

//处理各种交互事件
void UpdateEvents(HWND hWnd) 
{

}

//抬起空格时触发，开启对话
void HandleConversationEvents(HWND hWnd) 
{
	//当前已经开启对话，再按一次空格关闭
	if (in_conversation) {
		in_conversation = false;
		return;
	}
	//player与npc做碰撞检测，判断与哪个npc对话
	for (int i = 0; i < npcs.size(); i++) {
		NPC* npc = npcs[i];
		if (((player->x <= npc->x + 10 && npc->x <= player->x + HUMAN_SIZE_X + 10) || (npc->x <= player->x + 10 && player->x <= npc->x + HUMAN_SIZE_X + 10)) &&
			((player->y <= npc->y + 10 && npc->y <= player->y + HUMAN_SIZE_Y + 10) || (npc->y <= player->y + 10 && player->y <= npc->y + HUMAN_SIZE_X + 10))) {
			in_conversation = true;
			if (!npc->task_complete) {
				converstaion_content = npc->conversations_before[npc->next_conversation_id];
				if(npc->next_conversation_id < npc->conversations_before.size()-1)
					npc->next_conversation_id++;	//npc的这句话已经说完，下次该说下一句话了；如果已经说到最后一句话了，则一直重复
			}
			else {
				converstaion_content = npc->conversations_after[npc->next_conversation_id];
				if (npc->next_conversation_id < npc->conversations_after.size() - 1)
					npc->next_conversation_id++;	//npc的这句话已经说完，下次该说下一句话了；如果已经说到最后一句话了，则一直重复
			}
			
		}
	}
}

//玩家与NPC碰撞
void CheckCollision() {
	for (int i = 0; i < npcs.size(); i++) {
		NPC* npc = npcs[i];

		// 判断玩家和 NPC 是否重叠
		bool horizontalOverlap = (player->x < npc->x + HUMAN_SIZE_X) && (player->x + HUMAN_SIZE_X > npc->x);
		bool verticalOverlap = (player->y < npc->y + HUMAN_SIZE_Y - 10) && (player->y + HUMAN_SIZE_Y > npc->y);

		// 如果发生重叠，根据玩家的移动方向修正位置
		if (horizontalOverlap && verticalOverlap) {
			switch (player->direction) {
			case UNIT_DIRECT_LEFT:
				player->x = npc->x + HUMAN_SIZE_X + 1; // 偏移 1 像素，避免再次触发碰撞
				break;
			case UNIT_DIRECT_UP:
				player->y = npc->y + HUMAN_SIZE_Y - 7; // 偏移 1 像素
				break;
			case UNIT_DIRECT_RIGHT:
				player->x = npc->x - HUMAN_SIZE_X - 1; // 偏移 1 像素
				break;
			case UNIT_DIRECT_DOWN:
				player->y = npc->y - HUMAN_SIZE_Y - 1; // 偏移 1 像素
				break;
			default:
				break;
			}
		}
	}
}

// 碰撞检测函数
bool CheckCollision1(int x, int y, int direction, int map[20][28]) {
	// 根据方向计算玩家下一步的目标坐标
	int targetX = x;
	int targetY = y;
	switch (direction) {
	case UNIT_DIRECT_LEFT:
		targetX -= 1;
		break;
	case UNIT_DIRECT_UP:
		targetY -= 1;
		break;
	case UNIT_DIRECT_RIGHT:
		targetX += 1;
		break;
	case UNIT_DIRECT_DOWN:
		targetY += 1;
		break;
	default:
		break;
	}

	// 检查是否超出地图边界
	if (targetX < 0 || targetX >= 28 || targetY < 0 || targetY >= 20) {
		return true; // 碰撞，无法移动
	}

	// 检查是否碰到不可通过的障碍物（3, 7, 15）
	if (map[targetY][targetX] == 3 || map[targetY][targetX] == 7 || map[targetY][targetX] == 15 || map[targetY][targetX] == 12) {
		return true; // 碰撞，无法移动
	}

	return false; // 无碰撞，可以移动
}

//受伤扣血函数
void TakeDamage(int damageBlocks) {
	currentBloodBlocks -= damageBlocks; // 扣血
	if (currentBloodBlocks < 0) {
		currentBloodBlocks = 0; // 防止血条块数为负
	}
}

//用技能扣蓝函数
void TakeMp(int damageBlocks) {
	currentmpBlocks -= damageBlocks; // 扣血
	if (currentmpBlocks < 0) {
		currentmpBlocks = 0; // 防止血条块数为负
	}
}

//TODO: 添加游戏需要的更多函数

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y)
{
	Button* button = new Button();
	button->buttonID = buttonID;
	button->img = img;
	button->width = width;
	button->height = height;
	button->x = x;
	button->y = y;

	button->visible = false;
	return button;
}

// 添加主角函数
Player* CreatePlayer(int x, int y)
{
	Player* player = new Player();
	player->img = bmp_Player;
	player->x = x;
	player->y = y;
	player->direction = UNIT_DIRECT_RIGHT;
	player->vx = 5;
	player->vy = 5;

	player->state = UNIT_STATE_HOLD;
	player->frame_row = player->direction;
	player->frame_column = 0;
	player->frame_sequence = PLAYER_FRAMES_HOLD;
	player->frame_count = PLAYER_FRAMES_HOLD_COUNT;
	player->frame_id = 0;

	return player;
}

// 添加NPC函数
NPC* CreateNPC(int x, int y, int npc_id)
{
	NPC* npc = new NPC();
	npc->npcID = npc_id;
	npc->visible = true;
	npc->task_complete = false;
	npc->x = x;
	npc->y = y;
	npc->direction = UNIT_DIRECT_DOWN;
	npc->vx = 0;
	npc->vy = 0;
	npc->state = UNIT_STATE_HOLD;
	npc->frame_row = npc->direction;
	npc->frame_column = 0;
	npc->frame_sequence = NPC_FRAMES_HOLD;
	npc->frame_count = NPC_FRAMES_HOLD_COUNT;
	npc->frame_id = 0;
	npc->next_conversation_id = 0;
	//根据不同NPC初始化不同的图像和对话
	switch (npc_id)
	{
	case NPC_MAN1_ID: {
		npc->img = bmp_NPC_MAN1;
		npc->conversations_before.push_back(L"你好！我是花花镇的居民，很高兴认识你。");
		npc->conversations_before.push_back(L"最近花花镇有些不太平，幽暗森林里时不时会传来一些声响，镇子里的人们每天都惴惴不安，可以请你帮忙调查一下吗？");
		npc->conversations_before.push_back(L"你说幽暗森林怎么走？往城镇上方直走就是啦。");
		npc->conversations_after.push_back(L"这样啊，原来是有一只猫妖跑到森林里了，谢谢你把它赶走，这下镇子里又可以太平了。");
		break;
	}
	default:
		break;
	}

	return npc;
}

Monster* CreateMonster(int x, int y, int monster_id)
{
	Monster* monster = new Monster();
	monster->monsterID = monster_id;
	monster->visible = true;
	monster->task_complete = false;
	monster->x = x;
	monster->y = y;
	monster->direction = UNIT_DIRECT_DOWN;
	monster->vx = 0;
	monster->vy = 0;
	monster->state = UNIT_STATE_HOLD;
	monster->frame_row = monster->direction;
	monster->frame_column = 0;
	monster->frame_sequence = NPC_FRAMES_HOLD;
	monster->frame_count = NPC_FRAMES_HOLD_COUNT;
	monster->frame_id = 0;
	monster->next_conversation_id = 0;
	//根据不同ID初始化不同的图像和对话
	switch (monster_id)
	{
	case MONSTER_CAT_ID:
	{
		monster->img = bmp_monster1;
		monster->conversations_before.push_back(L"喵喵喵。");
		break;
	}
	default:
		break;
	}

	return monster;
}

// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例

	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;

	if (stageID == STAGE_STARTMENU) {
		currentStage->bg = bmp_Background;
		currentStage->timerOn = false;
		//显示开始界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (button->buttonID == BUTTON_STARTGAME)
				button->visible = true;
			else
				button->visible = false;
		}
	}
	//TODO：添加多个游戏场景
	else if (stageID == STAGE_1)
	{
		currentStage->bg = bmp_Background;
		currentStage->timerOn = true;
		memcpy(map, map_stage1, sizeof(map));	//初始化地图
		//显示游戏界面的按钮
		for (int i = 0; i < buttons.size(); i++)
		{
			if (false) //TODO：加载游戏界面需要的按钮
				buttons[i]->visible = true;
			else
				buttons[i]->visible = false;
		}
		if (player == NULL)
			player = CreatePlayer(200, 200);					//第一次调用：初始化player
		if (npcs.size() == 0) {
			npcs.push_back(CreateNPC(625, 200, NPC_MAN1_ID));	//第一次调用：初始化NPC
		}
		//NPC的可见性
		for (int i = 0; i < npcs.size(); i++)
		{
			NPC* npc = npcs[i];
			if (true) //TODO：加载游戏界面需要的按钮
				npc->visible = true;
			else
				npc->visible = false;
		}
		//Monster的可见性
		for (int i = 0; i < monsters.size(); i++)
		{
			Monster* monster = monsters[i];
			if (false) //TODO：加载游戏界面需要的按钮
				monster->visible = true;
			else
				monster->visible = false;
		}
	}
	else if (stageID == STAGE_2)
	{
		currentStage->bg = bmp_Background;
		currentStage->timerOn = true;
		memcpy(map, map_stage2, sizeof(map));
		for (int i = 0; i < buttons.size(); i++)
		{
			Button* button = buttons[i];
			if (false)
				button->visible = true;
			else
				button->visible = false;
		}
		if (player == NULL)
			player = CreatePlayer(200, 200);
		if (monsters.size() == 0) {
			monsters.push_back(CreateMonster(495, 205, MONSTER_CAT_ID));//初始化Monster
		}
		//NPC的可见性
		for (int i = 0; i < npcs.size(); i++)
		{
			NPC* npc = npcs[i];
			if (false) //TODO：加载游戏界面需要的按钮
				npc->visible = true;
			else
				npc->visible = false;
		}
		//Monster的可见性
		for (int i = 0; i < monsters.size(); i++)
		{
			Monster* monster = monsters[i];
			if (true) //TODO：加载游戏界面需要的按钮
				monster->visible = true;
			else
				monster->visible = false;
		}
	}

	//刷新显示
	InvalidateRect(hWnd, NULL, FALSE);
}

// 绘图函数
void Paint(HWND hWnd)
{

	PAINTSTRUCT ps;
	HDC hdc_window = BeginPaint(hWnd, &ps);

	HDC hdc_memBuffer = CreateCompatibleDC(hdc_window);
	HDC hdc_loadBmp = CreateCompatibleDC(hdc_window);

	//初始化缓存
	HBITMAP	blankBmp = CreateCompatibleBitmap(hdc_window, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(hdc_memBuffer, blankBmp);


	// 按场景分类绘制内容到缓存
	if (currentStage->stageID == STAGE_STARTMENU) {
		// 绘制背景到缓存
		SelectObject(hdc_loadBmp, currentStage->bg);
		TransparentBlt(
			hdc_memBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			hdc_loadBmp, 0, 0, BG_BITMAP_WIDTH, BG_BITMAP_HEIGHT,
			RGB(255, 255, 255));

	}
	else if (currentStage->stageID >= STAGE_1 && currentStage->stageID <= STAGE_2) //TODO：添加多个游戏场景
	{
		//绘制地图
		SelectObject(hdc_loadBmp, bmp_map);
		for (int i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
			for (int j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++) {
				TransparentBlt(
					hdc_memBuffer,
					j * BLOCK_SIZE_X, i * BLOCK_SIZE_Y,							// 界面上起始绘制点
					BLOCK_SIZE_X, BLOCK_SIZE_Y,									// 界面上绘制宽度高度
					hdc_loadBmp,
					(map[i][j] % 4) * BLOCK_BITMAP_SIZE_X,						// 位图上起始绘制点
					(map[i][j] / 4) * BLOCK_BITMAP_SIZE_Y,
					BLOCK_BITMAP_SIZE_X, BLOCK_BITMAP_SIZE_Y,					// 位图上绘制宽度高度
					RGB(255, 255, 255));										// 位图上的哪个颜色会被视为背景
			}
		}

		// 绘制玩家
		SelectObject(hdc_loadBmp, player->img);
		TransparentBlt(
			hdc_memBuffer,
			player->x - 0.5 * HUMAN_SIZE_X, player->y - 0.5 * HUMAN_SIZE_Y,			// 界面上起始绘制点
			HUMAN_SIZE_X, HUMAN_SIZE_Y,											// 界面上绘制宽度高度
			hdc_loadBmp,
			HUMAN_BITMAP_SIZE_X * player->frame_column, HUMAN_BITMAP_SIZE_Y * player->frame_row,	// 位图上起始绘制点
			HUMAN_BITMAP_SIZE_X, HUMAN_BITMAP_SIZE_Y,											// 位图上绘制宽度高度
			RGB(255, 255, 255)
		);
		//绘制npc
		for (int i = 0; i < npcs.size(); i++) {
			if (npcs[i]->visible) {
				SelectObject(hdc_loadBmp, npcs[i]->img);
				TransparentBlt(
					hdc_memBuffer,
					npcs[i]->x - 0.5 * HUMAN_SIZE_X, npcs[i]->y - 0.5 * HUMAN_SIZE_Y,			// 界面上起始绘制点
					HUMAN_SIZE_X, HUMAN_SIZE_Y,											// 界面上绘制宽度高度
					hdc_loadBmp,
					HUMAN_BITMAP_SIZE_X * npcs[i]->frame_column, HUMAN_BITMAP_SIZE_Y * npcs[i]->frame_row,	// 位图上起始绘制点
					HUMAN_BITMAP_SIZE_X, HUMAN_BITMAP_SIZE_Y,											// 位图上绘制宽度高度
					RGB(255, 255, 255)
				);
			}
		}
		//绘制怪物
		for (int i = 0; i < monsters.size(); i++) {
			if (monsters[i]->visible) {
				SelectObject(hdc_loadBmp, monsters[i]->img);
				TransparentBlt(
					hdc_memBuffer,
					monsters[i]->x - 0.5 * MOSTER_SIZE_X, monsters[i]->y - 0.5 * MOSTER_SIZE_Y,		// 界面上起始绘制点
					MOSTER_SIZE_X, MOSTER_SIZE_Y,											// 界面上绘制宽度高度
					hdc_loadBmp,
					MOSTER_BITMAP_SIZE_X * monsters[i]->frame_column, MOSTER_BITMAP_SIZE_Y * monsters[i]->frame_row,	// 位图上起始绘制点
					MOSTER_BITMAP_SIZE_X, MOSTER_BITMAP_SIZE_Y,											// 位图上绘制宽度高度
					RGB(255, 255, 255)
				);
			}
		}
		//如果正处在对话状态：绘制对话框
		if (in_conversation) {
			SelectObject(hdc_loadBmp, bmp_dialog);
			TransparentBlt(
				hdc_memBuffer,
				0, WINDOW_HEIGHT - DIALOG_SIZE_Y - 38, WINDOW_WIDTH - 16, DIALOG_SIZE_Y,					// 界面上绘制位置
				hdc_loadBmp,
				0, 0, DIALOG_BITMAP_SIZE_X, DIALOG_BITMAP_SIZE_Y,	// 位图上绘制位置
				RGB(255, 255, 255)
			);
			//绘制文字
			HFONT hFont = CreateFontW(
				20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
				L"SimSun");		//创建字体
			SelectObject(hdc_memBuffer, hFont);
			SetTextColor(hdc_memBuffer, RGB(0, 0, 0));	// 设置颜色:黑色字体白色背景
			SetBkColor(hdc_memBuffer, RGB(255, 255, 255));
			RECT rect;
			rect.left = 50;
			rect.top = WINDOW_HEIGHT - DIALOG_SIZE_Y - 18;
			rect.right = WINDOW_WIDTH - 110;
			rect.bottom = WINDOW_HEIGHT - 50;
			DrawTextW(hdc_memBuffer, converstaion_content, -1, &rect, DT_WORDBREAK);
		}

		//绘制血条蓝条背景框
		SelectObject(hdc_loadBmp, bmp_hp_mp_box);
		TransparentBlt(
			hdc_memBuffer,
			HP_MP_BOX_START_X, HP_MP_BOX_START_Y,    // 背景框在界面上的起始位置
			HP_MP_BOX_WIDTH, HP_MP_BOX_HEIGHT,      // 背景框宽高
			hdc_loadBmp,
			0, 0,                                   // 背景框在 BMP 图上的起始位置
			HP_MP_BOX_WIDTH, HP_MP_BOX_HEIGHT,      // BMP 图中背景框的宽高
			RGB(255, 255, 255)                      // 背景透明色
		);

		//绘制蓝条
		for (int i = 0; i < currentmpBlocks; ++i) {
			SelectObject(hdc_loadBmp, bmp_mpbar);
			TransparentBlt(
				hdc_memBuffer,
				MP_START_X + i * MP_BLOCK_WIDTH, // 每块横向排列
				MP_START_Y,                           // 固定纵坐标
				MP_BLOCK_WIDTH, MP_BLOCK_HEIGHT, // 每块血条的宽高
				hdc_loadBmp,
				0, 0,
				MP_BLOCK_WIDTH, MP_BLOCK_HEIGHT,
				RGB(255, 255, 255) // 背景透明色
			);
		}

		//绘制血条
		for (int i = 0; i < currentBloodBlocks; ++i) {
			SelectObject(hdc_loadBmp, bmp_bloodbar);
			TransparentBlt(
				hdc_memBuffer,
				BLOOD_START_X + i * BLOOD_BLOCK_WIDTH, // 每块横向排列
				BLOOD_START_Y,                           // 固定纵坐标
				BLOOD_BLOCK_WIDTH, BLOOD_BLOCK_HEIGHT, // 每块血条的宽高
				hdc_loadBmp,
				0, 0,
				BLOOD_BLOCK_WIDTH, BLOOD_BLOCK_HEIGHT,
				RGB(255, 255, 255) // 背景透明色
			);
		}
	}

	// 绘制按钮到缓存
	for (int i = 0; i < buttons.size(); i++)
	{
		Button* button = buttons[i];
		if (button->visible)
		{
			SelectObject(hdc_loadBmp, button->img);
			TransparentBlt(
				hdc_memBuffer, button->x, button->y,
				button->width, button->height,
				hdc_loadBmp, 0, 0, button->width, button->height,
				RGB(255, 255, 255)
			);
		}
	}

	// 最后将所有的信息绘制到屏幕上
	BitBlt(hdc_window, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hdc_memBuffer, 0, 0, SRCCOPY);

	// 回收资源所占的内存（非常重要）
	DeleteObject(blankBmp);
	DeleteDC(hdc_memBuffer);
	DeleteDC(hdc_loadBmp);

	// 结束绘制
	EndPaint(hWnd, &ps);
}
