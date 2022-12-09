#pragma once
#include "DxLib.h"
#include "enum.h"

// �E�B���h�E����
const int WIN_WIDTH = 1280;

// �E�B���h�E�c��
const int WIN_HEIGHT = 720;

//�A�j���[�V�����p
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
	//UI���
	//�w�i���
	int backX;
	int backY;
	//�Q�[���w�i
	int stageGH1;
	int stageGH2;
	//�`���[�g���A���w�i
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

