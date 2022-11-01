#include "stage.h"

Stage::Stage() {
	//���\�[�X
	blockGH = LoadGraph("Resource/pict/block.png");
	objBlockGH = LoadGraph("Resource/pict/object.png");
	mObjBlockGH = LoadGraph("Resource/pict/move_object.png");

	//�v���C���[���
	player.x = 4;
	player.y = 7;

	//�X�e�[�W���
	moveObjX[0] = 6;
	moveObjY[0] = 7;

	//�X�e�[�W�̉�]�������
	muki = 0;
	roll = 0;
	Roll_d = 0;

	state = 0;
}

Stage::~Stage() {

}

void Stage::Update(char* keys, char* oldkeys) {
	//�����u���b�N���}�b�v�`�b�v�ɋL�^
	map[moveObjY[0]][moveObjX[0]] = M_OBJ;

	//�����ɂ���Ĉړ��ʕω�
	switch (muki) {
	case 0: fallX = 0; fallY = 1; break;
	case 1: fallX = 1; fallY = 0; break;
	case 2: fallX = 0; fallY = -1; break;
	case 3: fallX = -1; fallY = 0; break;
	}

	// ��Ԃɂ���ď����𕪊�
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
			if (map[player.y + movY][player.x + movX] == NONE) {
				//��Q�����Ȃ������Ƃ��Ă��������Q�}�X�ȏゾ������ړ��s��
				if (map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] == NONE) {
					break;
				}

				// ��Ԃ����ړ����ɂ���
				state = 1;
				count = 0;
			}
			//�ړ���̃}�X�ɏ�Q�����������ꍇ1�}�X�Ȃ��ɏ��
			else if (map[player.y + movY][player.x + movX] == OBJ
				|| map[player.y + movY][player.x + movX] == M_OBJ) {
				if (map[player.y + movY - fallY][player.x + movX - fallX] == NONE) {
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
			if (map[player.y + movY][player.x + movX] == NONE) {
				//��Q�����Ȃ������Ƃ��Ă��������Q�}�X�ȏゾ������ړ��s��
				if (map[player.y + movY + fallY * 2][player.x + movX + fallX * 2] == NONE) {
					break;
				}

				// ��Ԃ��E�ړ����ɂ���
				state = 2;
				count = 0;
			}
			//�ړ���̃}�X�ɏ�Q�����������ꍇ1�}�X�Ȃ��ɏ��
			else if (map[player.y + movY][player.x + movX] == OBJ
				|| map[player.y + movY][player.x + movX] == M_OBJ) {
				if (map[player.y + movY - fallY][player.x + movX - fallX] == NONE) {
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
	if (map[player.y + fallY][player.x + fallX] == NONE) {
		player.x += fallX;
		player.y += fallY;
	}

	//�����u���b�N
	if (map[moveObjY[0] + fallY][moveObjX[0] + fallX] == NONE) {
		map[moveObjY[0]][moveObjX[0]] = NONE;
		moveObjX[0] += fallX;
		moveObjY[0] += fallY;
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
			else {
				graph = 0;
			}

			//��ʂ̒��S�ɂȂ�悤�ɕ`��
			int posX = x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
			int posY = y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
			DrawRotaGraph2(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 2 - posX, WIN_HEIGHT / 2 - posY,
				1.0f, roll + Roll_d, graph, false, 0);
		}
	}

	//�����u���b�N�`��
	{
		int posX = moveObjX[0] * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
		int posY = moveObjY[0] * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
		DrawRotaGraph2(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 2 - posX, WIN_HEIGHT / 2 - posY,
			1.0f, roll + Roll_d, mObjBlockGH, false, 0);
	}


	//�v���C���[
	{
		int posX = player.x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
		int posY = player.y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
		player.Draw(posX, posY, roll + Roll_d);
	}

	//�f�o�b�N
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