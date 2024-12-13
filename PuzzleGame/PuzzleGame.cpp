// PuzzleGame.cpp : 定义应用程序的入口点。
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
HBITMAP bmp_backpack;		//背包图像
HBITMAP bmp_pokemon1;		//pokemon1图像
HBITMAP bmp_btn_backpack;	//背包按钮图像
HBITMAP bmp_delete_box;		//背包关闭键图像
HBITMAP bmp_start_bg;		//背包关闭键图像
HBITMAP bmp_pause_bg;		//暂停界面图像
HBITMAP bmp_sword;			//道具剑图标
HBITMAP bmp_shield;			//道具盾图标
HBITMAP bmp_pokemon_ball;	//道具精灵球图标
HBITMAP bmp_battlemap;		//战斗背景图标
HBITMAP bmp_victory;		//胜利图标
HBITMAP bmp_lose;			//失败图标

Stage* currentStage = NULL; //当前场景状态
vector<NPC*> npcs;			//NPC列表
vector<Monster*> monsters;	//怪物列表
vector<Pokemon*> pokemons;	//宝可梦列表
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
int currentpokemonmpBlocks = POKEMON_MP_BLOCK_COUNT;
int currentpokemonBloodBlocks = POKE_BLOOD_BLOCK_COUNT;
bool isBackpackOpen = false; // 用于记录背包是否打开
RECT backpack = { BACKPACK_START_X, BACKPACK_START_Y, BACKPACK_START_X + BACKPACK_WIDTH, BACKPACK_START_Y + BACKPACK_HEIGHT };//定义背包的尺寸
RECT backpackButton = { BTN_BACKPACK_START_X, BTN_BACKPACK_START_Y, BTN_BACKPACK_START_X + BTN_BACKPACK_WIDTH, BTN_BACKPACK_START_Y + BTN_BACKPACK_HEIGHT };//定义背包按钮的尺寸
RECT backpackDelete = { BTN_DELETE_BOX_START_X, BTN_DELETE_BOX_START_Y, BTN_DELETE_BOX_START_X + BTN_DELETE_BOX_WIDTH, BTN_DELETE_BOX_START_Y + BTN_DELETE_BOX_HEIGHT };//定义背包关闭按钮的尺寸
int x, y;//记录鼠标点击位置的坐标
bool isPaused = false; // 游戏是否处于暂停状态
bool show_reward_popup = false;
int inventory[4][4]; // 0表示空，1表示被剑占用（示例）
bool isInbattle;
struct BackpackItem {
	int itemID;      // 物品唯一标识符
	int inventoryX;  // 背包中的位置 X
	int inventoryY;  // 背包中的位置 Y
	int width;       // 物品宽度
	int height;      // 物品高度
	bool inInventory; // 是否在背包中
};
std::vector<BackpackItem> globalBackpack; // 全局背包数据
bool isMyturn = true;
bool isVictory = false;
bool isLose = false;
bool isBattled = false;
bool isPokemonAcceptance = false;
const wchar_t** battleDialogs = nullptr; // 动态指向当前的对话数组
int* dialogIndex = nullptr;             // 动态指向当前对话索引

Item sword1 = {
	false, // inInventory
	0,     // inventoryX
	0,     // inventoryY
	1,     // width
	3,     // height
	false, // isDragging
	0, 0,  // offsetX, offsetY
	200, 200, // screenX, screenY
	200, 200,
	bmp_sword  // img
};

Item sword2 = {
	false, // inInventory
	0,     // inventoryX
	0,     // inventoryY
	1,     // width
	3,     // height
	false, // isDragging
	0, 0,  // offsetX, offsetY
	300, 200, // screenX, screenY
	300, 200,
	bmp_sword  // img
};

Item shield = {
	false, // inInventory
	0,     // inventoryX
	0,     // inventoryY
	2,     // width
	2,     // height
	false, // isDragging
	0, 0,  // offsetX, offsetY
	400, 200, // screenX, screenY
	400, 200,
	bmp_shield  // img
};

Item pokemon_ball1 = {
	false, // inInventory
	0,     // inventoryX
	0,     // inventoryY
	1,     // width
	1,     // height
	false, // isDragging
	0, 0,  // offsetX, offsetY
	550, 250, // screenX, screenY
	550, 250,
	bmp_pokemon_ball  // img
};

Item pokemon_ball2 = {
	false, // inInventory
	0,     // inventoryX
	0,     // inventoryY
	1,     // width
	1,     // height
	false, // isDragging
	0, 0,  // offsetX, offsetY
	650, 250, // screenX, screenY
	650, 250,
	bmp_pokemon_ball  // img
};


//TODO 更多的全局变量


//帧
int PLAYER_FRAMES_HOLD[] = { 0 };
int PLAYER_FRAMES_HOLD_COUNT = sizeof(PLAYER_FRAMES_HOLD) / sizeof(int);
int NPC_FRAMES_HOLD[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 };
int NPC_FRAMES_HOLD_COUNT = sizeof(NPC_FRAMES_HOLD) / sizeof(int);
int FIRE_DRAGON_FRAMES[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 };
int FIRE_DRAGON_FRAME_COUNT(sizeof(FIRE_DRAGON_FRAMES) / sizeof(int));
int FRAMES_WALK[] = { 0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3, };
int FRAMES_WALK_COUNT = sizeof(FRAMES_WALK) / sizeof(int);

//地图
//0空地 1草 2红花 3+7树 4/5/6/8/9/10/12/13/14土地 11蓝花 15路牌
int map_stage1[20][28] = {
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,},
	{ 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 17, 18, 19, 20, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 27, 28, 29, 30, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 32, 33, 34, 35, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 37, 38, 39, 40, 0, 0, 0, 0, 0, 3,},
	{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 13, 13, 13, 14, 0, 0, 0, 0, 0, 7,},
	{ 3, 0, 0, 0, 2, 2, 2, 2,11,11,11, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,},
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
const wchar_t* battleDialogs1[] = {
	L"你：喷火龙！我记得你！村民们都把你当守护神，你为什么要伤害他们？之前村民们对你不好吗？吃我一击！…………按“X”键继续",
	L"喷火龙：你还不是一样！村民们把我当守护神，你却装备着两把剑一面盾攻击我！少废话！我早就看透你们了！看招！…………按“X”键继续",
	L"你：我只是不忍心看你再堕落下去了，你既然已经对村民出手，那我也无需留情，再见吧！…………按“X”键继续"
};
int dialogIndex1 = 0; // 当前显示的对话索引

const wchar_t* battleDialogs2[] = {
	L"你：喷火龙！我记得你！村民们都把你当守护神，你为什么要伤害他们？之前村民们对你不好吗？吃我一击！…………按“X”键继续",
	L"喷火龙：我已经没有回头之路了……你也无须再多说什么了，专心战斗就是对我最大的尊重…………按“X”键继续",
	L"你：你真的要继续堕落下去吗？或许天无绝人之路啊！…………按“X”键继续",
	L"喷火龙：我这副样子……已经没办法了，村民们不会再认可我了，回不去了…………按“X”键继续",
	L"你：但是也许我们可以先聊聊？来吧，加入我的队伍！…………按“U”键收服…………按“X”键继续"
};
int dialogIndex2 = 0; // 当前显示的对话索引

const wchar_t* battleDialogs3[] = {
	L"你：喷火龙！我记得你！村民们都把你当守护神，你为什么要伤害他们？之前村民们对你不好吗？吃我一击！…………按“X”键继续",
	L"喷火龙：竟然如此小瞧我！一把剑都不装备吗？少废话！吃我一击！…………按“X”键继续",
	L"你：我哪怕死在这里也要与你奋战到底！…………按“X”键继续",
	L"喷火龙：呵，不自量力，去死吧！…………按“X”键继续"
};
int dialogIndex3 = 0; // 当前显示的对话索引

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
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PUZZLEGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PUZZLEGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
	int swordW1 = 56.25 * sword1.width;
	int swordH1 = 56.25 * sword1.height;
	int swordW2 = 56.25 * sword2.width;
	int swordH2 = 56.25 * sword2.height;
	int shieldW = 56.25 * shield.width;
	int shieldH = 56.25 * shield.height;
	int pokemonballW1 = 56.25 * pokemon_ball1.width;
	int pokemonballH1 = 56.25 * pokemon_ball1.height;
	int pokemonballW2 = 56.25 * pokemon_ball2.width;
	int pokemonballH2 = 56.25 * pokemon_ball2.height;

	int sword1ScreenX = sword1.inInventory ? (BACKPACK_START_X + sword1.inventoryX * CELL_WIDTH) : sword1.screenX;
	int sword1ScreenY = sword1.inInventory ? (BACKPACK_START_Y + sword1.inventoryY * CELL_HEIGHT) : sword1.screenY;
	int sword2ScreenX = sword2.inInventory ? (BACKPACK_START_X + sword2.inventoryX * CELL_WIDTH) : sword2.screenX;
	int sword2ScreenY = sword2.inInventory ? (BACKPACK_START_Y + sword2.inventoryY * CELL_HEIGHT) : sword2.screenY;
	int shieldScreenX = shield.inInventory ? (BACKPACK_START_X + shield.inventoryX * CELL_WIDTH) : shield.screenX;
	int shieldScreenY = shield.inInventory ? (BACKPACK_START_Y + shield.inventoryY * CELL_HEIGHT) : shield.screenY;
	int pokemonballScreenX1 = pokemon_ball1.inInventory ? (BACKPACK_START_X + pokemon_ball1.inventoryX * CELL_WIDTH) : pokemon_ball1.screenX;
	int pokemonballScreenY1 = pokemon_ball1.inInventory ? (BACKPACK_START_Y + pokemon_ball1.inventoryY * CELL_HEIGHT) : pokemon_ball1.screenY;
	int pokemonballScreenX2 = pokemon_ball2.inInventory ? (BACKPACK_START_X + pokemon_ball2.inventoryX * CELL_WIDTH) : pokemon_ball2.screenX;
	int pokemonballScreenY2 = pokemon_ball2.inInventory ? (BACKPACK_START_Y + pokemon_ball2.inventoryY * CELL_HEIGHT) : pokemon_ball2.screenY;

	switch (message)
	{
	case WM_CREATE:
	{
		// 初始化游戏窗体
		InitGame(hWnd, wParam, lParam);
		break;
	}
	case WM_KEYDOWN:
	{
		// 键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		if (wParam == VK_ESCAPE) { // 判断是否按下 ESC 键
			isPaused = !isPaused; // 切换暂停状态
			InvalidateRect(hWnd, NULL, TRUE); // 触发重绘
		}
		if (wParam == 0x42) { // 假设按 B 键开关背包
			isBackpackOpen = !isBackpackOpen;
			InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
		}
		int swordCount = 0;
		for (const auto& item : globalBackpack) {
			if (item.itemID == 1 || item.itemID == 2) {
				swordCount++;
			}
		}
		switch (swordCount)
		{
		case 0:
			battleDialogs = battleDialogs3;
			dialogIndex = &dialogIndex3;
			break;
		case 1:
			battleDialogs = battleDialogs2;
			dialogIndex = &dialogIndex2;
			break;
		case 2:
			battleDialogs = battleDialogs1;
			dialogIndex = &dialogIndex1;
			break;
		default:
			break;
		}
		size_t dialogSize = 0;
		if (battleDialogs == battleDialogs1) {
			dialogSize = 3;
		}
		else if (battleDialogs == battleDialogs2) {
			dialogSize = 5;
		}
		else if (battleDialogs == battleDialogs3) {
			dialogSize = 4;
		}
		if (wParam == 0x58 && isInbattle) {
			if (*dialogIndex < dialogSize - 1) {
				(*dialogIndex)++; // 切换到下一个对话
				InvalidateRect(hWnd, NULL, TRUE); // 触发重绘
			}
			if (isPokemonAcceptance)
			{
				isBattled = true;
				isInbattle = false;
				InitStage(hWnd, STAGE_1);
				InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
			}
			else
			{
				if (isMyturn && currentmpBlocks > 0 && currentBloodBlocks > 0)
				{
					Sleep(2);
					currentpokemonBloodBlocks -= (swordCount+1); // 扣血
					if (currentpokemonBloodBlocks <= 0) {
						currentpokemonBloodBlocks = 0;// 防止血条块数为负
						isVictory = true;
					}
					currentmpBlocks -= 1; // 扣血
					if (currentmpBlocks < 0) {
						currentmpBlocks = 0; // 防止血条块数为负
					}
					isMyturn = false;
					InvalidateRect(hWnd, NULL, FALSE); // 触发重绘

				}
				else if (!isMyturn && currentpokemonmpBlocks > 0 && currentpokemonBloodBlocks > 0)
				{
					PokemonAttack(hWnd);
					InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
				}
				else if (isVictory)
				{
					isBattled = true;
					isInbattle = false;
					InitStage(hWnd, STAGE_1);
					InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
				}
				else if (isLose)
				{
					isInbattle = false;
					ResetGame(hWnd);
					InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
				}
			}
		}
		if (wParam == 0x55 && isInbattle)
		{
			if (isMyturn && currentBloodBlocks > 0 && currentmpBlocks > 0)
			{
				Sleep(10);
				if (currentpokemonBloodBlocks == 1)
				{
					isPokemonAcceptance = true;
					InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
				}
				else
				{
					MessageBox(hWnd, L"收服失败！", L"收服状态", MB_OK | MB_ICONINFORMATION);
					isMyturn = !isMyturn;
					InvalidateRect(hWnd, NULL, FALSE); // 触发重绘
				}
			}
		}
		break;
	}
	case WM_KEYUP:
	{
		// 键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	}
	case WM_MOUSEMOVE:
	{
		// 鼠标移动事件
		MouseMove(hWnd, wParam, lParam);

		if (sword1.isDragging) {
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);
			// 更新剑在屏幕上的位置
			sword1.screenX = mx - sword1.offsetX;
			sword1.screenY = my - sword1.offsetY;
			InvalidateRect(hWnd, NULL, FALSE); // 刷新绘图
		}
		if (sword2.isDragging) {
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);
			// 更新剑在屏幕上的位置
			sword2.screenX = mx - sword2.offsetX;
			sword2.screenY = my - sword2.offsetY;
			InvalidateRect(hWnd, NULL, FALSE); // 刷新绘图
		}
		if (shield.isDragging) {
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);
			// 更新剑在屏幕上的位置
			shield.screenX = mx - shield.offsetX;
			shield.screenY = my - shield.offsetY;
			InvalidateRect(hWnd, NULL, FALSE); // 刷新绘图
		}
		if (pokemon_ball1.isDragging) {
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);
			// 更新剑在屏幕上的位置
			pokemon_ball1.screenX = mx - pokemon_ball1.offsetX;
			pokemon_ball1.screenY = my - pokemon_ball1.offsetY;
			InvalidateRect(hWnd, NULL, FALSE); // 刷新绘图
		}
		if (pokemon_ball2.isDragging) {
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);
			// 更新剑在屏幕上的位置
			pokemon_ball2.screenX = mx - pokemon_ball2.offsetX;
			pokemon_ball2.screenY = my - pokemon_ball2.offsetY;
			InvalidateRect(hWnd, NULL, FALSE); // 刷新绘图
		}

		break;
	}
	case WM_LBUTTONDOWN:
	{
		LButtonDown(hWnd, wParam, lParam);
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		swordW1 = 56.25 * sword1.width;
		swordH1 = 56.25 * sword1.height;
		shieldW = 56.25 * shield.width;
		shieldH = 56.25 * shield.height;
		pokemonballW1 = 56.25 * pokemon_ball1.width;
		pokemonballH1 = 56.25 * pokemon_ball1.height;
		pokemonballW2 = 56.25 * pokemon_ball2.width;
		pokemonballH2 = 56.25 * pokemon_ball2.height;
		sword1ScreenX = sword1.inInventory ?
			(BACKPACK_START_X + sword1.inventoryX * CELL_WIDTH) : sword1.screenX;
		sword1ScreenY = sword1.inInventory ?
			(BACKPACK_START_Y + sword1.inventoryY * CELL_HEIGHT) : sword1.screenY;
		sword2ScreenX = sword2.inInventory ?
			(BACKPACK_START_X + sword2.inventoryX * CELL_WIDTH) : sword2.screenX;
		sword2ScreenY = sword2.inInventory ?
			(BACKPACK_START_Y + sword2.inventoryY * CELL_HEIGHT) : sword2.screenY;
		shieldScreenX = shield.inInventory ?
			(BACKPACK_START_X + shield.inventoryX * CELL_WIDTH) : shield.screenX;
		shieldScreenY = shield.inInventory ?
			(BACKPACK_START_Y + shield.inventoryY * CELL_HEIGHT) : shield.screenY;
		pokemonballScreenX1 = pokemon_ball1.inInventory ?
			(BACKPACK_START_X + pokemon_ball1.inventoryX * CELL_WIDTH) : pokemon_ball1.screenX;
		pokemonballScreenY1 = pokemon_ball1.inInventory ?
			(BACKPACK_START_Y + pokemon_ball1.inventoryY * CELL_HEIGHT) : pokemon_ball1.screenY;
		pokemonballScreenX2 = pokemon_ball2.inInventory ?
			(BACKPACK_START_X + pokemon_ball2.inventoryX * CELL_WIDTH) : pokemon_ball2.screenX;
		pokemonballScreenY2 = pokemon_ball2.inInventory ?
			(BACKPACK_START_Y + pokemon_ball2.inventoryY * CELL_HEIGHT) : pokemon_ball2.screenY;

		// 然后再做点击检测、拖拽逻辑
		if (x >= sword1ScreenX && x < sword1ScreenX + swordW1 &&
			y >= sword1ScreenY && y < sword1ScreenY + swordH1) {
			// 点中了剑
			sword1.isDragging = true;
			sword1.offsetX = x - sword1ScreenX;
			sword1.offsetY = y - sword1ScreenY;

			sword1.startX = sword1.screenX;
			sword1.startY = sword1.screenY;

			if (sword1.inInventory) {
				// 清空背包格子
				for (int i = 0; i < sword1.width; i++) {
					inventory[sword1.inventoryY][sword1.inventoryX + i] = 0;
				}
				sword1.inInventory = false;
			}
		}
		if (x >= sword2ScreenX && x < sword2ScreenX + swordW2 &&
			y >= sword2ScreenY && y < sword2ScreenY + swordH2) {
			// 点中了剑
			sword2.isDragging = true;
			sword2.offsetX = x - sword2ScreenX;
			sword2.offsetY = y - sword2ScreenY;

			sword2.startX = sword2.screenX;
			sword2.startY = sword2.screenY;

			if (sword2.inInventory) {
				// 清空背包格子
				for (int i = 0; i < sword2.width; i++) {
					inventory[sword2.inventoryY][sword2.inventoryX + i] = 0;
				}
				sword2.inInventory = false;
			}
		}
		if (x >= shieldScreenX && x < shieldScreenX + shieldW &&
			y >= shieldScreenY && y < shieldScreenY + shieldH) {
			// 点中了剑
			shield.isDragging = true;
			shield.offsetX = x - shieldScreenX;
			shield.offsetY = y - shieldScreenY;
			if (shield.inInventory) {
				// 清空背包格子
				for (int i = 0; i < shield.width; i++) {
					inventory[shield.inventoryY][shield.inventoryX + i] = 0;
				}
				shield.inInventory = false;
			}
		}
		if (x >= pokemonballScreenX1 && x < pokemonballScreenX1 + pokemonballW1 &&
			y >= pokemonballScreenY1 && y < pokemonballScreenY1 + pokemonballH1) {
			// 点中了剑
			pokemon_ball1.isDragging = true;
			pokemon_ball1.offsetX = x - pokemonballScreenX1;
			pokemon_ball1.offsetY = y - pokemonballScreenY1;
			if (pokemon_ball1.inInventory) {
				// 清空背包格子
				for (int i = 0; i < pokemon_ball1.width; i++) {
					inventory[pokemon_ball1.inventoryY][pokemon_ball1.inventoryX + i] = 0;
				}
				pokemon_ball1.inInventory = false;
			}
		}
		if (x >= pokemonballScreenX2 && x < pokemonballScreenX2 + pokemonballW2 &&
			y >= pokemonballScreenY2 && y < pokemonballScreenY2 + pokemonballH2) {
			// 点中了剑
			pokemon_ball2.isDragging = true;
			pokemon_ball2.offsetX = x - pokemonballScreenX2;
			pokemon_ball2.offsetY = y - pokemonballScreenY2;
			if (pokemon_ball2.inInventory) {
				// 清空背包格子
				for (int i = 0; i < pokemon_ball2.width; i++) {
					inventory[pokemon_ball2.inventoryY][pokemon_ball2.inventoryX + i] = 0;
				}
				pokemon_ball2.inInventory = false;
			}
		}


		// 然后处理背包按钮等其他逻辑
		if (isBackpackOpen && x >= backpackDelete.left && x <= backpackDelete.right &&
			y >= backpackDelete.top && y <= backpackDelete.bottom) {
			isBackpackOpen = false;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (x >= backpackButton.left && x <= backpackButton.right &&
			y >= backpackButton.top && y <= backpackButton.bottom && !isBackpackOpen) {
			isBackpackOpen = !isBackpackOpen;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (isPaused && x >= 390 && x <= 570 && y >= 280 && y <= 365) {
			isPaused = !isPaused;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (isPaused && x >= 390 && x <= 570 && y >= 385 && y <= 460) {
			PostQuitMessage(0);
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		// 鼠标左键松开事件
		LButtonUp(hWnd, wParam, lParam);
		if (sword1.isDragging && isBackpackOpen) {
			sword1.isDragging = false;

			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			// 尝试将剑放入背包
			// 计算鼠标松开时对应的背包格子坐标
			int cellX = (mx - BACKPACK_START_X) / CELL_WIDTH;
			int cellY = (my - BACKPACK_START_Y) / CELL_HEIGHT;

			// 检查是否拖到背包区域外
			bool isOutsideBackpack = (mx < BACKPACK_START_X || mx >= BACKPACK_START_X + BACKPACK_WIDTH ||
				my < BACKPACK_START_Y || my >= BACKPACK_START_Y + BACKPACK_HEIGHT);

			if (isOutsideBackpack) {
				// 弹出确认框
				int result = MessageBox(hWnd, L"是否丢弃该道具？", L"丢弃确认", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					// 用户选择“是”，从全局背包中删除
					globalBackpack.erase(
						std::remove_if(globalBackpack.begin(), globalBackpack.end(),
							[](const BackpackItem& item) { return item.itemID == 1; }),
						globalBackpack.end());
					sword1.inInventory = false; // 确保道具状态更新
					sword1.screenX = -51;        // 设置为不可见的屏幕坐标
					sword1.screenY = -51;
				}
				else {
					// 用户选择“否”，将道具放回原位
					sword1.screenX = sword1.startX;
					sword1.screenY = sword1.startY;
					sword1.inInventory = false; // 不在背包中
				}
			}
			else
			{
				// 检查是否能放置剑（需要3个连续横向格子）
				bool canPlace = false;
				if (cellX >= 0 && cellY >= 0 && cellX + sword1.width - 1 < 4 && cellY < 4) {
					// 检查这3个格子是否为空
					bool allEmpty = true;
					for (int i = 0; i < sword1.width; i++) {
						if (inventory[cellY][cellX + i] != 0) {
							allEmpty = false;
							break;
						}
					}
					if (allEmpty) {
						canPlace = true;
					}
				}

				if (canPlace) {
					// 放置剑
					for (int i = 0; i < sword1.width; i++) {
						for (int j = 0; j < sword1.height; j++)
						{
							inventory[cellY + j][cellX + i] = 1;
						}
					}
					sword1.inInventory = true;
					sword1.inventoryX = cellX;
					sword1.inventoryY = cellY;


					// 更新全局背包
					bool sword1Exists = false;
					for (auto& backpackSword1 : globalBackpack) {
						if (backpackSword1.itemID == 1) {
							backpackSword1.inventoryX = cellX;
							backpackSword1.inventoryY = cellY;
							backpackSword1.inInventory = true;
							sword1Exists = true;
							break;
						}
					}
					if (!sword1Exists) {
						globalBackpack.push_back({ 1, cellX, cellY, sword1.width, sword2.height, true });
					}
				}
				else {
					sword1.screenX = sword1.startX;
					sword1.screenY = sword1.startY;
					sword1.inInventory = false; // 确保状态更新正确
				}
			}
		}

		if (sword2.isDragging && isBackpackOpen) {
			sword2.isDragging = false;

			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			// 尝试将剑放入背包
			// 计算鼠标松开时对应的背包格子坐标
			int cellX = (mx - BACKPACK_START_X) / CELL_WIDTH;
			int cellY = (my - BACKPACK_START_Y) / CELL_HEIGHT;

			// 检查是否拖到背包区域外
			bool isOutsideBackpack = (mx < BACKPACK_START_X || mx >= BACKPACK_START_X + BACKPACK_WIDTH ||
				my < BACKPACK_START_Y || my >= BACKPACK_START_Y + BACKPACK_HEIGHT);

			if (isOutsideBackpack) {
				// 弹出确认框
				int result = MessageBox(hWnd, L"是否丢弃该道具？", L"丢弃确认", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					// 用户选择“是”，从全局背包中删除
					globalBackpack.erase(
						std::remove_if(globalBackpack.begin(), globalBackpack.end(),
							[](const BackpackItem& item) { return item.itemID == 2; }),
						globalBackpack.end());
					sword2.inInventory = false; // 确保道具状态更新
					sword2.screenX = -51;        // 设置为不可见的屏幕坐标
					sword2.screenY = -51;
				}
				else {
					// 用户选择“否”，将道具放回原位
					sword2.screenX = sword2.startX;
					sword2.screenY = sword2.startY;
					sword2.inInventory = false; // 不在背包中
				}
			}
			else
			{
				// 检查是否能放置剑（需要3个连续横向格子）
				bool canPlace = false;
				if (cellX >= 0 && cellY >= 0 && cellX + sword2.width - 1 < 4 && cellY < 4) {
					// 检查这3个格子是否为空
					bool allEmpty = true;
					for (int i = 0; i < sword2.width; i++) {
						if (inventory[cellY][cellX + i] != 0) {
							allEmpty = false;
							break;
						}
					}
					if (allEmpty) {
						canPlace = true;
					}
				}

				if (canPlace) {
					// 放置剑
					for (int i = 0; i < sword2.width; i++) {
						for (int j = 0; j < sword2.height; j++)
						{
							inventory[cellY + j][cellX + i] = 2;
						}
					}
					sword2.inInventory = true;
					sword2.inventoryX = cellX;
					sword2.inventoryY = cellY;

					// 更新全局背包
					bool sword2Exists = false;
					for (auto& backpackSword2 : globalBackpack) {
						if (backpackSword2.itemID == 2) {
							backpackSword2.inventoryX = cellX;
							backpackSword2.inventoryY = cellY;
							backpackSword2.inInventory = true;
							sword2Exists = true;
							break;
						}
					}
					if (!sword2Exists) {
						globalBackpack.push_back({ 2, cellX, cellY, sword2.width, sword2.height, true });
					}
				}
				else {
					sword2.screenX = sword2.startX;
					sword2.screenY = sword2.startY;
					sword2.inInventory = false; // 确保状态更新正确
				}
			}
		}

		if (shield.isDragging && isBackpackOpen) {
			shield.isDragging = false;

			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			// 尝试将剑放入背包
			// 计算鼠标松开时对应的背包格子坐标
			int cellX = (mx - BACKPACK_START_X) / CELL_WIDTH;
			int cellY = (my - BACKPACK_START_Y) / CELL_HEIGHT;

			// 检查是否拖到背包区域外
			bool isOutsideBackpack = (mx < BACKPACK_START_X || mx >= BACKPACK_START_X + BACKPACK_WIDTH ||
				my < BACKPACK_START_Y || my >= BACKPACK_START_Y + BACKPACK_HEIGHT);
			
			if (isOutsideBackpack) {
				// 弹出确认框
				int result = MessageBox(hWnd, L"是否丢弃该道具？", L"丢弃确认", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					// 用户选择“是”，从全局背包中删除
					globalBackpack.erase(
						std::remove_if(globalBackpack.begin(), globalBackpack.end(),
							[](const BackpackItem& item) { return item.itemID == 3; }),
						globalBackpack.end());
					shield.inInventory = false; // 确保道具状态更新
					shield.screenX = -51;        // 设置为不可见的屏幕坐标
					shield.screenY = -51;
				}
				else {
					// 用户选择“否”，将道具放回原位
					shield.screenX = shield.startX;
					shield.screenY = shield.startY;
					shield.inInventory = false; // 不在背包中
				}
			}
			else
			{
				// 检查是否能放置剑（需要3个连续横向格子）
				bool canPlace = false;
				if (cellX >= 0 && cellY >= 0 && cellX + shield.width - 1 < 4 && cellY < 4) {
					// 检查这3个格子是否为空
					bool allEmpty = true;
					for (int i = 0; i < shield.width; i++) {
						if (inventory[cellY][cellX + i] != 0) {
							allEmpty = false;
							break;
						}
					}
					if (allEmpty) {
						canPlace = true;
					}
				}

				if (canPlace) {
					// 放置剑
					for (int i = 0; i < shield.width; i++) {
						for (int j = 0; j < shield.height; j++)
						{
							inventory[cellY + j][cellX + i] = 3;
						}
					}
					shield.inInventory = true;
					shield.inventoryX = cellX;
					shield.inventoryY = cellY;

					// 更新全局背包
					bool shieldExists = false;
					for (auto& backpackshield : globalBackpack) {
						if (backpackshield.itemID == 3) {
							backpackshield.inventoryX = cellX;
							backpackshield.inventoryY = cellY;
							backpackshield.inInventory = true;
							shieldExists = true;
							break;
						}
					}
					if (!shieldExists) {
						globalBackpack.push_back({ 3, cellX, cellY, shield.width, shield.height, true });

					}
					else {
						shield.screenX = shield.startX;
						shield.screenY = shield.startY;
						shield.inInventory = false; // 确保状态更新正确
					}
				}
			}
		}

		if (pokemon_ball1.isDragging && isBackpackOpen) {
			pokemon_ball1.isDragging = false;

			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			// 尝试将剑放入背包
			// 计算鼠标松开时对应的背包格子坐标
			int cellX = (mx - BACKPACK_START_X) / CELL_WIDTH;
			int cellY = (my - BACKPACK_START_Y) / CELL_HEIGHT;

			// 检查是否拖到背包区域外
			bool isOutsideBackpack = (mx < BACKPACK_START_X || mx >= BACKPACK_START_X + BACKPACK_WIDTH ||
				my < BACKPACK_START_Y || my >= BACKPACK_START_Y + BACKPACK_HEIGHT);

			if (isOutsideBackpack) {
				// 弹出确认框
				int result = MessageBox(hWnd, L"是否丢弃该道具？", L"丢弃确认", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					// 用户选择“是”，从全局背包中删除
					globalBackpack.erase(
						std::remove_if(globalBackpack.begin(), globalBackpack.end(),
							[](const BackpackItem& item) { return item.itemID == 4; }),
						globalBackpack.end());
					pokemon_ball1.inInventory = false; // 确保道具状态更新
					pokemon_ball1.screenX = -51;        // 设置为不可见的屏幕坐标
					pokemon_ball1.screenY = -51;
				}
				else {
					// 用户选择“否”，将道具放回原位
					pokemon_ball1.screenX = pokemon_ball1.startX;
					pokemon_ball1.screenY = pokemon_ball1.startY;
					pokemon_ball1.inInventory = false; // 不在背包中
				}
			}
			else
			{
				// 检查是否能放置剑（需要3个连续横向格子）
				bool canPlace = false;
				if (cellX >= 0 && cellY >= 0 && cellX + pokemon_ball1.width - 1 < 4 && cellY < 4) {
					// 检查这3个格子是否为空
					bool allEmpty = true;
					for (int i = 0; i < pokemon_ball1.width; i++) {
						if (inventory[cellY][cellX + i] != 0) {
							allEmpty = false;
							break;
						}
					}
					if (allEmpty) {
						canPlace = true;
					}
				}

				if (canPlace) {
					// 放置剑
					for (int i = 0; i < pokemon_ball1.width; i++) {
						for (int j = 0; j < pokemon_ball1.height; j++)
						{
							inventory[cellY + j][cellX + i] = 4;
						}
					}
					pokemon_ball1.inInventory = true;
					pokemon_ball1.inventoryX = cellX;
					pokemon_ball1.inventoryY = cellY;

					// 更新全局背包
					bool pokemonball1Exists = false;
					for (auto& backpackpokemonball1 : globalBackpack) {
						if (backpackpokemonball1.itemID == 4) {
							backpackpokemonball1.inventoryX = cellX;
							backpackpokemonball1.inventoryY = cellY;
							backpackpokemonball1.inInventory = true;
							pokemonball1Exists = true;
							break;
						}
					}
					if (!pokemonball1Exists) {
						globalBackpack.push_back({ 4, cellX, cellY, pokemon_ball1.width, pokemon_ball1.height, true });
					}
				}
				else {
					pokemon_ball1.screenX = pokemon_ball1.startX;
					pokemon_ball1.screenY = pokemon_ball1.startY;
					pokemon_ball1.inInventory = false; // 确保状态更新正确
				}
			}
		}

		if (pokemon_ball2.isDragging && isBackpackOpen) {
			pokemon_ball2.isDragging = false;

			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			// 尝试将剑放入背包
			// 计算鼠标松开时对应的背包格子坐标
			int cellX = (mx - BACKPACK_START_X) / CELL_WIDTH;
			int cellY = (my - BACKPACK_START_Y) / CELL_HEIGHT;

			// 检查是否拖到背包区域外
			bool isOutsideBackpack = (mx < BACKPACK_START_X || mx >= BACKPACK_START_X + BACKPACK_WIDTH ||
				my < BACKPACK_START_Y || my >= BACKPACK_START_Y + BACKPACK_HEIGHT);

			if (isOutsideBackpack) {
				// 弹出确认框
				int result = MessageBox(hWnd, L"是否丢弃该道具？", L"丢弃确认", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					// 用户选择“是”，从全局背包中删除
					globalBackpack.erase(
						std::remove_if(globalBackpack.begin(), globalBackpack.end(),
							[](const BackpackItem& item) { return item.itemID == 5; }),
						globalBackpack.end());
					pokemon_ball2.inInventory = false; // 确保道具状态更新
					pokemon_ball2.screenX = -51;        // 设置为不可见的屏幕坐标
					pokemon_ball2.screenY = -51;
				}
				else {
					// 用户选择“否”，将道具放回原位
					pokemon_ball2.screenX = pokemon_ball2.startX;
					pokemon_ball2.screenY = pokemon_ball2.startY;
					pokemon_ball2.inInventory = false; // 不在背包中
				}
			}
			else
			{
				// 检查是否能放置剑（需要3个连续横向格子）
				bool canPlace = false;
				if (cellX >= 0 && cellY >= 0 && cellX + pokemon_ball2.width - 1 < 4 && cellY < 4) {
					// 检查这3个格子是否为空
					bool allEmpty = true;
					for (int i = 0; i < pokemon_ball2.width; i++) {
						if (inventory[cellY][cellX + i] != 0) {
							allEmpty = false;
							break;
						}
					}
					if (allEmpty) {
						canPlace = true;
					}
				}

				if (canPlace) {
					// 放置剑
					for (int i = 0; i < pokemon_ball2.width; i++) {
						for (int j = 0; j < pokemon_ball2.height; j++)
						{
							inventory[cellY + j][cellX + i] = 2;
						}
					}
					pokemon_ball2.inInventory = true;
					pokemon_ball2.inventoryX = cellX;
					pokemon_ball2.inventoryY = cellY;

					// 更新全局背包
					bool pokemonball2Exists = false;
					for (auto& backpackpokemonball2 : globalBackpack) {
						if (backpackpokemonball2.itemID == 5) {
							backpackpokemonball2.inventoryX = cellX;
							backpackpokemonball2.inventoryY = cellY;
							backpackpokemonball2.inInventory = true;
							pokemonball2Exists = true;
							break;
						}
					}

					if (!pokemonball2Exists) {
						globalBackpack.push_back({ 5, cellX, cellY, pokemon_ball2.width, pokemon_ball2.height, true });
					}
				}
				else {
					pokemon_ball2.screenX = pokemon_ball2.startX;
					pokemon_ball2.screenY = pokemon_ball2.startY;
					pokemon_ball2.inInventory = false; // 确保状态更新正确
				}
			}
			if (show_reward_popup) {
				if (sword1.inInventory && sword2.inInventory && shield.inInventory && pokemon_ball1.inInventory && pokemon_ball2.inInventory)
				{
					show_reward_popup = false; // 关闭奖励弹窗
				}
				else if (!isBackpackOpen)
				{
					show_reward_popup = false; // 关闭奖励弹窗
				}
			}
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		}
	case WM_TIMER:
	{
		// 定时器事件
		if (currentStage != NULL && currentStage->timerOn) {
			TimerUpdate(hWnd, wParam, lParam);
			CheckCollision();
			UpdatePokemons(hWnd);
		}
		break;
	}
	case WM_PAINT:
	{
		// 绘图
		Paint(hWnd);
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
	}
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
	bmp_backpack = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BACKPACK));
	bmp_pokemon1 = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_POKEMON1));
	bmp_btn_backpack = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BTN_BACKPACK));
	bmp_delete_box = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_DELETE_BOX));
	bmp_start_bg = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_START_BG));
	bmp_pause_bg = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_PAUSE));
	bmp_sword = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_SWORD));
	bmp_shield = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_SHIELD));
	bmp_pokemon_ball = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_POKEMON_BALL));
	bmp_battlemap = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BATTLEMAP));
	bmp_victory = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_VICTORY));
	bmp_lose = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_LOSE));


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
		GetInBattleMap(hWnd);
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

//更新宝可梦状态
void UpdatePokemons(HWND hWnd) {
	for (int i = 0; i < pokemons.size(); i++) {
		// 动画运行到下一帧
		pokemons[i]->frame_id++;
		pokemons[i]->frame_id = pokemons[i]->frame_id % pokemons[i]->frame_count; // 环绕回到0
		pokemons[i]->frame_column = pokemons[i]->frame_sequence[pokemons[i]->frame_id];
		if (pokemons[i]->moving) {
			// 更新位置
			pokemons[i]->x += pokemons[i]->vx;
			pokemons[i]->y += pokemons[i]->vy;

			// 检查边界，防止超出范围
			if (pokemons[i]->x < 672) {
				pokemons[i]->x = 672; // 调整位置到边界内
				pokemons[i]->vx = -pokemons[i]->vx; // 反转水平速度
			}
			if (pokemons[i]->x > 832) {
				pokemons[i]->x = 832;
				pokemons[i]->vx = -pokemons[i]->vx;
			}
			if (pokemons[i]->y < 416) {
				pokemons[i]->y = 416;
				pokemons[i]->vy = -pokemons[i]->vy; // 反转垂直速度
			}
			if (pokemons[i]->y > 576) {
				pokemons[i]->y = 576;
				pokemons[i]->vy = -pokemons[i]->vy;
			}

			// 减少停顿时间，直到停止运动
			pokemons[i]->waitTime--;
			if (pokemons[i]->waitTime <= 0) {
				pokemons[i]->moving = false;
			}
		}
		else {
			// 随机化运动参数，准备下一次运动
			RandomizePokemonMovement(pokemons[i]);
		}
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
	if (currentStage->stageID == STAGE_1 && isInbattle)
	{
		InitStage(hWnd, STAGE_3);
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
		for (int i = 0; i < npcs.size(); i++) {
			NPC* npc = npcs[i];
			// 判断当前NPC是否对话已到最后一句
			// 这里假设 conversations_before 有 3句，当 next_conversation_id 达到3时说明前面3句已说完
			if (!npc->task_complete && npc->next_conversation_id >= npc->conversations_before.size() - 1) {
				show_reward_popup = true;
				npc->task_complete = true;
			}
		}
		return;
	}
	//player与npc做碰撞检测，判断与哪个npc对话
	for (int i = 0; i < npcs.size(); i++) {
		NPC* npc = npcs[i];
		if (((player->x <= npc->x + 10 && npc->x <= player->x + HUMAN_SIZE_X + 10) || (npc->x <= player->x + 10 && player->x <= npc->x + HUMAN_SIZE_X + 10)) &&
			((player->y <= npc->y + 10 && npc->y <= player->y + HUMAN_SIZE_Y + 10) || (npc->y <= player->y + 10 && player->y <= npc->y + HUMAN_SIZE_X + 10))) {
			in_conversation = true;
			if (!npc->task_complete) {
				// 先检查 next_conversation_id 是否在范围内
				if (npc->next_conversation_id >= npc->conversations_before.size()) {
					// 超出范围，说明对话已结束，不再读取
					// 可根据需要直接 break 或设置一个默认内容
					converstaion_content = L"---------------";
				}
				else {
					converstaion_content = npc->conversations_before[npc->next_conversation_id];
				}

				// 再决定是否递增 next_conversation_id
				if (npc->next_conversation_id < npc->conversations_before.size() - 1)
					npc->next_conversation_id++;

			}
			else {
				if (npc->next_conversation_id >= npc->conversations_after.size()) {
					converstaion_content = L"---------------";
				}
				else {
					converstaion_content = npc->conversations_after[npc->next_conversation_id];
				}

				if (npc->next_conversation_id < npc->conversations_after.size() - 1)
					npc->next_conversation_id++;
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
	if (!isBattled)
	{
		for (int i = 0; i < pokemons.size(); i++) {
			Pokemon* pokemon = pokemons[i];

			// 判断玩家和 NPC 是否重叠
			bool poke_horizontalOverlap = (player->x < pokemon->x + HUMAN_SIZE_X) && (player->x + HUMAN_SIZE_X > pokemon->x);
			bool poke_verticalOverlap = (player->y < pokemon->y + HUMAN_SIZE_Y - 10) && (player->y + HUMAN_SIZE_Y > pokemon->y);

			// 如果发生重叠，根据玩家的移动方向修正位置
			if (poke_horizontalOverlap && poke_verticalOverlap) {
				switch (player->direction) {
				case UNIT_DIRECT_LEFT:
					player->x = pokemon->x + HUMAN_SIZE_X + 1; // 偏移 1 像素，避免再次触发碰撞
					break;
				case UNIT_DIRECT_UP:
					player->y = pokemon->y + HUMAN_SIZE_Y - 7; // 偏移 1 像素
					break;
				case UNIT_DIRECT_RIGHT:
					player->x = pokemon->x - HUMAN_SIZE_X - 1; // 偏移 1 像素
					break;
				case UNIT_DIRECT_DOWN:
					player->y = pokemon->y - HUMAN_SIZE_Y - 1; // 偏移 1 像素
					break;
				default:
					break;
				}
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
	if (map[targetY][targetX] == 3 || map[targetY][targetX] == 7 || map[targetY][targetX] == 15 || map[targetY][targetX] == 12 || map[targetY][targetX] == 16 || map[targetY][targetX] == 17 || map[targetY][targetX] == 18 || map[targetY][targetX] == 19 || map[targetY][targetX] == 20 || map[targetY][targetX] == 21 || map[targetY][targetX] == 25 || map[targetY][targetX] == 26 || map[targetY][targetX] == 30 || map[targetY][targetX] == 31 || map[targetY][targetX] == 35 || map[targetY][targetX] == 36 || map[targetY][targetX] == 33 || map[targetY][targetX] == 39 || map[targetY][targetX] == 40) {
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

//宝可梦随机运动函数
void RandomizePokemonMovement(Pokemon* pokemon) {
	pokemon->direction = rand() % 4; // 随机生成方向
	int speed = rand() % 2 + 1; // 随机生成速度 (1到5)
	switch (pokemon->direction) {
	case 0: // 向上
		pokemon->vx = 0;
		pokemon->vy = -speed;
		break;
	case 1: // 向右
		pokemon->vx = speed;
		pokemon->vy = 0;
		break;
	case 2: // 向下
		pokemon->vx = 0;
		pokemon->vy = speed;
		break;
	case 3: // 向左
		pokemon->vx = -speed;
		pokemon->vy = 0;
		break;
	}
	pokemon->waitTime = rand() % 100 + 50; // 将停留时间范围设为 50-150
	pokemon->moving = true;
}

//进入战斗
void GetInBattleMap(HWND hWnd) {
	for (int i = 0; i < pokemons.size(); i++) {
		Pokemon* pokemon = pokemons[i];
		if (((player->x <= pokemon->x + 10 && pokemon->x <= player->x + HUMAN_SIZE_X + 10) || (pokemon->x <= player->x + 10 && player->x <= pokemon->x + HUMAN_SIZE_X + 10)) &&
			((player->y <= pokemon->y + 10 && pokemon->y <= player->y + HUMAN_SIZE_Y + 10) || (pokemon->y <= player->y + 10 && player->y <= pokemon->y + HUMAN_SIZE_X + 10))) {
			isInbattle = true;
		}
	}

}

HBITMAP GetItemBitmap(int itemID) {
	switch (itemID) {
	case 1: // 剑
		return bmp_sword;
	case 2: // 盾
		return bmp_sword;
	case 3: // 精灵球1
		return bmp_shield;
	case 4: // 精灵球2
		return bmp_pokemon_ball;
	case 5: // 精灵球2
		return bmp_pokemon_ball;
	default:
		return NULL; // 返回空值表示未找到
	}
}

void PokemonAttack(HWND hWnd) {
	if (isMyturn == false)
	{
		Sleep(2);
		int shieldCount = 1;
		for (const auto& item : globalBackpack) {
			if (item.itemID == 3) {
				shieldCount++;
			}
		}
		shieldCount = shieldCount - 1;
		currentBloodBlocks -= (2-shieldCount); // 扣血
		if (currentBloodBlocks <= 0) {
			currentBloodBlocks = 0; // 防止血条块数为负
			isLose = true;
		}
		currentpokemonmpBlocks -= 1; // 扣血
		if (currentmpBlocks < 0) {
			currentmpBlocks = 0; // 防止血条块数为负
		}
		isMyturn = true;
	}
}

void ResetGame(HWND hWnd) {
	// 重置玩家的初始状态
	currentBloodBlocks = BLOOD_BLOCK_COUNT;
	currentmpBlocks = MP_BLOCK_COUNT;

	// 重置敌人状态
	currentpokemonBloodBlocks = POKE_BLOOD_BLOCK_COUNT;
	currentpokemonmpBlocks = POKEMON_MP_BLOCK_COUNT;

	// 重置游戏逻辑状态
	isInbattle = false;
	isVictory = false;
	isLose = false;
	isMyturn = true;
	const wchar_t* converstaion_content = nullptr;	//当前对话的内容

	// 清空背包或重置关卡等其他状态
	globalBackpack.clear();
	InitStage(hWnd, STAGE_STARTMENU); // 初始化第一个关卡
}

void DrawBattleDialog(HDC hdc) {
	// 获取当前对话内容
	const wchar_t* currentDialog = battleDialogs[*dialogIndex];

	// 创建字体
	HFONT hFont = CreateFontW(
		25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		L"Microsoft YaHei");
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 0, 0)); // 黑色字体
	SetBkColor(hdc, RGB(255, 255, 255)); // 白色背景

	// 绘制对话框内容
	RECT rect;
	rect.left = 130;
	rect.top = WINDOW_HEIGHT - DIALOG_SIZE_Y - 18;
	rect.right = WINDOW_WIDTH - 110;
	rect.bottom = WINDOW_HEIGHT - 50;

	DrawTextW(hdc, currentDialog, -1, &rect, DT_WORDBREAK);
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
		npc->conversations_before.push_back(L"村长：Ciallo！勇者！欢迎来到宝可梦：背包乱斗！");
		npc->conversations_before.push_back(L"村长：在这个世界中，宝可梦没有等级，只能通过道具不断变强！");
		npc->conversations_before.push_back(L"村长：最近下面的房子都被那只喷火龙糟蹋的不成样子了,TA以前明明不是这样的……");
		npc->conversations_before.push_back(L"你：啊？可是我还没有宝可梦？这怎么打？！");
		npc->conversations_before.push_back(L"村长：没关系！送给你这些基础道具，请全部拿走他们，每个道具会占据的空间，打开背包，将他们合理安放");
		npc->conversations_before.push_back(L"合理运用道具！击败或收服这只宝可梦吧！");
		npc->conversations_before.push_back(L"");
		npc->conversations_after.push_back(L"这样啊，原来是有一只猫妖跑到森林里了，谢谢你把它赶走，这下镇子里又可以太平了。");
		break;
	}
	default:
		break;
	}

	return npc;
}

//添加怪物函数
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

//添加宝可梦函数
Pokemon* CreatePokemon(int x, int y, int pokemon_id)
{
	Pokemon* pokemon = new Pokemon();
	pokemon->PokemonID = pokemon_id;
	pokemon->visible = true;
	pokemon->x = x;
	pokemon->y = y;
	pokemon->direction = UNIT_DIRECT_DOWN;
	pokemon->vx = 0;
	pokemon->vy = 0;
	pokemon->state = UNIT_STATE_HOLD;
	pokemon->frame_row = pokemon->direction;
	pokemon->frame_column = 0;
	pokemon->frame_sequence = FIRE_DRAGON_FRAMES;
	pokemon->frame_count = FIRE_DRAGON_FRAME_COUNT;
	pokemon->frame_id = 0;

	// 根据 PokemonID 设置帧序列
	if (pokemon_id == POKEMON_FIRE_DRAGON_ID) {
		pokemon->frame_sequence = FIRE_DRAGON_FRAMES;
		pokemon->frame_count = FIRE_DRAGON_FRAME_COUNT;
	}
	else {
		pokemon->frame_sequence = FIRE_DRAGON_FRAMES;
		pokemon->frame_count = FIRE_DRAGON_FRAME_COUNT;
	}
	// 加载对应的宝可梦图像资源
	if (pokemon_id == POKEMON_FIRE_DRAGON_ID) {
		pokemon->img = bmp_pokemon1;  // 假设 Fire Dragon 对应 bmp_pokemon1
	}

	return pokemon;
}

// 初始化游戏场景函数
void InitStage(HWND hWnd, int stageID)
{
	// 初始化场景实例

	if (currentStage != NULL) delete currentStage;
	currentStage = new Stage();
	currentStage->stageID = stageID;

	if (stageID == STAGE_STARTMENU) {
		currentStage->bg = bmp_start_bg;
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
			npcs.push_back(CreateNPC(625, 300, NPC_MAN1_ID));	//第一次调用：初始化NPC
		}
		if (pokemons.size() == 0 && !isBattled) {
			pokemons.push_back(CreatePokemon(800, 200, POKEMON_FIRE_DRAGON_ID));	//第一次调用：初始化NPC
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
		//宝可梦的可见性
		for (int i = 0; i < pokemons.size(); i++)
		{
			Pokemon* pokemon = pokemons[i];
			if (!isBattled) //TODO：加载游戏界面需要的按钮
				pokemon->visible = true;
			else
				pokemon->visible = false;
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

		if (isPaused)
		{
			SelectObject(hdc_loadBmp, bmp_pause_bg);
			TransparentBlt(
				hdc_memBuffer,
				PAUSE_BG_START_X, PAUSE_BG_START_Y,    // 背景框在界面上的起始位置
				PAUSE_BG_WIDTH, PAUSE_BG_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				PAUSE_BG_WIDTH, PAUSE_BG_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);
		}
		else
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
						(map[i][j] % 8) * BLOCK_BITMAP_SIZE_X,						// 位图上起始绘制点
						(map[i][j] / 8) * BLOCK_BITMAP_SIZE_Y,
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
			if (!isVictory)
			{
				//绘制宝可梦
				for (int i = 0; i < pokemons.size(); i++) {
					if (pokemons[i]->visible) {
						SelectObject(hdc_loadBmp, pokemons[i]->img);
						TransparentBlt(
							hdc_memBuffer,
							pokemons[i]->x - 0.5 * POKEMON_SIZE_X, pokemons[i]->y - 0.5 * POKEMON_SIZE_Y,		// 界面上起始绘制点
							POKEMON_SIZE_X, POKEMON_SIZE_Y,											// 界面上绘制宽度高度
							hdc_loadBmp,
							POKEMON_BITMAP_SIZE_X * pokemons[i]->frame_column, POKEMON_BITMAP_SIZE_Y * pokemons[i]->frame_row,	// 位图上起始绘制点
							POKEMON_BITMAP_SIZE_X, POKEMON_BITMAP_SIZE_Y,											// 位图上绘制宽度高度
							RGB(255, 255, 255)
						);
					}
				}
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

			if (isBackpackOpen)
			{
				//绘制背包背景
				SelectObject(hdc_loadBmp, bmp_backpack);
				TransparentBlt(
					hdc_memBuffer,
					BACKPACK_START_X, BACKPACK_START_Y,    // 背景框在界面上的起始位置
					BACKPACK_WIDTH, BACKPACK_HEIGHT,      // 背景框宽高
					hdc_loadBmp,
					0, 0,                                   // 背景框在 BMP 图上的起始位置
					BACKPACK_WIDTH, BACKPACK_HEIGHT,      // BMP 图中背景框的宽高
					RGB(255, 255, 255)                      // 背景透明色
				);
				SelectObject(hdc_loadBmp, bmp_delete_box);
				TransparentBlt(
					hdc_memBuffer,
					BTN_DELETE_BOX_START_X, BTN_DELETE_BOX_START_Y,    // 背景框在界面上的起始位置
					BTN_DELETE_BOX_WIDTH, BTN_DELETE_BOX_HEIGHT,      // 背景框宽高
					hdc_loadBmp,
					0, 0,                                   // 背景框在 BMP 图上的起始位置
					BTN_DELETE_BOX_WIDTH, BTN_DELETE_BOX_HEIGHT,      // BMP 图中背景框的宽高
					RGB(255, 255, 255)                      // 背景透明色
				);
			}

			if (!isBackpackOpen)
			{
				//绘制背包按钮
				SelectObject(hdc_loadBmp, bmp_btn_backpack);
				TransparentBlt(
					hdc_memBuffer,
					BTN_BACKPACK_START_X, BTN_BACKPACK_START_Y,    // 背景框在界面上的起始位置
					BTN_BACKPACK_WIDTH, BTN_BACKPACK_HEIGHT,      // 背景框宽高
					hdc_loadBmp,
					0, 0,                                   // 背景框在 BMP 图上的起始位置
					BTN_BACKPACK_WIDTH, BTN_BACKPACK_HEIGHT,      // BMP 图中背景框的宽高
					RGB(255, 255, 255)                      // 背景透明色
				);
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
			bool swordVisible = false;
			if (show_reward_popup)
			{
				if (sword1.screenX >= -50 && sword1.screenY >= -50)
				{
					SelectObject(hdc_loadBmp, bmp_sword);
					TransparentBlt(
						hdc_memBuffer,
						-1000, -1000,    // 背景框在界面上的起始位置
						SWORD_WIDTH, SWORD_HEIGHT,      // 背景框宽高
						hdc_loadBmp,
						0, 0,                                   // 背景框在 BMP 图上的起始位置
						SWORD_WIDTH, SWORD_HEIGHT,      // BMP 图中背景框的宽高
						RGB(255, 255, 255)                      // 背景透明色
					);
					// 绘制剑
					SelectObject(hdc_loadBmp, sword1.img);
					// 假设剑的单个方格对应的图案大小为32x32
					int swordDrawW1 = 56.25 * sword1.width;
					int swordDrawH1 = 56.25 * sword1.height;

					int drawX, drawY;
					if (sword1.inInventory) {
						drawX = BACKPACK_START_X + sword1.inventoryX * CELL_WIDTH;
						drawY = BACKPACK_START_Y + sword1.inventoryY * CELL_HEIGHT;
					}
					else {
						drawX = sword1.screenX;
						drawY = sword1.screenY;
					}

					TransparentBlt(
						hdc_memBuffer,
						drawX, drawY,
						swordDrawW1, swordDrawH1,
						hdc_loadBmp,
						0, 0,
						swordDrawW1, swordDrawH1,
						RGB(255, 255, 255)
					);

				}
				if (sword2.screenX >= -50 && sword2.screenY >= -50)
				{
					SelectObject(hdc_loadBmp, bmp_sword);
					TransparentBlt(
						hdc_memBuffer,
						-1000, -1000,    // 背景框在界面上的起始位置
						SWORD_WIDTH, SWORD_HEIGHT,      // 背景框宽高
						hdc_loadBmp,
						0, 0,                                   // 背景框在 BMP 图上的起始位置
						SWORD_WIDTH, SWORD_HEIGHT,      // BMP 图中背景框的宽高
						RGB(255, 255, 255)                      // 背景透明色
					);
					// 绘制剑
					SelectObject(hdc_loadBmp, sword2.img);
					// 假设剑的单个方格对应的图案大小为32x32
					int swordDrawW2 = 56.25 * sword2.width;
					int swordDrawH2 = 56.25 * sword2.height;

					int drawX0, drawY0;
					if (sword2.inInventory) {
						drawX0 = BACKPACK_START_X + sword2.inventoryX * CELL_WIDTH;
						drawY0 = BACKPACK_START_Y + sword2.inventoryY * CELL_HEIGHT;
					}
					else {
						drawX0 = sword2.screenX;
						drawY0 = sword2.screenY;
					}

					TransparentBlt(
						hdc_memBuffer,
						drawX0, drawY0,
						swordDrawW2, swordDrawH2,
						hdc_loadBmp,
						0, 0,
						swordDrawW2, swordDrawH2,
						RGB(255, 255, 255)
					);
				}
				if (shield.screenX >= -50 && shield.screenY >= -50)
				{
					SelectObject(hdc_loadBmp, bmp_shield);
					TransparentBlt(
						hdc_memBuffer,
						-1000, -1000,    // 背景框在界面上的起始位置
						SHIELD_WIDTH, SHIELD_HEIGHT,      // 背景框宽高
						hdc_loadBmp,
						0, 0,                                   // 背景框在 BMP 图上的起始位置
						SHIELD_WIDTH, SHIELD_HEIGHT,      // BMP 图中背景框的宽高
						RGB(255, 255, 255)                      // 背景透明色
					);
					SelectObject(hdc_loadBmp, shield.img);
					// 假设剑的单个方格对应的图案大小为32x32
					int shieldDrawW = 56.25 * shield.width;
					int shieldDrawH = 56.25 * shield.height;

					int drawX1, drawY1;
					if (shield.inInventory) {
						drawX1 = BACKPACK_START_X + shield.inventoryX * CELL_WIDTH;
						drawY1 = BACKPACK_START_Y + shield.inventoryY * CELL_HEIGHT;
					}
					else {
						drawX1 = shield.screenX;
						drawY1 = shield.screenY;
					}

					TransparentBlt(
						hdc_memBuffer,
						drawX1, drawY1,
						shieldDrawW, shieldDrawH,
						hdc_loadBmp,
						0, 0,
						shieldDrawW, shieldDrawH,
						RGB(255, 255, 255)
					);
				}
				if (pokemon_ball1.screenX >= -50 && pokemon_ball1.screenY >= -50)
				{
					SelectObject(hdc_loadBmp, bmp_pokemon_ball);
					TransparentBlt(
						hdc_memBuffer,
						-1000, -1000,    // 背景框在界面上的起始位置
						POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // 背景框宽高
						hdc_loadBmp,
						0, 0,                                   // 背景框在 BMP 图上的起始位置
						POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // BMP 图中背景框的宽高
						RGB(255, 255, 255)                      // 背景透明色
					);

					SelectObject(hdc_loadBmp, pokemon_ball2.img);
					// 假设剑的单个方格对应的图案大小为32x32
					int pokemonDrawW2 = 56.25 * pokemon_ball2.width;
					int pokemonDrawH2 = 56.25 * pokemon_ball2.height;

					int drawX3, drawY3;
					if (pokemon_ball2.inInventory) {
						drawX3 = BACKPACK_START_X + pokemon_ball2.inventoryX * CELL_WIDTH;
						drawY3 = BACKPACK_START_Y + pokemon_ball2.inventoryY * CELL_HEIGHT;
					}
					else {
						drawX3 = pokemon_ball2.screenX;
						drawY3 = pokemon_ball2.screenY;
					}

					TransparentBlt(
						hdc_memBuffer,
						drawX3, drawY3,
						pokemonDrawW2, pokemonDrawH2,
						hdc_loadBmp,
						0, 0,
						pokemonDrawW2, pokemonDrawH2,
						RGB(255, 255, 255)
					);

				}
				if (pokemon_ball2.screenX >= -50 && pokemon_ball2.screenY >= -50)
				{
					SelectObject(hdc_loadBmp, bmp_pokemon_ball);
					TransparentBlt(
						hdc_memBuffer,
						-1000, -1000,    // 背景框在界面上的起始位置
						POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // 背景框宽高
						hdc_loadBmp,
						0, 0,                                   // 背景框在 BMP 图上的起始位置
						POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // BMP 图中背景框的宽高
						RGB(255, 255, 255)                      // 背景透明色
					);

					SelectObject(hdc_loadBmp, pokemon_ball1.img);
					// 假设剑的单个方格对应的图案大小为32x32
					int pokemonDrawW1 = 56.25 * pokemon_ball1.width;
					int pokemonDrawH1 = 56.25 * pokemon_ball1.height;

					int drawX2, drawY2;
					if (pokemon_ball1.inInventory) {
						drawX2 = BACKPACK_START_X + pokemon_ball1.inventoryX * CELL_WIDTH;
						drawY2 = BACKPACK_START_Y + pokemon_ball1.inventoryY * CELL_HEIGHT;
					}
					else {
						drawX2 = pokemon_ball1.screenX;
						drawY2 = pokemon_ball1.screenY;
					}

					TransparentBlt(
						hdc_memBuffer,
						drawX2, drawY2,
						pokemonDrawW1, pokemonDrawH1,
						hdc_loadBmp,
						0, 0,
						pokemonDrawW1, pokemonDrawH1,
						RGB(255, 255, 255)
					);

				}
			}

			else if (sword1.inInventory || sword2.inInventory || shield.inInventory || sword1.isDragging || sword2.isDragging || shield.isDragging)
			{
				if (isBackpackOpen)
				{
					if (sword1.screenX >= -50 && sword1.screenY >= -50)
					{
						SelectObject(hdc_loadBmp, bmp_sword);
						TransparentBlt(
							hdc_memBuffer,
							-1000, -1000,    // 背景框在界面上的起始位置
							SWORD_WIDTH, SWORD_HEIGHT,      // 背景框宽高
							hdc_loadBmp,
							0, 0,                                   // 背景框在 BMP 图上的起始位置
							SWORD_WIDTH, SWORD_HEIGHT,      // BMP 图中背景框的宽高
							RGB(255, 255, 255)                      // 背景透明色
						);
						// 绘制剑
						SelectObject(hdc_loadBmp, sword1.img);
						// 假设剑的单个方格对应的图案大小为32x32
						int swordDrawW = 56.25 * sword1.width;
						int swordDrawH = 56.25 * sword1.height;

						int drawX, drawY;
						if (sword1.inInventory) {
							drawX = BACKPACK_START_X + sword1.inventoryX * CELL_WIDTH;
							drawY = BACKPACK_START_Y + sword1.inventoryY * CELL_HEIGHT;
						}
						else {
							drawX = sword1.screenX;
							drawY = sword1.screenY;
						}

						TransparentBlt(
							hdc_memBuffer,
							drawX, drawY,
							swordDrawW, swordDrawH,
							hdc_loadBmp,
							0, 0,
							swordDrawW, swordDrawH,
							RGB(255, 255, 255)
						);

					}
					if (sword2.screenX >= -50 && sword2.screenY >= -50)
					{
						SelectObject(hdc_loadBmp, bmp_sword);
						TransparentBlt(
							hdc_memBuffer,
							-1000, -1000,    // 背景框在界面上的起始位置
							SWORD_WIDTH, SWORD_HEIGHT,      // 背景框宽高
							hdc_loadBmp,
							0, 0,                                   // 背景框在 BMP 图上的起始位置
							SWORD_WIDTH, SWORD_HEIGHT,      // BMP 图中背景框的宽高
							RGB(255, 255, 255)                      // 背景透明色
						);
						// 绘制剑
						SelectObject(hdc_loadBmp, sword2.img);
						// 假设剑的单个方格对应的图案大小为32x32
						int swordDrawW1 = 56.25 * sword2.width;
						int swordDrawH1 = 56.25 * sword2.height;

						int drawX0, drawY0;
						if (sword2.inInventory) {
							drawX0 = BACKPACK_START_X + sword2.inventoryX * CELL_WIDTH;
							drawY0 = BACKPACK_START_Y + sword2.inventoryY * CELL_HEIGHT;
						}
						else {
							drawX0 = sword2.screenX;
							drawY0 = sword2.screenY;
						}

						TransparentBlt(
							hdc_memBuffer,
							drawX0, drawY0,
							swordDrawW1, swordDrawH1,
							hdc_loadBmp,
							0, 0,
							swordDrawW1, swordDrawH1,
							RGB(255, 255, 255)
						);
					}
					if (shield.screenX >= -50 && shield.screenY >= -50)
					{
						SelectObject(hdc_loadBmp, bmp_shield);
						TransparentBlt(
							hdc_memBuffer,
							-1000, -1000,    // 背景框在界面上的起始位置
							SHIELD_WIDTH, SHIELD_HEIGHT,      // 背景框宽高
							hdc_loadBmp,
							0, 0,                                   // 背景框在 BMP 图上的起始位置
							SHIELD_WIDTH, SHIELD_HEIGHT,      // BMP 图中背景框的宽高
							RGB(255, 255, 255)                      // 背景透明色
						);
						SelectObject(hdc_loadBmp, shield.img);
						// 假设剑的单个方格对应的图案大小为32x32
						int shieldDrawW = 56.25 * shield.width;
						int shieldDrawH = 56.25 * shield.height;

						int drawX1, drawY1;
						if (shield.inInventory) {
							drawX1 = BACKPACK_START_X + shield.inventoryX * CELL_WIDTH;
							drawY1 = BACKPACK_START_Y + shield.inventoryY * CELL_HEIGHT;
						}
						else {
							drawX1 = shield.screenX;
							drawY1 = shield.screenY;
						}

						TransparentBlt(
							hdc_memBuffer,
							drawX1, drawY1,
							shieldDrawW, shieldDrawH,
							hdc_loadBmp,
							0, 0,
							shieldDrawW, shieldDrawH,
							RGB(255, 255, 255)
						);
					}
					if (pokemon_ball1.screenX >= -50 && pokemon_ball1.screenY >= -50)
					{
						SelectObject(hdc_loadBmp, bmp_pokemon_ball);
						TransparentBlt(
							hdc_memBuffer,
							-1000, -1000,    // 背景框在界面上的起始位置
							POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // 背景框宽高
							hdc_loadBmp,
							0, 0,                                   // 背景框在 BMP 图上的起始位置
							POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // BMP 图中背景框的宽高
							RGB(255, 255, 255)                      // 背景透明色
						);
						SelectObject(hdc_loadBmp, pokemon_ball1.img);
						// 假设剑的单个方格对应的图案大小为32x32
						int pokemonballDrawW1 = 56.25 * pokemon_ball1.width;
						int poekmonballDrawH1 = 56.25 * pokemon_ball1.height;

						int drawX2, drawY2;
						if (pokemon_ball1.inInventory) {
							drawX2 = BACKPACK_START_X + pokemon_ball1.inventoryX * CELL_WIDTH;
							drawY2 = BACKPACK_START_Y + pokemon_ball1.inventoryY * CELL_HEIGHT;
						}
						else {
							drawX2 = pokemon_ball1.screenX;
							drawY2 = pokemon_ball1.screenY;
						}

						TransparentBlt(
							hdc_memBuffer,
							drawX2, drawY2,
							pokemonballDrawW1, poekmonballDrawH1,
							hdc_loadBmp,
							0, 0,
							pokemonballDrawW1, poekmonballDrawH1,
							RGB(255, 255, 255)
						);
					}
					if (pokemon_ball2.screenX >= -50 && pokemon_ball2.screenY >= -50)
					{
						SelectObject(hdc_loadBmp, bmp_pokemon_ball);
						TransparentBlt(
							hdc_memBuffer,
							-1000, -1000,    // 背景框在界面上的起始位置
							POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // 背景框宽高
							hdc_loadBmp,
							0, 0,                                   // 背景框在 BMP 图上的起始位置
							POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // BMP 图中背景框的宽高
							RGB(255, 255, 255)                      // 背景透明色
						);
						SelectObject(hdc_loadBmp, pokemon_ball2.img);
						// 假设剑的单个方格对应的图案大小为32x32
						int pokemonballDrawW2 = 56.25 * pokemon_ball2.width;
						int poekmonballDrawH2 = 56.25 * pokemon_ball2.height;

						int drawX3, drawY3;
						if (pokemon_ball2.inInventory) {
							drawX3 = BACKPACK_START_X + pokemon_ball2.inventoryX * CELL_WIDTH;
							drawY3 = BACKPACK_START_Y + pokemon_ball2.inventoryY * CELL_HEIGHT;
						}
						else {
							drawX3 = pokemon_ball2.screenX;
							drawY3 = pokemon_ball2.screenY;
						}

						TransparentBlt(
							hdc_memBuffer,
							drawX3, drawY3,
							pokemonballDrawW2, poekmonballDrawH2,
							hdc_loadBmp,
							0, 0,
							pokemonballDrawW2, poekmonballDrawH2,
							RGB(255, 255, 255)
						);
					}
				}
			}

			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					// 这里可以绘制格子边框等
					int gx = BACKPACK_START_X + c * CELL_WIDTH;
					int gy = BACKPACK_START_Y + r * CELL_HEIGHT;
					// 可选：绘制格子框
					// Rectangle(hdc_memBuffer, gx, gy, gx + CELL_WIDTH, gy + CELL_HEIGHT);
				}
			}
		}

	}
	else if (currentStage->stageID == STAGE_3)
	{
		if (isPaused)
		{
			SelectObject(hdc_loadBmp, bmp_pause_bg);
			TransparentBlt(
				hdc_memBuffer,
				PAUSE_BG_START_X, PAUSE_BG_START_Y,    // 背景框在界面上的起始位置
				PAUSE_BG_WIDTH, PAUSE_BG_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				PAUSE_BG_WIDTH, PAUSE_BG_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);
		}
		else
		{

		SelectObject(hdc_loadBmp, bmp_battlemap);
		TransparentBlt(
			hdc_memBuffer,
			BATTLEMAP_START_X, BATTLEMAP_START_Y,    // 背景框在界面上的起始位置
			BATTLEMAP_WIDTH, BATTLEMAP_HEIGHT,      // 背景框宽高
			hdc_loadBmp,
			0, 0,                                   // 背景框在 BMP 图上的起始位置
			BATTLEMAP_WIDTH, BATTLEMAP_HEIGHT,      // BMP 图中背景框的宽高
			RGB(255, 255, 255)                      // 背景透明色
		);
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

		SelectObject(hdc_loadBmp, bmp_dialog);
		TransparentBlt(
			hdc_memBuffer,
			0, WINDOW_HEIGHT - DIALOG_SIZE_Y - 38, WINDOW_WIDTH - 16, DIALOG_SIZE_Y,					// 界面上绘制位置
			hdc_loadBmp,
			0, 0, DIALOG_BITMAP_SIZE_X, DIALOG_BITMAP_SIZE_Y,	// 位图上绘制位置
			RGB(255, 255, 255)
		);

		DrawBattleDialog(hdc_memBuffer);

		if (isBackpackOpen)
		{
			//绘制背包背景
			SelectObject(hdc_loadBmp, bmp_backpack);
			TransparentBlt(
				hdc_memBuffer,
				BACKPACK_START_X, BACKPACK_START_Y,    // 背景框在界面上的起始位置
				BACKPACK_WIDTH, BACKPACK_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				BACKPACK_WIDTH, BACKPACK_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);
			SelectObject(hdc_loadBmp, bmp_delete_box);
			TransparentBlt(
				hdc_memBuffer,
				BTN_DELETE_BOX_START_X, BTN_DELETE_BOX_START_Y,    // 背景框在界面上的起始位置
				BTN_DELETE_BOX_WIDTH, BTN_DELETE_BOX_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				BTN_DELETE_BOX_WIDTH, BTN_DELETE_BOX_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);
			for (const auto& item : globalBackpack) {
				if (item.inInventory) {
					// 选择对应的位图
					HBITMAP itemBitmap = GetItemBitmap(item.itemID); // 自定义函数返回对应的位图
					SelectObject(hdc_loadBmp, itemBitmap);

					// 绘制物品
					TransparentBlt(
						hdc_memBuffer,
						BACKPACK_START_X + item.inventoryX * CELL_WIDTH,
						BACKPACK_START_Y + item.inventoryY * CELL_HEIGHT,
						item.width * CELL_WIDTH,
						item.height * CELL_HEIGHT,
						hdc_loadBmp,
						0, 0,
						item.width * CELL_WIDTH,
						item.height * CELL_HEIGHT,
						RGB(255, 255, 255) // 背景透明色
					);
				}
			}
		}

		if (!isBackpackOpen)
		{
			//绘制背包按钮
			SelectObject(hdc_loadBmp, bmp_btn_backpack);
			TransparentBlt(
				hdc_memBuffer,
				BTN_BACKPACK_START_X, BTN_BACKPACK_START_Y,    // 背景框在界面上的起始位置
				BTN_BACKPACK_WIDTH, BTN_BACKPACK_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				BTN_BACKPACK_WIDTH, BTN_BACKPACK_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);
		}
		const wchar_t* enemy_pokemon_name = L"喷火龙";
		HFONT hFont = CreateFontW(
			40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
			L"Microsoft YaHei"); // 使用支持中文的字体
		SelectObject(hdc_memBuffer, hFont);
		SetTextColor(hdc_memBuffer, RGB(0, 0, 0)); // 设置颜色:黑色字体
		SetBkMode(hdc_memBuffer, OPAQUE); // 设置背景模式为不透明
		// 定义绘制区域
		RECT rect;
		rect.left = 670;
		rect.top = WINDOW_HEIGHT - 630;
		rect.right = WINDOW_WIDTH - 110;
		rect.bottom = WINDOW_HEIGHT - 18;
		// 绘制文本
		DrawTextW(hdc_memBuffer, enemy_pokemon_name, -1, &rect, DT_WORDBREAK);

		if (isPokemonAcceptance == false)
		{
			//绘制背包背景
			SelectObject(hdc_loadBmp, bmp_pokemon1);
			TransparentBlt(
				hdc_memBuffer,
				BATTLEPOKEMON_START_X, BATTLEPOKEMON_START_Y,    // 背景框在界面上的起始位置
				BATTLEPOKEMON_WIDTH, BATTLEPOKEMON_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				BATTLEPOKEMON_WIDTH, BATTLEPOKEMON_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);
		}
		else if (isPokemonAcceptance == true)
		{
			SelectObject(hdc_loadBmp, bmp_pokemon_ball);
			TransparentBlt(
				hdc_memBuffer,
				POKEMON_BALLACC_START_X, POKEMON_BALLACC_START_Y,    // 背景框在界面上的起始位置
				POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				POKEMON_BALL_WIDTH, POKEMON_BALL_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);
			MessageBox(hWnd, L"收服成功!", L"收服状态", MB_OK | MB_ICONINFORMATION);
		}
		//绘制蓝条
		for (int i = 0; i < currentpokemonmpBlocks; ++i) {
			SelectObject(hdc_loadBmp, bmp_mpbar);
			TransparentBlt(
				hdc_memBuffer,
				POKEMON_MP_START_X + i * POKEMON_MP_BLOCK_WIDTH, // 每块横向排列
				POKEMON_MP_START_Y,                           // 固定纵坐标
				POKEMON_MP_BLOCK_WIDTH, POKEMON_MP_BLOCK_HEIGHT, // 每块血条的宽高
				hdc_loadBmp,
				0, 0,
				POKEMON_MP_BLOCK_WIDTH, POKEMON_MP_BLOCK_HEIGHT,
				RGB(255, 255, 255) // 背景透明色
			);
		}

		//绘制血条
		for (int i = 0; i < currentpokemonBloodBlocks; ++i) {
			SelectObject(hdc_loadBmp, bmp_bloodbar);
			TransparentBlt(
				hdc_memBuffer,
				POKE_BLOOD_START_X + i * POKE_BLOOD_BLOCK_WIDTH, // 每块横向排列
				POKE_BLOOD_START_Y,                           // 固定纵坐标
				POKE_BLOOD_BLOCK_WIDTH, POKE_BLOOD_BLOCK_HEIGHT, // 每块血条的宽高
				hdc_loadBmp,
				0, 0,
				POKE_BLOOD_BLOCK_WIDTH, POKE_BLOOD_BLOCK_HEIGHT,
				RGB(255, 255, 255) // 背景透明色
			);
		}

		SelectObject(hdc_loadBmp, bmp_backpack);
		TransparentBlt(
			hdc_memBuffer,
			BATTLE_BACKPACK_START_X, BATTLE_BACKPACK_START_Y,    // 背景框在界面上的起始位置
			BACKPACK_WIDTH, BACKPACK_HEIGHT,      // 背景框宽高
			hdc_loadBmp,
			0, 0,                                   // 背景框在 BMP 图上的起始位置
			BACKPACK_WIDTH, BACKPACK_HEIGHT,      // BMP 图中背景框的宽高
			RGB(255, 255, 255)                      // 背景透明色
		);
		SelectObject(hdc_loadBmp, bmp_sword);
		TransparentBlt(
			hdc_memBuffer,
			735, 150,    // 背景框在界面上的起始位置
			SWORD_WIDTH, SWORD_HEIGHT,      // 背景框宽高
			hdc_loadBmp,
			0, 0,                                   // 背景框在 BMP 图上的起始位置
			SWORD_WIDTH, SWORD_HEIGHT,      // BMP 图中背景框的宽高
			RGB(255, 255, 255)                      // 背景透明色
		);
		if (isInbattle && isLose)
		{
			SelectObject(hdc_loadBmp, bmp_lose);
			TransparentBlt(
				hdc_memBuffer,
				LOSE_START_X, LOSE_START_Y,    // 背景框在界面上的起始位置
				LOSE_WIDTH, LOSE_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				LOSE_WIDTH, LOSE_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);

		}
		else if (isInbattle && isVictory)
		{
			SelectObject(hdc_loadBmp, bmp_victory);
			TransparentBlt(
				hdc_memBuffer,
				VICTORY_START_X, VICTORY_START_Y,    // 背景框在界面上的起始位置
				VICTORY_WIDTH, VICTORY_HEIGHT,      // 背景框宽高
				hdc_loadBmp,
				0, 0,                                   // 背景框在 BMP 图上的起始位置
				VICTORY_WIDTH, VICTORY_HEIGHT,      // BMP 图中背景框的宽高
				RGB(255, 255, 255)                      // 背景透明色
			);
		}
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