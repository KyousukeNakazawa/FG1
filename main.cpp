#include "DxLib.h"
#include "screen.h"
#include "stage.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE_[] = "";
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE_);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0xff);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み


	// ゲームループで使う変数の宣言
	Screen* screen = new Screen;
	Stage* stage = new Stage;

	int scene_ = TITLE;
	int stage_ = STAGE1;

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		for (int i = 0; i < 256; ++i) {
			oldkeys[i] = keys[i];
		}

		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		switch (scene_) {
		case TITLE:
			//ステージ選択
			//上
			if (keys[KEY_INPUT_W] && !oldkeys[KEY_INPUT_W]) {
				stage_--;
				if (stage_ < STAGE1) stage_ = STAGE3;
			}

			//下
			if (keys[KEY_INPUT_S] && !oldkeys[KEY_INPUT_S]) {
				stage_++;
				if (stage_ > STAGE3) stage_ = STAGE1;
			}

			//決定
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				scene_ = GAME;
				stage->Reset(stage_);
			}

			//デバック
			DrawFormatString(0, 0, 0xffffff, "タイトル");
			if (stage_ == STAGE1) DrawFormatString(0, 15, 0xffffff, "STAGE1");
			else if (stage_ == STAGE2) DrawFormatString(0, 15, 0xffffff, "STAGE2");
			else if (stage_ == STAGE3) DrawFormatString(0, 15, 0xffffff, "STAGE3");

			DrawFormatString(0, 40, 0xffffff, "W,S でステージ選択");
			DrawFormatString(0, 55, 0xffffff, "Spaceで決定");
			break;

		case GAME:
			stage->Update(scene_, keys, oldkeys);

			//描画処理
			screen->Draw(scene_, stage->HpGet(), stage->TimerGet());

			stage->Draw();

			//デバック
			//DrawFormatString(0, 0, 0xffffff, "ゲーム");
			break;
		case CLEAR:
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				scene_ = TITLE;
			}

			//デバック
			DrawFormatString(0, 0, 0xffffff, "クリア");
			DrawFormatString(0, 15, 0xffffff, "Spaceでタイトルに戻る");
			break;
		}

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}

	//デリート
	delete screen;
	delete stage;

	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}
