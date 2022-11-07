#pragma once
#include "DxLib.h"
#include "enum.h"
#include "screen.h"

class Player
{
public:
	Player();

	~Player();

	void Update();

	void Draw(int posX, int posY, float roll);

	//基本情報
	int x;
	int y;
	const int  sizeX = 64;
	const int  sizeY = 64;
	int hp;

private:
	//リソース
	int playerGH;

	
};