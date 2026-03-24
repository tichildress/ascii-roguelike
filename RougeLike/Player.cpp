#include "Player.h"
#include <random>
#include <ctime>
#include <string>

using namespace std;

Player::Player() {
	_x = 0;
	_y = 0;
}

void Player::init(int level, int health, int attack, int defense, int experience) {

	_level = level;
	_health = health;
	_attack = attack;
	_defense = defense;
	_experience = experience;
}

int Player::attack()
{
	static default_random_engine randomEngine(time(NULL));
	uniform_int_distribution<int> attackRoll(1, _attack);
	int tempRan;
	tempRan = attackRoll(randomEngine);

	printf("ATTACK ROLL IS:%d\n", tempRan);
	system("PAUSE");
	return tempRan;
}

int Player::takeDamage(int attack){
	attack -= _defense;

	if (attack > 0) {
		_health -= attack;
		printf("Your health is now: %d\n", _health);
		system("PAUSE");
		//check if he died
		if (_health <= 0) {
			return 1;
		}
	}
	else {
		return 0;
	}
}

//Setters
void Player::setPosition(int x, int y) {
	_x = x;
	_y = y;
}

void Player::addExperience(int xp)
{
	_experience += xp;

	if (_experience > 60000) { //Bug catch
		_experience = 60000;
	}

	//Level Up
	while (_experience >= 50) {
		_experience -= 50;
		_attack += 10;
		_defense += 10;
		_health += 10;
		_level++;
		printf("You are now level: %d\n", _level);
	}
}



//Getters
void Player::getPosition(int  &x, int& y) {
	x = _x;
	y = _y;
}

