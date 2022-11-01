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
	//���\�[�X
	int blockGH;
	int objBlockGH;
	int mObjBlockGH;

	//�v���C���[���
	Player player;
	int movX = 0, movY = 0;	// �ړ���̍��W
	int fallX = 0, fallY = 0;
	int playerX;	//�`��p
	int playerY;

	//�u���b�N���
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
	const int objNum = 10;
	int moveObjX[10];
	int moveObjY[10];
	int objPosX[10];	//�`��p
	int objPosY[10];

	//�X�e�[�W�̉�]�������
	const int moveFrame = 32;
	int count;
	int mapRote;
	float roll;
	float Roll_d;
	int state;
	int muki;

	//�L�[���͏��
	char keys;
	char oldkeys;

	//�֐�
	bool MapChipCollision(int left, int top, int right, int bottom);
};

