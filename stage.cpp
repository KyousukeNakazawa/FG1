#include "stage.h"

Stage::Stage() {
	//���\�[�X
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


	//�v���C���[���
	player.x = 4;
	player.y = 7;
	hp = 3;

	//�X�e�[�W���
	for (int i = 0; i < objNum; i++) {
		moveObjX[i] = -50;
		moveObjY[i] = -50;
		damageObjX[i] = -50;
		damageObjY[i] = -50;
	}
	moveObjCount = 0;
	damageObjCount = 0;

	//�X�e�[�W�̉�]�������
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

	//���Z�b�g����
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

	//�ŏ��̃Q�[���X�^�[�g�Ȃ�hp�ƃ^�C�}�[���Z�b�g
	if (farstGame) {
		farstGame = false;
		hp = 3;
	}

	//�I�����ꂽ�X�e�[�W�ɂ���ă}�b�v�𐶐�
	switch (stage_) {
	case TUTORIAL:
		if (tutorialFlag) {
			for (int y = 0; y < mapY1; y++) {
				//��
				for (int x = 0; x < mapX1; x++) {
					map[y][x] = tutorial1[y][x];
				}
			}
			//�v���C���[���
			player.x = 4;
			player.y = 6;
		}
		else {
			for (int y = 0; y < mapY1; y++) {
				//��
				for (int x = 0; x < mapX1; x++) {
					map[y][x] = tutorial2[y][x];
				}
			}
			//�v���C���[���
			player.x = 4;
			player.y = 5;
		}

		break;
	case STAGE1:
		tutorialFlag = false;
		for (int y = 0; y < mapY1; y++) {
			//��
			for (int x = 0; x < mapX1; x++) {
				map[y][x] = map1[y][x];
			}
		}
		//�v���C���[���
		player.x = 4;
		player.y = 7;
		break;
	case STAGE2:
		tutorialFlag = false;
		for (int y = 0; y < mapY1; y++) {
			//��
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
			//��
			for (int x = 0; x < mapX1; x++) {
				map[y][x] = map4[y][x];
			}
		}
		player.x = 7;
		player.y = 7;
		break;
	}

	for (int y = 0; y < mapY1; y++) {
		//��
		for (int x = 0; x < mapX1; x++) {
			//�}�b�v�`�b�v��̃S�[�����L�^
			if (map[y][x] == GOAL) {
				goalX = x;
				goalY = y;
			}
			//�}�b�v�`�b�v��̓����u���b�N���L�^
			if (map[y][x] == M_OBJ) {
				moveObjX[moveObjCount] = x;
				moveObjY[moveObjCount] = y;
				moveObjCount++;
			}
			//�}�b�v�`�b�v��̃_���[�W�u���b�N���L�^
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
	//�����u���b�N���}�b�v�`�b�v�ɋL�^
	for (int i = 0; i < moveObjCount; i++) {
		if (moveObjX[i] >= 0 && moveObjY[i] >= 0) {
			map[moveObjY[i]][moveObjX[i]] = M_OBJ;
		}
	}
	//�_���[�W�u���b�N���}�b�v�`�b�v��ɋL�^
	for (int i = 0; i < damageObjCount; i++) {
		if (map[damageObjY[i]][damageObjX[i]] == NONE) {
			map[damageObjY[i]][damageObjX[i]] = DAMAGE;
		}
	}

	//�����u���b�N���������u���b�N�𕜊�
	//�S�[��
	if (map[goalY][goalX] == NONE) {
		map[goalY][goalX] = GOAL;
	}

	//��Ԃɂ���ď����𕪊�
	switch (state) {
	case 0:	// ���͑҂����

		//A�������ꂽ�獶�����Ɉړ������ԂɈڍs����
		if (keys[KEY_INPUT_A] && !oldkeys[KEY_INPUT_A]) {
			PlaySoundMem(moveSE, DX_PLAYTYPE_BACK, true);

			//�����ɂ���Ĉړ��������ς��
			switch (muki) {
			case 0: movX = -1; movY = 0; break;
			case 1: movX = 0; movY = 1; break;
			case 2: movX = 1; movY = 0; break;
			case 3: movX = 0; movY = -1; break;
			}

			// �ړ���̃}�X�ɏ�Q�����Ȃ���Έړ�����
			if (map[player.y + movY][player.x + movX] < BLOCK
				|| map[player.y + movY][player.x + movX] > M_OBJ) {
				//��Q�����Ȃ������Ƃ��Ă��������Q�}�X�ȏゾ������ړ��s��
				if ((map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] < BLOCK
					|| map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] > M_OBJ)
					&& (map[player.y + movY + fallY][player.x + movX + fallX] < BLOCK
						|| map[player.y + movY + fallY][player.x + movX + fallX] >M_OBJ)) {
					break;
				}

				//��Ԃ����ړ����ɂ���
				state = 1;
				count = 0;
			}
			//�ړ���̃}�X�ɏ�Q�����������ꍇ1�}�X�Ȃ��ɏ��
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


		//D�������ꂽ�獶�����Ɉړ������ԂɈڍs����
		if (keys[KEY_INPUT_D] && !oldkeys[KEY_INPUT_D]) {
			PlaySoundMem(moveSE, DX_PLAYTYPE_BACK, true);
			//�����ɂ���Ĉړ��������ς��
			switch (muki) {
			case 0: movX = 1; movY = 0; break;
			case 1: movX = 0; movY = -1; break;
			case 2: movX = -1; movY = 0; break;
			case 3: movX = 0; movY = 1; break;
			}

			// �ړ���̃}�X�ɏ�Q�����Ȃ���Έړ�����
			if (map[player.y + movY][player.x + movX] < BLOCK
				|| map[player.y + movY][player.x + movX] > M_OBJ) {
				//��Q�����Ȃ������Ƃ��Ă��������Q�}�X�ȏゾ������ړ��s��
				if ((map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] < BLOCK
					|| map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] > M_OBJ)
					&& (map[player.y + movY + fallY][player.x + movX + fallX] < BLOCK 
					|| map[player.y + movY + fallY][player.x + movX + fallX] >M_OBJ)) {
					break;
				}

				// ��Ԃ��E�ړ����ɂ���
				state = 2;
				count = 0;
			}
			//�ړ���̃}�X�ɏ�Q�����������ꍇ1�}�X�Ȃ��ɏ��
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

		// E��������Ă�����E�ɂX�O�x�ύX����
		if (keys[KEY_INPUT_E] && !oldkeys[KEY_INPUT_E]) {
			// ��Ԃ��E���񒆂ɂ���
			state = 3;
			count = 0;
			PlaySoundMem(rotationSE, DX_PLAYTYPE_BACK, true);
		}

		// Q��������Ă����獶�ɂX�O�x�ύX����
		if (keys[KEY_INPUT_Q] && !oldkeys[KEY_INPUT_Q]) {
			// ��Ԃ������񒆂ɂ���
			state = 4;
			count = 0;
			PlaySoundMem(rotationSE, DX_PLAYTYPE_BACK, true);
		}
		break;

	case 1:	// ���ړ������
		// �J�E���g��i�߂�
		count++;

		// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
		if (count == moveFrame / 2) {
			player.x += movX - fallX;
			player.y += movY - fallY;
			state = 0;
			count = 0;
		}
		break;

	case 2:	// �E�ړ������
	// �J�E���g��i�߂�
		count++;

		// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
		if (count == moveFrame / 2) {
			player.x += movX - fallX;
			player.y += movY - fallY;
			state = 0;
			count = 0;
		}
		break;

	case 3:	// �E���񒆏��
		// �J�E���g��i�߂�
		count++;
		mapRote = 1;

		Roll_d = ((float)count / 32.0f) * DX_PI_F / 2 * mapRote; //��]

		// �J�E���g�����ڎ��ԂɒB�������������ύX���ē��͑҂���Ԃɖ߂�
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

	case 4:	// �����񒆏��
	// �J�E���g��i�߂�
		count++;
		mapRote = -1;

		Roll_d = ((float)count / 32.0f) * DX_PI_F / 2 * mapRote; //��]

		// �J�E���g�����ڎ��ԂɒB�������������ύX���ē��͑҂���Ԃɖ߂�
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


	//��]��̗�������
	Fall();

	//�_���[�W����
	Damage();

	//R���������烊�Z�b�g
	if (keys[KEY_INPUT_R] && !oldkeys[KEY_INPUT_R]) {
		Reset(stage_);
	}

	//�N���A����
	if (Clear()) {
		PlaySoundMem(clearSE, DX_PLAYTYPE_BACK, true);
		farstGame = true;
		//�`���[�g���A�����łȂ���΃N���A��ʂ�
		if (!tutorialFlag) {
			scene = CLEAR;
		}
		//�`���[�g���A�����Ȃ�Ύ��̐�����
		else {
			//�S�[��������I���悤�Ƀ`���[�g���A���t���O��off
			tutorialFlag = false;
			Reset(stage_);
		}
	}

	//�^�C�}�[����
	//timer--;

	//�Q�[���I�[�o�[����
	if (hp <= 0) {
		farstGame = true;
		//�`���[�g���A�����Ȃ�ŏ��̏�Ԃɖ߂�
		if (stage_ == TUTORIAL) {
			Reset(stage_);
		}
		else {
			scene = TITLE;
		}
	}

	//�^�C�g���ɖ߂鏈��
	if (keys[KEY_INPUT_B] && !oldkeys[KEY_INPUT_B]) {
		farstGame = true;
		tutorialFlag = false;
		scene = TITLE;
	}
}

void Stage::Draw() {
	//�`���[�g���A���w�i
	if (stage_ == TUTORIAL) {
		if (tutorialFlag) DrawGraph(0, 0, tutorialGH[0], true);
		else DrawGraph(0, 0, tutorialGH[1], true);
	}

	//�}�b�v�`�b�v
	//�s
	for (int y = 0; y < mapY1; y++) {
		//��
		for (int x = 0; x < mapX1; x++) {
			int graph;
			if (map[y][x] == BLOCK) {
				//�X�e�[�W���ƂɃu���b�N��ς���
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

			//��ʂ̒��S�ɂȂ�悤�ɕ`��
			RollDraw(x, y, graph);
		}
	}

	//�v���C���[
	{
		int posX = player.x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
		int posY = player.y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
		player.Draw(posX, posY, roll + Roll_d);
	}

	//�f�o�b�N
	//DrawFormatString(0, 15, 0xffffff, "%d", moveObjCount);
	/*DrawLine(0, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT / 2, 0xffffff, 2);
	DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, 0xffffff, 2);*/
}

//�_���[�W����
void Stage::Damage() {
	//�_���[�W�u���b�N�Əd�Ȃ�������hp�������ă��Z�b�g
	if (map[player.y][player.x] == DAMAGE) {
		PlaySoundMem(damageSE, DX_PLAYTYPE_BACK, true);
		hp--;
		Reset(stage_);
	}

	//�����I�u�W�F�N�g�Əd�Ȃ�����hp�������ă��Z�b�g
	if (map[player.y][player.x] == M_OBJ) {
		PlaySoundMem(damageSE, DX_PLAYTYPE_BACK, true);
		hp--;
		Reset(stage_);
	}
}

//��������
void Stage::Fall() {
	//�����ɂ���ė��������ω�
	switch (muki) {
	case 0: fallX = 0; fallY = 1; break;
	case 1: fallX = 1; fallY = 0; break;
	case 2: fallX = 0; fallY = -1; break;
	case 3: fallX = -1; fallY = 0; break;
	}

	//��Q�����Ȃ����艺�Ɉړ�
	//�����u���b�N
	for (int i = 0; i < moveObjCount; i++) {
		if (map[moveObjY[i] + fallY][moveObjX[i] + fallX] >= NONE && (map[moveObjY[i] + fallY][moveObjX[i] + fallX] < BLOCK
			|| map[moveObjY[i] + fallY][moveObjX[i] + fallX] > M_OBJ)) {
			map[moveObjY[i]][moveObjX[i]] = NONE;
			moveObjX[i] += fallX;
			moveObjY[i] += fallY;
		}
	}

	//�v���C���[
	if (map[player.y + fallY][player.x + fallX] < BLOCK
		|| map[player.y + fallY][player.x + fallX] > M_OBJ) {
		player.x += fallX;
		player.y += fallY;
	}
}

//�S�[���ɐG�ꂽ��N���A
bool Stage::Clear() {
	//�S�[���ɐG�ꂽ�Ƃ�
	if (map[player.y][player.x] == GOAL) {
		//�v���C���[���n�ʂɐG��Ă���Ȃ�N���A
		if (map[player.y + fallY][player.x + fallX] != NONE) return true;
	}
	return false;
}

//��]�`��
void Stage::RollDraw(int x, int y, int GH) {
	int posX = x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
	int posY = y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
	DrawRotaGraph2(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 2 - posX, WIN_HEIGHT / 2 - posY,
		1.0f, roll + Roll_d, GH, false, 0);
}
