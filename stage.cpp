#include "stage.h"

Stage::Stage() {
	//リソース
	blockGH = LoadGraph("Resource/pict/block.png");
	objBlockGH = LoadGraph("Resource/pict/object.png");

	//プレイヤー情報
	player.x = 4;
	player.y = 7;
	playerX = WIN_WIDTH / 2 - player.sizeX;
	playerX = WIN_HEIGHT / 2 - player.sizeX * 2.5;

	//ステージの回転処理情報
	muki = 0;
	roll = 0;
	Roll_d = 0;

	state = 0;
}

Stage::~Stage() {

}

void Stage::Update(char* keys, char* oldkeys) {
	// 状態によって処理を分岐
	switch (state) {
	case 0:	// 入力待ち状態

		//Aが押されたら左方向に移動する状態に移行する
		if (keys[KEY_INPUT_A] && !oldkeys[KEY_INPUT_A]) {
			//向きによって移動方向が変わる
			switch (muki) {
			case 0: movx = -1; movy = 0; break;
			case 1: movx = 0; movy = 1; break;
			case 2: movx = 1; movy = 0; break;
			case 3: movx = 0; movy = -1; break;
			}

			// 移動先のマスに障害物がなければ移動する
			if (map[player.y + movy][player.x + movx] == NONE) {
				// 状態を左移動中にする
				state = 1;
				count = 0;
			}
		}


		//Dが押されたら左方向に移動する状態に移行する
		if (keys[KEY_INPUT_D] && !oldkeys[KEY_INPUT_D]) {
			//向きによって移動方向が変わる
			switch (muki) {
			case 0: movx = 1; movy = 0; break;
			case 1: movx = 0; movy = -1; break;
			case 2: movx = -1; movy = 0; break;
			case 3: movx = 0; movy = 1; break;
			}

			// 移動先のマスに障害物がなければ移動する
			if (map[player.y + movy][player.x + movx] == NONE) {
				// 状態を右移動中にする
				state = 2;
				count = 0;
			}
		}

		// Eが押されていたら左に９０度変更する
		if (keys[KEY_INPUT_E] && !oldkeys[KEY_INPUT_E]) {
			// 状態を左旋回中にする
			state = 3;
			count = 0;
		}

		// Qが押されていたら右に９０度変更する
		if (keys[KEY_INPUT_Q] && !oldkeys[KEY_INPUT_Q]) {
			// 状態を右旋回中にする
			state = 4;
			count = 0;
		}
		break;

	case 1:	// 左移動中状態
		// カウントを進める
		count++;
		playerX += movx * 2;
		playerY += movy * 2;

		// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
		if (count == moveFrame) {
			player.x += movx;
			player.y += movy;
			state = 0;
			count = 0;
		}
		break;

	case 2:	// 右移動中状態
	// カウントを進める
		count++;

		// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
		if (count == moveFrame) {
			player.x += movx;
			player.y += movy;
			state = 0;
			count = 0;
		}
		break;

	case 3:	// 左旋回中状態
		// カウントを進める
		count++;
		mapRote = 1;

		Roll_d = ((float)count / 32.0f) * DX_PI_F / 2 * mapRote; //回転

		// カウントが推移時間に達したら実方向を変更して入力待ち状態に戻る
		if (count == moveFrame) {
			if (muki == 3)
			{
				muki = 0;
			}
			else
			{
				muki++;
			}
			roll += Roll_d;
			Roll_d = 0.0f;
			state = 0;
			count = 0;
		}
		break;

	case 4:	// 右旋回中状態
	// カウントを進める
		count++;
		mapRote = -1;

		Roll_d = ((float)count / 32.0f) * DX_PI_F / 2 * mapRote; //回転

		// カウントが推移時間に達したら実方向を変更して入力待ち状態に戻る
		if (count == moveFrame) {
			if (muki == 0)
			{
				muki = 3;
			}
			else
			{
				muki--;
			}
			roll += Roll_d;
			Roll_d = 0.0f;
			state = 0;
			count = 0;
		}
		break;
	}
}

void Stage::Draw() {
	//マップチップ
	//行
	for (int y = 0; y < mapY1; y++) {
		//列
		for (int x = 0; x < mapX1; x++) {
			int graph;
			if (map[y][x] == BLOCK) {
				graph = blockGH;
			}
			else if (map[y][x] == OBJ) {
				graph = objBlockGH;
			}
			else {
				graph = 0;
			}

			//画面の中心になるように描画
			int posX = x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
			int posY = y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
			DrawRotaGraph2(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 2 - posX, WIN_HEIGHT / 2 - posY,
				1.0f, roll + Roll_d, graph, false, 0);
		}
	}


	//プレイヤー
	int posX = player.x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
	int posY = player.y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
	player.Draw(posX, posY, roll + Roll_d);

	//デバック
	DrawFormatString(0, 0, 0xffffff, "%d", muki);
	DrawLine(0, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT / 2, 0xffffff, 2);
	DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, 0xffffff, 2);
}

bool Stage::MapChipCollision(int left, int top, int right, int bottom) {
	if (map[top / blockSize][left / blockSize] == OBJ) return true;
	if (map[top / blockSize][right / blockSize] == OBJ) return true;
	if (map[bottom / blockSize][left / blockSize] == OBJ) return true;
	if (map[bottom / blockSize][right / blockSize] == OBJ) return true;
	return false;
}