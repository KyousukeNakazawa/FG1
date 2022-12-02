#include "DxLib.h"
#include "screen.h"
#include "stage.h"

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE_[] = "";
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE_);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0xff);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	//�T�E���h
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

	//���\�[�X
	int titleGH[4];
	LoadDivGraph("Resource/pict/title.png", 4, 2, 2, WIN_WIDTH, WIN_HEIGHT, titleGH);
	int clearGH = LoadGraph("Resource/pict/clear.png");

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	Screen* screen = new Screen;
	Stage* stage = new Stage;

	int scene_ = TITLE;
	int stage_ = TUTORIAL;

	int backX = 0;
	int backY = 0;

	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; ++i) {
			oldkeys[i] = keys[i];
		}

		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//
		//�X�V����
		switch (scene_) {
		case TITLE:
			StopSoundMem(gameBGM);
			StopSoundMem(endBGM);
			//BGM
			if (!CheckSoundMem(titleBGM)) {
				PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP, true);
			}
			//�X�e�[�W�I��
			//��
			if ((keys[KEY_INPUT_W] && !oldkeys[KEY_INPUT_W])
				|| (keys[KEY_INPUT_UP] && !oldkeys[KEY_INPUT_UP])) {
				stage_--;
				if (stage_ < TUTORIAL) stage_ = STAGE3;
				PlaySoundMem(selectSE, DX_PLAYTYPE_BACK, true);
			}

			//��
			if (keys[KEY_INPUT_S] && !oldkeys[KEY_INPUT_S]
				|| (keys[KEY_INPUT_DOWN] && !oldkeys[KEY_INPUT_DOWN])) {
				stage_++;
				if (stage_ > STAGE3) stage_ = TUTORIAL;
				PlaySoundMem(selectSE, DX_PLAYTYPE_BACK, true);
			}

			//����
			if (keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE]) {
				PlaySoundMem(decisionSE, DX_PLAYTYPE_BACK, true);
				//�`���[�g���A���Ȃ�t���O��on
				if (stage_ == TUTORIAL) stage->SetTutorialFlag();
				scene_ = GAME;
				stage->Reset(stage_);
			}

			//�`��
			if (stage_ == TUTORIAL) DrawGraph(backX, backY, titleGH[0], true);
			else if (stage_ == STAGE1) DrawGraph(backX, backY, titleGH[1], true);
			else if (stage_ == STAGE2) DrawGraph(backX, backY, titleGH[2], true);
			else if (stage_ == STAGE3) DrawGraph(backX, backY, titleGH[3], true);

			//�f�o�b�N
			DrawFormatString(0, 0, 0xffffff, "�^�C�g��");
			if (stage_ == TUTORIAL) DrawFormatString(0, 15, 0xffffff, "�`���[�g���A��");
			else if (stage_ == STAGE1) DrawFormatString(0, 15, 0xffffff, "STAGE1");
			else if (stage_ == STAGE2) DrawFormatString(0, 15, 0xffffff, "STAGE2");
			else if (stage_ == STAGE3) DrawFormatString(0, 15, 0xffffff, "STAGE3");

			DrawFormatString(0, 40, 0xffffff, "W,S �ŃX�e�[�W�I��");
			DrawFormatString(0, 55, 0xffffff, "Space�Ō���");
			break;

		case GAME:
			StopSoundMem(titleBGM);
			StopSoundMem(endBGM);
			//BGM
			if (!CheckSoundMem(gameBGM)) {
				PlaySoundMem(gameBGM, DX_PLAYTYPE_LOOP, true);
			}

			stage->Update(scene_, keys, oldkeys);

			//�`�揈��
			screen->Draw(scene_, stage->HpGet());

			stage->Draw();

			//�f�o�b�N
			//DrawFormatString(0, 0, 0xffffff, "�Q�[��");
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

			//�`��
			//�w�i
			DrawGraph(backX, backY, clearGH, true);

			//�f�o�b�N
			DrawFormatString(0, 0, 0xffffff, "�N���A");
			DrawFormatString(0, 15, 0xffffff, "Space�Ń^�C�g���ɖ߂�");
			break;
		}

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}

	//�f���[�g
	delete screen;
	delete stage;

	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}
