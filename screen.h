#pragma once
#include "DxLib.h"
#include "enum.h"

// ウィンドウ横幅
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅
const int WIN_HEIGHT = 720;

//アニメーション用
struct AniGHTimer {
	const int GHNum;
	const int GHTime;
	int GHTimer;
};


class Screen
{
public:
	Screen();
	~Screen();

	void Update();

	void Draw(int scene,int stage, int hp, bool tutorial);

private:
	//UI情報
	//背景情報
	int backX;
	int backY;
	//ゲーム背景
	int stageGH1;
	int stageGH2;
	//チュートリアル背景
	int tutorialGH[2];
	int stageEndGH1;
	int stageEndGH2;

	//HP
	int hpGHX;
	int hpGHY;
	AniGHTimer hpEffect{ 8, 40, 0 };
	int hpGH[8];

	void HpUI(int hp);
};

