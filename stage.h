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

	//�Q�b�^�[
	int HpGet() { return hp; }
	int TimerGet() { return timer; }

private:
	//���\�[�X
	int blockGH;
	int objBlockGH;
	int mObjBlockGH;
	int goalGH;
	int damageGH;

	//�v���C���[���
	Player player;
	int movX = 0, movY = 0;	// �ړ���̍��W
	int fallX = 0, fallY = 0;
	//int playerX;	//�`��p
	//int playerY;
	int hp = 3;

	//�u���b�N���
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

	//�S�[��
	int goalX;
	int goalY;

	//�����I�u�W�F�N�g
	const int objNum = 10;
	int moveObjX[10];
	int moveObjY[10];
	int moveObjCount;	//�}�b�v��̓����I�u�W�F�N�g�̐��𐔂���

	//�_���[�W�I�u�W�F�N�g
	int damageObjX[10];
	int damageObjY[10];
	int damageObjCount;	//�}�b�v��̃_���[�W�I�u�W�F�N�g�̐��𐔂���

	//�X�e�[�W�̉�]�������
	const int moveFrame = 32;
	int count;
	int mapRote;
	float roll;
	float Roll_d;
	int state;
	int muki;

	//�L�[���͏��
	/*char keys;
	char oldkeys;*/
	//�X�e�[�W���ۑ�
	int stage_;
	//�Q�[���X�^�[�g�����Z�b�g���������f
	bool farstGame;
	//�^�C�}�[
	const int time = 30 * 60;
	int timer;

	//�֐�
	//�_���[�W����
	void Damage();

	//��������
	void Fall();

	//�S�[���ɐG�ꂽ��N���A
	bool Clear();

	//��]�`��
	void RollDraw(int x, int y, int GH);

	//�X�e�[�W�z�u
	//�X�e�[�W1
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