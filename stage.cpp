#include "stage.h"

Stage::Stage() {
	//リソース
	blockGH = LoadGraph("Resource/pict/block.png");
	objBlockGH = LoadGraph("Resource/pict/object.png");
	mObjBlockGH = LoadGraph("Resource/pict/move_object.png");

	//プレイヤー情報
	player.x = 4;
	player.y = 7;

	//ステージ情報
	moveObjX[0] = 6;
	moveObjY[0] = 7;

	//ステージの回転処理情報
	muki = 0;
	roll = 0;
	Roll_d = 0;

	state = 0;
}

Stage::~Stage() {

}

void Stage::Update(char* keys, char* oldkeys) {
	//動くブロックをマップチップに記録
	map[moveObjY[0]][moveObjX[0]] = M_OBJ;

	//向きによって移動量変化
	switch (muki) {
	case 0: fallX = 0; fallY = 1; break;
	case 1: fallX = 1; fallY = 0; break;
	case 2: fallX = 0; fallY = -1; break;
	case 3: fallX = -1; fallY = 0; break;
	}

	// 状態によって処理を分岐
	switch (state) {
	case 0:	// 入力待ち状態

		//Aが押されたら左方向に移動する状態に移行する
		if (keys[KEY_INPUT_A] && !oldkeys[KEY_INPUT_A]) {
			//向きによって移動方向が変わる
			switch (muki) {
			case 0: movX = -1; movY = 0; break;
			case 1: movX = 0; movY = 1; break;
			case 2: movX = 1; movY = 0; break;
			case 3: movX = 0; movY = -1; break;
			}

			// 移動先のマスに障害物がなければ移動する
			if (map[player.y + movY][player.x + movX] == NONE) {
				//障害物がなかったとしても高さが２マス以上だったら移動不可
				if (map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] == NONE) {
					break;
				}

				// 状態を左移動中にする
				state = 1;
				count = 0;
			}
			//移動先のマスに障害物があった場合1マスなら上に乗る
			else if (map[player.y + movY][player.x + movX] == OBJ
				|| map[player.y + movY][player.x + movX] == M_OBJ) {
				if (map[player.y + movY - fallY][player.x + movX - fallX] == NONE) {
					state = 1;
					count = 0;
				}
			}
		}


		//Dが押されたら左方向に移動する状態に移行する
		if (keys[KEY_INPUT_D] && !oldkeys[KEY_INPUT_D]) {
			//向きによって移動方向が変わる
			switch (muki) {
			case 0: movX = 1; movY = 0; break;
			case 1: movX = 0; movY = -1; break;
			case 2: movX = -1; movY = 0; break;
			case 3: movX = 0; movY = 1; break;
			}

			// 移動先のマスに障害物がなければ移動する
			if (map[player.y + movY][player.x + movX] == NONE) {
				//障害物がなかったとしても高さが２マス以上だったら移動不可
				if (map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] == NONE) {
					break;
				}

				// 状態を右移動中にする
				state = 2;
				count = 0;
			}
			//移動先のマスに障害物があった場合1マスなら上に乗る
			else if (map[player.y + movY][player.x + movX] == OBJ
				|| map[player.y + movY][player.x + movX] == M_OBJ) {
				if (map[player.y + movY - fallY][player.x + movX - fallX] == NONE) {
					state = 2;
					count = 0;
				}
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

		// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
		if (count == moveFrame) {
			player.x += movX - fallX;
			player.y += movY - fallY;
			state = 0;
			count = 0;
		}
		break;

	case 2:	// 右移動中状態
	// カウントを進める
		count++;

		// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
		if (count == moveFrame) {
			player.x += movX - fallX;
			player.y += movY - fallY;
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

	//回転後の落下処理
	//障害物がない限り下に移動
	//プレイヤー
	if (map[player.y + fallY][player.x + fallX] == NONE) {
		player.x += fallX;
		player.y += fallY;
	}

	//動くブロック
	if (map[moveObjY[0] + fallY][moveObjX[0] + fallX] == NONE) {
		map[moveObjY[0]][moveObjX[0]] = NONE;
		moveObjX[0] += fallX;
		moveObjY[0] += fallY;
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

	//動くブロック描画
	{
		int posX = moveObjX[0] * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
		int posY = moveObjY[0] * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
		DrawRotaGraph2(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 2 - posX, WIN_HEIGHT / 2 - posY,
			1.0f, roll + Roll_d, mObjBlockGH, false, 0);
	}


	//プレイヤー
	{
		int posX = player.x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
		int posY = player.y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
		player.Draw(posX, posY, roll + Roll_d);
	}

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