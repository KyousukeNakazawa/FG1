#include "stage.h"

Stage::Stage() {
	//���\�[�X
	blockGH = LoadGraph("Resource/pict/block.png");
	objBlockGH = LoadGraph("Resource/pict/object.png");
	mObjBlockGH = LoadGraph("Resource/pict/move_object.png");
	goalGH = LoadGraph("Resource/pict/goal.png");
	damageGH = LoadGraph("Resource/pict/damage.png");

	//�v���C���[���
	player.x = 4;
	player.y = 7;
	player.hp = 3;

	//�X�e�[�W���
	for (int i = 0; i < objNum; i++) {
		moveObjX[i] = -50;
		moveObjX[i] = -50;
		damageObjX[i] = -50;
		damageObjY[i] = -50;
	}

	//�X�e�[�W�̉�]�������
	muki = 0;
	roll = 0;
	Roll_d = 0;

	state = 0;

	farstGame = true;
	timer = time;
}

Stage::~Stage() {

}

void Stage::Reset(int stage) {
	stage_ = stage;

	//���Z�b�g����
	muki = 0;
	roll = 0;
	Roll_d = 0;

	state = 0;

	for (int i = 0; i < objNum; i++) {
		moveObjX[i] = -50;
		moveObjX[i] = -50;
		damageObjX[i] = -50;
		damageObjY[i] = -50;
	}

	//�ŏ��̃Q�[���X�^�[�g�Ȃ�hp�ƃ^�C�}�[���Z�b�g
	if (farstGame) {
		farstGame = false;
		player.hp = 3;
		timer = time;
	}

	//�I�����ꂽ�X�e�[�W�ɂ���ă}�b�v�𐶐�
	switch (stage) {
	case STAGE1:
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
		for (int y = 0; y < mapY1; y++) {
			//��
			for (int x = 0; x < mapX1; x++) {
				map[y][x] = map2[y][x];
			}
		}
		player.x = 4;
		player.y = 6;
		break;
	}

	int i = 0;
	int j = 0;
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
				moveObjX[i] = x;
				moveObjY[i] = y;
				i++;
			}
			//�}�b�v�`�b�v��̃_���[�W�u���b�N���L�^
			else if (map[y][x] == DAMAGE) {
				damageObjX[j] = x;
				damageObjY[j] = y;
				j++;
			}
		}
	}
}

void Stage::Update(int& scene, char* keys, char* oldkeys) {
	//�����u���b�N���}�b�v�`�b�v�ɋL�^
	map[moveObjY[0]][moveObjX[0]] = M_OBJ;

	//�����u���b�N���������u���b�N�𕜊�
	//�S�[��
	if (map[goalY][goalX] == NONE) {
		map[goalY][goalX] = GOAL;
	}
	//�_���[�W�u���b�N
	for (int i = 0; i < objNum; i++) {
		if (map[damageObjY[i]][damageObjX[i]] == NONE) {
			map[damageObjY[i]][damageObjX[i]] = DAMAGE;
		}
	}

	//�����ɂ���Ĉړ��ʕω�
	switch (muki) {
	case 0: fallX = 0; fallY = 1; break;
	case 1: fallX = 1; fallY = 0; break;
	case 2: fallX = 0; fallY = -1; break;
	case 3: fallX = -1; fallY = 0; break;
	}

	//��Ԃɂ���ď����𕪊�
	switch (state) {
	case 0:	// ���͑҂����

		//A�������ꂽ�獶�����Ɉړ������ԂɈڍs����
		if (keys[KEY_INPUT_A] && !oldkeys[KEY_INPUT_A]) {
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
				if (map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] == NONE
					&& map[player.y + movY + fallY][player.x + movX + fallX] != BLOCK) {
					break;
				}

				//��Ԃ����ړ����ɂ���
				state = 1;
				count = 0;
			}
			//�ړ���̃}�X�ɏ�Q�����������ꍇ1�}�X�Ȃ��ɏ��
			else if (map[player.y + movY][player.x + movX] == OBJ
				|| map[player.y + movY][player.x + movX] == M_OBJ) {
				if (map[player.y + movY - fallY][player.x + movX - fallX] == NONE
					|| map[player.y + movY - fallY][player.x + movX - fallX] == GOAL) {
					state = 1;
					count = 0;
				}
			}
		}


		//D�������ꂽ�獶�����Ɉړ������ԂɈڍs����
		if (keys[KEY_INPUT_D] && !oldkeys[KEY_INPUT_D]) {
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
				if (map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] == NONE
					&& map[player.y + movY + fallY][player.x + movX + fallX] != BLOCK) {
					break;
				}

				// ��Ԃ��E�ړ����ɂ���
				state = 2;
				count = 0;
			}
			//�ړ���̃}�X�ɏ�Q�����������ꍇ1�}�X�Ȃ��ɏ��
			else if (map[player.y + movY][player.x + movX] == OBJ
				|| map[player.y + movY][player.x + movX] == M_OBJ) {
				if (map[player.y + movY - fallY][player.x + movX - fallX] == NONE
					|| map[player.y + movY - fallY][player.x + movX - fallX] == GOAL) {
					state = 2;
					count = 0;
				}
			}
		}

		// E��������Ă����獶�ɂX�O�x�ύX����
		if (keys[KEY_INPUT_E] && !oldkeys[KEY_INPUT_E]) {
			// ��Ԃ������񒆂ɂ���
			state = 3;
			count = 0;
		}

		// Q��������Ă�����E�ɂX�O�x�ύX����
		if (keys[KEY_INPUT_Q] && !oldkeys[KEY_INPUT_Q]) {
			// ��Ԃ��E���񒆂ɂ���
			state = 4;
			count = 0;
		}
		break;

	case 1:	// ���ړ������
		// �J�E���g��i�߂�
		count++;

		// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
		if (count == moveFrame) {
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
		if (count == moveFrame) {
			player.x += movX - fallX;
			player.y += movY - fallY;
			state = 0;
			count = 0;
		}
		break;

	case 3:	// �����񒆏��
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

	case 4:	// �E���񒆏��
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
	//��Q�����Ȃ����艺�Ɉړ�
	//�v���C���[
	if (map[player.y + fallY][player.x + fallX] < BLOCK
		|| map[player.y + fallY][player.x + fallX] > M_OBJ) {
		player.x += fallX;
		player.y += fallY;
	}

	//�����u���b�N
	if (map[moveObjY[0] + fallY][moveObjX[0] + fallX] < BLOCK
		|| map[moveObjY[0] + fallY][moveObjX[0] + fallX] > M_OBJ) {
		map[moveObjY[0]][moveObjX[0]] = NONE;
		moveObjX[0] += fallX;
		moveObjY[0] += fallY;
	}

	//�_���[�W����
	Damage();

	//�N���A����
	if (Clear()) {
		DrawFormatString(0, 15, 0xffffff, "�N���A");
		farstGame = true;
		scene = CLEAR;
	}

	//�^�C�}�[����
	timer--;

	//�Q�[���I�[�o�[����
	if (player.hp <= 0 || timer <= 0) {
		farstGame = true;
		scene = TITLE;
	}
}

void Stage::Draw() {
	//�}�b�v�`�b�v
	//�s
	for (int y = 0; y < mapY1; y++) {
		//��
		for (int x = 0; x < mapX1; x++) {
			int graph;
			if (map[y][x] == BLOCK) {
				graph = blockGH;
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
	//DrawFormatString(0, 15, 0xffffff, "%d", muki);
	/*DrawLine(0, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT / 2, 0xffffff, 2);
	DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, 0xffffff, 2);*/
}

//�_���[�W����
void Stage::Damage() {
	//�_���[�W�u���b�N�Əd�Ȃ�������hp�������ă��Z�b�g
	if (map[player.y][player.x] == DAMAGE) {
		player.hp--;
		Reset(stage_);
		DrawFormatString(0, 30, 0xffffff, "�_���[�W");
	}

	//�����I�u�W�F�N�g�Əd�Ȃ�����hp�������ă��Z�b�g
	if (map[player.y][player.x] == M_OBJ) {
		player.hp--;
		Reset(stage_);
		DrawFormatString(0, 30, 0xffffff, "�_���[�W");
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
