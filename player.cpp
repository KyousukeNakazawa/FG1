#include "player.h"

Player::Player() {
	//ÉäÉ\Å[ÉX
	playerGH = LoadGraph("Resource/pict/player.png");
}

Player::~Player() {

}

void Player::Update() {

}

void Player::Draw(int posX, int posY, float roll) {
	DrawRotaGraph2(WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH / 2 - posX, WIN_HEIGHT / 2 - posY,
		1.0f, roll, playerGH, false, 0);
}