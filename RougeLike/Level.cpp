#include "Level.h"
#include "Player.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <queue>
#include <tuple>
#include <vector>
#include <cmath>

using namespace std;

struct Point {
	int x, y;

	Point(int _x, int _y) : x(_x), y(_y) {}
};
Level::Level() {

}
void Level::load(string fileName, Player &player) {

	//Loads Level

	ifstream file;
	file.open(fileName);
	if (file.fail()) {
		perror(fileName.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;

	while (getline(file, line)) {
		_levelData.push_back(line);
		_originalLevelData.push_back(line);
	}

	file.close();

	//Process Level

	char tile;
	for (int i = 0; i < _levelData.size(); i++) {
		for (int j = 0; j < _levelData[i].size(); j++) {
			tile = _levelData[i][j];

			switch (tile) {
			case '@': //Player
				player.setPosition(j, i);
				break;
			case 'S': //Player
				_enemies.push_back(Enemy("Snake", tile, 1, 3, 1, 10, 10));
				_enemies.back().setPosition(j, i);
				break;
			case 's': //Snail
				_enemies.push_back(Enemy("Snail", tile, 1, 1, 1, 1, 100));
				_enemies.back().setPosition(j, i);
				break;
			case 'G': //Goblin
				_enemies.push_back(Enemy("Goblin", tile, 1, 50, 50, 50, 150));
				_enemies.back().setPosition(j, i);
				break;
			case 'D': //Dragon
				_enemies.push_back(Enemy("Dragon", tile, 100, 5000, 5000, 2000, 1000));
				_enemies.back().setPosition(j, i);
				break;
			}
		}
	}

}
void Level::print(Player &player) {

	int playerX, playerY;
	player.getPosition(playerX, playerY);

	system("CLS");

	for (int i = 0; i < _levelData.size(); i++) {
		printf("%s", _levelData[i].c_str());
		switch (i) {
		case 0:
			printf("\n");
			break;
		case 1:
			printf("#===================#\n");
			break;
		case 2:
			printf("|=Health: %d\n", player.getHealth());
			break;
		case 3:
			printf("|=LEVEL:  %d\n", player.getLevel());
			break;
		case 4:
			printf("|==%d==(", player.getExperience());
			for (int i = 0; i < player.getExperience() / 10; i++) {
				printf("O");
			}
			printf(")\n");
			break;
		case 5:
			printf("|\n");
			break;
		case 6:
			printf("|=ATTACK:  %d\n", player.getAttack());
			break;
		case 7:
			printf("|=DEFENSE: %d\n", player.getDefense());
			break;
		case 8:
			printf("|=ATTACK:  %d\n", player.getAttack());
			break;
		case 9:
			printf("|===================|\n");
			break;
		case 10:
			printf("|=====POSITION======|\n");
			break;
		case 11:
			printf("|==X: %d ===Y: %d ==|\n", playerX, playerY);
			break;
		case 12:
			printf("#===================#\n");
			break;
		default:
			printf("\n");
			break;
		}
	}
	printf("\n");
}

bool Level::isAnEnemy(int x, int y) {
	for (int i = 0; i < _enemies.size(); i++) {
		int enemyX, enemyY;
		_enemies[i].getPosition(enemyX, enemyY);

		if (x == enemyX && y == enemyY) {
			return true;  // Position contains an enemy
		}
	}
	return false;  // Position does not contain an enemy
}
bool Level::hasObstacleBetween(int x1, int y1, int x2, int y2) const {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		if (_levelData[y1][x1] == '#') {
			return true;  // Obstacle (wall) found
		}

		if (x1 == x2 && y1 == y2) {
			break;  // Reached the target position
		}

		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x1 += sx;
		}

		if (e2 < dx) {
			err += dx;
			y1 += sy;
		}
	}

	return false;  // No obstacle found
}
void Level::performBFS(Player& player) {
	int playerX, playerY;
	player.getPosition(playerX, playerY);

	// Specify the vision radius (adjust as needed)
	const double visionRadius = 5.0;

	// Mark all tiles as not visited
	vector<vector<bool>> visited(_levelData.size(), vector<bool>(_levelData[0].size(), false));

	// Store the original state of the level tiles
	vector<string> originalLevelData = _levelData;

	// Queue for BFS
	queue<tuple<int, int, int>> bfsQueue;

	// Mark the current tile as visited and enqueue it with distance 0
	visited[playerY][playerX] = true;
	bfsQueue.push(make_tuple(playerX, playerY, 0));

	while (!bfsQueue.empty()) {
		int currentX, currentY, distance;
		tie(currentX, currentY, distance) = bfsQueue.front();
		bfsQueue.pop();

		// Check if the path is not a wall ('#') and does not contain an enemy
		if (_levelData[currentY][currentX] != '#' && !isAnEnemy(currentX, currentY)) {
			// Check for obstacles (walls) between the player and the current tile
			if (!hasObstacleBetween(playerX, playerY, currentX, currentY)) {
				// Calculate the Euclidean distance from the player to the current tile
				double euclideanDistance = sqrt(pow(currentX - playerX, 2) + pow(currentY - playerY, 2));

				// Set the tile to '.' if it is within the vision radius
				if (euclideanDistance <= visionRadius) {
					if (currentX != playerX || currentY != playerY) {
						_levelData[currentY][currentX] = '.';
					}
				}

				// Get all adjacent tiles
				vector<Point> neighbors = {
					{currentX - 1, currentY},
					{currentX + 1, currentY},
					{currentX, currentY - 1},
					{currentX, currentY + 1}
				};

				for (const auto& neighbor : neighbors) {
					int nx = neighbor.x;
					int ny = neighbor.y;

					// Check if the neighbor is within the bounds of the level
					if (nx >= 0 && nx < _levelData[0].size() && ny >= 0 && ny < _levelData.size()) {
						// Check if the neighbor is not a wall, not visited, and does not contain an enemy
						if (_levelData[ny][nx] != '#' && !visited[ny][nx] && !isAnEnemy(nx, ny)) {
							visited[ny][nx] = true;
							bfsQueue.push(make_tuple(nx, ny, distance + 1));

							// You can perform additional actions here, such as updating the tile
							// or performing some game logic based on the BFS traversal
						}
					}
				}
			}
			else {
				// Revert the tile back to its original state if obstructed by a wall
				_levelData[currentY][currentX] = originalLevelData[currentY][currentX];
			}
		}
	}

	// Set tiles back to '~' if more than 5 spaces away from the player
	for (int i = 0; i < _levelData.size(); i++) {
		for (int j = 0; j < _levelData[i].size(); j++) {
			double euclideanDistance = sqrt(pow(j - playerX, 2) + pow(i - playerY, 2));
			if (_levelData[i][j] == '.' && euclideanDistance > visionRadius) {
				_levelData[i][j] = '~';
			}
		}
	}

	// Reveal enemies within the vision radius
	for (int i = 0; i < _enemies.size(); i++) {
		int enemyX, enemyY;
		_enemies[i].getPosition(enemyX, enemyY);

		double enemyDistance = sqrt(pow(enemyX - playerX, 2) + pow(enemyY - playerY, 2));
		if (enemyDistance <= visionRadius && !hasObstacleBetween(playerX, playerY, enemyX, enemyY)) {
			_levelData[enemyY][enemyX] = _enemies[i].getTile();
		}
		else {
			_levelData[enemyY][enemyX] = '~';  // Hide enemies outside the vision radius or behind walls
		}
	}
}

void Level::movePlayer(char input, Player& player) {

	int playerX;
	int playerY;
	player.getPosition(playerX, playerY);

	switch (input) {
	case 'b':
		performBFS(player); //// Testing!!!!
		break;
	case 'w': //up
	case 'W':
		processPlayerMove(player, playerX, playerY - 1);
		break;
	case 's': //down
	case 'S':
		processPlayerMove(player, playerX, playerY + 1);
		break;
	case 'a': //left
	case 'A':
		processPlayerMove(player, playerX - 1, playerY);
		break;
	case 'd': //right
	case 'D':
		processPlayerMove(player, playerX + 1, playerY);
		break;
	default:
		printf("Error: Not a command");
		system("PAUSE");
		break;
	}
}

void Level::updateEnemies(Player& player){
	char aiMove;
	int playerX;
	int playerY;
	int enemyX;
	int enemyY;

	player.getPosition(playerX, playerY);

	for (int i = 0; i < _enemies.size(); i++) {
		aiMove = _enemies[i].getMove(playerX, playerY);
		_enemies[i].getPosition(enemyX, enemyY);
		switch (aiMove) {
		case 'w': //up
			processEnemyMove(player, i, enemyX, enemyY - 1);
			break;
		case 's': //down
			processEnemyMove(player, i, enemyX, enemyY + 1);
			break;
		case 'a': //left
			processEnemyMove(player, i, enemyX - 1, enemyY);
			break;
		case 'd': //right
			processEnemyMove(player, i, enemyX + 1, enemyY);
			break;
		default:
			break;
		}
	}
}

char Level::getTile(int x, int y)
{
	return _levelData[y][x];
}

void Level::setTile(int x, int y, char tile)
{
	_levelData[y][x] = tile;
}

void Level::processPlayerMove(Player &player, int targetX, int targetY){

	int playerX;
	int playerY;
	player.getPosition(playerX, playerY);

	char moveTile = getTile(targetX, targetY);

	switch (moveTile) {
	case '~':
	case '.':
		player.setPosition(targetX, targetY);
		setTile(playerX, playerY, '~');
		setTile(targetX, targetY, '@');
		performBFS(player);
		break;
	case '#':
		break;
	default:
		battleMonster(player, targetX, targetY);
		break;
	}
}

void Level::processEnemyMove(Player& player, int enemyIndex, int targetX, int targetY){
	int playerX;
	int playerY;
	int enemyX;
	int enemyY;

	_enemies[enemyIndex].getPosition(enemyX, enemyY);

	player.getPosition(playerX, playerY);

	char moveTile = getTile(targetX, targetY);
	
	switch (moveTile) {
	case '~':
		_enemies[enemyIndex].setPosition(targetX, targetY);
		setTile(enemyX, enemyY, '~');
		setTile(targetX, targetY, _enemies[enemyIndex].getTile());
		break;
	case '@':
		battleMonster(player, enemyX, enemyY);
	default:
		break;
	}
}

void Level::battleMonster(Player& player, int targetX, int targetY) {
	int enemyX;
	int enemyY;
	int playerX;
	int playerY;
	int attackRoll;
	int attackResult;
	string enemyName;

	player.getPosition(playerX, playerY);

	for (int i = 0; i < _enemies.size(); i++) {
		_enemies.at(i).getPosition(enemyX, enemyY);
		enemyName = _enemies.at(i).getName();

		if (targetX == enemyX && targetY == enemyY) {
			// Battle!
			attackRoll = player.attack();
			printf("Player attacked %s with a roll of %d\n", enemyName.c_str(), attackRoll);
			attackResult = _enemies.at(i).takeDamage(attackRoll);
			if (attackResult != 0) {
				setTile(targetX, targetY, '.');
				print(player);
				printf("Player killed %s and gained %dXP\n", enemyName.c_str(), attackResult);

				// Remove Enemy
				_enemies.at(i) = _enemies.back();
				_enemies.pop_back();
				i--;

				player.addExperience(attackResult);
				system("PAUSE");

				return;
			}

			// Monster Turn
			attackRoll = _enemies[i].attack();
			printf("%s attacked Player with a roll of %d\n", enemyName.c_str(), attackRoll);
			system("PAUSE");
			attackResult = player.takeDamage(attackRoll);

			if (attackResult != 0) {
				setTile(playerX, playerY, 'X');
				print(player);
				cout << "You Died!" << endl;
				system("PAUSE");

				exit(1);
			}
		}
	}
}
