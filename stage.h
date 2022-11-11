#pragma once
#include "DxLib.h"
#include "enum.h"
#include "screen.h"
#include "player.h"

class Stage
{
public:
	Stage();

	~Stage();

	void Reset(int stage);

	void Update(int& scene, char* keys, char* oldkeys);

	void Draw();

	//ゲッター
	int HpGet() { return hp; }
	int TimerGet() { return timer; }

private:
	//リソース
	int blockGH;
	int objBlockGH;
	int mObjBlockGH;
	int goalGH;
	int damageGH;

	//プレイヤー情報
	Player player;
	int movX = 0, movY = 0;	// 移動先の座標
	int fallX = 0, fallY = 0;
	//int playerX;	//描画用
	//int playerY;
	int hp = 3;

	//ブロック情報
	const int blockSize = 64;
	const int mapX1 = 9;
	const int mapY1 = 9;
	int map[9][9] = {
		{0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,},
		{0,0,0,0,0,0,0,0,0,},
	};

	//ゴール
	int goalX;
	int goalY;

	//動くオブジェクト
	const int objNum = 10;
	int moveObjX[10];
	int moveObjY[10];
	int moveObjCount;	//マップ上の動くオブジェクトの数を数える

	//ダメージオブジェクト
	int damageObjX[10];
	int damageObjY[10];
	int damageObjCount;	//マップ上のダメージオブジェクトの数を数える

	//ステージの回転処理情報
	const int moveFrame = 32;
	int count;
	int mapRote;
	float roll;
	float Roll_d;
	int state;
	int muki;

	//キー入力情報
	/*char keys;
	char oldkeys;*/
	//ステージ情報保存
	int stage_;
	//ゲームスタートかリセット処理か判断
	bool farstGame;
	//タイマー
	const int time = 30 * 60;
	int timer;

	//関数
	//ダメージ処理
	void Damage();

	//落下処理
	void Fall();

	//ゴールに触れたらクリア
	bool Clear();

	//回転描画
	void RollDraw(int x, int y, int GH);

	//ステージ配置
	//ステージ1
	int map1[9][9] = {
		{2,2,2,2,2,2,2,2,2,},
		{2,0,0,0,3,0,0,0,2,},
		{2,0,0,0,0,0,0,0,2,},
		{2,0,0,0,0,0,0,0,2,},
		{2,0,0,0,0,0,0,3,2,},
		{2,0,0,0,0,0,0,0,2,},
		{2,0,0,1,0,0,0,0,2,},
		{2,0,3,0,0,4,0,0,2,},
		{2,2,2,2,2,2,2,2,2,},

	};
	int map2[9][9] = {
		{2,2,2,2,2,2,2,2,2,},
		{2,0,0,4,1,4,0,0,2,},
		{2,0,0,3,0,3,0,0,2,},
		{2,0,0,0,0,0,0,0,2,},
		{2,0,0,0,0,0,0,0,2,},
		{2,0,0,0,0,0,0,0,2,},
		{2,5,0,0,0,0,0,5,2,},
		{2,5,0,0,4,0,0,5,2,},
		{2,2,2,2,2,2,2,2,2,},
	};
	int map3[9][9] = {
		{2,2,2,2,2,2,2,2,2,},
		{2,3,0,0,0,0,0,0,2,},
		{2,0,0,0,4,0,0,0,2,},
		{2,0,0,0,3,0,0,3,2,},
		{2,0,0,0,1,0,3,0,2,},
		{2,0,0,0,0,0,0,0,2,},
		{2,0,0,0,0,0,0,0,2,},
		{2,0,4,0,0,0,0,3,2,},
		{2,2,2,2,2,2,2,2,2,},
	};
};