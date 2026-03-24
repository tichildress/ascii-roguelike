#include "GameSystem.h"
#include <iostream>
#include <conio.h>

GameSystem::GameSystem(string startLevelFileName){

	_player.init(1, 100, 10, 10, 0);

	_level.load(startLevelFileName, _player);
	
	system("PAUSE");
}

void GameSystem::playGame() {

	bool isDone = false;

	while (!isDone) {
		_level.print(_player);
		_level.updateEnemies(_player);
		playerMove();
		

	}

}

void GameSystem::playerMove() {
	char input;
	printf("Enter a move command (w/a/s/d): ");
	input = _getch();

	_level.movePlayer(input, _player);
	
}

