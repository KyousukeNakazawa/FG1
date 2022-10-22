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

	//�v���C���[���
	Player player;
	int movx = 0, movy = 0;	// �ړ���̍��W
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

