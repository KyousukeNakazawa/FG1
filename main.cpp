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
	//サウンド
	int selectSE = LoadSoundMem("Resource/sound/selectSE.mp3");
	ChangeVolumeSoundMem(150, selectSE);
	int decisionSE = LoadSoundMem("Resource/sound/decisionSE.mp3");
	ChangeVolumeSoundMem(150, decisionSE);
	int titleBGM = LoadSoundMem("Resource/sound/titleBGM.mp3");
	ChangeVolumeSoundMem(150, titleBGM);
	int gameBGM = LoadSoundMem("Resource/sound/gameBGM.mp3");
	ChangeVolumeSoundMem(150, gameBGM);
	int endBGM = LoadSoundMem("Resource/sound/endBGM.mp3");
	ChangeVolumeSoundMem(100, endBGM);

	//リソース
	int titleGH[4];
	LoadDivGraph("Resource/pict/title.png", 4, 2, 2, WIN_WIDTH, WIN_HEIGHT, titleGH);
	int clearGH = LoadGraph("Resource/pict/clear.png");

	// ゲームループで使う変数の宣言
	Screen* screen = new Screen;
	Stage* stage = new Stage;

	int scene_ = TITLE;
	int stage_ = TUTORIAL;

	int backX = 0;
	int backY = 0;

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
		//更新処理
		switch (scene_) {
		case TITLE:
			StopSoundMem(gameBGM);
			StopSoundMem(endBGM);
			//BGM
			if (!CheckSoundMem(titleBGM)) {
				PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, true);
			}
			//ステージ選択
			//上
			if ((keys[KEY_INPUT_W] && !oldkeys[KEY_INPUT_W])
				|| (keys[KEY_INPUT_UP] && !oldkeys[KEY_INPUT_UP])) {
				stage_--;
				if (stage_ < TUTORIAL) stage_ = STAGE3;
				PlaySoundMem(selectSE, DX_PLAYTYPE_BACK, true);
			}

			//下
			if (keys[KEY_INPUT_S] && !oldkeys[KEY_INPUT_S]
				|| (keys[KEY_INPUT_DOWN] && !oldkeys[KEY_INPUT_DOWN])) {
				stage_++;
				if (stage_ > STAGE3) stage_ = TUTORIAL;
				PlaySoundMem(selectSE, DX_PLAYTYPE_BACK, true);
			}

			//決定
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK, true);
				//チュートリアルならフラグをon
				if (stage_ == TUTORIAL) stage->SetTutorialFlag();
				scene_ = GAME;
				stage->Reset(stage_);
			}

			//描画
			if (stage_ == TUTORIAL) DrawGraph(backX, backY, titleGH[0], true);
			else if (stage_ == STAGE1) DrawGraph(backX, backY, titleGH[1], true);
			else if (stage_ == STAGE2) DrawGraph(backX, backY, titleGH[2], true);
			else if (stage_ == STAGE3) DrawGraph(backX, backY, titleGH[3], true);

			//デバック
			DrawFormatString(0, 0, 0xffffff, "タイトル");
			if (stage_ == TUTORIAL) DrawFormatString(0, 15, 0xffffff, "チュートリアル");
			else if (stage_ == STAGE1) DrawFormatString(0, 15, 0xffffff, "STAGE1");
			else if (stage_ == STAGE2) DrawFormatString(0, 15, 0xffffff, "STAGE2");
			else if (stage_ == STAGE3) DrawFormatString(0, 15, 0xffffff, "STAGE3");

			DrawFormatString(0, 40, 0xffffff, "W,S でステージ選択");
			DrawFormatString(0, 55, 0xffffff, "Spaceで決定");
			break;

		case GAME:
			StopSoundMem(titleBGM);
			StopSoundMem(endBGM);
			//BGM
			if (!CheckSoundMem(gameBGM)) {
				PlaySoundMem(gameBGM, DX_PLAYTYPE_LOOP, true);
			}

			stage->Update(scene_, keys, oldkeys);

			//描画処理
			screen->Draw(scene_, stage->HpGet());

			stage->Draw();

			//デバック
			//DrawFormatString(0, 0, 0xffffff, "ゲーム");
			break;
		case CLEAR:
			StopSoundMem(titleBGM);
			StopSoundMem(gameBGM);
			//BGM
			if (!CheckSoundMem(endBGM)) {
				PlaySoundMem(endBGM, DX_PLAYTYPE_LOOP, true);
			}
			stage->soundStop();
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				scene_ = TITLE;
			}

			//描画
			//背景
			DrawGraph(backX, backY, clearGH, true);

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
