#pragma once
#include <string>


using namespace std;

class Enemy
{
public:
	Enemy(string name, char tile, int level, int attack, int defense, int health, int xp);

	int attack();
	int takeDamage(int attack);

	//Setters
	void setPosition(int x, int y);

	//Getters
	void getPosition(int& x, int& y);
	string getName() { return _name; }
	char getTile() { return _tile; }

	//Ai Move Command
	char getMove(int playerX, int playerY);

private:

	string _name;
	char _tile;
	
	int _level;
	int _attack;
	int _defense;
	int _health;
	int _experienceValue;

	int _x;
	int _y;
};

