#pragma once
#include <vector>
#include <string>

#include "Player.h"
#include "Enemy.h"

using namespace std;

class Level{
public:
	Level();

	void load(string fileName, Player &player);
	void print(Player &player);
	bool isAnEnemy(int x, int y);

	bool hasObstacleBetween(int x1, int y1, int x2, int y2) const;
	void performBFS(Player& player);

	void movePlayer(char input, Player &player);
	void updateEnemies(Player& player);

	//Getters
	char getTile(int x, int y);

	// Setters
	void setTile(int x, int y, char tile);
private:
	void processPlayerMove(Player &player, int targetX, int targetY);
	void processEnemyMove(Player& player, int enemyIndex, int targetX, int targetY);
	void battleMonster(Player& player, int targerX, int targetY);

	vector <string> _levelData;
	vector <string> _originalLevelData;
	vector<Enemy> _enemies;
};

