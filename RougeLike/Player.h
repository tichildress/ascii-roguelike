#pragma once

class Player {
public:
	Player();
	void init(int level, int health, int attack, int defense, int experience);

	int attack();
	int takeDamage(int attack);

	//Setters
	void setPosition(int x, int y);
	void addExperience(int xp);

	//Getters
	void getPosition(int& x, int& y);
	int getLevel() { return _level; };
	int getHealth() { return _health; };
	int getAttack() { return _attack; };
	int getDefense() { return _defense; };
	int getExperience() { return _experience; };
	

private:

	int _level;
	int _health;
	int _attack;
	int _defense;
	int _experience;

	int _x;
	int _y;

};