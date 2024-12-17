#pragma once

#pragma region ͷ�ļ�����

#include <windows.h>
#include <algorithm>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
#include "resource.h"
#pragma comment(lib, "Msimg32.lib")			//ͼ����ĺ����ӿڣ����磺͸��ɫ��λͼ�Ļ���TransparentBlt����
using namespace std;
#pragma endregion

#pragma region �궨��

#define WINDOW_TITLEBARHEIGHT	32			//�������߶�
#define WINDOW_WIDTH			896+16		//��Ϸ���ڿ��
#define WINDOW_HEIGHT			640+32		//��Ϸ���ڸ߶�

#define STAGE_STARTMENU			0		//��ʼ�����ID
#define STAGE_1					1		//��һ����Ϸ������ID
#define STAGE_2					2
#define STAGE_3					3
#define STAGE_4					4

//�ߴ�
#define BG_BITMAP_WIDTH			960		//����ͼƬ��λͼ���
#define BG_BITMAP_HEIGHT		640		//����ͼƬ��λͼ�߶�
#define BG_COLUMNS				28		//��������
#define BG_ROWS					20		//��������
#define HUMAN_SIZE_X			38		//��������Ļ�ϵĿ��
#define HUMAN_SIZE_Y			52		
#define HUMAN_BITMAP_SIZE_X		19		//������λͼ�ϵĿ��
#define HUMAN_BITMAP_SIZE_Y		26		
#define BLOCK_SIZE_X			32		//������������Ļ�ϵĿ��
#define BLOCK_SIZE_Y			32		//������������Ļ�ϵĸ߶�
#define CELL_SIZE				56.25
#define BLOCK_BITMAP_SIZE_X		16		//��ͼ����λͼ�ϵĿ��
#define BLOCK_BITMAP_SIZE_Y		16
#define DIALOG_SIZE_X			896		//�Ի��򱳾�
#define DIALOG_SIZE_Y			120
#define DIALOG_BITMAP_SIZE_X	250
#define DIALOG_BITMAP_SIZE_Y	44
#define MOSTER_SIZE_X			28		//����
#define MOSTER_SIZE_Y			30
#define MOSTER_BITMAP_SIZE_X	14
#define MOSTER_BITMAP_SIZE_Y	15
#define POKEMON_SIZE_X			64		//������
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
#define CELL_WIDTH				56.25
#define CELL_HEIGHT				56.25
#define POKEMON_BALL_WIDTH		56.25
#define POKEMON_BALL_HEIGHT		56.25
#define POKEMON_BALL_START_X	500
#define POKEMON_BALL_START_Y	200
#define BATTLEMAP_WIDTH			960
#define BATTLEMAP_HEIGHT		640
#define BATTLEMAP_START_X		0
#define BATTLEMAP_START_Y		0
#define BATTLEPOKEMON_WIDTH		64
#define BATTLEPOKEMON_HEIGHT	64
#define BATTLEPOKEMON_START_X	600
#define BATTLEPOKEMON_START_Y	330
#define POKEMON_MP_BLOCK_COUNT	2
#define POKEMON_MP_BLOCK_WIDTH	32
#define POKEMON_MP_BLOCK_HEIGHT	32
#define POKEMON_MP_START_X		770
#define POKEMON_MP_START_Y		100
#define POKE_BLOOD_BLOCK_COUNT	5
#define POKE_BLOOD_BLOCK_WIDTH	32
#define POKE_BLOOD_BLOCK_HEIGHT	32
#define POKE_BLOOD_START_X		600
#define POKE_BLOOD_START_Y		100
#define BATTLE_BACKPACK_START_X	680
#define BATTLE_BACKPACK_START_Y	150
#define VICTORY_WIDTH			160
#define VICTORY_HEIGHT			66
#define VICTORY_START_X			400
#define VICTORY_START_Y			250
#define LOSE_WIDTH				160
#define LOSE_HEIGHT				66
#define LOSE_START_X			400
#define LOSE_START_Y			250
#define POKEMON_BALLACC_START_X	600
#define POKEMON_BALLACC_START_Y	330
#define ENEMY_POKEMON_MP_BLOCK_COUNT	3
#define ENEMY_POKE_BLOOD_BLOCK_COUNT	7



//��λ״̬����
#define UNIT_STATE_HOLD			0		//��ֹ
#define UNIT_STATE_WALK			1		//����
#define UNIT_STATE_ATTACK		2		//����

//��λ������
#define UNIT_DIRECT_RIGHT		1		//����
#define UNIT_DIRECT_LEFT		3		//����
#define UNIT_DIRECT_UP			2		//����
#define UNIT_DIRECT_DOWN		0		//����

//��������
#define BUTTON_STARTGAME			1001	//��ʼ��Ϸ��ťID
#define BUTTON_STARTGAME_WIDTH		212		//��ʼ��Ϸ��ť���
#define BUTTON_STARTGAME_HEIGHT		76		//��ʼ��Ϸ��ť�߶�

#define TIMER_GAMETIMER				1		//��Ϸ��Ĭ�ϼ�ʱ��ID
#define TIMER_GAMETIMER_ELAPSE		30		//Ĭ�ϼ�ʱ��ˢ�¼���ĺ�����
#define UNIT_SPEED					3.0		//��λ�����ٶ�

#define NPC_MAN1_ID				2001		//ÿ��NPC��ID��
#define NPC_WOMAN1_ID			2002

#define MONSTER_CAT_ID			3001

#define POKEMON_FIRE_DRAGON_ID	4001
///

#pragma endregion


#pragma region �ṹ������

// �����ṹ��
struct Stage
{
	int stageID;		//�������
	HBITMAP bg;			//����ͼƬ
	int timeCountDown;	//��Ϸʱ�䵹��ʱ
	bool timerOn;		//��ʱ���Ƿ����У���Ϸ�Ƿ���ͣ��

};


// ��ť�ṹ��
struct Button
{
	int buttonID;	//��ť���
	bool visible;	//��ť�Ƿ�ɼ�
	HBITMAP img;	//ͼƬ
	int x;			//����x
	int y;			//����y
	int width;		//���
	int height;		//�߶�
};

// NPC�ṹ��
struct NPC
{
	int npcID;				//NPC���
	HBITMAP img;			//ͼƬ
	bool visible;			//��NPC�Ƿ�ɼ�
	bool task1_start = true;
	bool task1_complete = false;	//��npc�������Ƿ���ɡ���������npc�Ի�ʱ����˵ʲô���Լ�������Ϊ
	bool task2_start = false;
	bool task2_complete = false;

	int frame_row;			//��ǰ��ʾ����ͼ��ĵڼ���
	int frame_column;		//��ǰ��ʾ����ͼ��ĵڼ���

	int* frame_sequence;	//��ǰ��֡����
	int frame_count;		//֡���еĳ���
	int frame_id;			//��ǰ��ʾ����֡���еĵڼ�֡

	int state;		//��λ״̬
	int direction;	//��λ����

	int x;			//����x
	int y;			//����y
	double vx;		//�ٶ�x
	double vy;		//�ٶ�y
	int health;		//����ֵ

	vector<const wchar_t*> conversations1_before;	//�������ǰNPC��̨��
	vector<const wchar_t*> conversations1_after;	//������ɺ�NPC��̨��
	vector<const wchar_t*> conversations2_before;	//�������ǰNPC��̨��
	vector<const wchar_t*> conversations2_after;	//������ɺ�NPC��̨��
	int next_conversation_id_before1 = 0;				//NPC��һ��Ҫ˵�ڼ���̨��
	int next_conversation_id_after1 = 0;				//NPC��һ��Ҫ˵�ڼ���̨��
	int next_conversation_id_before2 = 0;				//NPC��һ��Ҫ˵�ڼ���̨��
	int next_conversation_id_after2 = 0;				//NPC��һ��Ҫ˵�ڼ���̨��
};

// ��ҽṹ��
struct Player
{
	HBITMAP img;	//ͼƬ

	int frame_row;			//��ǰ��ʾ����ͼ��ĵڼ���
	int frame_column;		//��ǰ��ʾ����ͼ��ĵڼ���

	int* frame_sequence;	//��ǰ��֡����
	int frame_count;		//֡���еĳ���
	int frame_id;			//��ǰ��ʾ����֡���еĵڼ�֡

	int state;		//��λ״̬
	int direction;	//��λ����

	int x;			//����x
	int y;			//����y
	double vx;		//�ٶ�x
	double vy;		//�ٶ�y
	int health;		//����ֵ
};

// �����νṹ��
struct Pokemon
{
	HBITMAP img;	//ͼƬ

	int PokemonID;			//������ID
	int frame_row;			//��ǰ��ʾ����ͼ��ĵڼ���
	int frame_column;		//��ǰ��ʾ����ͼ��ĵڼ���
	bool visible;			//�Ƿ�ɼ�

	int waitTime; // ͣ��ʱ��
	bool moving; // �Ƿ������ƶ�

	int* frame_sequence;	//��ǰ��֡����
	int frame_count;		//֡���еĳ���
	int frame_id;			//��ǰ��ʾ����֡���еĵڼ�֡

	int state;		//��λ״̬
	int direction;	//��λ����

	int x;			//����x
	int y;			//����y
	double vx;		//�ٶ�x
	double vy;		//�ٶ�y
	int health;		//����ֵ
};

// ����ṹ��
struct Monster
{
	int monsterID;			//������
	HBITMAP img;			//ͼƬ
	bool visible;			//�Ƿ�ɼ�
	bool task_complete;		//�����Ƿ����

	int frame_row;			//��ǰ��ʾ����ͼ��ĵڼ���
	int frame_column;		//��ǰ��ʾ����ͼ��ĵڼ���

	int* frame_sequence;	//��ǰ��֡����
	int frame_count;		//֡���еĳ���
	int frame_id;			//��ǰ��ʾ����֡���еĵڼ�֡

	int state;		//��λ״̬
	int direction;	//��λ����

	int x;			//����x
	int y;			//����y
	double vx;		//�ٶ�x
	double vy;		//�ٶ�y
	int health;		//����ֵ

	vector<const wchar_t*> conversations_before;	//�������ǰ��̨��
	vector<const wchar_t*> conversations_after;		//������ɺ��̨��
	int next_conversation_id;						//��һ��Ҫ˵�ڼ���̨��
};

struct Item {
	bool inInventory;    // �Ƿ��ڱ�����
	int inventoryX;      // �����е���ʼ���� X
	int inventoryY;      // �����е���ʼ���� Y
	int width;           // ���߿��
	int height;          // ���߸߶�
	bool isDragging;     // �Ƿ�������ק
	int offsetX, offsetY; // �������ڵ������Ͻǵ�ƫ��
	int screenX, screenY; // ���ߵ�ǰ��Ļ����
	int startX, startY;  // ��קǰ����Ļ���꣨����ӣ�
	HBITMAP img;         // ����ͼ��
};



//TODO: �����Ϸ��Ҫ�ĸ��������ݣ����ש��ȣ�


#pragma endregion


#pragma region �¼�����������


// ��ʼ����Ϸ���庯��
void InitGame(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ���̰����¼�������
void KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����ɿ��¼�������
void KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ����ƶ��¼�������
void MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �����������¼�������
void LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

// �������ɿ��¼�������
void LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);

// ��ʱ���¼�������
void TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);

//��ײ��
void CheckCollision();

//��ײ��2
bool CheckCollision1(int x, int y, int direction, int map[20][28], int stageid);
#pragma endregion

//����������˶�����
void RandomizePokemonMovement(Pokemon* pokemon);

//����ս��
void GetInBattleMap(HWND hWnd);

HBITMAP GetItemBitmap(int itemID);

void PokemonAttack(HWND hWnd);

void ResetGame(HWND hWnd);

void DrawBattleDialog(HDC hdc);

void notwhite(HWND hWnd, int swordCount, int yanlongCount);

void chushihua(HWND hWnd);

void messageloop(HWND hWnd);

#pragma region ������Ϸ״̬����������

// ��Ӱ�ť����
Button* CreateButton(int buttonID, HBITMAP img, int width, int height, int x, int y);

// ��ӵ�λ����
Player* CreatePlayer(int x, int y);
NPC* CreateNPC(int x, int y, int npc_id);
Monster* CreateMonster(int x, int y, int monster_id);
Pokemon* CreatePokemon(int x, int y, int pokemon_id);


// ��ʼ����������
void InitStage(HWND hWnd, int stageID);


//��Ϊ�ͽ���������update***�ڶ�ʱ���¼����Թ̶�Ƶ�ʵ��ã�handle***�ڷ�������/����¼�ʱ����
void UpdatePlayer(HWND hWnd);
void UpdateNPCs(HWND hWnd);
void UpdateMonsters(HWND hWnd);
void UpdateMaps(HWND hWnd);
void UpdatePokemons(HWND hWnd);

void HandleConversationEvents(HWND hWnd);

//TODO: �����Ϸ��Ҫ�ĸ��ຯ��


#pragma endregion 


#pragma region ��ͼ��������

// ��ͼ����
void Paint(HWND hWnd);

#pragma endregion

