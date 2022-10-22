#include "stage.h"

Stage::Stage() {
	//���\�[�X
	blockGH = LoadGraph("Resource/pict/block.png");
	objBlockGH = LoadGraph("Resource/pict/object.png");

	//�v���C���[���
	player.x = 4;
	player.y = 7;
	playerX = WIN_WIDTH / 2 - player.sizeX;
	playerX = WIN_HEIGHT / 2 - player.sizeX * 2.5;

	//�X�e�[�W�̉�]�������
	muki = 0;
	roll = 0;
	Roll_d = 0;

	state = 0;
}

Stage::~Stage() {

}

void Stage::Update(char* keys, char* oldkeys) {
	// ��Ԃɂ���ď����𕪊�
	switch (state) {
	case 0:	// ���͑҂����

		//A�������ꂽ�獶�����Ɉړ������ԂɈڍs����
		if (keys[KEY_INPUT_A] && !oldkeys[KEY_INPUT_A]) {
			//�����ɂ���Ĉړ��������ς��
			switch (muki) {
			case 0: movx = -1; movy = 0; break;
			case 1: movx = 0; movy = 1; break;
			case 2: movx = 1; movy = 0; break;
			case 3: movx = 0; movy = -1; break;
			}

			// �ړ���̃}�X�ɏ�Q�����Ȃ���Έړ�����
			if (map[player.y + movy][player.x + movx] == NONE) {
				// ��Ԃ����ړ����ɂ���
				state = 1;
				count = 0;
			}
		}


		//D�������ꂽ�獶�����Ɉړ������ԂɈڍs����
		if (keys[KEY_INPUT_D] && !oldkeys[KEY_INPUT_D]) {
			//�����ɂ���Ĉړ��������ς��
			switch (muki) {
			case 0: movx = 1; movy = 0; break;
			case 1: movx = 0; movy = -1; break;
			case 2: movx = -1; movy = 0; break;
			case 3: movx = 0; movy = 1; break;
			}

			// �ړ���̃}�X�ɏ�Q�����Ȃ���Έړ�����
			if (map[player.y + movy][player.x + movx] == NONE) {
				// ��Ԃ��E�ړ����ɂ���
				state = 2;
				count = 0;
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
		playerX += movx * 2;
		playerY += movy * 2;

		// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
		if (count == moveFrame) {
			player.x += movx;
			player.y += movy;
			state = 0;
			count = 0;
		}
		break;

	case 2:	// �E�ړ������
	// �J�E���g��i�߂�
		count++;

		// �J�E���g���ړ����ԂɒB����������W���ړ����ē��͑҂���Ԃɖ߂�
		if (count == moveFrame) {
			player.x += movx;
			player.y += movy;
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


	//�v���C���[
	int posX = player.x * blockSize + WIN_WIDTH / 2 - (mapX1 / 2 * blockSize + blockSize / 2);
	int posY = player.y * blockSize + WIN_HEIGHT / 2 - (mapY1 / 2 * blockSize + blockSize / 2);
	player.Draw(posX, posY, roll + Roll_d);

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