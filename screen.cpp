#include "screen.h"

Screen::Screen() {
	//�w�i���
	backX = 0;
	backY = 0;
	LoadDivGraph("Resource/pict/tutorial.png", 2, 2, 1, WIN_WIDTH, WIN_HEIGHT, tutorialGH);
	stageGH1 = LoadGraph("Resource/pict/stage.png");
	stageGH2 = LoadGraph("Resource/pict/stage2.png");

	stageEndGH1 = LoadGraph("Resource/pict/stage1End.png");
	stageEndGH2 = LoadGraph("Resource/pict/stage2End.png");

	//UI���
	//HP
	hpGHX = 10;
	hpGHY = 10;
	LoadDivGraph("Resource/pict/hp.png", 8, 8, 1, 32, 32, hpGH);
}

Screen::~Screen() {

}

void Screen::Update() {

}

void Screen::Draw(int scene, int stage, int hp, bool tutorial) {
	//�Q�[���w�i
	if (scene == GAME) DrawGraph(backX, backY, stageGH1, true);

	//�`���[�g���A���w�i
	if (stage == TUTORIAL) {
		if (tutorial) DrawGraph(0, 0, tutorialGH[0], true);
		else DrawGraph(0, 0, tutorialGH[1], true);
	}

	//UI
	//HP
	HpUI(hp);
	
	//�f�o�b�N
	/*DrawFormatString(0, 40, 0xffffff, "Q:����] E:�E��]");
	DrawFormatString(0, 55, 0xffffff, "A,D:���ړ�");
	DrawFormatString(0, 70, 0xffffff, "R:���Z�b�g");
	DrawFormatString(0, 105, 0xffffff, "B:�^�C�g���ɖ߂�");*/
}

void Screen::HpUI(int hp) {
	//�A�j���[�V���������邽�߂̌v�Z
	int j = hpEffect.GHTimer / (hpEffect.GHTime / hpEffect.GHNum);

	//�A�j���[�V�����^�C�}�[�������߈�莞�Ԍ�0�ɖ߂�
	if (++hpEffect.GHTimer >= hpEffect.GHTime) hpEffect.GHTimer = 0;

	for (int i = 0; i < hp; i++) {
		DrawGraph(hpGHX + (i * 32), hpGHY, hpGH[j], true);
	}
}