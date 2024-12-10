#pragma once

#pragma region 头文件引用

#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <vector>
#include <math.h>
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")			//图象处理的函数接口，例如：透明色的位图的绘制TransparentBlt函数
using namespace std;
#pragma endregion

#pragma region 宏定义

#define WINDOW_TITLEBARHEIGHT	32			//标题栏高度
#define WINDOW_WIDTH			896+16		//游戏窗口宽度
#define WINDOW_HEIGHT			640+32		//游戏窗口高度

#define STAGE_STARTMENU			0		//开始画面的ID
#define STAGE_1					1		//第一个游戏场景的ID
#define STAGE_2					2

//尺寸
#define BG_BITMAP_WIDTH			960		//背景图片的位图宽度
#define BG_BITMAP_HEIGHT		640		//背景图片的位图高度
#define BG_COLUMNS				28		//背景列数
#define BG_ROWS					20		//背景行数
#define HUMAN_SIZE_X			38		//人物在屏幕上的宽度
#define HUMAN_SIZE_Y			52		
#define HUMAN_BITMAP_SIZE_X		19		//人物在位图上的宽度
#define HUMAN_BITMAP_SIZE_Y		26		
#define BLOCK_SIZE_X			32		//背景单格在屏幕上的宽度
#define BLOCK_SIZE_Y			32		//背景单格在屏幕上的高度
#define BLOCK_BITMAP_SIZE_X		16		//地图块在位图上的宽度
#define BLOCK_BITMAP_SIZE_Y		16
#define DIALOG_SIZE_X			896		//对话框背景
#define DIALOG_SIZE_Y			120
#define DIALOG_BITMAP_SIZE_X	250
#define DIALOG_BITMAP_SIZE_Y	44
#define MOSTER_SIZE_X			28		//怪物
#define MOSTER_SIZE_Y			30
#define MOSTER_BITMAP_SIZE_X	14
#define MOSTER_BITMAP_SIZE_Y	15
#define POKEMON_SIZE_X			64		//宝可梦
#define POKEMON_SIZE_Y			64
#define POKEMON_BITMAP_SIZE_X	64
#define POKEMON_BITMAP_SIZE_Y	64
#define BLOOD_BLOCK_COUNT		3
#define BLOOD_BLOCK_WIDTH		32
#define BLOOD_BLOCK_HEIGHT		32
#define BLOOD_START_X			100
#define BLOOD_START_Y			20
#define MP_BLOCK_COUNT			3
#define MP_BLOCK_WIDTH			32
#define MP_BLOCK_HEIGHT			32
#define MP_START_X				100
#define MP_START_Y				100
#define HP_MP_BOX_WIDTH			225
#define HP_MP_BOX_HEIGHT		150
#define HP_MP_BOX_START_X		0
#define HP_MP_BOX_START_Y		0
#define BACKPACK_WIDTH			225
#define BACKPACK_HEIGHT			225
#define BACKPACK_START_X		0
#define BACKPACK_START_Y		410
#define BACKPACK_ROWS			4
#define BACKPACK_COLS			4
#define BTN_BACKPACK_WIDTH		56
#define BTN_BACKPACK_HEIGHT		58
#define BTN_BACKPACK_START_X	50
#define BTN_BACKPACK_START_Y	550
#define BTN_DELETE_BOX_WIDTH	25
#define BTN_DELETE_BOX_HEIGHT	25
#define BTN_DELETE_BOX_START_X	225
#define BTN_DELETE_BOX_START_Y	410
#define PAUSE_BG_WIDTH			480
#define PAUSE_BG_HEIGHT			320
#define PAUSE_BG_START_X		240
#define PAUSE_BG_START_Y		160
#define SWORD_WIDTH				56.25
#define SWORD_HEIGHT			168.75
#define SWORD_START_X			200
#define SWORD_START_Y			200
#define SHIELD_WIDTH			112.5
#define SHIELD_HEIGHT			112.5
#define SHIELD_START_X			400
#define SHIELD_START_Y			200


//单位状态定义
#define UNIT_STATE_HOLD			0		//静止
#define UNIT_STATE_WALK			1		//行走
#define UNIT_STATE_ATTACK		2		//攻击

//单位方向定义
#define UNIT_DIRECT_RIGHT		1		//向右
#define UNIT_DIRECT_LEFT		3		//向左
#define UNIT_DIRECT_UP			2		//向上
#define UNIT_DIRECT_DOWN		0		//向下

//其它定义
#define BUTTON_STARTGAME			1001	//开始游戏按钮ID
#define BUTTON_STARTGAME_WIDTH		212		//开始游戏按钮宽度
#define BUTTON_STARTGAME_HEIGHT		76		//开始游戏按钮高度

#define TIMER_GAMETIMER				1		//游戏的默认计时器ID
#define TIMER_GAMETIMER_ELAPSE		30		//默认计时器刷新间隔的毫秒数
#define UNIT_SPEED					3.0		//单位行走速度

#define NPC_MAN1_ID				2001		//每个NPC的ID号
#define NPC_WOMAN1_ID			2002

#define MONSTER_CAT_ID			3001

#define POKEMON_FIRE_DRAGON_ID	4001
///

#pragma endregion


#pragma region 结构体声明

// 场景结构体
struct Stage
{
	int stageID;		//场景编号
	HBITMAP bg;			//背景图片
	int timeCountDown;	//游戏时间倒计时
	bool timerOn;		//计时器是否运行（游戏是否被暂停）

};


// 按钮结构体
struct Button
{
	int buttonID;	//按钮编号
	bool visible;	//按钮是否可见
	HBITMAP img;	//图片
	int x;			//坐标x
	int y;			//坐标y
	int width;		//宽度
	int height;		//高度
};

// NPC结构体
struct NPC
{
	int npcID;				//NPC编号
	HBITMAP img;			//图片
	bool visible;			//该NPC是否可见
	bool task_complete;		//该npc的任务是否完成。决定了与npc对话时他会说什么，以及其它行为

	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列

	int* frame_sequence;	//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几帧

	int state;		//单位状态
	int direction;	//单位方向

	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	int health;		//生命值

	vector<const wchar_t*> conversations_before;	//任务完成前NPC的台词
	vector<const wchar_t*> conversations_after;		//任务完成后NPC的台词
	int next_conversation_id;				//NPC下一次要说第几句台词
};

// 玩家结构体
struct Player
{
	HBITMAP img;	//图片

	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列

	int* frame_sequence;	//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几帧

	int state;		//单位状态
	int direction;	//单位方向

	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	int health;		//生命值
};

// 宝可梦结构体
struct Pokemon
{
	HBITMAP img;	//图片

	int PokemonID;			//宝可梦ID
	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列
	bool visible;			//是否可见

	int waitTime; // 停顿时间
	bool moving; // 是否正在移动

	int* frame_sequence;	//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几帧

	int state;		//单位状态
	int direction;	//单位方向

	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	int health;		//生命值
};

// 怪物结构体
struct Monster
{
	int monsterID;			//怪物编号
	HBITMAP img;			//图片
	bool visible;			//是否可见
	bool task_complete;		//任务是否完成

	int frame_row;			//当前显示的是图像的第几行
	int frame_column;		//当前显示的是图像的第几列

	int* frame_sequence;	//当前的帧序列
	int frame_count;		//帧序列的长度
	int frame_id;			//当前显示的是帧序列的第几帧

	int state;		//单位状态
	int direction;	//单位方向

	int x;			//坐标x
	int y;			//坐标y
	double vx;		//速度x
	double vy;		//速度y
	int health;		//生命值

	vector<const wchar_t*> conversations_before;	//任务完成前的台词
	vector<const wchar_t*> conversations_after;		//任务完成后的台词
	int next_conversation_id;						//下一次要说第几句台词
};

//道具结构体
struct Item {
	bool inInventory; // 剑是否已经在背包内
	int inventoryX;   // 剑在背包中的起始格子坐标X（列）
	int inventoryY;   // 剑在背包中的起始格子坐标Y（行）
	int width;        // =3
	int height;       // =1 (此例中为一行三列)
	bool isDragging;  // 当前是否正在被拖拽
	int offsetX, offsetY; // 鼠标点击时与剑左上角的偏移，用于平滑拖动
	int screenX, screenY; // 剑在屏幕上的坐标（当未放入背包时）
	HBITMAP img;
};



//TODO: 添加游戏需要的更多种数据（地物、砖块等）


#pragma endregion


#pragma region 事件处理函数声明


// 初始化游戏窗体函数
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘按下事件处理函数
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 键盘松开事件处理函数
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标移动事件处理函数
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键按下事件处理函数
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 鼠标左键松开事件处理函数
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// 定时器事件处理函数
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

//碰撞箱
void CheckCollision();

//碰撞箱2
bool CheckCollision1(int x, int y, int direction, int map[20][28]);
#pragma endregion

//宝可梦随机运动函数
void RandomizePokemonMovement(Pokemon* pokemon);

#pragma region 其它游戏状态处理函数声明

// 添加按钮函数
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

// 添加单位函数
Player* CreatePlayer(int x, int y);
NPC* CreateNPC(int x, int y, int npc_id);
Monster* CreateMonster(int x, int y, int monster_id);
Pokemon* CreatePokemon(int x, int y, int pokemon_id);


// 初始化场景函数
void InitStage(HWND hWnd, int stageID);


//行为和交互函数：update***在定时器事件中以固定频率调用；handle***在发生键盘/鼠标事件时触发
void UpdatePlayer(HWND hWnd);
void UpdateNPCs(HWND hWnd);
void UpdateMonsters(HWND hWnd);
void UpdateMaps(HWND hWnd);
void UpdatePokemons(HWND hWnd);

void HandleConversationEvents(HWND hWnd);

//TODO: 添加游戏需要的更多函数


#pragma endregion 


#pragma region 绘图函数声明

// 绘图函数
void Paint(HWND hWnd);

#pragma endregion

