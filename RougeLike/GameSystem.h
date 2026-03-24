#pragma once

#include "Level.h"
#include "Player.h"
#include "Enemy.h"
#include <string>

using namespace std;


class GameSystem {
public:
	GameSystem(string levelFileName);

	void playGame();
	void playerMove();

private:
	Level _level;
	Player _player;
};