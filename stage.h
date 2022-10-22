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

	void Update(char* keys, char* oldkeys);

	void Draw();

private:
	//リソース
	int blockGH;
	int objBlockGH;

	//プレイヤー情報
	Player player;
	int movx = 0, movy = 0;	// 移動先の座標
	int playerX;	//描画用
	int playerY;

	//ブロック情報
	const int blockSize = 64;
	const int mapX1 = 9;
	const int mapY1 = 9;
	int map[9][9] = {
		{1,1,1,1,1,1,1,1,1,},
		{1,0,0,0,0,0,0,0,1,},
		{1,0,0,0,0,0,0,0,1,},
		{1,0,0,0,0,0,0,0,1,},
		{1,0,0,0,0,0,0,0,1,},
		{1,0,0,0,0,0,0,0,1,},
		{1,0,0,0,0,0,0,0,1,},
		{1,0,3,0,0,0,0,0,1,},
		{1,1,1,1,1,1,1,1,1,},
	};

	//ステージの回転処理情報
	const int moveFrame = 32;
	int count;
	int mapRote;
	float roll;
	float Roll_d;
	int state;
	int muki;

	//キー入力情報
	char keys;
	char oldkeys;

	//関数
	bool MapChipCollision(int left, int top, int right, int bottom);
};

