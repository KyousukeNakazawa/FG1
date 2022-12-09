#include "screen.h"

Screen::Screen() {
	//背景情報
	backX = 0;
	backY = 0;
	LoadDivGraph("Resource/pict/tutorial.png", 2, 2, 1, WIN_WIDTH, WIN_HEIGHT, tutorialGH);
	stageGH1 = LoadGraph("Resource/pict/stage.png");
	stageGH2 = LoadGraph("Resource/pict/stage2.png");

	stageEndGH1 = LoadGraph("Resource/pict/stage1End.png");
	stageEndGH2 = LoadGraph("Resource/pict/stage2End.png");

	//UI情報
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
	//ゲーム背景
	if (scene == GAME) DrawGraph(backX, backY, stageGH1, true);

	//チュートリアル背景
	if (stage == TUTORIAL) {
		if (tutorial) DrawGraph(0, 0, tutorialGH[0], true);
		else DrawGraph(0, 0, tutorialGH[1], true);
	}

	//UI
	//HP
	HpUI(hp);
	
	//デバック
	/*DrawFormatString(0, 40, 0xffffff, "Q:左回転 E:右回転");
	DrawFormatString(0, 55, 0xffffff, "A,D:横移動");
	DrawFormatString(0, 70, 0xffffff, "R:リセット");
	DrawFormatString(0, 105, 0xffffff, "B:タイトルに戻る");*/
}

void Screen::HpUI(int hp) {
	//アニメーションさせるための計算
	int j = hpEffect.GHTimer / (hpEffect.GHTime / hpEffect.GHNum);

	//アニメーションタイマーをすすめ一定時間後0に戻す
	if (++hpEffect.GHTimer >= hpEffect.GHTime) hpEffect.GHTimer = 0;

	for (int i = 0; i < hp; i++) {
		DrawGraph(hpGHX + (i * 32), hpGHY, hpGH[j], true);
	}
}