#include "stage.h"

Stage::Stage() {
	//リソース
	LoadDivGraph("Resource/pict/block.png", 4, 2, 2, 64, 64, blockGH);
	objBlockGH = LoadGraph("Resource/pict/object.png");
	mObjBlockGH = LoadGraph("Resource/pict/move_object.png");
	goalGH = LoadGraph("Resource/pict/goal.png");
	damageGH = LoadGraph("Resource/pict/damage.png");

	LoadDivGraph("Resource/pict/tutorial.png", 2, 2, 1, WIN_WIDTH, WIN_HEIGHT, tutorialGH);

	rotationSE = LoadSoundMem("Resource/sound/rotationSE.mp3");
	ChangeVolumeSoundMem(150, rotationSE);
	moveSE = LoadSoundMem("Resource/sound/moveSE.mp3");
	ChangeVolumeSoundMem(150, moveSE);
	clearSE = LoadSoundMem("Resource/sound/clearSE.mp3");
	ChangeVolumeSoundMem(150, clearSE);
	damageSE = LoadSoundMem("Resource/sound/damageSE.mp3");
	ChangeVolumeSoundMem(150, damageSE);


	//プレイヤー情報
	player.x = 4;
	player.y = 7;
	hp = 3;

	//ステージ情報
	for (int i = 0; i < objNum; i++) {
		moveObjX[i] = -50;
		moveObjY[i] = -50;
		damageObjX[i] = -50;
		damageObjY[i] = -50;
	}
	moveObjCount = 0;
	damageObjCount = 0;

	//ステージの回転処理情報
	muki = 0;
	roll = 0;
	Roll_d = 0;
	count = 0;

	state = 0;

	farstGame = true;

	tutorialFlag = false;
}

Stage::~Stage() {

}

void Stage::Reset(int stage) {
	stage_ = stage;

	//リセット処理
	muki = 0;
	roll = 0;
	Roll_d = 0;
	count = 0;

	state = 0;

	for (int i = 0; i < objNum; i++) {
		moveObjX[i] = -50;
		moveObjY[i] = -50;
		damageObjX[i] = -50;
		damageObjY[i] = -50;
	}
	moveObjCount = 0;
	damageObjCount = 0;

	//最初のゲームスタートならhpとタイマーをセット
	if (farstGame) {
		farstGame = false;
		hp = 3;
	}

	//選択されたステージによってマップを生成
	switch (stage_) {
	case TUTORIAL:
		if (tutorialFlag) {
			for (int y = 0; y < mapY1; y++) {
				//列
				for (int x = 0; x < mapX1; x++) {
					map[y][x] = tutorial1[y][x];
				}
			}
			//プレイヤー情報
			player.x = 4;
			player.y = 6;
		}
		else {
			for (int y = 0; y < mapY1; y++) {
				//列
				for (int x = 0; x < mapX1; x++) {
					map[y][x] = tutorial2[y][x];
				}
			}
			//プレイヤー情報
			player.x = 4;
			player.y = 5;
		}

		break;
	case STAGE1:
		tutorialFlag = false;
		for (int y = 0; y < mapY1; y++) {
			//列
			for (int x = 0; x < mapX1; x++) {
				map[y][x] = map1[y][x];
			}
		}
		//プレイヤー情報
		player.x = 4;
		player.y = 7;
		break;
	case STAGE2:
		tutorialFlag = false;
		for (int y = 0; y < mapY1; y++) {
			//列
			for (int x = 0; x < mapX1; x++) {
				map[y][x] = map5[y][x];
			}
		}
		player.x = 4;
		player.y = 7;
		break;
	case STAGE3:
		tutorialFlag = false;
		for (int y = 0; y < mapY1; y++) {
			//列
			for (int x = 0; x < mapX1; x++) {
				map[y][x] = map4[y][x];
			}
		}
		player.x = 7;
		player.y = 7;
		break;
	}

	for (int y = 0; y < mapY1; y++) {
		//列
		for (int x = 0; x < mapX1; x++) {
			//マップチップ上のゴールを記録
			if (map[y][x] == GOAL) {
				goalX = x;
				goalY = y;
			}
			//マップチップ上の動くブロックを記録
			if (map[y][x] == M_OBJ) {
				moveObjX[moveObjCount] = x;
				moveObjY[moveObjCount] = y;
				moveObjCount++;
			}
			//マップチップ上のダメージブロックを記録
			else if (map[y][x] == DAMAGE) {
				damageObjX[damageObjCount] = x;
				damageObjY[damageObjCount] = y;
				damageObjCount++;
			}
		}
	}
}

void Stage::soundStop() {
	StopSoundMem(rotationSE);
	StopSoundMem(moveSE);
	//StopSoundMem(clearSE);
}

void Stage::Update(int& scene, char* keys, char* oldkeys) {
	//動くブロックをマップチップに記録
	for (int i = 0; i < moveObjCount; i++) {
		if (moveObjX[i] >= 0 && moveObjY[i] >= 0) {
			map[moveObjY[i]][moveObjX[i]] = M_OBJ;
		}
	}
	//ダメージブロックをマップチップ上に記録
	for (int i = 0; i < damageObjCount; i++) {
		if (map[damageObjY[i]][damageObjX[i]] == NONE) {
			map[damageObjY[i]][damageObjX[i]] = DAMAGE;
		}
	}

	//動くブロックが消したブロックを復活
	//ゴール
	if (map[goalY][goalX] == NONE) {
		map[goalY][goalX] = GOAL;
	}

	//状態によって処理を分岐
	switch (state) {
	case 0:	// 入力待ち状態

		//Aが押されたら左方向に移動する状態に移行する
		if (keys[KEY_INPUT_A] && !oldkeys[KEY_INPUT_A]) {
			PlaySoundMem(moveSE, DX_PLAYTYPE_BACK, true);

			//向きによって移動方向が変わる
			switch (muki) {
			case 0: movX = -1; movY = 0; break;
			case 1: movX = 0; movY = 1; break;
			case 2: movX = 1; movY = 0; break;
			case 3: movX = 0; movY = -1; break;
			}

			// 移動先のマスに障害物がなければ移動する
			if (map[player.y + movY][player.x + movX] < BLOCK
				|| map[player.y + movY][player.x + movX] > M_OBJ) {
				//障害物がなかったとしても高さが２マス以上だったら移動不可
				if ((map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] < BLOCK
					|| map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] > M_OBJ)
					&& (map[player.y + movY + fallY][player.x + movX + fallX] < BLOCK
						|| map[player.y + movY + fallY][player.x + movX + fallX] >M_OBJ)) {
					break;
				}

				//状態を左移動中にする
				state = 1;
				count = 0;
			}
			//移動先のマスに障害物があった場合1マスなら上に乗る
			else if (map[player.y + movY][player.x + movX] == OBJ
				|| map[player.y + movY][player.x + movX] == M_OBJ) {
				if ((map[player.y + movY - fallY][player.x + movX - fallX] == NONE
					|| map[player.y + movY - fallY][player.x + movX - fallX] == GOAL)
					&& map[player.y - fallY][player.x - fallX] == NONE) {
					state = 1;
					count = 0;
				}
			}
		}


		//Dが押されたら左方向に移動する状態に移行する
		if (keys[KEY_INPUT_D] && !oldkeys[KEY_INPUT_D]) {
			PlaySoundMem(moveSE, DX_PLAYTYPE_BACK, true);
			//向きによって移動方向が変わる
			switch (muki) {
			case 0: movX = 1; movY = 0; break;
			case 1: movX = 0; movY = -1; break;
			case 2: movX = -1; movY = 0; break;
			case 3: movX = 0; movY = 1; break;
			}

			// 移動先のマスに障害物がなければ移動する
			if (map[player.y + movY][player.x + movX] < BLOCK
				|| map[player.y + movY][player.x + movX] > M_OBJ) {
				//障害物がなかったとしても高さが２マス以上だったら移動不可
				if ((map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] < BLOCK
					|| map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] > M_OBJ)
					&& (map[player.y + movY + fallY][player.x + movX + fallX] < BLOCK 
					|| map[player.y + movY + fallY][player.x + movX + fallX] >M_OBJ)) {
					break;
				}

				// 状態を右移動中にする
				state = 2;
				count = 0;
			}
			//移動先のマスに障害物があった場合1マスなら上に乗る
			else if (map[player.y + movY][player.x + movX] == OBJ
				|| map[player.y + movY][player.x + movX] == M_OBJ) {
				if ((map[player.y + movY - fallY][player.x + movX - fallX] == NONE
					|| map[player.y + movY - fallY][player.x + movX - fallX] == GOAL)
					&& map[player.y - fallY][player.x - fallX] == NONE) {
					state = 2;
					count = 0;
				}
			}
		}

		// Eが押されていたら右に９０度変更する
		if (keys[KEY_INPUT_E] && !oldkeys[KEY_INPUT_E]) {
			// 状態を右旋回中にする
			state = 3;
			count = 0;
			PlaySoundMem(rotationSE, DX_PLAYTYPE_BACK, true);
		}

		// Qが押されていたら左に９０度変更する
		if (keys[KEY_INPUT_Q] && !oldkeys[KEY_INPUT_Q]) {
			// 状態を左旋回中にする
			state = 4;
			count = 0;
			PlaySoundMem(rotationSE, DX_PLAYTYPE_BACK, true);
		}
		break;

	case 1:	// 左移動中状態
		// カウントを進める
		count++;

		// カウントが移動時間に達したら実座標を移動して入力待ち状態に戻る
		if (count == moveFrame / 2) {
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
		if (count == moveFrame / 2) {
			player.x += movX - fallX;
			player.y += movY - fallY;
			state = 0;
			count = 0;
		}
		break;

	case 3:	// 右旋回中状態
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

	case 4:	// 左旋回中状態
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
	Fall();

	//ダメージ処理
	Damage();

	//Rを押したらリセット
	if (keys[KEY_INPUT_R] && !oldkeys[KEY_INPUT_R]) {
		Reset(stage_);
	}

	//クリア処理
	if (Clear()) {
		PlaySoundMem(clearSE, DX_PLAYTYPE_BACK, true);
		farstGame = true;
		//チュートリアル中でなければクリア画面へ
		if (!tutorialFlag) {
			scene = CLEAR;
		}
		//チュートリアル中ならば次の説明へ
		else {
			//ゴールしたら終わるようにチュートリアルフラグをoff
			tutorialFlag = false;
			Reset(stage_);
		}
	}

	//タイマー処理
	//timer--;

	//ゲームオーバー処理
	if (hp <= 0) {
		farstGame = true;
		//チュートリアル中なら最初の状態に戻す
		if (stage_ == TUTORIAL) {
			Reset(stage_);
		}
		else {
			scene = TITLE;
		}
	}

	//タイトルに戻る処理
	if (keys[KEY_INPUT_B] && !oldkeys[KEY_INPUT_B]) {
		farstGame = true;
		tutorialFlag = false;
		scene = TITLE;
	}
}

void Stage::Draw() {
	//チュートリアル背景
	if (stage_ == TUTORIAL) {
		if (tutorialFlag) DrawGraph(0, 0, tutorialGH[0], true);
		else DrawGraph(0, 0, tutorialGH[1], true);
	}

	//マップチップ
	//行
	for (int y = 0; y < mapY1; y++) {
		//列
		for (int x = 0; x < mapX1; x++) {
			int graph;
			if (map[y][x] == BLOCK) {
				//ステージごとにブロックを変える
				if (stage_ == STAGE2) graph = blockGH[1];
				else if (stage_ == STAGE3) graph = blockGH[2];
				else graph = blockGH[0];
			}
			else if (map[y][x] == OBJ) {
				graph = objBlockGH;
			}
			else if (map[y][x] == M_OBJ) {
				graph = mObjBlockGH;
			}
			else if (map[y][x] == GOAL) {
				graph = goalGH;
			}
			else if (map[y][x] == DAMAGE) {
				graph = damageGH;
			}
			else {
				graph = 0;
			}

			//画面の中心になるように描画
			RollDraw(x, y, graph);
		}
	}

	//プレイヤー
	{
		int posX = player.x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
		int posY = player.y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
		player.Draw(posX, posY, roll + Roll_d);
	}

	//デバック
	//DrawFormatString(0, 15, 0xffffff, "%d", moveObjCount);
	/*DrawLine(0, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT / 2, 0xffffff, 2);
	DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, 0xffffff, 2);*/
}

//ダメージ処理
void Stage::Damage() {
	//ダメージブロックと重なったっらhpが減ってリセット
	if (map[player.y][player.x] == DAMAGE) {
		PlaySoundMem(damageSE, DX_PLAYTYPE_BACK, true);
		hp--;
		Reset(stage_);
	}

	//動くオブジェクトと重なったらhpが減ってリセット
	if (map[player.y][player.x] == M_OBJ) {
		PlaySoundMem(damageSE, DX_PLAYTYPE_BACK, true);
		hp--;
		Reset(stage_);
	}
}

//落下処理
void Stage::Fall() {
	//向きによって落下方向変化
	switch (muki) {
	case 0: fallX = 0; fallY = 1; break;
	case 1: fallX = 1; fallY = 0; break;
	case 2: fallX = 0; fallY = -1; break;
	case 3: fallX = -1; fallY = 0; break;
	}

	//障害物がない限り下に移動
	//動くブロック
	for (int i = 0; i < moveObjCount; i++) {
		if (map[moveObjY[i] + fallY][moveObjX[i] + fallX] >= NONE && (map[moveObjY[i] + fallY][moveObjX[i] + fallX] < BLOCK
			|| map[moveObjY[i] + fallY][moveObjX[i] + fallX] > M_OBJ)) {
			map[moveObjY[i]][moveObjX[i]] = NONE;
			moveObjX[i] += fallX;
			moveObjY[i] += fallY;
		}
	}

	//プレイヤー
	if (map[player.y + fallY][player.x + fallX] < BLOCK
		|| map[player.y + fallY][player.x + fallX] > M_OBJ) {
		player.x += fallX;
		player.y += fallY;
	}
}

//ゴールに触れたらクリア
bool Stage::Clear() {
	//ゴールに触れたとき
	if (map[player.y][player.x] == GOAL) {
		//プレイヤーが地面に触れているならクリア
		if (map[player.y + fallY][player.x + fallX] != NONE) return true;
	}
	return false;
}

//回転描画
void Stage::RollDraw(int x, int y, int GH) {
	int posX = x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
	int posY = y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
	DrawRotaGraph2(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 2 - posX, WIN_HEIGHT / 2 - posY,
		1.0f, roll + Roll_d, GH, false, 0);
}
